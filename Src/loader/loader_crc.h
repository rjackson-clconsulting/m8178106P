/**
@file		loader_crc.h
@brief		Loader crc calculation and initialization
@author		Nicola Molinazzi
@date		09/01/2011
@version	01.01
*/

#ifndef _LOADER_CRC_H_
#define _LOADER_CRC_H_

#include "stdDataTypes.h"
#include "loader_types.h"
#include "mb9bf50x.h"

#ifdef OPTIMIZED_VERSION
#define APPLICATION_START_ADD			0x8301 //0x4301
#define APPLICATION_INFO_TABLE_ADD		0x8110 //0x4110
#else
#define APPLICATION_START_ADD			0x20301 //0x4301
#define APPLICATION_INFO_TABLE_ADD		0x20110 //0x4110
#endif

#define LOADER_START_ADD                0x00000000
#define LOADER_END_ADD                  0x0001FFFF
#define LOADER_SEGMENT_NUMB             1

extern loaderStatus_t g_loaderStatus;

extern void (*g_fpJumpToApplication)(void);
#ifdef __cplusplus
extern "C"{
#endif

void loader_status_init();
bool loader_status_checksumVerify();

#ifdef __cplusplus
}
#endif

#endif
