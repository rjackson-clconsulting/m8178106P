/**
@file		mftEncoderPSUF.cpp
@brief		Module to implement encoder reading using MTF - For PS and UF
@author		Fernando Morani
@date		13/11/2017
@version	01.00
@source     M75-CPU Protective
*/
/**
NOTES on version 01.00
In this file we can calculate the time period between two falling edge of the signal coming
from the encoder connected to both the PS (Aspirazione) and UF (mandata) motors
ICU reading works on interrupt. The ISR update the following variables:
g_PSMainCounter and g_UFMainCounter incremented each front intercepted
g_PSFreqCounter and g_UFFreqCounter that stores the period between 2 front are detected
**/

#include "mftEncoderPSUF.h"

/*****************************************************
** GLOBAL AND STATIC VAR SECTION
*****************************************************/
mftUnit *pumpsPS_UF;  //Select MFT Unit to manage PS pump input and UF pump input
float   fUFBasePeriod;
#ifdef UF_FREQUENCY_CALC
uint32_t g_UFIntCounter;
#endif
uint32_t g_UFPeakIntCounter;
float   fPSBasePeriod;
#ifdef PS_FREQUENCY_CALC
uint32_t g_PSIntCounter;
#endif
uint32_t g_PSPeakIntCounter;

/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/
extern uint32_t g_PSMainCounter;
extern uint32_t g_PSFreqCounter;
extern uint32_t g_UFMainCounter;
extern uint32_t g_UFFreqCounter;
//extern uint32_t g_freqPS;
//extern uint32_t g_freqUF;


/*******************************************************
** encoderInit(void)
** \brief: Init the MFT unit to serve PS and UF encoders
** \return: Error if it fails
*******************************************************/
uint8_t encoderInit(void)
{
    uint8_t u8RetVal = MFT_INIT_OK;
    int i =0;
    uint16_t u16MultiplyFactor = 1;
    float fPclk = 30000000.0;
    uint16_t u16TCCPValue = 0;
    float fPeriodTCCPVal = 0.0;
    float fUFBaseFreq = 0.0;
    float fPSBaseFreq = 0.0;

    pumpsPS_UF = new mftUnit(PS_UF_MFT_UNIT_SELECTED); //Object for both PS and UF, because they belong to the same MFT Unit
    if (pumpsPS_UF->isInstalled == true){
        /**Here is the PS Setting
        PS is associated with the MFT Unit 1, ICU Unit 1, channel 0
        Ps use the FRT 0 of the MFT Unit 1*/

        pumpsPS_UF->setTSCARegister(PS_ASSOCIATE_FREE_COUNTER,
                                      PS_ASSOCIATE_FRT_CLK_MULTIPLIER,
                                      CANCEL_REQUEST,
                                      UP_MODE,
                                      STOPPING_MODE,
                                      BUFFER_DISABLE,
                                      INT_ON_MATCH_ENABLED,
                                      INT_ON_MATCH_DISABLED,
                                      INTERNAL_CLOCK);



        pumpsPS_UF->setFrtPeakRegister(PS_ASSOCIATE_FREE_COUNTER,
                                         PS_ROUND_VALUE);

        /**Counter is updated at every fall front
        ICU generates interrupt*/

        pumpsPS_UF->setIcuRegister(PS_ICU_UNIT_IN_MFT,
                                     PS_ICU_CHANNEL_IN_MFT,
                                     ICU_OPER_FALL_ENABLE,
                                     ICU_GENERATE_INTERRUPT);



        pumpsPS_UF->assignFRTtoICU(PS_ASSIGN_ICU_TO_FRT,
                                    PS_ICU_UNIT_IN_MFT,
                                    PS_ICU_CHANNEL_IN_MFT);

        /**
            attach the ISR
        **/
        pumpsPS_UF->attachICUCallback( PS_ISR,
                                        PS_ICU_UNIT_IN_MFT,
                                        PS_ICU_CHANNEL_IN_MFT);


        pumpsPS_UF->attachFrtCallback( PS_PEAK_INT_ISR,
                                        PS_ASSOCIATE_FREE_COUNTER);



        if (PS_ASSOCIATE_FRT_CLK_MULTIPLIER != CYCLE_RATIO_1){
            if(PS_ASSOCIATE_FRT_CLK_MULTIPLIER == CYCLE_RATIO_256){
                u16MultiplyFactor = 256;
            }else{
                for (i=1; i<PS_ASSOCIATE_FRT_CLK_MULTIPLIER+1;i++){
                    //Power calculation
                    u16MultiplyFactor = u16MultiplyFactor*2;
                }
            }
        }else {
            u16MultiplyFactor = 1;
        }

        fPSBasePeriod = 1/fPclk;
        fPSBasePeriod = fPSBasePeriod*u16MultiplyFactor;
        fPSBaseFreq = fPclk/u16MultiplyFactor;

        fPeriodTCCPVal = ((fPSBaseFreq)*PS_FRT_SET_VAL);//50 ms

        u16TCCPValue = fPeriodTCCPVal -1;

        pumpsPS_UF->setFrtPeakRegister(PS_ASSOCIATE_FREE_COUNTER,
                                        u16TCCPValue);


        //Starts the counter
        pumpsPS_UF->startsFrtCounting(PS_ASSOCIATE_FREE_COUNTER);

        /**Reset the value i have to reuse*/
        u16MultiplyFactor = 1;
        fPeriodTCCPVal = 0;
        u16TCCPValue = 0;



        /**Here is the UF Setting
        UF is associated with the MFT Unit 1, ICU Unit 0, channel 1
        UF use the FRT 1 of the MFT Unit 1*/
        pumpsPS_UF->setTSCARegister(UF_ASSOCIATE_FREE_COUNTER,
                                      UF_ASSOCIATE_FRT_CLK_MULTIPLIER,
                                      ISSUE_REQUEST,
                                      UP_MODE,
                                      STOPPING_MODE,
                                      BUFFER_DISABLE,
                                      INT_ON_MATCH_ENABLED,
                                      INT_ON_MATCH_DISABLED,
                                      INTERNAL_CLOCK);



        /**Counter is updated at every fall front
        ICU generates interrupt*/
        pumpsPS_UF->setIcuRegister(UF_ICU_UNIT_IN_MFT,
                                     UF_ICU_CHANNEL_IN_MFT,
                                     ICU_OPER_FALL_ENABLE,
                                     ICU_GENERATE_INTERRUPT);



        pumpsPS_UF->assignFRTtoICU(UF_ASSIGN_ICU_TO_FRT,
                                    UF_ICU_UNIT_IN_MFT,
                                    UF_ICU_CHANNEL_IN_MFT);


        /**
            attach the ISR
        **/
        pumpsPS_UF->attachICUCallback( UF_ISR,
                                        UF_ICU_UNIT_IN_MFT,
                                        UF_ICU_CHANNEL_IN_MFT);

        pumpsPS_UF->attachFrtCallback( UF_PEAK_INT_ISR,
                                        UF_ASSOCIATE_FREE_COUNTER);


        if (UF_ASSOCIATE_FRT_CLK_MULTIPLIER != CYCLE_RATIO_1){
            if(UF_ASSOCIATE_FRT_CLK_MULTIPLIER == CYCLE_RATIO_256){
                u16MultiplyFactor = 256;
            }else{
                for (i=1; i<UF_ASSOCIATE_FRT_CLK_MULTIPLIER+1;i++){
                //Power calculation
                u16MultiplyFactor = u16MultiplyFactor*2;
                }
            }
        }else {
            u16MultiplyFactor = 1;
        }

    fUFBasePeriod = 1/fPclk;
    fUFBasePeriod = fUFBasePeriod*u16MultiplyFactor;
    fUFBaseFreq = fPclk/u16MultiplyFactor;

    fPeriodTCCPVal = ((fUFBaseFreq)*UF_FRT_SET_VAL);//50 ms

    u16TCCPValue = fPeriodTCCPVal -1;

    pumpsPS_UF->setFrtPeakRegister(UF_ASSOCIATE_FREE_COUNTER,
                                        u16TCCPValue);


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

/*******************************************************
** PS_ISR()
** \brief: Serves the irq coming from ICU for PS Motor
** \return: 0
/*******************************************************/
void PS_ISR()
{
    int iRetVal = NOT_CLEARED_REGISTER;
    uint32_t u32TempVal = 0;
    float fPeriodPS = 0.0;
    bool bValidEdge = false;

    u32TempVal = PS_Read();


    if (u32TempVal != 0xFFFF){
            //Valid edge
            bValidEdge = true;
        }

    //I'm here to answer the IRQ - I clean the interrupt source
    iRetVal = pumpsPS_UF->clearICUICP(PS_ICU_UNIT_IN_MFT,
                            PS_ICU_CHANNEL_IN_MFT);

    if (iRetVal == 0xFF ){
        writeF(); //We've got failure
        while (1); //We've got serious problem - Please check
    }else{
        if (iRetVal == CLEARED_REGISTER){
            //It was my irq
            if (bValidEdge == true){
                #ifdef PS_FREQUENCY_CALC
                        //Clear the counter
                pumpsPS_UF->stopsFrtCounting(PS_ASSOCIATE_FREE_COUNTER);
                pumpsPS_UF->clearFrtCounting(PS_ASSOCIATE_FREE_COUNTER);
                pumpsPS_UF->startsFrtCounting(PS_ASSOCIATE_FREE_COUNTER);

                g_PSIntCounter = (g_PSPeakIntCounter * PS_ROUND_VALUE) + u32TempVal;
                fPeriodPS = g_PSIntCounter*fPSBasePeriod;
                g_freqPS = fPeriodPS * 1000;  //From second to ms
                g_PSIntCounter = 0;
                #endif
                g_PSPeakIntCounter = 0;
                g_PSMainCounter++;
            }else {
                //Edge is not valid
            }
        }else{
            //Dummy
            iRetVal = 20;
        }
    }
    return;

}


/*******************************************************
** UF_ISR()
** \brief: Serves the irq coming from ICU for UF Motor
** \return: 0
/*******************************************************/
void UF_ISR()
{
    int iRetVal = NOT_CLEARED_REGISTER;
    uint32_t u32TempVal = 0;
    float fPeriodUF = 0.0;
    bool bValidEdge = false;

    u32TempVal = UF_Read();


    if (u32TempVal != 0xFFFF){
            //Valid edge
            bValidEdge = true;
        }

    //I'm here to answer the IRQ - I clean the interrupt source
    iRetVal = pumpsPS_UF->clearICUICP(UF_ICU_UNIT_IN_MFT,
                            UF_ICU_CHANNEL_IN_MFT);

    if (iRetVal == 0xFF ){
        writeF(); //We've got failure
        while (1); //We've got serious problem - Please check
    }else{
        if (iRetVal == CLEARED_REGISTER){
            //It was my irq
            if (bValidEdge == true){

                #ifdef UF_FREQUENCY_CALC
                //Clear the counter
                pumpsPS_UF->stopsFrtCounting(UF_ASSOCIATE_FREE_COUNTER);
                pumpsPS_UF->clearFrtCounting(UF_ASSOCIATE_FREE_COUNTER);
                pumpsPS_UF->startsFrtCounting(UF_ASSOCIATE_FREE_COUNTER);*/

                g_UFIntCounter = (g_UFPeakIntCounter * UF_ROUND_VALUE) + u32TempVal;
                fPeriodUF = g_UFIntCounter*fUFBasePeriod;
                g_freqUF = fPeriodUF * 1000000;  //From second to ms
                g_UFIntCounter = 0;
                #endif
                g_UFPeakIntCounter = 0;
                g_UFMainCounter++;
            }else {
                //Edge is not valid
            }
        }else{
            //Dummy
            iRetVal = 20;
        }
    }
    return;
}


/*******************************************************
** UF_PEAK_INT_ISR
** \brief: Serves the irq coming from FRT for UF -
** it intercepts the PEAK
** \return: 0
/*******************************************************/
void UF_PEAK_INT_ISR(frtChannel_Type chan)
{
/**A FRT interrupt has been detected for this channel
Verify if is it plausible for this channel*/

    bool found = false;
    uint8_t regVal = 0;



    if (pumpsPS_UF->isFrtPeakInterruptEnabled(UF_ASSOCIATE_FREE_COUNTER) == true){
        /**Frt Peak is enabled .. let's verify
        Read the register*/
        regVal = pumpsPS_UF->readFRTICLR(UF_ASSOCIATE_FREE_COUNTER);
        if (regVal != 0xFF) {
            //Read the value of reg
            if (regVal == NOT_CLEARED_REGISTER){
                // The interrupt has been issued
                found = true;

                // Insert code to serve the peak reached
                g_UFPeakIntCounter++;

                if (g_UFMainCounter >0){
                    /***Main counter multiplied by 2 to keep coerency with old implementation
                    that checked both rise and fall front. In the controller we can't monitor
                    both rise and foll front because we are using external interrupt resource and
                    not MFT resource as we are doing in protective*/
                    g_UFMainCounter = g_UFMainCounter*UF_CONTROLLER_ALIGNEMENT;
                    g_UFFreqCounter = g_UFMainCounter;
                    g_UFMainCounter = 0;
                }else{
                    g_UFFreqCounter = 0;
                }

                if (g_UFPeakIntCounter >= UF_STOPPED_PRESUMED){
                    //Presumed no activities on ICU
                    g_UFPeakIntCounter = 0;
                    #ifdef  UF_FREQUENCY_CALC
                    g_UFIntCounter = 0;
                    g_freqUF = 0;
                    #endif
                    g_UFMainCounter = 0;
                }

                //Clear the interrupt
                pumpsPS_UF->clearFRTICLR(UF_ASSOCIATE_FREE_COUNTER);
                }
            }
        }

        if (found == false){
         // Interrupt cathced but not served...
         regVal = 0xFF;
         }



return;

}


/*******************************************************
** PS_PEAK_INT_ISR
** \brief: Serves the irq coming from FRT for PS -
** it intercepts the PEAK
** \return: 0
/*******************************************************/
void PS_PEAK_INT_ISR(frtChannel_Type chan)
{
//A FRT interrupt has been detected for this channel
//Verify if is it plausible for this channel
    bool found = false;
    uint8_t regVal = 0;
    bool bPeakEnabled = false;
    bool bNrzEnabled = false;


    if (pumpsPS_UF->isFrtPeakInterruptEnabled(PS_ASSOCIATE_FREE_COUNTER) == true){
        //Frt Peak is enabled .. let's verify
        bPeakEnabled = true;
        //Read the register
        regVal = pumpsPS_UF->readFRTICLR(PS_ASSOCIATE_FREE_COUNTER);
        if (regVal != 0xFF) {
            //Read the value of reg
            if (regVal == NOT_CLEARED_REGISTER){
                // The interrupt has been issued
                found = true;
                // Insert code to serve the peak reached
                g_PSPeakIntCounter++;

                if (g_PSMainCounter >0){
                    /***Main counter multiplied by 2 to keep coerency with old implementation
                    that checked both rise and fall front. In the controller we can't monitor
                    both rise and foll front because we are using external interrupt resource and
                    not MFT resource as we are doing in protective*/
                    g_PSMainCounter = g_PSMainCounter*PS_CONTROLLER_ALIGNEMENT;
                    g_PSFreqCounter = g_PSMainCounter;
                    g_PSMainCounter = 0;
                }else{
                    g_PSFreqCounter = 0;
                }


                if (g_PSPeakIntCounter >= PS_STOPPED_PRESUMED){
                    //Presumed no activities on ICU
                    g_PSPeakIntCounter = 0;
                     #ifdef  PS_FREQUENCY_CALC
                    g_PSIntCounter = 0;
                    g_freqPS = 0;
                    #endif
                    g_PSMainCounter = 0;
                }
                //Clear the interrupt
                pumpsPS_UF->clearFRTICLR(PS_ASSOCIATE_FREE_COUNTER);
                }
            }
        }

        if (found == false){
         // Interrupt cathced but not served...
         regVal = 0xFF;
         }

return;
}
