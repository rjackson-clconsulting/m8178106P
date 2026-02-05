/**
@file		UART_FM3.h
@brief		UART function in order to be used with comproto.cpp in FM3
            based project
@author		Fernando Morani (Inherited from Nicola 06/12/2012)
@date		05/03/2018
@version	01.00.01

Notes on version 01.00.01
    - Add some macros
    - Add support for 57600 baudRate
*/


#ifndef _UART_FM3_H
#define _UART_FM3_H

#include "global.h"
#include "fm3_define.h"
#include "mb9bf50x.h"
#include "ringbuf.h"
#include "base_types.h"

#define UART_57600
#undef UART_38400

struct uart_fm3_t{
	  __IO  uint8_t SMR;
	  __IO  uint8_t SCR;
	        uint8_t RESERVED0[2];
	  __IO  uint8_t ESCR;
	  __IO  uint8_t SSR;
	        uint8_t RESERVED1[2];
	  union {
	    __IO uint16_t RDR;
	    __IO uint16_t TDR;
	  };
	        uint8_t RESERVED2[2];
	  union {
	    __IO uint16_t BGR;
	    struct {
	      __IO  uint8_t BGR0;
	      __IO  uint8_t BGR1;
	    };
	  };
};
#ifdef __cplusplus
#else
typedef struct uart_fm3_t uart_fm3_t;
#endif

/**
** Version 01.00.01 - Macros for UART Polling
**/
#define RX_DIRECTION            0x0
#define TX_DIRECTION            0x1

/**
 * Overrun error flag
 */
#define UART_SSR_OVE	BIT3

/**
 * Framing error flag
 */
#define UART_SSR_FRE 	BIT4

/**
 * Parity error flag
 */
#define UART_SSR_PE 	BIT5

/**
 * Received error flag clear
 */
#define UART_SSR_CLR	BIT7

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
                            // definizioni per baudrate generico nelle impostazioni
#define BAUD460800      460800
#define BAUD230400      230400
#define BAUD115200      115200
#define BAUD76800       76800
#define BAUD57600       57600
#define BAUD38400       38400
#define BAUD28800       28800
#define BAUD19200       19200
#define BAUD14400       14400
#define BAUD9600        9600
#define BAUD4800        4800
#define BAUD2400        2400

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

enum uart_hw_error_masks_t
{
	EUART_HW_ERR_MASK_NONE 			= 0,
	EUART_HW_ERR_MASK_OVERRUN 			= BIT0,
	EUART_HW_ERR_MASK_FRAME 			= BIT1,
	EUART_HW_ERR_MASK_PARITY 			= BIT2,
	EUART_HW_ERR_MASK_TX_BUF_OVERFLOW 	= BIT3,
	EUART_HW_ERR_MASK_RX_BUF_OVERFLOW 	= BIT4
};

struct uart_t{					// doppio buffer per comunicazione
	uart_fm3_t  	*pReg;
	ringBuffer_t	txBuf;					// flat buffer tx
	ringBuffer_t 	rxBuf;					// flat buffer rx
	uint8_t			lastTxChar;
	uint8_t			lastRxChar;
	uint8_t			hwErrorMask;
};
#ifdef __cplusplus
#else
typedef struct uart_t uart_t;
#endif

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

#ifdef __cplusplus
extern "C" {
#endif

void uart3_init(void) 													__attribute__ (( section(".ramcode") ));

int uart_putc(uart_t *__pUart, uint8_t c) 								__attribute__ (( section(".ramcode") ));
int uart_getc(uart_t *__pUart, uint8_t *__c) 							__attribute__ (( section(".ramcode") ));
int uart_puts(uart_t *__pUart, char *__pStr) 							__attribute__ (( section(".ramcode") ));
bool uart_startTx(uart_t *__pUart, uint16_t __packLen) 					__attribute__ (( section(".ramcode") ));
void uart_resetRxBuf(uart_t *__pUart)  									__attribute__ (( section(".ramcode") ));
uint16_t uart_getRxCount(uart_t *__pUart) 								__attribute__ (( section(".ramcode") ));
void uart_resetTxBuf(uart_t *__pUart) 									__attribute__ (( section(".ramcode") ));
uint16_t uart_getTxCount(uart_t *__pUart) 								__attribute__ (( section(".ramcode") ));
uint16_t uart_getFreeTxCount(uart_t *__pUart) 							__attribute__ (( section(".ramcode") ));
bool uart_isTxEmpty(uart_t *__pUart) 									__attribute__ (( section(".ramcode") ));
uint8_t uart_getHwError(uart_t *__pUart)  								__attribute__ (( section(".ramcode") ));

void uart_changeBaudRate(uart_t *__pUart, int __rate) 					__attribute__ (( section(".ramcode") ));
int uart_getBrg(uart_t *__pUart) 										__attribute__ (( section(".ramcode") ));
void uart_turnOn(uart_t *__pUart) 										__attribute__ (( section(".ramcode") ));
void uart_turnOff(uart_t *__pUart) 										__attribute__ (( section(".ramcode") ));
void uart_changeBaudRate_115200(uart_t *__pUart) 						__attribute__ (( section(".ramcode") ));
void uart_changeBaudRate_57600(uart_t *__pUart) 						__attribute__ (( section(".ramcode") ));
void uart_changeBaudRate_38400(uart_t *__pUart) 						__attribute__ (( section(".ramcode") ));
void uart_changeBaudRate_9600(uart_t *__pUart) 							__attribute__ (( section(".ramcode") ));
void uart_changeBaudRate_debug(uart_t *__pUart) 						__attribute__ (( section(".ramcode") ));
#ifdef LOADER_ENGINE_INT
void uart_poller(uart_t *__pUart, uint8_t __direction,boolean_t* bVerify)   __attribute__ (( section(".ramcode") ));
#else
void uart_poller(uart_t *__pUart, uint8_t __direction) 					__attribute__ (( section(".ramcode") ));
#endif
void uart_emptyTxBuf(uart_t *__pUart)									__attribute__ (( section(".ramcode") ));

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern uart_t g_uart3;

#endif





