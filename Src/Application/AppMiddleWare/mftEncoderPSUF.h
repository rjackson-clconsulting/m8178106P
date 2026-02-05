/**
@file		mftEncoderPSUF.h
@brief		Module to implement some encoder test code using MTF
@author		Fernando Morani
@date		11/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _MFT_ENCODER_TEST_H
#define _MFT_ENCODER_TEST_H

#include "multiFunctionTimer.h"
#include "sevenSegDrv.h" //Output function - TO use with micro that has 7-seg Display



/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
#define PS_UF_MFT_UNIT_SELECTED MFT_UNIT_1          //Both encoder device are in the same MFT unit

#define UF_ASSOCIATE_FREE_COUNTER           FRT_CHANNEL_0
#define UF_ROUND_VALUE                      0xFFF0
#define UF_ICU_UNIT_IN_MFT                  ICU_UNIT_1              //IC2 input port belongs to UNIT 1
#define UF_ICU_CHANNEL_IN_MFT               ICU_CHANNEL_0         //IC2 input port is on CHANNEL 0
#define UF_ASSIGN_ICU_TO_FRT                OCU_ICU_CONNECT_FRT_0
#define UF_ASSOCIATE_FRT_CLK_MULTIPLIER     CYCLE_RATIO_256
#define UF_CONTROLLER_ALIGNEMENT            2
#define UF_FRT_SET_VAL                      0.05 //Time expressed in milliseconds

#define PS_STOPPED_PRESUMED     5 //After 5 times it means no activity on ICU
#define UF_STOPPED_PRESUMED     5 //After 5 times it means no activity on ICU

#define PS_ASSOCIATE_FREE_COUNTER           FRT_CHANNEL_1
#define PS_ROUND_VALUE                      0xFFF0
#define PS_ICU_UNIT_IN_MFT                  ICU_UNIT_0              //IC1 input port belongs to UNIT 0
#define PS_ICU_CHANNEL_IN_MFT               ICU_CHANNEL_1         //IC1 input port is on CHANNEL 1
#define PS_ASSIGN_ICU_TO_FRT                OCU_ICU_CONNECT_FRT_1
#define PS_ASSOCIATE_FRT_CLK_MULTIPLIER     CYCLE_RATIO_256
#define PS_CONTROLLER_ALIGNEMENT            2
#define PS_FRT_SET_VAL                      0.05 //Time expressed in milliseconds

#define MFT_INIT_OK             0
#define MFT_INIT_FAIL           1

/**This define should be enabled in application that needs to calculate the frequency
In VetSmart we don't use it. In Flexiper is foreseen**/
#undef UF_FREQUENCY_CALC
#undef PS_FREQUENCY_CALC

/**********************************************************
**     TYPEDEF SECTION
***********************************************************/

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
uint8_t encoderInit(void);
uint16_t UF_Read();
uint16_t PS_Read();
void PS_ISR();
void UF_ISR();
void PS_PEAK_INT_ISR(frtChannel_Type chan);
void UF_PEAK_INT_ISR(frtChannel_Type chan);

#endif
