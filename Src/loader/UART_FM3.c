/**
@file		UART_FM3.c
@brief		UART function in order to be used with comproto.cpp in FM3
            based project
@author		Fernando Morani (Inherited from Nicola 06/12/2012)
@date		05/03/2018
@version	01.00.01

Notes on version 01.00.01
    - Add support for 57600 baudRate
*/


#include "UART_FM3.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

/**
 * UART 3 defines and buffers
 */
#define RX_BUF_DIM_UART3	2048
#define TX_BUF_DIM_UART3	512
uart_t g_uart3;
uint8_t g_aUart3_pRxBuf[RX_BUF_DIM_UART3];
uint8_t g_aUart3_pTxBuf[TX_BUF_DIM_UART3];

uint16_t progressiv;
//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------
void uart_initBuf(uart_t *__pUart,
				  uart_fm3_t *__pReg,
				  uint8_t *__pTx_buf, int __tx_buf_size,
				  uint8_t *__pRx_buf, int __rx_buf_size) 					__attribute__ (( section(".ramcode") ));

void uart_init_regs(uart_t *__pUart) 										__attribute__ (( section(".ramcode") ));

void uart_write_Tx_reg(uart_t *__pUart, uint16_t __c) 						__attribute__ (( section(".ramcode") ));
bool uart_Tx_reg_empty(uart_t *__pUart) 									__attribute__ (( section(".ramcode") ));
void uart_read_Rx_reg(uart_t *__pUart, uint16_t *__pData, uint8_t *__pErr) 	__attribute__ (( section(".ramcode") ));
bool uart_Rx_reg_full(uart_t *__pUart) 										__attribute__ (( section(".ramcode") ));
//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Corpo funzioni di questo modulo



/* Cerca di inizializzare la gestione uart (universal asinc rx tx)
*/
void uart3_init(void)
{

	#ifndef STAND_ALONE_BOOTLOADER
	uart_initBuf(	&g_uart3,
					(uart_fm3_t*)FM3_MFS3_UART,
					g_aUart3_pTxBuf, TX_BUF_DIM_UART3,
					g_aUart3_pRxBuf, RX_BUF_DIM_UART3);	// reset dati "sensibili"*/
	#else
	/**The following is the init for standAlone bootloader**/
    uart_initBuf(	&g_uart3,
					(uart_fm3_t*)FM3_MFS0_UART,
					g_aUart3_pTxBuf, TX_BUF_DIM_UART3,
					g_aUart3_pRxBuf, RX_BUF_DIM_UART3);	// reset dati "sensibili"
    #endif

	uart_init_regs(&g_uart3);
	uart_turnOn(&g_uart3);
}


void uart_initBuf(uart_t *__pUart,
				  uart_fm3_t *__pReg,
				  uint8_t *__pTx_buf, int __tx_buf_size,
				  uint8_t *__pRx_buf, int __rx_buf_size)
{
	__pUart->pReg = __pReg;

	ringBuffer_static_init(&__pUart->txBuf, __pTx_buf, __tx_buf_size, sizeof(uint8_t));
	ringBuffer_static_init(&__pUart->rxBuf, __pRx_buf, __rx_buf_size, sizeof(uint8_t));
}


/* Spara un carattere in un canale di comunicazione precedentemente aperto
*/
int uart_putc(uart_t *__pUart, uint8_t __c)
{
	if(ringBuffer_isfull(&__pUart->txBuf))
	{
		__pUart->hwErrorMask |= EUART_HW_ERR_MASK_TX_BUF_OVERFLOW;
		return( -1 );
	}

	ringBuffer_put(&__pUart->txBuf, &__c);

	__pUart->lastTxChar = __c;

	return(0);
}


/* Spara una stringa in un canale di comunicazione precedentemente aperto
*/
int uart_puts(uart_t *__pUart, char *__pStr)
{
	while( *__pStr )
	{
		__pUart->lastTxChar = *__pStr;
		if(uart_putc(__pUart, *__pStr++) == -1)
			return( -1 );
	}
	return(0);
}

/* Piglia un carattere in un canale di comunicazione precedentemente aperto
*/
int uart_getc(uart_t *__pUart, uint8_t *__c)
{
	if(ringBuffer_isEmpty(&__pUart->rxBuf))
		return( -1 );

	ringBuffer_get(&__pUart->rxBuf, __c);
	__pUart->lastRxChar = *__c;
    return (0);       // Get character from buffer
}


/* Avvia la trasmissione tramite irq
*/
bool uart_startTx(uart_t *__pUart, uint16_t __packLen)
{
	uint8_t data;
	if ((__pUart->pReg->SSR & BIT1) == BIT1 && !ringBuffer_isEmpty(&__pUart->txBuf))
	{
		ringBuffer_get(&__pUart->txBuf, &data);
		__pUart->pReg->TDR = data;
		//txIrqEnable(True);
	}
	return(True);
}


/* Azzera il buffer di ricezione
*/
void uart_resetRxBuf(uart_t *__pUart)
{
	ringBuffer_clear(&__pUart->rxBuf);
}


/* Ritorna il numero di caratteri presenti nel buffer di ricezione
*/
uint16_t uart_getRxCount(uart_t *__pUart)
{
	return ringBuffer_used(&__pUart->rxBuf);
}


/* Azzera il buffer di trasmissione
*/
void uart_resetTxBuf(uart_t *__pUart)
{
	ringBuffer_clear(&__pUart->txBuf);
}


/* Ritorna il numero di caratteri presenti nel buffer di trasmissione
*/
uint16_t uart_getTxCount(uart_t *__pUart)
{
	return ringBuffer_used(&__pUart->rxBuf);
}

/* Ritorna lo spazio disponibile nel buffer di trasmissione
*/
uint16_t uart_getFreeTxCount(uart_t *__pUart)
{
	return (ringBuffer_size(&__pUart->txBuf) - ringBuffer_used(&__pUart->txBuf));
}

/* Verifica se il buffer di trasmissione è vuoto
*/
bool uart_isTxEmpty(uart_t *__pUart)
{
	return ringBuffer_isEmpty(&__pUart->txBuf);
}

uint8_t uart_getHwError(uart_t *__pUart)
{
	return __pUart->hwErrorMask;
}

//////////////////////////////////////////////////////////////////////////
// qui iniziano le funzioni di basso livello per semplicità di debug...
//////////////////////////////////////////////////////////////////////////

void uart_init_regs(uart_t *__pUart)
{
	  // Initialize UART asynchronous mode

	__pUart->pReg->SCR |=  BIT7;   				// UART reset

	__pUart->pReg->SMR &= ~(BIT5 + BIT6 + BIT7); // UART0 mode selected
	__pUart->pReg->SMR &= ~(BIT4); // Disables the Wake-up function.
	__pUart->pReg->SMR &= ~(BIT2); // LSB first (The least significant bit is first transferred.)

	__pUart->pReg->SSR |= (BIT7);   // clear receive error flags

	__pUart->pReg->ESCR  &= ~(BIT7); //disable Hw flow control

	__pUart->pReg->ESCR &= ~(BIT4); //Disables parity.

	__pUart->pReg->ESCR &= ~(BIT0 + BIT1 + BIT2); // 8 frame bits

	#ifdef UART_38400
	uart_changeBaudRate_38400(__pUart);
	#endif
	#ifdef UART_57600
	uart_changeBaudRate_57600(__pUart);
	#endif

	__pUart->pReg->SMR &= ~(BIT3); // 1 stop bit
	__pUart->pReg->ESCR &= ~(BIT6);// 1 stop bit

    #ifdef LOADER_ENGINE_INT
    __pUart->pReg->SCR |= BIT4; // enable RX IRQ
    #else
    __pUart->pReg->SCR &= ~(BIT4); // disable RX IRQ
    #endif

	__pUart->pReg->SCR &= ~(BIT3); // disable TX IRQ

	uart_turnOff(__pUart);
}


/* Imposta il baud richiesto (se possibile)
*/

void uart_changeBaudRate_115200(uart_t *__pUart)
{
	__pUart->pReg->BGR = FM3_UARTCLK / BAUD115200 - 1;
}

void uart_changeBaudRate_57600(uart_t *__pUart)
{
	__pUart->pReg->BGR = FM3_UARTCLK / BAUD57600 - 1;
}

void uart_changeBaudRate_38400(uart_t *__pUart)
{
	__pUart->pReg->BGR = FM3_UARTCLK / BAUD38400 - 1;
}


void uart_changeBaudRate_9600(uart_t *__pUart)
{
	__pUart->pReg->BGR = FM3_UARTCLK / BAUD9600 - 1;
}

void uart_changeBaudRate_debug(uart_t *__pUart)
{
	asm("nop");
}

void uart_changeBaudRate(uart_t *__pUart, int __rate)
{
	__pUart->pReg->BGR = FM3_UARTCLK / __rate - 1;
}


/* Ritorna il valore da piazzare nel registro BRG per avere il baud richiesto
*/
int uart_getBrg(uart_t *__pUart)
{
	return (int) (FM3_UARTCLK  / (__pUart->pReg->BGR + 1));
}


void uart_turnOn(uart_t *__pUart)
{
	__pUart->pReg->SMR |= (BIT0); // Enables a serial data output.
	__pUart->pReg->SCR |= (BIT0 + BIT1); //Enables data received + Enables the transmission
}


void uart_turnOff(uart_t *__pUart)
{
	__pUart->pReg->SMR &= ~(BIT0); // Disables a serial data output.
	__pUart->pReg->SCR &= ~(BIT0 + BIT1); //Disables data received + Disables the transmission
}

void uart_write_Tx_reg(uart_t *__pUart, uint16_t __c)
{
	__pUart->pReg->TDR = __c;
}

bool uart_Tx_reg_empty(uart_t *__pUart)
{
	return ( ((__pUart->pReg->SSR & BIT1) == BIT1) ? True : False);
}

void uart_read_Rx_reg(uart_t *__pUart, uint16_t *__pData, uint8_t *__pErr)
{
	*__pData = __pUart->pReg->RDR; // received data
	*__pErr = __pUart->pReg->SSR; //reception error
}

bool uart_Rx_reg_full(uart_t *__pUart)
{
	return ( ((__pUart->pReg->SSR & BIT2) == BIT2) ? True : False);
}

/** Poller per servizio uart a polling
*/
#ifdef LOADER_ENGINE_INT
void uart_poller(uart_t *__pUart, uint8_t __direction,boolean_t* bVerify)
{
uint16_t c16;
uint8_t  c8;
uint8_t e;


if (__direction == 0){
	//------------------------------------------------------------------------
	//-------------------------------- RX ------------------------------------
	//------------------------------------------------------------------------
	if( uart_Rx_reg_full(__pUart) )		// se abbiamo un dato pronto
	{
		uart_read_Rx_reg(__pUart, &c16, &e);

		if( (e & (UART_SSR_OVE | UART_SSR_FRE)) != 0 )
		{
			if( (e & UART_SSR_OVE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_OVERRUN;
			}
			if( (e & UART_SSR_FRE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_FRAME;
			}
			__pUart->pReg->SSR |= UART_SSR_CLR;
		}
		else
		{
			if( (e & UART_SSR_PE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_PARITY;
				__pUart->pReg->SSR |= UART_SSR_CLR;
			}
			else
			{
				if(!ringBuffer_isfull(&__pUart->rxBuf))
				{
					c8 = c16;
					ringBuffer_put(&__pUart->rxBuf, &c8);
					*bVerify = TRUE;
				}else
				{
					__pUart->hwErrorMask |= EUART_HW_ERR_MASK_RX_BUF_OVERFLOW;
				}
			}
		}
	}

}
	//------------------------------------------------------------------------
	//-------------------------------- TX ------------------------------------
	//------------------------------------------------------------------------

if (__direction == 1){
	if(uart_Tx_reg_empty(__pUart) && !ringBuffer_isEmpty(&__pUart->txBuf))
	{
		ringBuffer_get(&__pUart->txBuf, &c8);
		__pUart->pReg->TDR = c8;
		*bVerify = TRUE;
	}
}
}

#else
void uart_poller(uart_t *__pUart, uint8_t __direction)
{
uint16_t c16;
uint8_t  c8;
uint8_t e;

if (__direction == 0){
	//------------------------------------------------------------------------
	//-------------------------------- RX ------------------------------------
	//------------------------------------------------------------------------
	if( uart_Rx_reg_full(__pUart) )		// se abbiamo un dato pronto
	{
		uart_read_Rx_reg(__pUart, &c16, &e);

		if( (e & (UART_SSR_OVE | UART_SSR_FRE)) != 0 )
		{
			if( (e & UART_SSR_OVE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_OVERRUN;
			}
			if( (e & UART_SSR_FRE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_FRAME;
			}
			__pUart->pReg->SSR |= UART_SSR_CLR;
		}
		else
		{
			if( (e & UART_SSR_PE) != 0 )
			{
				__pUart->hwErrorMask |= EUART_HW_ERR_MASK_PARITY;
				__pUart->pReg->SSR |= UART_SSR_CLR;
			}
			else
			{
				if(!ringBuffer_isfull(&__pUart->rxBuf))
				{
					c8 = c16;
					ringBuffer_put(&__pUart->rxBuf, &c8);
				}else
				{
					__pUart->hwErrorMask |= EUART_HW_ERR_MASK_RX_BUF_OVERFLOW;
				}
			}
		}
	}

}
	//------------------------------------------------------------------------
	//-------------------------------- TX ------------------------------------
	//------------------------------------------------------------------------

if (__direction == 1){
	if(uart_Tx_reg_empty(__pUart) && !ringBuffer_isEmpty(&__pUart->txBuf))
	{
		ringBuffer_get(&__pUart->txBuf, &c8);
		__pUart->pReg->TDR = c8;
	}
}
}
#endif

void uart_emptyTxBuf(uart_t *__pUart)
{
	uint8_t  c8;
	uint32_t i;
	while(!ringBuffer_isEmpty(&__pUart->txBuf))
	{
		if(uart_Tx_reg_empty(__pUart))
		{
			ringBuffer_get(&__pUart->txBuf, &c8);
			__pUart->pReg->TDR = c8;
		}
	}
	while (!uart_Tx_reg_empty(__pUart));

	for(i = 0; i < 100000; i++) //waiting that the last character will be sent....
	{
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
	}
}







