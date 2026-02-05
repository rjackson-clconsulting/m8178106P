/**
@file		externVar.h
@brief		C File where to put external variables to use in separate module
@author		Fernando Morani
@date		10/03/2018
@version	01.00.00
@source     VetSmart Protective Application
*/

#ifndef _EXTERN_VAR_H
#define _EXTERN_VAR_H

#include "stdDataTypes.h"

#define COMPROTO_SEMAPHORE_BUSY     0
#define COMPROTO_SEMAPHORE_FREE     1

extern uint8_t g_ComProtoUartSemaphore;

#endif
