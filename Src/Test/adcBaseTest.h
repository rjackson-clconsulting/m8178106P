/**
@file		adcBaseTest.h
@brief		Header file
@author		Fernando Morani
@date		05/05/2017
@version	01.00
@source     M75-CPU Flexiper Protective
*/

#ifndef _ADC_BASE_TEST_H
#define _ADC_BASE_TEST_H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "adcMapFlexip.h"

/******************************************************************************/
/* Define for the source code                                                             */
/******************************************************************************/


/******************************************************************************/
/* Function declaration                                                              */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
uint8_t adcPeriphTest(uint32_t adDevice);

#ifdef __cplusplus
}
#endif

#endif
