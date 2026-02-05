/**
@file		mainTest.h
@brief		Module to implement Test
@author		Fernando Morani
@date		10/04/2017
@version	01.00
@source     M75-CPU Flexiper Protective
*/

#ifndef _MAIN_TEST_H
#define _MAIN_TEST_H

#include "gesXfr_p.h"  //SPI Communication
#include "sevenSegDrv.h" //7Seg driver
#include "adcBaseTest.h"
#include "uartTest.h" //Uart Test
#include "framTest.h" //FRAM test
#include "mftEncoderTest.h" //MFT encoder Test

/**********************************************************
**     DEFINE  SECTION
***********************************************************/

#define SAMPLE_CSIO_SLAVE_RX_BUFFSIZE       8   //Use for SPI Test
#define TEST_APP_VAL                        0xAF
#define TEST_REL_VAL                        0x20
#define TEST_LED_VAL                        0x30
#define TEST_ENC_VAL                        0x40
#define TEST_FRM_VAL                        0x50

#define LED_LIFE_BLINK_TIME                 5000000
/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/

//ADD define as soon as you add a new test
enum TEST_TODO{ TEST_SPI_TO_PROT_MICRO,
				TEST_SPI_SINGLE_BYTE_LOCAL,
				TEST_SPI_BLOCK,
				TEST_SPI_SLAVE_RX_BUFFER,
				TEST_UDP_SINGLE_MSG,
				TEST_UDP_RECEIVE_MSG,
				TEST_DISPLAY,
				TEST_PERIPH_ADC,
				TEST_UART,
				TEST_SELECTOR_KEY,
				TEST_KEYBOARD,
				TEST_RTC_INPUT,
				TEST_FRAM,
				TEST_APPLICATION,
				TEST_MFT_ENCODER,
				TEST_LED,
				TEST_FIXED_RELE
			};

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
int startTest(TEST_TODO testVar);
bool CompareData(uint8_t* pBuf1, uint8_t* pBuf2, uint8_t u8Length);
int switchKey (void);
int switchButton (void);
int rtcInpuTest(void);
int blinkLedLife(int param);
int blockRele();

#endif
