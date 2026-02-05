/**
@file		COMPROTO_LOADER.h
@brief		Routines to manage (decode-encode) loader serial protocol

@author		Nicola
@date		08/01/2013
@version	01.00.00
*/

#ifndef _COMPROTO_LOADER_H
#define _COMPROTO_LOADER_H

#include "FlashAlgorithm.h"
#include "loader_crc.h"
#include "COMPROTO.h"
#include "protocol_CRRT_PC_Types.h"
#include "base_types.h"
#include "swtimer.h"
#include "externVar.h"

/**
 * Loader protocol opCodes
 */
enum loader_opCodes_t{

	EPROTO_LOADER_CMD_ACK 			 = CMD_ACK,     			// codice del comando ack response
	EPROTO_LOADER_CMD_READ_STATUS	 = CMD_READ_STATUS,		/* richiesta stato come su applicativo*/

	EPROTO_LOADER_CMD_CHK_STATUS	 = CMD_LOADER_READ_STATUS, /* richiesta del pacco completo delle info di stato */
															/* comandi speciali per prog. micro*/
	EPROTO_LOADER_CMD_INFO 			 = CMD_LOADER_INFO,
	EPROTO_LOADER_CMD_LOADED_APP_INFO= CMD_LOADED_APP_INFO,

	EPROTO_LOADER_CMD_INIT 			 = CMD_LOADER_FLASH_INIT,
	EPROTO_LOADER_CMD_UNINIT 		 = CMD_LOADER_FLASH_UNINIT,
	EPROTO_LOADER_CMD_BLKCHK 		 = CMD_LOADER_BLANK_CHECK,
	EPROTO_LOADER_CMD_CHIPERASE		 = CMD_LOADER_CHIP_ERASE,
	EPROTO_LOADER_CMD_WRITEBLOCK	 = CMD_LOADER_WRITE_BLOCK,
	EPROTO_LOADER_CMD_READBLOCK 	 = CMD_LOADER_READ_BLOCK,
	EPROTO_LOADER_CMD_VERIFBLOCK	 = CMD_LOADER_VERIFY_BLOCK,
	EPROTO_LOADER_CMD_BLOCKERASE 	 = CMD_LOADER_BLOCK_ERASE,
	EPROTO_LOADER_CMD_REWRLOADER	 = CMD_LOADER_REWR_LOADER,
	EPROTO_LOADER_CMD_JMPTOAPP		 = CMD_LOADER_JUMP_TO_APP,
	EPROTO_LOADER_CMD_JMPTOLOAD		 = CMD_LOADER_JUMP_TO_LOADER
};


#ifdef __cplusplus
extern "C" {
#endif

void loader_proto_init(	const comProto_t *__pProto,
						uart_t *__pUart,
						uint16_t __rxProtoDataBufLength,
						uint8_t __masterId, uint8_t __slaveId)				__attribute__ (( section(".ramcode") ));

void loader_proto_static_init(	const comProto_t *__pProto,
								uart_t *__pUart,
								uint8_t *__pRxProtoDataBuf,
								uint16_t __rxProtoDataBufLength,
								uint8_t __masterId, uint8_t __slaveId)		__attribute__ (( section(".ramcode") ));

void loader_proto_query(const comProto_t *__pProto)                         __attribute__ (( section(".ramcode") ));
void loader_proto_irqFromController()                                       __attribute__ (( section(".ramcode") ));

#ifdef __cplusplus
}
#endif
#endif
