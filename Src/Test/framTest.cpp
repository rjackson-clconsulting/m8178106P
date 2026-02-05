/**
@file		framTest.cpp
@brief		Module to implement some FRAM test code
@author		Fernando Morani
@date		09/05/2017
@version	01.00
@source     M75-CPU Protective
*/


#include "framTest.h"

/*****************************************************
** GLOBAL AND STATIC VAR SECTION
*****************************************************/
static testPoint points[TEST_NUM_POINTS];
static testPoint pointsR[TEST_NUM_POINTS];

/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/

/*****************************************************
** uint8_t framTest(uint8_t param)
** \brief Base test
** \param uint8_t param: choose the test
*****************************************************/
uint8_t framTest(uint8_t param)
{
    uint8_t u8RetVal = 0;
    switch (param){
        case TEST_R_W_SAME_BLOCK:
            u8RetVal = framRWSameBlock();
            break;
        case TEST_R_W_SAME_PARAMS:
            u8RetVal = framRWParams();
            break;
        case TEST_R_PARAMS:
            u8RetVal = framReadParams();
            break;
        default:
            u8RetVal = TEST_FRAM_INVALID;
            break;
    }
    return u8RetVal;
}
/*****************************************************
** uint8_t framRWParams(void)
** \brief First Write and then read the params
*****************************************************/
uint8_t framRWParams(void)
{
uint8_t u8RetVal = 0;
int iCounter = 0;
int i = 0;
bool exitFlag = false;
bool writeOk = false;
bool chkOk = false;
bool bResult = false;

    writeP();  //Start Param Test
    eep_init(0); //Init
    framWriteTestData();
    while (exitFlag != true){
        //Write params
        writeOk = eep_writeParam(points, sizeof(points), TEST_START_ADDRESS, &chkOk);
        if (writeOk == false){
            writeF(); //Test Fail
            exitFlag = true;
            u8RetVal = TEST_FAILED;
            break;
        }
        //wait cycle
        for (i=0; i<TEST_WAIT_FOR_CYCLE;i++);
        //read Params
        writeOk = eep_readParam(pointsR, sizeof(pointsR), TEST_START_ADDRESS, &chkOk);
        if (writeOk == false){
            writeF(); //Test Fail
            exitFlag = true;
            u8RetVal = TEST_FAILED;
            break;
        }
        //wait cycle
        for (i=0; i<TEST_WAIT_FOR_CYCLE;i++);
        //Compare Params
        for (i=0; i<TEST_NUM_POINTS;i++){
            bResult = (points[i].u16Value == pointsR[i].u16Value) && (points[i].u32Value == pointsR[i].u32Value);
        }
        if (bResult == false){
            writeF(); //Test Fail
            exitFlag = true;
            u8RetVal = TEST_FAILED;
            break;
        }else {
            u8RetVal = TEST_FRAM_OK;
            exitFlag = true;
            break;
        }
        iCounter++;
    }

    //wait cycle
    for (i=0; i<TEST_WAIT_FOR_CYCLE;i++);

    if (u8RetVal != TEST_FAILED)
        writeZERO(); //Test Completed

    return u8RetVal;

}
/*****************************************************
** uint8_t framRWSameBlock(void)
** \brief First Write and then read the block
*****************************************************/
uint8_t framRWSameBlock(void)
{
    return 0;
}

/*****************************************************
** uint8_t framWriteTestData(void)
** \brief Init the block to write in mem
** \return number of TEST_NUM_POINTS
*****************************************************/
uint8_t framWriteTestData(void)
{
int i = 0;

    for (i=0;i<TEST_NUM_POINTS;i++){
        points[i].u16Value = i;
        points[i].u32Value = i*2;
    }

    return i;
}


/*****************************************************
** uint8_t framReadParams(void)
** \brief Read the params
*****************************************************/
uint8_t framReadParams(void)
{
uint8_t u8RetVal = 0;
int iCounter = 0;
int i = 0;
bool exitFlag = false;
bool writeOk = false;
bool chkOk = false;

    writeP();  //Start Param Test
    eep_init(0); //Init
    while (exitFlag != true){
        //read Params
        writeOk = eep_readParam(pointsR, sizeof(pointsR), TEST_START_ADDRESS, &chkOk);
        if (writeOk == false){
            writeF(); //Test Fail
            exitFlag = true;
            u8RetVal = TEST_FAILED;
            break;
        }else {
            u8RetVal = TEST_FRAM_OK;
            exitFlag = true;
            break;
        }
        iCounter++;
    }

    //wait cycle
    for (i=0; i<TEST_WAIT_FOR_CYCLE;i++);

    if (u8RetVal != TEST_FAILED)
        writeZERO(); //Test Completed

    return u8RetVal;

}
