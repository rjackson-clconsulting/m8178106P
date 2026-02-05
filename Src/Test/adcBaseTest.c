/**
@file		adcBaseTest.c
@brief		Header file
@author		Fernando Morani
@date		05/05/2017
@version	01.00
@source     M75-CPU Flexiper Protective
*/
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "adcBaseTest.h"

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Extern variable                                                             */
/******************************************************************************/
extern ADChan adcSet[MAX_AD];

/******************************************************************************
** Base Test:
** Select the peripheral to test and test it
******************************************************************************/

uint8_t adcPeriphTest(uint32_t adDevice)
{
uint8_t u8RetVal = 0;
int tempAnalogIndex = 0;
int i = 0;
ADChan *pAdc = NULL; //&adcSet[0];
uint32_t u32ReadVal = 0;

    tempAnalogIndex = extratcIndexFromBitMask(adDevice);
    pAdc = &adcSet[tempAnalogIndex];
    u32ReadVal = pAdc->value;

    while (1){
        for (i=0;i<1000000;i++);
        if (u32ReadVal != pAdc->value){
            u32ReadVal = pAdc->value;
            writeU(); //Update
        }else {
            writeS(); //Same value
        }
        for (i=0;i<1000000;i++);

    }

return u8RetVal;
}
