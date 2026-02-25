/**
@file		COMPROTO_LOADER.c
@brief		Routines to manage (decode-encode) loader serial protocol

@author		Fernando Morani (from Nicola)
@date		05/03/2018
@version	01.00.02
@ NOTE: tnoergaard 10/17/2025 sonar qube updates
Notes on version 01.00.01
    Add NVIC_SystemReset(); when jumping to Loader or Application
Notes on version 01.00.01
    Add support for store STAY_IN_LOADER signature in SRAM
*/

#include "COMPROTO_LOADER.h"

#ifndef SONAR_QUBE_EXCLUDE
#include <stdint.h>
#endif

void (*g_fpJumpToLoader)(void);

/***
    DEBUG STATIC VAR
**/
#ifdef LOADER_ENGINE_INT
uint32_t debugVar_CharRxFromController = 0; /*Debug var*/
uint32_t debugVar_UnExpectedBranch = 0; /*Debug var*/
uint32_t debugVar_CharTxToController = 0; /*Debug Var*/
#endif

/**
 * Response packet's data
 */
uint8_t	g_responsePacketData[200] = {0};

/**
 * Received data packet's data
 */
uint8_t	g_receivedData[1200] = {0};
/**
 * Application firmware's status. So the UI understand that we are in loader
 */
CRRT_working_status_t g_appFirmwareStatus;

/**
 * Ram copy of the loader flash end address
 */

#ifdef SONAR_QUBE_EXCLUDE
uint32_t g_loaderFlashEndAdd;
#else
uintptr_t g_loaderFlashEndAdd = 0;
#endif

/**
Ram Location to be used for STAY_IN_LOADER key
**/
uint32_t *g_JumpingCodeMemLocation = NULL;


/**Waiting Timer*/
decTimer_t pollerWaiter = {0,0};

extern comProto_t g_protoLoader;
extern uint32_t _etext;
/**
 * Reset Handler reset
 */
extern void ResetHandler();
extern void (*g_fpJumpToLoader)(void);  //FERLOADER
//----------------------------------------------------------------
//------------- STATIC FUNCTIONS ---------------------------------
//----------------------------------------------------------------
static void loader_proto_callbakSet(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));

static void loader_proto_nack(const comProto_t *__pProto)			__attribute__ (( section(".ramcode") ));
static void loader_proto_ack(const comProto_t *__pProto)			__attribute__ (( section(".ramcode") ));
static void loader_proto_read_status(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));
static void loader_proto_chkStatus(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_info(const comProto_t *__pProto)			__attribute__ (( section(".ramcode") ));
static void loader_proto_loaded_app_info(const comProto_t *__pProto)__attribute__ (( section(".ramcode") ));
static void loader_proto_flash_init(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_flash_unInit(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));
static void loader_proto_blankCheck(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_chipErase(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_writeBlock(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_readBlock(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_verifyBlock(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));
static void loader_proto_blockErase(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_reWriteLoader(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));
static void loader_proto_jmpToApp(const comProto_t *__pProto)		__attribute__ (( section(".ramcode") ));
static void loader_proto_jmpToLoader(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));

static void loader_proto_blinkaLed(void) 							__attribute__ (( section(".ramcode") ));

static int copyReceivedData(const ringBuffer_t *__pRingBuf,
							uint8_t *_pData, uint32_t __size) 		__attribute__ (( section(".ramcode") ));

/**
 * Callback functions
 */
bool callback_checkOpcode(const uint8_t __opCode)				__attribute__ (( section(".ramcode") ));
void callback_commandHadler(const comProto_t *__pProto)			__attribute__ (( section(".ramcode") ));
void call_back_errorHandler_uartHw(const comProto_t *__pProto)	__attribute__ (( section(".ramcode") ));
void call_back_errorHandler_protocol(const comProto_t *__pProto)__attribute__ (( section(".ramcode") ));

void loader_proto_init(	const comProto_t *__pProto,
						uart_t *__pUart,
						uint16_t __rxProtoDataBufLength,
						uint8_t __masterId, uint8_t __slaveId)
{
	uint16_t i = 0;
	comproto_init(	(comProto_t*)__pProto,
					__pUart,
					__rxProtoDataBufLength,
					__masterId, __slaveId,
					&callback_checkOpcode,
					&callback_commandHadler);

	loader_proto_callbakSet(__pProto);

	comproto_setRxTimer((comProto_t*)__pProto, 500);
	comproto_setDecodeMaxWhileCycles((comProto_t*)__pProto, 1);

	for(i = 0; i < sizeof(CRRT_working_status_t); i++)
		*((uint8_t*)(&g_appFirmwareStatus)) = 0;

	g_appFirmwareStatus.phase = EPHASE_loader;
#ifdef SONAR_QUBE_EXCLUDE
	g_loaderFlashEndAdd = (uint32_t)&_etext;
#else
	g_loaderFlashEndAdd = (uintptr_t)&_etext;
#endif
}

void loader_proto_static_init(	const comProto_t *__pProto,
								uart_t *__pUart,
								uint8_t *__pRxProtoDataBuf,
								uint16_t __rxProtoDataBufLength,
								uint8_t __masterId, uint8_t __slaveId)
{
	uint16_t i = 0;

	preset(pollerWaiter, 2);

	comproto_static_init(	(comProto_t*)__pProto,
							__pUart,
							__pRxProtoDataBuf,
							__rxProtoDataBufLength,
							__masterId, __slaveId,
							&callback_checkOpcode,
							&callback_commandHadler);

	loader_proto_callbakSet(__pProto);

	comproto_setRxTimer((comProto_t*)__pProto, 500);
	comproto_setDecodeMaxWhileCycles((comProto_t*)__pProto, 1);

	for(i = 0; i < sizeof(CRRT_working_status_t); i++)
			*((uint8_t*)(&g_appFirmwareStatus)) = 0;

	g_appFirmwareStatus.phase = EPHASE_loader;

#ifdef SONAR_QUBE_EXCLUDE
	g_loaderFlashEndAdd = (uint32_t)&_etext;
#else
	g_loaderFlashEndAdd = (uintptr_t)&_etext;
#endif


	g_fpJumpToLoader = (void (*)()) LOADER_JUMP_ADDRESS;
	g_JumpingCodeMemLocation = (uint32_t *)SRAM_1_STRING_ADDR;

}

static void loader_proto_callbakSet(const comProto_t *__pProto)
{
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_HW_OVERRUN, &call_back_errorHandler_uartHw);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_HW_FRAME, &call_back_errorHandler_uartHw);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_PARITY, &call_back_errorHandler_uartHw);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_HW_TX_BUF_OVERFLOW, &call_back_errorHandler_uartHw);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_HW_RX_BUF_OVERFLOW, &call_back_errorHandler_uartHw);

	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_UNKNOWN_OPCODE, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_STX_NOT_RECEIVED, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_ETX_NOT_RECEIVED, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_TIMEOUT_RX, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_TIMEOUT_TX, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_CHECKSUM, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_DATA_BUF_OVERFLOW, &call_back_errorHandler_protocol);
	comproto_error_handler_callback((comProto_t*)__pProto, ECOMPROTO_ERR_ID_ENCODING, &call_back_errorHandler_protocol);
}

/**
 * Command handler callback function
 */
void callback_commandHadler(const comProto_t *__pProto)
{

    switch( __pProto->rxCmdInfo.opCode )     /* Risposta a Comando */
    {
        case EPROTO_LOADER_CMD_ACK:
			loader_proto_ack(__pProto);   			/* Richiesta di Comunicazione */
			break;
        case EPROTO_LOADER_CMD_READ_STATUS:
        	loader_proto_read_status(__pProto);
        	break;

		case EPROTO_LOADER_CMD_CHK_STATUS:   //Command Get Status in the CM3
			loader_proto_chkStatus(__pProto);	/* Richiesta di Stato maggiorata */
			break;
		case EPROTO_LOADER_CMD_INFO:  //Command Get Loader Info
			loader_proto_info(__pProto);
			break;
		case EPROTO_LOADER_CMD_LOADED_APP_INFO:  //Command Get Loaded App Info in the CM3
			loader_proto_loaded_app_info(__pProto);
			break;
                                    /* comandi specialibreak; per prog. micro*/
		case EPROTO_LOADER_CMD_INIT:
			loader_proto_flash_init(__pProto);
			break;
		case EPROTO_LOADER_CMD_UNINIT:
			loader_proto_flash_unInit(__pProto);
			break;
		case EPROTO_LOADER_CMD_BLKCHK:
        	loader_proto_blankCheck(__pProto);
        	break;
        case EPROTO_LOADER_CMD_CHIPERASE:
        	loader_proto_chipErase(__pProto);
        	break;
        case EPROTO_LOADER_CMD_WRITEBLOCK:
        	loader_proto_writeBlock(__pProto);
        	break;
        case EPROTO_LOADER_CMD_READBLOCK:
        	loader_proto_readBlock(__pProto);
        	break;
        case EPROTO_LOADER_CMD_VERIFBLOCK:
        	loader_proto_verifyBlock(__pProto);
        	break;
        case EPROTO_LOADER_CMD_BLOCKERASE:
        	loader_proto_blockErase(__pProto);
        	break;
        case EPROTO_LOADER_CMD_REWRLOADER:
        	loader_proto_reWriteLoader(__pProto);
        	break;
        case EPROTO_LOADER_CMD_JMPTOAPP:
        	loader_proto_jmpToApp(__pProto);
        	break;
		case EPROTO_LOADER_CMD_JMPTOLOAD:
			loader_proto_jmpToLoader(__pProto);
			break;
        default:					/* Comando Sconosciuto o non gestito */
        	loader_proto_nack(__pProto);
          break;
    }

}

/**
 * Check opcode handler callback function
 */
bool callback_checkOpcode(const uint8_t __opCode)
{
	if (__opCode != EPROTO_LOADER_CMD_ACK)
	if (__opCode != EPROTO_LOADER_CMD_READ_STATUS)
	if (__opCode != EPROTO_LOADER_CMD_CHK_STATUS)
	if (__opCode != EPROTO_LOADER_CMD_INFO)
	if (__opCode != EPROTO_LOADER_CMD_LOADED_APP_INFO)
	if (__opCode != EPROTO_LOADER_CMD_INIT)
	if (__opCode != EPROTO_LOADER_CMD_UNINIT)
	if (__opCode != EPROTO_LOADER_CMD_BLKCHK)
	if (__opCode != EPROTO_LOADER_CMD_CHIPERASE)
	if (__opCode != EPROTO_LOADER_CMD_WRITEBLOCK)
	if (__opCode != EPROTO_LOADER_CMD_READBLOCK)
	if (__opCode != EPROTO_LOADER_CMD_VERIFBLOCK)
	if (__opCode != EPROTO_LOADER_CMD_BLOCKERASE)
	if (__opCode != EPROTO_LOADER_CMD_REWRLOADER)
	if (__opCode != EPROTO_LOADER_CMD_JMPTOAPP)
	if (__opCode != EPROTO_LOADER_CMD_JMPTOLOAD)
		return False;

	return True;
}

/**
 * Uart hardware errors handler callback function
 */
void call_back_errorHandler_uartHw(const comProto_t *__pProto)
{

}

/**
 * Protocol errors handler callback function
 */
void call_back_errorHandler_protocol(const comProto_t *__pProto)
{
	loader_proto_nack(__pProto);
}

/**
 * Command NOT acknowledge
 */
static void loader_proto_nack(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;

	if (__pProto->msgRejected == MSG_ACCEPTED){
        /**Send a command ony if it is for me*/
        comproto_sendCommand(	(comProto_t *)__pProto,
                                __pProto->rxCmdInfo.masterId,
                                __pProto->slaveId,
                                __pProto->rxCmdInfo.msgCount,
                                EPROTO_LOADER_CMD_ACK,
                                g_responsePacketData, 1,
                                0);
    }/**else{
        /*Nothingh to do
    }*/
}
/**
 * Ack command handler
 */
static void loader_proto_ack(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_OK;

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_ACK,
							g_responsePacketData,
							1,
							0);
}

/**
 * Answer to read status command
 */
static void loader_proto_read_status(const comProto_t *__pProto)
{
	g_appFirmwareStatus.phase = EPHASE_loader;

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_READ_STATUS,
							((uint8_t*)(&g_appFirmwareStatus)),
							CMD_READ_STATUS_TX_PAR_SLAVE,
							0);
}
/* Ritorna lo state corrente del sistema
*/
static void loader_proto_chkStatus(const comProto_t *__pProto)
{
	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_CHK_STATUS,
							((uint8_t*)(&g_loaderStatus)),
							CMD_LOADER_READ_STATUS_TX_PAR_SLAVE,
							0);
}

static void loader_proto_info(const comProto_t *__pProto)
{
	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_INFO,
							((uint8_t*)(&g_appInfo)),
							CMD_LOADER_INFO_TX_PAR_SLAVE,
							0);
}

static void loader_proto_loaded_app_info(const comProto_t *__pProto)
{
	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_LOADED_APP_INFO,
							((uint8_t*)((application_info_t*)(APPLICATION_INFO_TABLE_ADD))),
							CMD_LOADED_APP_INFO_TX_PAR_SLAVE,
							0);
}

static void loader_proto_flash_init(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_FLASH_INIT_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t baseAddr = *((uint32_t*)(&g_receivedData[0]));
			uint32_t clk = *((uint32_t*)(&g_receivedData[4]));
			uint32_t operateFuc = *((uint32_t*)(&g_receivedData[8]));
			g_responsePacketData[0] = FlashInit(baseAddr, clk, operateFuc);
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_INIT,
							g_responsePacketData,
							CMD_LOADER_FLASH_INIT_TX_PAR_SLAVE,
							0);
}

static void loader_proto_flash_unInit(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_FLASH_UNINIT_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t operateFuc = *((uint32_t*)(&g_receivedData[0]));
			g_responsePacketData[0] = FlashUnInit(operateFuc);
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_UNINIT,
							g_responsePacketData,
							CMD_LOADER_FLASH_UNINIT_TX_PAR_SLAVE,
							0);
}

static void loader_proto_blankCheck(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_BLANK_CHECK_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t checkAddr = *((uint32_t*)(&g_receivedData[0]));
			uint32_t size = *((uint32_t*)(&g_receivedData[4]));
			g_responsePacketData[0] = FlashBlankCheck(checkAddr, size);
		}
	}

	comproto_sendCommand(	(comProto_t*)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_BLKCHK,
							g_responsePacketData,
							CMD_LOADER_BLANK_CHECK_TX_PAR_SLAVE,
							0);
}

static void loader_proto_chipErase(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_CHIP_ERASE_RX_PAR_SLAVE) // Wrong number of parameters
	{
		g_responsePacketData[0] = TO_DRV_RET_ERROR;
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_ACK,
							g_responsePacketData,
							CMD_LOADER_CHIP_ERASE_TX_PAR_SLAVE,
							0);
}

static void loader_proto_writeBlock(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_WRITE_BLOCK_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t pageAddr = *((uint32_t*)(&g_receivedData[0]));
			uint32_t size = *((uint32_t*)(&g_receivedData[4]));
			if((pageAddr < g_loaderFlashEndAdd) && (g_loaderStatus.loaderReWriteEn != FLASH_OPERATION_PW))
			{
				g_responsePacketData[0] = TO_DRV_RET_ERROR;
			}else
			{
				g_responsePacketData[0] = FlashProgramPage(pageAddr, size, &g_receivedData[8]);
			}
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_WRITEBLOCK,
							g_responsePacketData,
							CMD_LOADER_WRITE_BLOCK_TX_PAR_SLAVE,
							0);
}

static void loader_proto_readBlock(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_READ_BLOCK_RX_PAR_SLAVE) // Wrong number of parameters
	{
		g_responsePacketData[0] = TO_DRV_RET_ERROR;
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_READBLOCK,
							g_responsePacketData,
							CMD_LOADER_READ_BLOCK_TX_PAR_SLAVE,
							0);
}

static void loader_proto_verifyBlock(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_VERIFY_BLOCK_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t verifyAddr = *((uint32_t*)(&g_receivedData[0]));
			uint32_t size = *((uint32_t*)(&g_receivedData[4]));
			g_responsePacketData[0] = FlashVerify(verifyAddr, size, &g_receivedData[8]);
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_VERIFBLOCK,
							g_responsePacketData, CMD_LOADER_VERIFY_BLOCK_TX_PAR_SLAVE,
							0);
}

static void loader_proto_blockErase(const comProto_t *__pProto)
{
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_BLOCK_ERASE_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			uint32_t add = *((uint32_t*)(&g_receivedData[0]));
			if((add < g_loaderFlashEndAdd) && (g_loaderStatus.loaderReWriteEn != FLASH_OPERATION_PW))
			{
				g_responsePacketData[0] = TO_DRV_RET_ERROR;
			}else
			{
				g_responsePacketData[0] = FlashEraseSector(add);
			}
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_BLOCKERASE,
							g_responsePacketData, CMD_LOADER_BLOCK_ERASE_TX_PAR_SLAVE,
							0);
}

static void loader_proto_reWriteLoader(const comProto_t *__pProto)
{
	uint32_t pw = 0;
	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_REWR_LOADER_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			pw = *((uint32_t*)(&g_receivedData[0]));
			if(pw == FLASH_OPERATION_PW)
			{
				g_loaderStatus.loaderReWriteEn = FLASH_OPERATION_PW;
				g_responsePacketData[0] = TO_DRV_RET_OK;
			}else
			{
				g_loaderStatus.loaderReWriteEn = 0;
			}
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_REWRLOADER,
							g_responsePacketData, CMD_LOADER_REWR_LOADER_TX_PAR_SLAVE,
							0);
}

static void loader_proto_jmpToApp(const comProto_t *__pProto)
{
	uint32_t pw = 0;
	void (*fpJump)(void);
	//#define ORA_NON_VOGLIO_TEST
    #ifdef ORA_NON_VOGLIO_TEST
	uint32_t i = 0; //FERDEBUG D
	#endif

	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_JUMP_TO_APP_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			pw = *((uint32_t*)(&g_receivedData[0]));
			if(pw == FLASH_OPERATION_PW)
				g_responsePacketData[0] = TO_DRV_RET_OK;
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_JMPTOAPP,
							g_responsePacketData, CMD_LOADER_JUMP_TO_APP_TX_PAR_SLAVE,
							0);

	if(g_responsePacketData[0] == TO_DRV_RET_OK)
	{
		uart_emptyTxBuf(__pProto->pUart);
        *g_JumpingCodeMemLocation = 0;  /*Version 01.00.01*/
		FM3_CRC->CRCIN = 0;
		NVIC_SystemReset();
		fpJump = (void(*)())ResetHandler;
		(*fpJump)();
	}
}

static void loader_proto_jmpToLoader(const comProto_t *__pProto)
{
	uint32_t pw = 0;
	void (*fpJump)(void);


	g_responsePacketData[0] = TO_DRV_RET_ERROR;
	if(__pProto->rxCmdInfo.dataLen == CMD_LOADER_JUMP_TO_LOADER_RX_PAR_SLAVE) // Wrong number of parameters
	{
		if(copyReceivedData(&__pProto->rxCmdInfo.data, g_receivedData, __pProto->rxCmdInfo.dataLen))
		{
			pw = *((uint32_t*)(&g_receivedData[0]));
			if(pw == FLASH_OPERATION_PW)
				g_responsePacketData[0] = TO_DRV_RET_OK;
		}
	}

	comproto_sendCommand(	(comProto_t *)__pProto,
							__pProto->rxCmdInfo.masterId,
							__pProto->slaveId,
							__pProto->rxCmdInfo.msgCount,
							EPROTO_LOADER_CMD_JMPTOLOAD,
							g_responsePacketData, CMD_LOADER_JUMP_TO_LOADER_TX_PAR_SLAVE,
							0);

	if(g_responsePacketData[0] == TO_DRV_RET_OK)
	{
		uart_emptyTxBuf(__pProto->pUart);
        *g_JumpingCodeMemLocation = STAY_IN_LOADER;
		FM3_CRC->CRCIN = STAY_IN_LOADER;
		NVIC_SystemReset();
		/*fpJump = (void(*)())ResetHandler;*/ /*Commented in Version 01.00.02 */
		fpJump = (void(*)())LOADER_JUMP_ADDRESS; /*(*g_fpJumpToLoader)();*/ /*Commented in Version 01.00.02 */
		(*fpJump)();


	}

}

/***************************************************
loader_proto_blinkaLed()
@brief: Blink the led on the M75 Protetcive board
@params: none
****************************************************/
static void loader_proto_blinkaLed()
{
	static uint32_t count = 0;

	count++;

	if( count == 8000 )
	{
		PinLedLife_1 = 1;
		PinLedLife_2 = 1;
	}

	if( count > 20000 )
	{
		PinLedLife_1 = 0;
		PinLedLife_2 = 0;
		count =  0;
	}
}

static int copyReceivedData(const ringBuffer_t *__pRingBuf, uint8_t *__pData, uint32_t __size)
{
	int i = 0;
	for(i= 0; i < __size; i++)
	{
		if(!ringBuffer_isEmpty((ringBuffer_t*)__pRingBuf))
			ringBuffer_get((ringBuffer_t*)__pRingBuf, (__pData + i));
		else
			return 0;
	}
	return 1;
}
/**
Query the protocol*/
void loader_proto_query(const comProto_t *__pProto)
{
	incTimer_t scanTick = {0};

	uint8_t u8TempChar = 0;
	boolean_t bVerify = FALSE;


    #ifndef LOADER_ENGINE_INT
    uart_poller((uart_t*)__pProto->pUart, RX_DIRECTION);  //0 è direzione RX
    #endif


    comproto_manager((comProto_t*)__pProto);


    if ((__pProto->msgRejected == MSG_ACCEPTED) &&
        (__pProto->rxState == ECOMPROTO_RX_STATE_WAIT_FIRST_STX)){
         if (match(pollerWaiter)){

        /**The message is for me - I'm sending back the answer*/
        #ifdef LOADER_ENGINE_INT
        uart_poller((uart_t*)__pProto->pUart, TX_DIRECTION, &bVerify);
        if (bVerify == TRUE){
            debugVar_CharTxToController++;
        }else{
            /**We empty the buffer completely
            The message is ended */
            comproto_setMessageType((comProto_t *)__pProto, MSG_UNDEFINED);
        }
        #else
        uart_poller((uart_t*)__pProto->pUart, TX_DIRECTION);
        #endif
        stop(pollerWaiter);
        }else{
            start(pollerWaiter);
        }
        }else{
            /**The message is not for me - I need to empty
            both the buffer **/
            if (__pProto->msgRejected == MSG_REJECTED){
                uart_getc((uart_t*)__pProto->pUart, &u8TempChar);
            }/**else{
            }**/

        }


		//-- update communication statistics
		g_loaderStatus.cntCommErrTotal = __pProto->error.cntTotal;
		g_loaderStatus.cntCommErrWrongOpCode = __pProto->error.cntWrongOpCode;
		g_loaderStatus.cntCommErrBadStx = __pProto->error.cntBadStx;
		g_loaderStatus.cntCommErrBadEtx = __pProto->error.cntBadEtx;
		g_loaderStatus.cntCommErrTimeoutRx = __pProto->error.cntTimeoutRx;
		g_loaderStatus.cntCommErrTimeoutTx = __pProto->error.cntTimeoutTx;
		g_loaderStatus.cntCommErrBadCrc = __pProto->error.cntBadCrc;
		g_loaderStatus.cntCommErrDataBufOverflow = __pProto->error.cntDataBufOverflow;
		g_loaderStatus.cntCommErrBadEncoding = __pProto->error.cntBadEncoding;
		g_loaderStatus.cntUartHwError = __pProto->error.cntUartHwError;

		g_loaderStatus.cntComTx	= __pProto->cntComTx;
		g_loaderStatus.cntComRx = __pProto->cntComRx;
		g_loaderStatus.cntFail = __pProto->cntFail;


		g_loaderStatus.systemTickCount = getGlobalTick();
		g_loaderStatus.actScanTick = elapsed(scanTick);	//updated scan time
		if(g_loaderStatus.actScanTick > g_loaderStatus.maxScanTick)
			g_loaderStatus.maxScanTick = g_loaderStatus.actScanTick;
}
#ifdef LOADER_ENGINE_INT
/**IRQ service rotine to activate when receiving char from the Controller
micro by means of UART 3*/
void loader_proto_irqFromController()
{
    uint8_t u8RetVal = 0;
    boolean_t bVerify = FALSE;
    uart_t *__pUartController= &g_uart3;
    int iLoop = 0;

    #ifdef EN_DIS_THIS_UART
    __pUartController->pReg->SCR &= ~(BIT4); // disable RX IRQ
    #endif

    /**Here we receive from Controller*/
    uart_poller((uart_t*) __pUartController, RX_DIRECTION, &bVerify);

    if (bVerify == TRUE){
        /**
        Here we have correctly received from controller**/
        debugVar_CharRxFromController++; /*Debug var*/
        }else {
            debugVar_UnExpectedBranch++; /*Debug var*/
        }

    #ifdef EN_DIS_THIS_UART
     __pUartController->pReg->SCR |= BIT4; // enable RX IRQ
     #endif

     comproto_decode(&g_protoLoader);

}
#endif
