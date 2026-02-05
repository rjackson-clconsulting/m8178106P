/**
@file		mainTest.cpp
@brief		Module to implement test
@author		Fernando Morani
@date		17/02/2017
@version	01.00
@source     M75-CPU Controller
*/

#include "mainTest.h"

/*****************************************************
** GLOBAL VAR SECTION
*****************************************************/

/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/
extern CSIHandle hCsiDevA;
//extern uint8_t TestUartInt(int param);
/*****************************************************
** STATIC VAR SECTION
*****************************************************/


/*****************************************************
** startTest
** Return 0 if user choose to carry on with application
** Other value if not
*****************************************************/

int startTest(TEST_TODO testVar)
{
    int iRetVal = 0;
    int iTimeDelay = 0;
    struct CsiXfrBlockTx bigBlock;		// blocco dati personale
    uint16_t ui16ByteToReceive = 0xCC; //Used in SPI Test
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL; //Used in SPI TEST
    int iSpiSendCounter = 0;
    uint8_t u8Cnt = 0;  //Index of the buffer - SPI TEST
    uint8_t u8CntPinUP = 0; //Count the number of times we rise the sync PIN
    uint8_t au8CsioSlaveRxBuf[SAMPLE_CSIO_SLAVE_RX_BUFFSIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t au8CsioSlaveTxBuf[SAMPLE_CSIO_SLAVE_RX_BUFFSIZE] = {0xA1,0xA2,0xB1,0xB2,0xB3,0xB4,0xC5,0xD6};
    uint8_t au8CsioMasterTxBuf[SAMPLE_CSIO_SLAVE_RX_BUFFSIZE] = {0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x01,0x02};

    switch (testVar)
    {
    case TEST_SPI_TO_PROT_MICRO:
        iRetVal = IniXfr_P();
        if  (iRetVal != ERROR_NONE){
                break;
            }
        while( iRetVal ==0 )
        {
            iRetVal = GesXfr_P();
            sio_poll();
            //for (iTimeDelay=0;iTimeDelay<1000;iTimeDelay++);
        }
        break;
    case TEST_SPI_SINGLE_BYTE_LOCAL:
        IniXfr_P();
        writeS(); //Start Test
        pFM3_MFSx = FM3_MFS4_CSIO;
        PinSAFETY_BUSY = 1;
        while (iSpiSendCounter < 10)
        {
            while (pFM3_MFSx->SSR_f.RDRF == OFF)        //Rx Register is empty
            {
            //Rx Empty ??
            pFM3_MFSx->TDR = 0xAA; //Send a dummy byte to start
            writeE(); //Write E as Error
            }
            ui16ByteToReceive = pFM3_MFSx->RDR; // Reading From RDR Register
            iSpiSendCounter++;
            if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is free
            {
                //pFM3_MFSx->SCR_f.TXE = SET_BIT_HIGH;           //Enable the Transmission
                ui16ByteToReceive++;
                pFM3_MFSx->TDR = ui16ByteToReceive;    //Write in the register
                while(pFM3_MFSx->SSR_f.TDRE == OFF);    //Wait the tx data is shifted out from the register
                //pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;    //Disable the Transmission
            }
            else
            {
                //Tx is not ON
                 writeA(); //Try Again
            }
        }
        PinSAFETY_BUSY = 0;
        writeZERO(); //End Test
        iRetVal = TEST_SPI_SINGLE_BYTE_LOCAL;
        break;
    case TEST_SPI_BLOCK:
        IniXfr_P();
        while (1)
        {
        writeS(); //Start Test
        csi_write(hCsiDevA, (byte *) &bigBlock, sizeof(bigBlock) );		// spara per il prossimo giro
        for (iTimeDelay=0;iTimeDelay<1000000;iTimeDelay++);
        writeE(); //End Test
        for (iTimeDelay=0;iTimeDelay<1000000;iTimeDelay++);
        }
        iRetVal = TEST_SPI_BLOCK;
        break;
    case TEST_SPI_SLAVE_RX_BUFFER:
        writeI(); //Init the test
        IniXfr_P();  //Channel set up
        pFM3_MFSx = FM3_MFS4_CSIO; //Assign the MFS 4 channel
		PinSAFETY_BUSY = SET_BIT_LOW;  //Master could not send
		pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH; // Enable RX function of CSIO4
   		pFM3_MFSx->SCR_f.TXE = SET_BIT_HIGH; // Enable TX function of CSIO4
		pFM3_MFSx->TDR = au8CsioSlaveTxBuf[0];	//Prepare the 1st byte to send
        PinSAFETY_BUSY = SET_BIT_HIGH;  //Master now could send
		u8CntPinUP++;
        writeS(); //Start Test
        while(u8Cnt < SAMPLE_CSIO_SLAVE_RX_BUFFSIZE)
        {
        	/* Slave receives data */
            while (pFM3_MFSx->SSR_f.RDRF != ON);
            au8CsioSlaveRxBuf[u8Cnt] = pFM3_MFSx->RDR;  //Data stored
            u8Cnt++;
            /* Slave sends data */
			while (pFM3_MFSx->SSR_f.TDRE != 1);
			if (u8Cnt < SAMPLE_CSIO_SLAVE_RX_BUFFSIZE)
				pFM3_MFSx->TDR = au8CsioSlaveTxBuf[u8Cnt];
        }  //End While

		pFM3_MFSx->SCR_f.RXE = SET_BIT_LOW; // Disable RX function of CSIO4
		pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW; // Disable TX function of CSIO4
		u8Cnt = 0;
		u8CntPinUP = 0;
        iRetVal = TEST_SPI_SLAVE_RX_BUFFER;
        if(true == CompareData(au8CsioMasterTxBuf, au8CsioSlaveRxBuf, SAMPLE_CSIO_SLAVE_RX_BUFFSIZE))
        {
            writeO(); //Test end OK
        }
        else
            writeF(); //Test Failed
        break;
    case TEST_PERIPH_ADC:
        writeS();   //Start Test
        adcPeriphTest(BLD_RX_P);
        iRetVal = TEST_PERIPH_ADC;
        break;
    case TEST_UART:
        writeU();
        iRetVal = TestUartInt(SIO_CHAN6, UART_FULL);
        break;
    case TEST_SELECTOR_KEY:
        iRetVal = switchKey();
        break;
    case TEST_KEYBOARD:
        iRetVal = switchButton();
        break;
    case TEST_RTC_INPUT:
        iRetVal = rtcInpuTest();
        break;
    case TEST_FRAM:
        iRetVal = framTest(TEST_R_PARAMS);
        if (iRetVal == TEST_FRAM_OK){
            iRetVal = TEST_FRM_VAL;
        }else{
            iRetVal = iRetVal + TEST_FRM_VAL;
        }
        break;
    case TEST_MFT_ENCODER:
        iRetVal = encoderInit();
        if (iRetVal != 0){
            //Init Error
            iRetVal = TEST_ENC_VAL + iRetVal;
            }else{
                iRetVal = TEST_ENC_VAL;
                while(1);
            }
        break;
    case TEST_LED:
        iRetVal = blinkLedLife(LED_LIFE_BLINK_TIME);
        break;
    case TEST_FIXED_RELE:
        blockRele();
        iRetVal = TEST_REL_VAL;
        writeTHREE();
        break;
    case TEST_APPLICATION:
        iRetVal = TEST_APP_VAL;
        writeA();
        break;
    default:
        iRetVal = 10;
        break;
    }
    return iRetVal;
}


/**
 ******************************************************************************
 ** \brief  Test the GPIO connected to the key selector.
 **
 ** \retval TEST_SELECTOR_KEY     No Problems
 ** \retval 1                     Any Problems
 ******************************************************************************/
int switchKey(void)
{
int countSelP = 0;
int countChAP = 0;
int countChBP = 0;
int countSelM = 0;
int countChAM = 0;
int countChBM = 0;
int retVal = TEST_SELECTOR_KEY;
bool exitFlag = false;

    writeP();
    while (exitFlag != true){
        if (PinSEL_SW == 0){
            countSelM++;
        }else {
            countSelP++;
            }
        if (PinSEL_CHA == 0){
            countChAM++;
        }else {
            countChAP++;
        }
        if (PinSEL_CHB == 0){
            countChBM++;
        }else {
            countChBP++;
        }
        if ((countSelM != 0) && (countChAM != 0) && (countChBM !=0) &&
            (countSelP != 0) && (countChAP != 0) && (countChBP !=0)){
            exitFlag = true;
            writeE();
        }
    }

    return retVal;

}

/**
 ******************************************************************************
 ** \brief  Test the GPIO connected to the RTC.
 **
 ** \retval TEST_RTC_INPUT       No Problems
 ** \retval 1                    Any Problems
 ******************************************************************************/
int rtcInpuTest(void)
{
int countRTCP = 0;
int countRTCM = 0;
int retVal = TEST_RTC_INPUT;
bool exitFlag = false;

    writeC();
    while (exitFlag != true){
        if (PinRTC_VCC == 0){
            countRTCM++;
        }else {
            countRTCP++;
            }
        if ((countRTCM != 0) && (countRTCP != 0)){
            exitFlag = true;
            writeE();
        }
    }

    return retVal;

}

/**
 ******************************************************************************
 ** \brief  Test the GPIO connected to the 3 main button of the keyboard.
 **
 ** \retval TEST_KEYBOARD         No Problems
 ** \retval 1                     Any Problems
 ******************************************************************************/
int switchButton(void)
{
int countTASTO_1_ON = 0;
int countTASTO_2_ON = 0;
int countTASTO_3_ON = 0;
int countTASTO_1_OFF = 0;
int countTASTO_2_OFF = 0;
int countTASTO_3_OFF = 0;
int retVal = TEST_KEYBOARD;
bool exitFlag = false;

    writeP();
    while (exitFlag != true){
        if (PinTASTO_1 == 0){
            countTASTO_1_OFF++;
        }else {
            countTASTO_1_ON ++;
            }
        if (PinTASTO_2 == 0){
            countTASTO_2_OFF++;
        }else {
            countTASTO_2_ON ++;
            }
        if (PinTASTO_3 == 0){
            countTASTO_3_OFF++;
        }else {
            countTASTO_3_ON ++;
            }
        if ((countTASTO_1_OFF != 0) && (countTASTO_2_OFF != 0) && (countTASTO_3_OFF !=0) &&
            (countTASTO_1_ON != 0) && (countTASTO_2_ON != 0) && (countTASTO_3_ON !=0)){
            exitFlag = true;
            writeE();
        }
    }

    return retVal;

}
/**
 ******************************************************************************
 ** \brief  Alternative blink of the led life
 **
 ** \param: int param: delay time (in number of instruction) for alternate the blink
 ** \retval TEST_LED_VAL
 ******************************************************************************/
int blinkLedLife(int param)
{
int i= 0; //index to turn

    PinLedLife_1 = 0;
    PinLedLife_2 = 0;

    while (1){
        writeA();
        for (i=0;i<param;i++);
        PinLedLife_1 = 0;
        PinLedLife_2 = 1;
        writeC();
        for (i=0;i<param;i++);
        PinLedLife_1 = 1;
        PinLedLife_2 = 1;
        writeE();
        for (i=0;i<param;i++);
        PinLedLife_1 = 1;
        PinLedLife_2 = 0;
        writeF();
        for (i=0;i<param;i++);
        PinLedLife_1 = 0;
        PinLedLife_2 = 0;
        }

return TEST_LED_VAL;
}

/**
 ******************************************************************************
 ** \brief  Stop the relè
 **
 ** \retval Ok     The data in buffer1 are same with that in buffer2
 ******************************************************************************/
int blockRele()
{
int i= 0; //index to turn
int param = 5000000;

    PinLedLife_1 = 0;
    PinLedLife_2 = 0;

    PinRelePS = RELE_OFF;
	PinReleUF = RELE_OFF;
	PinRelePinza = RELE_OFF;


    while (1){
        writeA();
        for (i=0;i<param;i++);
        PinLedLife_1 = 0;
        PinLedLife_2 = 1;
        writeC();
        for (i=0;i<param;i++);
        PinLedLife_1 = 1;
        PinLedLife_2 = 1;
        writeE();
        for (i=0;i<param;i++);
        PinLedLife_1 = 1;
        PinLedLife_2 = 0;
        writeF();
        for (i=0;i<param;i++);
        PinLedLife_1 = 0;
        PinLedLife_2 = 0;
        }

return 0;
}

/**
 ******************************************************************************
 ** \brief  Compare each data in the input two buffers
 **
 ** \relval Ok     The data in buffer1 are same with that in buffer2
 ** \relval Error  The data in buffer1 are not same with that in buffer2
 ******************************************************************************/
bool CompareData(uint8_t* pBuf1, uint8_t* pBuf2, uint8_t u8Length)
{
    while(u8Length--)
    {
        if(*pBuf1++ != *pBuf2++)
        {
            return false;
        }
    }

    return true;
}


