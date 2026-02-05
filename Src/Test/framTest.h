/**
@file		framTest.h
@brief		Module to implement some FRAM test code
@author		Fernando Morani
@date		09/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _FRAM_TEST_H
#define _FRAM_TEST_H

#include "EEPROM.H"
#include "sevenSegDrv.h" //Output function - TO use with micro that has 7-seg Display



/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
#define TEST_R_W_SAME_BLOCK      1
#define TEST_R_W_SAME_PARAMS     2
#define TEST_R_PARAMS            3

#define TEST_FRAM_INVALID       0xFF
#define TEST_FAILED             0xFE
#define TEST_FRAM_OK            0x00

#define TEST_WAIT_FOR_CYCLE     1000000
#define TEST_NUM_POINTS         10
#define	TEST_START_ADDRESS 		0x0280


/**********************************************************
**     TYPEDEF SECTION
***********************************************************/
struct testPoint{
        uint16_t	u16Value;
        uint32_t	u32Value;
    };
typedef struct testPoint testPoint;

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
uint8_t framTest(uint8_t param);
uint8_t framRWParams(void);
uint8_t framReadParams(void);
uint8_t framRWSameBlock(void);
uint8_t framWriteTestData(void);

#endif
