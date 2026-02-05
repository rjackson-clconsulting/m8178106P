/**
@file		util.h
@brief		General Purpose utility routines in C
@author		Fernando Morani
@date		04/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#ifndef _UTIL_H_
#define _UTIL_H_

#include "stdDataTypes.h"

void struct_memclr(uint8_t* pu32Address, uint32_t u32Count);
uint8_t extractBit(uint32_t bitMask);

#ifdef __cplusplus
extern "C" {
#endif
uint8_t extratcIndexFromBitMask (uint32_t bitMask);
#ifdef __cplusplus
}
#endif

#define RESET_ZERO_STRUCT(x) struct_memclr((uint8_t*)&(x), (uint32_t)(sizeof(x)))

#define NO_INDEX_AVAIL      0xFF

#endif /*_UTIL_H*/


