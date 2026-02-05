/**
@file		loader_types.h
@brief		loader types and global defines
@author		Nicola Molinazzi
@date		16/01/2011
@version	01.00
*/

#ifndef _LOADER_TYPES_H_
#define _LOADER_TYPES_H_

#include "stdDataTypes.h"
#include "app_info.h"

#define STAY_IN_LOADER					0x12345678
#define FLASH_OPERATION_PW      		0xCE123F41
#define LOADER_JUMP_ADDRESS 	        0x00000301  //FERLOADER
#define SRAM_1_STRING_ADDR              0x2007FFF0

/**
 * Causes because we are stopped into the loader
 */
enum loader_please_t
{
	ELOADER_PLEASE_NONE = 0,
	ELOADER_PLEASE_WRONG_LOADER_SEGMENTS_NUM,
	ELOADER_PLEASE_WRONG_LOADER_TEXT_AREA_END,
	ELOADER_PLEASE_WRONG_LOADER_ID,
	ELOADER_PLEASE_WRONG_APP_SEGMENTS_NUM,
	ELOADER_PLEASE_WRONG_APP_ID,
	ELOADER_PLEASE_WRONG_APP_CHECKSUM,
	ELOADER_PLEASE_LOADER_BUTT,
	ELOADER_PLEASE_APP_JUMP,
	ELOADER_PLEASE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum loader_please_t  loader_please_t;
#endif

/**
 * Loader status
 */
struct loaderStatus_t{
        char    	swVersLoader[APPLICATION_REVISION_LENGTH_MAX];// numero di revisione del loader
        char    	swNameLoader[APPLICATION_NAME_LENGTH_MAX];// nome del softw. di loading
        char    	swDateLoader[APPLICATION_DATE_LENGTH_MAX];
        char    	hwVersLoader[BOARD_HW_VERSION_LENGTH_MAX];

        char    	swVersApp[APPLICATION_REVISION_LENGTH_MAX];// numero di revisione del softw.
        char    	swNameApp[APPLICATION_NAME_LENGTH_MAX];// nome del softw.
        char    	swDateApp[APPLICATION_DATE_LENGTH_MAX];
        char    	hwVersApp[BOARD_HW_VERSION_LENGTH_MAX];

        uint32_t   	chkAppProg									__attribute__ ((packed));// chk applicativo programmato in memoria
        uint32_t   	chkAppCalc									__attribute__ ((packed));// chk applicativo calcolato
        uint32_t	chkLoaderProg								__attribute__ ((packed));// chk programmato del loader
        uint32_t	chkLoaderCalc								__attribute__ ((packed));// chk calcolato del loader

        uint32_t   	maxScanTick									__attribute__ ((packed));
        uint32_t   	actScanTick									__attribute__ ((packed));

        uint32_t    systemTickCount								__attribute__ ((packed));

        uint32_t    loaderReWriteEn								__attribute__ ((packed));

        uint16_t 	cntCommErrTotal								__attribute__ ((packed));
		uint16_t 	cntCommErrWrongOpCode						__attribute__ ((packed));
		uint16_t 	cntCommErrBadStx							__attribute__ ((packed));
		uint16_t 	cntCommErrBadEtx							__attribute__ ((packed));
		uint16_t  	cntCommErrTimeoutRx							__attribute__ ((packed));
		uint16_t  	cntCommErrTimeoutTx							__attribute__ ((packed));
		uint16_t  	cntCommErrBadCrc							__attribute__ ((packed));
		uint16_t  	cntCommErrDataBufOverflow					__attribute__ ((packed));
		uint16_t  	cntCommErrBadEncoding						__attribute__ ((packed));
		uint16_t  	cntUartHwError								__attribute__ ((packed));
		uint16_t    cntComTx									__attribute__ ((packed));
		uint16_t    cntComRx									__attribute__ ((packed));
		uint16_t    cntFail										__attribute__ ((packed));

        loader_please_t	loaderPlease;					// flag per richiesta stop nel loader
        uint8_t			loader_error;					// flash area where crc error was rised
    };
#ifdef __cplusplus
#else
typedef struct loaderStatus_t  loaderStatus_t;
#endif

#endif
