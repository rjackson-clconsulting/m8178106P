/**
@file		mftEncoderTest.h
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

#define PS_ASSOCIATE_FREE_COUNTER FRT_CHANNEL_0
#define PS_ROUND_VALUE 0x5FFF
#define PS_ICU_UNIT_IN_MFT  ICU_UNIT_1              //IC2 input port belongs to UNIT 1
#define PS_ICU_CHANNEL_IN_MFT ICU_CHANNEL_0         //IC2 input port is on CHANNEL 0
#define PS_ASSIGN_ICU_TO_FRT OCU_ICU_CONNECT_FRT_0

#define UF_ASSOCIATE_FREE_COUNTER FRT_CHANNEL_1
#define UF_ROUND_VALUE 0x5FFF
#define UF_ICU_UNIT_IN_MFT  ICU_UNIT_0              //IC1 input port belongs to UNIT 0
#define UF_ICU_CHANNEL_IN_MFT ICU_CHANNEL_1         //IC1 input port is on CHANNEL 1
#define UF_ASSIGN_ICU_TO_FRT OCU_ICU_CONNECT_FRT_1

#define MFT_INIT_OK             0
#define MFT_INIT_FAIL           1


/**********************************************************
**     TYPEDEF SECTION
***********************************************************/

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
uint8_t encoderInit(void);
uint16_t UF_Read();
uint16_t PS_Read();

#endif
