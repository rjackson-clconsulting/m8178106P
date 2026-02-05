/**
@file		COMPROTO.c
@brief		Routines to manage (decode-encode) serial protocol
			Starting project SCOMPROTO.CPP (COMETA).
			Modified code syntax according to the new Medica standard

@author		Nicola/Enrico
@date		07/01/2013
@version	01.00.00
*/

#include "comproto.h"

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------
static uint8_t comproto_putAndPad(uart_t *__pUart, uint8_t __c)	__attribute__ (( section(".ramcode") ));
static void comproto_local_init(comProto_t *__pProto)			__attribute__ (( section(".ramcode") ));
void comproto_decode(comProto_t *__pProto)				__attribute__ (( section(".ramcode") ));


//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
//---- functions pointers ----
bool (*g_fpCheckOpcode)(const uint8_t __opCode);
void (*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_NUM])(const comProto_t *__pProto);
void (*g_fpCommandHadler)(const comProto_t *__pProto);

//-------------------------------------------------------
// Funzioni di appoggio che devono risiedere in ram
//-------------------------------------------------------

// Inizializzazione
void comproto_init(	comProto_t *__pProto,
					uart_t *__pUart,
					uint16_t __rxProtoDataBufLength,
					uint8_t __masterId, uint8_t __slaveId,
					bool (*__fpCheckOpcode)(const uint8_t __opCode),
					void (*__fpCommandHadler)(const comProto_t *__p))
{

	__pProto->pUart = __pUart;

	ringBuffer_init(&__pProto->rxCmdInfo.data, __rxProtoDataBufLength, sizeof(uint8_t));

	__pProto->masterId = __masterId;

	__pProto->slaveId = __slaveId;

	g_fpCheckOpcode = __fpCheckOpcode;

	g_fpCommandHadler = __fpCommandHadler;

	comproto_local_init(__pProto);

}

void comproto_static_init(	comProto_t *__pProto,
							uart_t *__pUart,
							uint8_t *__pRxProtoDataBuf,
							uint16_t __rxProtoDataBufLength,
							uint8_t __masterId, uint8_t __slaveId,
							bool (*__fpCheckOpcode)(const uint8_t __opCode),
							void (*__fpCommandHadler)(const comProto_t *__p))
{

	int i;

	__pProto->pUart = __pUart;

	ringBuffer_static_init(&__pProto->rxCmdInfo.data, __pRxProtoDataBuf, __rxProtoDataBufLength, sizeof(uint8_t));

	__pProto->masterId = __masterId;

	__pProto->slaveId = __slaveId;

	g_fpCheckOpcode = __fpCheckOpcode;

	g_fpCommandHadler = __fpCommandHadler;

	for(i = 0; i < ECOMPROTO_ERR_ID_NUM; i++)
		g_fpErrorsHandlers[i] = NULL;

	comproto_local_init(__pProto);

}

static void comproto_local_init(comProto_t *__pProto)
{
	__pProto->validCommand = 0;

	__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;

	__pProto->error.mask = ECOMPROTO_ERR_MASK_NONE;

	__pProto->error.cntTotal = 0;
	__pProto->error.cntWrongOpCode = 0;
	__pProto->error.cntBadStx = 0;
	__pProto->error.cntBadEtx = 0;
	__pProto->error.cntTimeoutRx = 0;
	__pProto->error.cntTimeoutTx = 0;
	__pProto->error.cntBadCrc = 0;
	__pProto->error.cntDataBufOverflow = 0;
	__pProto->error.cntBadEncoding = 0;

	__pProto->error.cntUartHwError = 0;		// contatore errori di frame

	__pProto->cntComTx = 0;           // contatore numero di messaggi inviati
	__pProto->cntComRx = 0;           // contatore numero di messaggi ricevuti
	__pProto->cntFail = 0;            // contatore comunicazioni fallite

	__pProto->timeoutTx = 200;             // attesa ricezione pacco (in ms)
	__pProto->timeoutRx = 200;             // attesa ricezione pacco (in ms)

	stop(__pProto->timChkRx);
	stop(__pProto->timChkTx);

	__pProto->msgCount = 0;
	__pProto->maxWhileCycles = 10;
}

int comproto_error_handler_callback(comProto_t *__pProto, uint16_t __id,  void (*__fpErrorHandler)(const  comProto_t *__p))
{
	if (__id >= ECOMPROTO_ERR_ID_NUM)
		return -1;

	if(g_fpErrorsHandlers[__id] != NULL)
		return -1;

	g_fpErrorsHandlers[__id] = __fpErrorHandler;

	return 0;
}

void comproto_manager(comProto_t *__pProto)
{
	/**
	 * Timer RX
	 */
	if(match(__pProto->timChkRx))
	{
		__pProto->error.mask |= ECOMPROTO_ERR_MASK_TIMEOUT_RX;
		__pProto->error.cntTimeoutRx++;
		stop(__pProto->timChkRx);
		__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
	}

	/**
	 * Timer TX
	 */
	if(match(__pProto->timChkTx))
	{
		__pProto->error.mask |= ECOMPROTO_ERR_MASK_TIMEOUT_TX;
		__pProto->error.cntTimeoutTx++;
		stop(__pProto->timChkTx);
	}

	/**
	 * Decoding messages
	 */
	//comproto_decode(__pProto);

	/**
	 * Uart hardware errors managing
	 */
	if(__pProto->pUart->hwErrorMask != EUART_HW_ERR_MASK_NONE)
	{
		__pProto->error.cntUartHwError++;
		__pProto->error.cntTotal++;

		//---- OVERRUN ----
		if(( (__pProto->pUart->hwErrorMask & EUART_HW_ERR_MASK_OVERRUN) == EUART_HW_ERR_MASK_OVERRUN)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_OVERRUN] != NULL ))
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_OVERRUN])(__pProto);
		//---- FRAME -----
		if(( (__pProto->pUart->hwErrorMask & EUART_HW_ERR_MASK_FRAME) == EUART_HW_ERR_MASK_FRAME)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_FRAME] != NULL ))
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_FRAME])(__pProto);
		//---- PARITY ----
		if(( (__pProto->pUart->hwErrorMask & EUART_HW_ERR_MASK_PARITY) == EUART_HW_ERR_MASK_PARITY)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_PARITY] != NULL ))
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_PARITY])(__pProto);
		//---- OVERFLOW TX BUF -----
		if(( (__pProto->pUart->hwErrorMask & EUART_HW_ERR_MASK_TX_BUF_OVERFLOW) == EUART_HW_ERR_MASK_TX_BUF_OVERFLOW)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_TX_BUF_OVERFLOW] != NULL ))
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_TX_BUF_OVERFLOW])(__pProto);
		//---- OVERFLOW RX BUF -----
		if(( (__pProto->pUart->hwErrorMask & EUART_HW_ERR_MASK_RX_BUF_OVERFLOW) == EUART_HW_ERR_MASK_RX_BUF_OVERFLOW)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_RX_BUF_OVERFLOW] != NULL ))
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_HW_RX_BUF_OVERFLOW])(__pProto);

		//--- Reset uart hw error ---
		__pProto->pUart->hwErrorMask = EUART_HW_ERR_MASK_NONE;
	}

	/**
	 * Protocol's errors managing
	 */
	if(__pProto->error.mask != ECOMPROTO_ERR_MASK_NONE)
	{
		__pProto->error.cntTotal++;
		__pProto->cntFail++;
		//---- UNKNOWN OPCODE ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_UNKNOWN_OPCODE) == ECOMPROTO_ERR_MASK_UNKNOWN_OPCODE)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_UNKNOWN_OPCODE] != NULL ))
		{
			__pProto->error.cntWrongOpCode++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_UNKNOWN_OPCODE])(__pProto);
		}
		//---- WRONG STX ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_STX_NOT_RECEIVED) == ECOMPROTO_ERR_MASK_STX_NOT_RECEIVED)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_STX_NOT_RECEIVED] != NULL ))
		{
			__pProto->error.cntBadStx++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_STX_NOT_RECEIVED])(__pProto);
		}
		//---- WRONG ETX ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_ETX_NOT_RECEIVED) == ECOMPROTO_ERR_MASK_ETX_NOT_RECEIVED)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_ETX_NOT_RECEIVED] != NULL ))
		{
			__pProto->error.cntBadEtx++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_ETX_NOT_RECEIVED])(__pProto);
		}
		//---- TIMEOUT RX ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_TIMEOUT_RX) == ECOMPROTO_ERR_MASK_TIMEOUT_RX)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_TIMEOUT_RX] != NULL ))
		{
			__pProto->error.cntTimeoutRx++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_TIMEOUT_RX])(__pProto);
		}
		//---- TIMEOUT TX ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_TIMEOUT_TX) == ECOMPROTO_ERR_MASK_TIMEOUT_TX)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_TIMEOUT_TX] != NULL ))
		{
			__pProto->error.cntTimeoutTx++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_TIMEOUT_TX])(__pProto);
		}
		//---- CHECKSUM ERR ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_CHECKSUM) == ECOMPROTO_ERR_MASK_CHECKSUM)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_CHECKSUM] != NULL ))
		{
			__pProto->error.cntBadCrc++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_CHECKSUM])(__pProto);
		}
		//---- DATA BUF OVERFLOW ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_DATA_BUF_OVERFLOW) == ECOMPROTO_ERR_MASK_DATA_BUF_OVERFLOW)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_DATA_BUF_OVERFLOW] != NULL ))
		{
			__pProto->error.cntDataBufOverflow++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_DATA_BUF_OVERFLOW])(__pProto);
		}
		//---- ENCODING ERR ----
		if(( (__pProto->error.mask & ECOMPROTO_ERR_MASK_ENCODING) == ECOMPROTO_ERR_MASK_ENCODING)&&
			 (g_fpErrorsHandlers[ECOMPROTO_ERR_ID_ENCODING] != NULL ))
		{
			__pProto->error.cntBadEncoding++;
			(*g_fpErrorsHandlers[ECOMPROTO_ERR_ID_ENCODING])(__pProto);
		}

		//---- Reset protocol's errors ---
		__pProto->error.mask = ECOMPROTO_ERR_MASK_NONE;
	}

	/**
	 * Command managing
	 */
	if(__pProto->validCommand)
	{
		__pProto->cntComRx++;
		if(g_fpCommandHadler != NULL)
			(*g_fpCommandHadler)(__pProto);
		__pProto->validCommand = 0;
	}
}

bool comproto_sendCommand(	comProto_t *__pProto,
							uint8_t __masterId,
							uint8_t __slaveId,
							uint8_t __msgCount,
							uint8_t __command,
							const uint8_t *__pdata, uint16_t __dataSize,
							uint16_t __timeout)
{
int i;
uint16_t chkCalc = 0;
uint16_t packSize = __dataSize + 1;						// aggiungo 1 xchè in datasize non è compreso il comando
register const uint8_t *__pcpy = __pdata;

	uart_putc(__pProto->pUart, CHAR_STX);			// dentro la sequenza di STX
	uart_putc(__pProto->pUart, CHAR_STX);

    chkCalc = 0xFFFF;

    chkCalc += comproto_putAndPad(__pProto->pUart, __masterId);
    chkCalc += comproto_putAndPad(__pProto->pUart, __slaveId);
    chkCalc += comproto_putAndPad(__pProto->pUart, __msgCount);
    chkCalc += comproto_putAndPad(__pProto->pUart, packSize);
    chkCalc += comproto_putAndPad(__pProto->pUart, packSize >> 8);
    chkCalc += comproto_putAndPad(__pProto->pUart, __command);	// comando da spedire...uguale a quello ricevuto ??

	__pcpy = __pdata;

    for(i=0; i < __dataSize; i++)				// calcolo chk dati
        chkCalc += comproto_putAndPad(__pProto->pUart, *__pcpy++);

    comproto_putAndPad(__pProto->pUart, chkCalc & 0xFF);	// infila il chk-lo calcolato
    comproto_putAndPad(__pProto->pUart, chkCalc >> 8);	// infila il chk-hi calcolato

	uart_putc(__pProto->pUart, CHAR_STX);					// sequenze STX+ETX: fine del pacco
	uart_putc(__pProto->pUart, CHAR_ETX);

	uart_startTx(__pProto->pUart, 0);						// via libera al pacco

	__pProto->cntComTx++;

	if(__timeout != 0)
	{
		__pProto->timeoutTx = __timeout;
		preset(__pProto->timChkTx, __pProto->timeoutTx);
		start(__pProto->timChkTx);
	}
    return(True);
}

bool comproto_getRxData(comProto_t *__pProto, uint8_t *__pData, uint16_t __len)
{
	if(ringBuffer_used_isr(&__pProto->rxCmdInfo.data) < __len)
		return False;

	ringBuffer_read_isr(&__pProto->rxCmdInfo.data, __pData, __len);
	return True;
}

void comproto_incMsgCount(comProto_t *__pProto)
{
	__pProto->msgCount++;
}
void comproto_setMsgCount(comProto_t *__pProto, uint8_t __count)
{
	__pProto->msgCount = __count;
}

void comproto_rstMsgCount(comProto_t *__pProto)
{
	__pProto->msgCount = 0;
}

void comproto_setRxTimer(comProto_t *__pProto,uint32_t __milli)
{
	__pProto->timeoutRx = __milli;
}

void comproto_setTxTimer(comProto_t *__pProto,uint32_t __milli)
{
	__pProto->timeoutTx = __milli;
}

void comproto_setDecodeMaxWhileCycles(comProto_t *__pProto, __uint16_t __cycles)
{
	if(__cycles < 1)
		__cycles = 1;
	__pProto->maxWhileCycles = __cycles;
}

uint8_t comproto_putAndPad(uart_t *__pUart, uint8_t __c)
{
	if( __c == 0x02 )						// zero padding per i dati
	{
		uart_putc(__pUart, __c);
		uart_putc(__pUart, 0x00);			// zero pad inserito !!
		return( __c );
	}

	uart_putc(__pUart, __c);
	return( __c );
}

uint8_t comproto_setMessageType(comProto_t *__pProto, uint8_t __c)
{
	__pProto->msgRejected = __c;
	return 0;
}


void comproto_decode(comProto_t *__pProto)
{
	bool exitWhile = False;
	uint16_t whileCycles = 0;
	uart_t *pUart;
	uint8_t uartData;
	bool validOpcode = True;

	pUart = __pProto->pUart;
	while(	(uart_getc(pUart, &uartData) == 0) &&
			(!exitWhile) &&
			(whileCycles < __pProto->maxWhileCycles))
	{
		switch( __pProto->rxState )
		{
			case ECOMPROTO_RX_STATE_WAIT_FIRST_STX:
				if (uartData == CHAR_STX)
				{
					stop(__pProto->timChkTx);
					preset(__pProto->timChkRx, __pProto->timeoutRx);
					start(__pProto->timChkRx);

					__pProto->validCommand = 0;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_SECOND_STX;
					__pProto->waitingForEncodingByte = False;
					ringBuffer_clear_isr(&__pProto->rxCmdInfo.data);
				}else
				{
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_STX_NOT_RECEIVED;
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_SECOND_STX:
				if (uartData == CHAR_STX)
				{
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_MASTER_ID;
				}else
				{
					stop(__pProto->timChkRx);
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_STX_NOT_RECEIVED;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_MASTER_ID:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_SLAVE_ID;
					//checksum calculation start here.
					__pProto->rxCmdInfo.temporaryCheckSum = 0xFFFF;
					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
					__pProto->rxCmdInfo.masterId = uartData;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_SLAVE_ID:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_NUM_PACKET;
					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
					__pProto->rxCmdInfo.slaveId = uartData;
					if (uartData == __pProto->slaveId){
                        __pProto->msgRejected = MSG_ACCEPTED;
					}else{
                        __pProto->msgRejected  = MSG_REJECTED;
					}
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;


			case ECOMPROTO_RX_STATE_WAIT_NUM_PACKET:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_DATA_LEN_LSB;
					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
					__pProto->rxCmdInfo.msgCount = uartData;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_DATA_LEN_LSB:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_DATA_LEN_MSB;
					__pProto->rxCmdInfo.dataLen = (uint16_t)uartData;
					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;
			case ECOMPROTO_RX_STATE_WAIT_DATA_LEN_MSB:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxCmdInfo.dataLen += (((uint16_t)uartData) <<8 );

					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_OPCODE;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
			break;

			case ECOMPROTO_RX_STATE_WAIT_OPCODE:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					if(g_fpCheckOpcode != NULL)
						validOpcode = (*g_fpCheckOpcode)(uartData);

					if(validOpcode)
					{
						//resettiamo il contatore dei uint8_t per verificare il conteggio dei dati che arrivao
						//se CMD prevede dati
						__pProto->rxCmdInfo.numDataRec = 0;

						__pProto->rxCmdInfo.opCode = (int)uartData;
						if (__pProto->rxCmdInfo.dataLen > 1)
							__pProto->rxState = ECOMPROTO_RX_STATE_RECEIVING_DATA;
						else
							__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_CRC_LSB;

						__pProto->rxCmdInfo.dataLen--;

						__pProto->rxCmdInfo.temporaryCheckSum += uartData;
					}else
					{
						__pProto->error.mask |= ECOMPROTO_ERR_MASK_UNKNOWN_OPCODE;
						__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
						stop(__pProto->timChkRx);
					}
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;

			case ECOMPROTO_RX_STATE_RECEIVING_DATA:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}

					__pProto->rxCmdInfo.numDataRec++;

					if (ringBuffer_isfull_isr(&__pProto->rxCmdInfo.data))
					{
						__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
						stop(__pProto->timChkRx);
					}else
					{
						ringBuffer_put_isr(&__pProto->rxCmdInfo.data, &uartData);
					}

					if (__pProto->rxCmdInfo.numDataRec >=__pProto->rxCmdInfo.dataLen)
					{
						__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_CRC_LSB;
					}

					__pProto->rxCmdInfo.temporaryCheckSum += uartData;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_CRC_LSB:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxCmdInfo.checkSumExpected = (uint16_t)uartData;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_CRC_MSB;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}

				break;

			case ECOMPROTO_RX_STATE_WAIT_CRC_MSB:
				if((!__pProto->waitingForEncodingByte && uartData != CHAR_STX) ||
				   (__pProto->waitingForEncodingByte && uartData == 0x00))
				{
					if(__pProto->waitingForEncodingByte)
					{
						uartData = CHAR_STX;
						__pProto->waitingForEncodingByte = False;
					}
					__pProto->rxCmdInfo.checkSumExpected += (((uint16_t)uartData) << 8);
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_EXT;
				}else if(uartData == CHAR_STX && ! __pProto->waitingForEncodingByte)
				{
					__pProto->waitingForEncodingByte = True;
				}else //encoding error...
				{
					__pProto->waitingForEncodingByte = False;
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ENCODING;
					stop(__pProto->timChkRx);
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_FIRST_EXT:
				if (uartData == CHAR_STX)
				{
					__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_SECOND_ETX;
				}
				else
				{
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ETX_NOT_RECEIVED;
				}
				break;

			case ECOMPROTO_RX_STATE_WAIT_SECOND_ETX:
				__pProto->rxState = ECOMPROTO_RX_STATE_WAIT_FIRST_STX;
				stop(__pProto->timChkRx);
				exitWhile = True;

				if (uartData == CHAR_ETX)
				{
					if (__pProto->rxCmdInfo.checkSumExpected == __pProto->rxCmdInfo.temporaryCheckSum)
					{
						if (__pProto->msgRejected == MSG_ACCEPTED){
                            __pProto->validCommand = 1;
                            }else{
                                __pProto->validCommand = 0;
                            }
					}
					else
					{
						__pProto->error.mask |= ECOMPROTO_ERR_MASK_CHECKSUM;
					}
				}
				else
				{
					__pProto->error.mask |= ECOMPROTO_ERR_MASK_ETX_NOT_RECEIVED;
				}
				break;

			default:
				//!!! Fatal Error !!!
				break;
		} //end switch (__pProto->rxState)

		whileCycles++;
	}//end while
}



