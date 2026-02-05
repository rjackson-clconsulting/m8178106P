/**
@file		uartTest.h
@brief		Module to implement some UART test code
@author		Fernando Morani
@date		08/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _UART_TEST_H
#define _UART_TEST_H

#include "SIOFM3.H"
#include "UART.H"
#include "sevenSegDrv.h"  //To use the 7 Segment display for debug output



/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
#define BUF_NULL_HANDLE      0
#define BUF_NULL_ID          0
#define BUF_TX_BUFFER_SIZE   100
#define BUF_RX_BUFFER_SIZE   100
#define BUF_TX_BUFFER_TSHLD  1
#define BUF_RX_BUFFER_TSHLD  100
#define ERR_BUF_INIT         1
#define TEST_BUF_SIZE        30
#define TEST_R_TX_TIME       1000000
#define TEST_R_RX_TIME       500000


typedef enum uartTestMode{
    UART_TX,
    UART_RX,
    UART_FULL
}uartTestMode_t;

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
void ChipSelDevA(int en);
uint8_t TestUartInt(SIOChan param_0, uartTestMode_t mode);


#endif
