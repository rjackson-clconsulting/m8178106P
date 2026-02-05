/**
@file		COMPROTO.h
@brief		Routines to manage (decode-encode) serial protocol
			Starting project SCOMPROTO.CPP (COMETA).
			Modified code syntax according to the new Medica standard

@author		Nicola/Enrico/Fernando
@date		09/01/2018
@version	01.00.01
Notes on version 01.00.01
    Add SLAVE_ID_PROTECTIVE macro (and others)

*/

#ifndef _COMPROTO_H
#define _COMPROTO_H

#include "UART_FM3.h"
#include "swtimer.h"
#include "board_define.h"

// Note relative alla struttura dei pacchetti convenzionata per il protocollo
/*
STRUTTURA DELLA STRINGA DI TRASMISSIONE (ancora da codificare)
        E DELLA STRINGA DI RICEZIONE    (gia' decodificata)

A       STX         0x02
C       MA_NO_ID    0-64 numero stazione di chi chiama (master)
E       SL_NO_ID    0-16  numero della stazione chiamata (slave)
                          la stazione risponde solo al suo e allo '0'
F       COM_NO      0-255 numero progressivo della comunicazione
G       LUNG_TOT H  0-1   lunghezza uint8_t L
G       LUNG_TOT L  0-255 lunghezza uint8_t H  (lunghezza dei uint8_ts: H + I)
H       COMANDO     0-255 comando da eseguire
I       PARAMETRI   0-255 coda dei parametri
I          "         "           "
I          "         "           "
I       PARAMETRI   0-255        "
J       CHK_SUM  H  0-255 somma di controllo uint8_t L
J       CHK_SUM  L  0-255 somma di controllo uint8_t H (0xffff+B+C+D+E+F+G+H+I)
K       ETX         0x03


COME CODIFICARE / DECODIFICARE LA STRINGA :

STX     0x02, 0x02  viene indicato dalla sequenza dei due uint8_ts [UNIVOCA]

ETX     0x02, 0x03  viene indicato dalla sequenza dei due uint8_ts [UNIVOCA]

DATO 2  0x02, 0x00  viene indicato dalla sequenza dei due uint8_ts [UNIVOCA]
0x02

DATO                viene indicato dal singolo uint8_t [UNIVOCO]
0 - 1   0 - 1
3 - 255 3 - 255


QUINDI :

La sequenza 0x02, 0x02 indica l' STX.
La sequenza 0x02, 0x03 indica l' ETX.
La sequenza 0x02, 0x00 indica il DATO "2".
I numeri 0, 1, 3 ... 255 non vengono alterati.
*/


#define CHAR_STX	0x02
#define CHAR_ETX	0x03

#define MSG_REJECTED    1
#define MSG_ACCEPTED    2
#define MSG_UNDEFINED   0

#define SLAVE_ID_CONTROLLER     0x10
#define SLAVE_ID_PROTECTIVE     0x11
#define SLAVE_THIS_DEVICE       SLAVE_ID_PROTECTIVE


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//
/**
 * Stati del protocollo seriale
 */
enum comproto_rx_states_t{
	ECOMPROTO_RX_STATE_WAIT_FIRST_STX = 0,
	ECOMPROTO_RX_STATE_WAIT_SECOND_STX,
	ECOMPROTO_RX_STATE_WAIT_MASTER_ID,
	ECOMPROTO_RX_STATE_WAIT_SLAVE_ID,
	ECOMPROTO_RX_STATE_WAIT_NUM_PACKET,
	ECOMPROTO_RX_STATE_WAIT_DATA_LEN_LSB,
	ECOMPROTO_RX_STATE_WAIT_DATA_LEN_MSB,
	ECOMPROTO_RX_STATE_WAIT_OPCODE,
	ECOMPROTO_RX_STATE_RECEIVING_DATA,
	ECOMPROTO_RX_STATE_WAIT_CRC_LSB,
	ECOMPROTO_RX_STATE_WAIT_CRC_MSB,
	ECOMPROTO_RX_STATE_WAIT_FIRST_EXT,
	ECOMPROTO_RX_STATE_WAIT_SECOND_ETX,
	ECOMPROTO_RX_STATE_NUM
};
#ifdef __cplusplus
#else
typedef enum comproto_rx_states_t  comproto_rx_states_t;
#endif

/**
 * comproto errors' masks
 */
enum comproto_errors_masks_t
{
	ECOMPROTO_ERR_MASK_NONE 				= 0,
	ECOMPROTO_ERR_MASK_UNKNOWN_OPCODE		= BIT0,		/**< Unknown opcode*/
	ECOMPROTO_ERR_MASK_STX_NOT_RECEIVED	 	= BIT1,		/**< stx not recieved*/
	ECOMPROTO_ERR_MASK_ETX_NOT_RECEIVED 	= BIT2,		/**< etx not received*/
	ECOMPROTO_ERR_MASK_TIMEOUT_RX			= BIT3,		/**< Timeout RX*/
	ECOMPROTO_ERR_MASK_TIMEOUT_TX			= BIT4,		/**< Timeout TX*/
	ECOMPROTO_ERR_MASK_CHECKSUM				= BIT5,		/**< checksum error*/
	ECOMPROTO_ERR_MASK_DATA_BUF_OVERFLOW	= BIT6,		/**< data received buffer overflow*/
	ECOMPROTO_ERR_MASK_ENCODING				= BIT7,
};

/**
 * Comproto errors' IDs (uart hw error also included)
 */
enum comproto_errors_id_t
{
	ECOMPROTO_ERR_ID_NONE = 0,
	ECOMPROTO_ERR_ID_HW_OVERRUN,
	ECOMPROTO_ERR_ID_HW_FRAME,
	ECOMPROTO_ERR_ID_PARITY,
	ECOMPROTO_ERR_ID_HW_TX_BUF_OVERFLOW,
	ECOMPROTO_ERR_ID_HW_RX_BUF_OVERFLOW,

	ECOMPROTO_ERR_ID_UNKNOWN_OPCODE,		/**< Unknown opcode*/
	ECOMPROTO_ERR_ID_STX_NOT_RECEIVED,		/**< stx not recieved*/
	ECOMPROTO_ERR_ID_ETX_NOT_RECEIVED,		/**< etx not received*/
	ECOMPROTO_ERR_ID_TIMEOUT_RX,		/**< Timeout RX*/
	ECOMPROTO_ERR_ID_TIMEOUT_TX,		/**< Timeout TX*/
	ECOMPROTO_ERR_ID_CHECKSUM	,		/**< checksum error*/
	ECOMPROTO_ERR_ID_DATA_BUF_OVERFLOW,		/**< data received buffer overflow*/
	ECOMPROTO_ERR_ID_ENCODING,

	ECOMPROTO_ERR_ID_NUM
};

/**
 * Command header and data struct
 */
struct command_t{
	uint8_t 		masterId;			// id master in ricezione
	uint8_t 		slaveId;			// id slave in ricezione
	uint8_t 		msgCount;			// numero messaggio in ricezione
	uint8_t 		opCode;				// comando in ricezione
	uint16_t 		dataLen;          	// contatore per decodifica
	uint16_t 		temporaryCheckSum;
	uint16_t		checkSumExpected;
	uint16_t        numDataRec;
	ringBuffer_t 	data;				// dati comando
};
#ifdef __cplusplus
#else
typedef struct command_t  command_t;
#endif

/**
 * Errors and their counters struct
 */
struct comproto_err_cnt_t
{
	uint16_t            mask;

	uint16_t 			cntTotal;
	uint16_t 			cntWrongOpCode;
	uint16_t 			cntBadStx;
	uint16_t 			cntBadEtx;
	uint16_t  			cntTimeoutRx;
	uint16_t  			cntTimeoutTx;
	uint16_t  			cntBadCrc;
	uint16_t  			cntDataBufOverflow;
	uint16_t  			cntBadEncoding;

	uint16_t  			cntUartHwError;		// contatore errori di frame

};
#ifdef __cplusplus
#else
typedef struct comproto_err_cnt_t  comproto_err_cnt_t;
#endif

struct comProto_t{
	uart_t 				*pUart;
	command_t 			rxCmdInfo;				// bufferino temporneo per estrazione dati in ricezione

	uint8_t 			validCommand;
	comproto_rx_states_t rxState;       		// stato esecuzione gestore protocollo seriale
	uint8_t          	waitingForEncodingByte; //

	uint8_t 			masterId;			// id master in ricezione
	uint8_t 			slaveId;			// id slave in ricezione

	comproto_err_cnt_t error;				// errors and error's counters

	uint16_t        	cntComTx;           // contatore numero di messaggi inviati
	uint16_t        	cntComRx;           // contatore numero di messaggi ricevuti
	uint16_t         	cntFail;            // contatore comunicazioni fallite



	decTimer_t    		timChkTx;       	// timer controllo trasmissione pacco
	decTimer_t    		timChkRx;       	// timer controllo timeout ricezione pacco

	uint16_t        	timeoutTx;             // attesa ricezione pacco (in ms)
	uint16_t        	timeoutRx;             // attesa ricezione pacco (in ms)

	uint16_t 			maxWhileCycles;		// numero massimo di cicli while eseguiti dentro la comproto_decode
											// !!! NON PUO' VALERE ZERO !!!
	uint8_t 			msgCount;

	uint8_t             msgRejected;
};
#ifdef __cplusplus
#else
typedef struct comProto_t  comProto_t;
#endif


//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif


void comproto_init(	comProto_t *__pProto,
					uart_t *__pUart,
					uint16_t __rxProtoDataBufLength,
					uint8_t __masterId, uint8_t __slaveId,
					bool (*__fpCheckOpcode)(const uint8_t __opCode),
					void (*__fpCommandHadler)(const comProto_t *__p)) 				__attribute__ (( section(".ramcode") ));

void comproto_static_init(	comProto_t *__pProto,
							uart_t *__pUart,
							uint8_t *__pRxProtoDataBuf,
							uint16_t __rxProtoDataBufLength,
							uint8_t __masterId, uint8_t __slaveId,
							bool (*__fpCheckOpcode)(const uint8_t __opCode),
							void (*__fpCommandHadler)(const comProto_t *__p)) 		__attribute__ (( section(".ramcode") ));

int comproto_error_handler_callback(comProto_t *__pProto,
									uint16_t __id,
									void (*__fpErrorHandler)(const comProto_t *__p)) __attribute__ (( section(".ramcode") ));

void comproto_manager(comProto_t *__pProto)											 __attribute__ (( section(".ramcode") ));

bool comproto_sendCommand(	comProto_t *__pProto,
							uint8_t __masterId,
							uint8_t __slaveId,
							uint8_t __msgCount,
							uint8_t __command,
							const uint8_t *__pdata, uint16_t __dataSize,
							uint16_t __timeout) 									__attribute__ (( section(".ramcode") ));

bool comproto_getRxData(comProto_t *__pProto, uint8_t *__pData, uint16_t __len) 	__attribute__ (( section(".ramcode") ));

void comproto_incMsgCount(comProto_t *__pProto)					 					__attribute__ (( section(".ramcode") ));
void comproto_setMsgCount(comProto_t *__pProto, uint8_t __count) 					__attribute__ (( section(".ramcode") ));
void comproto_rstMsgCount(comProto_t *__pProto) 									__attribute__ (( section(".ramcode") ));

void comproto_setRxTimer(comProto_t *__pProto, uint32_t _milli)						__attribute__ (( section(".ramcode") ));
void comproto_setTxTimer(comProto_t *__pProto, uint32_t _milli)						__attribute__ (( section(".ramcode") ));

void comproto_setDecodeMaxWhileCycles(comProto_t *__pProto, __uint16_t __cycles) 	__attribute__ (( section(".ramcode") ));
uint8_t comproto_setMessageType(comProto_t *__pProto, uint8_t __c)                  __attribute__ (( section(".ramcode") ));
#ifdef __cplusplus
}

#endif


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//



#endif



