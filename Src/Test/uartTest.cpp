/**
@file		uartTest.cpp
@brief		Module to implement some UART Initialization and test code
@author		Fernando Morani
@date		16/02/2017
@version	01.00
@source     M75-CPU Controller
*/

#include "uartTest.h"

/*****************************************************
** GLOBAL VAR SECTION
*****************************************************/

/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/

/*****************************************************
** uint8_t TestUartInt(SIOChan param_0)
*****************************************************/

uint8_t TestUartInt(SIOChan channel, uartTestMode_t mode)
{
CBufHandle hBufUartA;			// un buffer per ogni porta
UARTHandle hUartDevA;
uint8_t u8RetVal = 0;

bool okA;
bool bUartSent = false;

long loopCnt = 0;
long testCnt = 0;
int iTxCountOk = 0;
int iTxCountFail = 0;
int iRxCountOk = 0;
int iRxCountFail = 0;

char s[TEST_BUF_SIZE] = {0xA,0xB,0xC,0xD,0xE,0xF,
             0xA,0xB,0xC,0xD,0xE,0xF,
             0xA,0xB,0xC,0xD,0xE,0xF,
             0xA,0xB,0xC,0xD,0xE,0xF,
             0xA,0xB,0xC,0xD,0xE,0xF};
char rx[TEST_BUF_SIZE] = {0,0,0,0,0,0,
             0,0,0,0,0,0,
             0,0,0,0,0,0,
             0,0,0,0,0,0,
             0,0,0,0,0,0};
byte chr1;

    writeU(); //Start UART Test
    hBufUartA.theHandle = BUF_NULL_HANDLE;
    hBufUartA.mod_id = BUF_NULL_ID;

	hBufUartA = combuf_create(BUF_TX_BUFFER_SIZE, BUF_RX_BUFFER_SIZE, BUF_TX_BUFFER_TSHLD, BUF_RX_BUFFER_TSHLD);

	if (hBufUartA.theHandle == BUF_NULL_HANDLE){
        u8RetVal = ERR_BUF_INIT;
        return u8RetVal;
        }

	okA = sio_cfgChan(channel, hBufUartA, SIO_UART, SIO_RATE9600, SIO_8BIT,
				                  SIO_1STOP, SIO_NOPARITY, SIO_MSB_FIRST,
				                  SIO_CLKHIGH, SIO_XFR_INT_ALL, IRQ_PRIO4);

											// se ha scannato una porta
	if( okA != True )
	{
		combuf_destroy(hBufUartA);			// reset buffers
		return u8RetVal;
	}


	hUartDevA = uart_create(hBufUartA, ChipSelDevA);		// creiamo il canale tipo UART

	if( !hUartDevA.theHandle )								// se non è valido
	{
		combuf_destroy(hBufUartA);			// reset buffers
		return u8RetVal;
	}

	if( uart_open(hUartDevA) != True )						// se non si apre
	{
		return u8RetVal;
	}

    switch (mode){
        case UART_FULL:
        // loop di test consistente
            for(loopCnt=testCnt=0; loopCnt<1000000; loopCnt++){
                if( (loopCnt % 100000) == 0 ){
                    testCnt++;
                    //sprintf(s, "testCnt=%ld\r\n", testCnt);
                    strncpy (s,"ciccio",7);
                    bUartSent = uart_puts(hUartDevA, s);
                    if (bUartSent == false){
                        }
                    else{
                    }

                if( loopCnt >= 800000 )
                    loopCnt = 1;

                //for(int conta = 0; conta<1000000; conta++); // Just wait before retransmitting
                if( uart_getRxCount(hUartDevA) )			// se ha ricevuto qualcosa
                {
                    if( uart_getc(hUartDevA, &chr1) == True ){
                    //Transmitting
                    }
                }
                }
            }
            break;
        case UART_TX:
            //Only Send Character
            loopCnt = 0;
            while (1){
                s[loopCnt] = 0xCC;
                bUartSent = uart_puts(hUartDevA, s);
                if (bUartSent == false){
                    iTxCountFail++;
                    }
                else{
                    iTxCountOk++;
                        }
                loopCnt++;
                if (loopCnt >= TEST_BUF_SIZE){
                    //reset counter
                    loopCnt = 0;
                    if (iTxCountFail == 0){
                        //No error
                        u8RetVal = 0;
                        break;
                        }
                    }
                for(testCnt = 0; testCnt<TEST_R_TX_TIME; testCnt++); // Just wait before retransmitting
            }
            break;
        case UART_RX:
            loopCnt = 0;
            testCnt = 0;
            while (1){
            if( uart_getRxCount(hUartDevA) ){
                if( uart_getc(hUartDevA, &chr1) == True ){
                    iRxCountOk++;
                    rx[loopCnt] = chr1;
                    testCnt++;
                    }
                else{
                    iRxCountFail++;
                    }
                loopCnt++;
                }
            if (testCnt >= TEST_BUF_SIZE*TEST_BUF_SIZE){
                break;
            }
            if (loopCnt == TEST_BUF_SIZE){
                loopCnt = 0;
                for(int conta = 0; conta<TEST_R_RX_TIME; conta++); // Just wait before check
            }

            }
            break;
        default:
            break;
    }

    return u8RetVal;
}


/*****************************************************
** void ChipSelDevA(int en)
** Dummy function kept for s/w compatibility
*****************************************************/
void ChipSelDevA(int en)
{
	if( en )
	{
        // ??
	}
	else
	{
        // ??
	}

}
