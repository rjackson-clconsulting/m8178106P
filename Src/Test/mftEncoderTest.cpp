/**
@file		mftEncoderTest.cpp
@brief		Module to implement some encoder test code using MTF
@author		Fernando Morani
@date		11/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#include "mftEncoderTest.h"

/*****************************************************
** GLOBAL AND STATIC VAR SECTION
*****************************************************/
mftUnit *pumpsPS_UF;  //Select MFT Unit to manage PS pump input and UF pump input

/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/


/*******************************************************
** encoderInit(void)
** \brief: Init the MFT unit to serve PS and UF encoders
** \return: Error if it fails
*******************************************************/
uint8_t encoderInit(void)
{
    uint8_t u8RetVal = MFT_INIT_OK;
    pumpsPS_UF = new mftUnit(PS_UF_MFT_UNIT_SELECTED); //Object for both PS and UF, because they belong to the same MFT Unit
    if (pumpsPS_UF->isInstalled == true){
        //Here is the PS Setting
        //PS is associated with the MFT Unit 1, ICU Unit 1, channel 0
        //Ps use the FRT 0 of the MFT Unit 1
        pumpsPS_UF->setTSCARegister(PS_ASSOCIATE_FREE_COUNTER,CYCLE_RATIO_128,CANCEL_REQUEST,UP_MODE,STOPPING_MODE,
                        BUFFER_DISABLE,INT_ON_MATCH_DISABLED,INT_ON_MATCH_DISABLED,INTERNAL_CLOCK);
        pumpsPS_UF->setFrtPeakRegister(PS_ASSOCIATE_FREE_COUNTER,PS_ROUND_VALUE);
        //Counter is updated at every front (rise and fall)
        pumpsPS_UF->setIcuRegister(PS_ICU_UNIT_IN_MFT,PS_ICU_CHANNEL_IN_MFT,ICU_OPER_RISE_FALL_ENABLE,ICU_NOT_GENERATE_INTERRUPT);
        pumpsPS_UF->assignFRTtoICU(PS_ASSIGN_ICU_TO_FRT,PS_ICU_UNIT_IN_MFT,PS_ICU_CHANNEL_IN_MFT);
        //Starts the counter
        pumpsPS_UF->startsFrtCounting(PS_ASSOCIATE_FREE_COUNTER);

        //Here is the UF Setting
        //UF is associated with the MFT Unit 1, ICU Unit 0, channel 1
        //UF use the FRT 1 of the MFT Unit 1
        pumpsPS_UF->setTSCARegister(UF_ASSOCIATE_FREE_COUNTER,CYCLE_RATIO_128,CANCEL_REQUEST,UP_MODE,STOPPING_MODE,
                        BUFFER_DISABLE,INT_ON_MATCH_DISABLED,INT_ON_MATCH_DISABLED,INTERNAL_CLOCK);
        pumpsPS_UF->setFrtPeakRegister(UF_ASSOCIATE_FREE_COUNTER,UF_ROUND_VALUE);
        //Counter is updated at every front (rise and fall)
        pumpsPS_UF->setIcuRegister(UF_ICU_UNIT_IN_MFT,UF_ICU_CHANNEL_IN_MFT,ICU_OPER_RISE_FALL_ENABLE,ICU_NOT_GENERATE_INTERRUPT);
        pumpsPS_UF->assignFRTtoICU(UF_ASSIGN_ICU_TO_FRT,UF_ICU_UNIT_IN_MFT,UF_ICU_CHANNEL_IN_MFT);
        //Starts the counter
        pumpsPS_UF->startsFrtCounting(UF_ASSOCIATE_FREE_COUNTER);
        }
        else{
            //Error to manage - MFT not initialized
            u8RetVal = MFT_INIT_FAIL;
            pumpsPS_UF->~mftUnit();
            }

    return u8RetVal;
}

/*******************************************************
** PS_Read()
** \brief: Read the encoder value connected to PS
** \return: Error if it fails
*******************************************************/
uint16_t PS_Read()
{
    uint16_t retVal = 0;

    if (pumpsPS_UF->isInstalled == true)
        retVal = pumpsPS_UF->getIcuICCP(PS_ICU_UNIT_IN_MFT,PS_ICU_CHANNEL_IN_MFT);

    return retVal;
}
/*******************************************************
** UF_Read()
** \brief: Read the encoder value connected to UF
** \return: Error if it fails
*******************************************************/
uint16_t UF_Read()
{
    uint16_t retVal = 0;

    if (pumpsPS_UF->isInstalled == true)
        retVal = pumpsPS_UF->getIcuICCP(UF_ICU_UNIT_IN_MFT,UF_ICU_CHANNEL_IN_MFT);

    return retVal;
}

