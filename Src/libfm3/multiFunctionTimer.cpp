/**
@file		multiFunctionTimer.cpp
@brief		Useful routines to use multiFunctionTimer of the Fujitsu M3 processor
@author		Fernando Morani
@date		06/13/2018
@version	01.23
@source     FM3 Base TYPE 2
*/
/**
This class has been built following the MFT programming instruction written in the chapter 6 of
FM3 Family PERIPHERAL MANUAL Timer Part
HYSTORY:
****************************
VER: 01.00: The Version 01.00 contains the fully capability to Program the FRT (free running counter),
the OCU (capability to generate the PWM) and the WFG (profiling and generate the waveform to output).
This version work with up to 3 Unit of MFT.
This version do not contains any utilities and methods to work with ICU, to interface with PPG input signal
and to use AD Converter.
Exemple use at the bottom of this file fully commented.
**********************************************************
VER:01.10: The version 01.10 add the management of the ICU
NOTES OR LIMITATIONS:
A) This class don't set the I/O port. Do it externally (i.e. file board.cpp).
B) The automatic assigmnent to the OCU channel giving the PIN Name as input is only valid
for the MFT Unit 0. Don't use it for other MFT Units.
C) The automatic assignment to the ICU channel giving the PIN name as input has not been implemented
D) The Interrupt vector should be enabled whith:
	NVIC_ClearPendingIRQ(MFT_0_IRQn);
	NVIC_EnableIRQ(MFT_0_IRQn);
	NVIC_SetPriority(MFT_0_IRQn, 15);

	NVIC_ClearPendingIRQ(MFT_1_IRQn);
	NVIC_EnableIRQ(MFT_1_IRQn);
	NVIC_SetPriority(MFT_1_IRQn, 15);

	NVIC_ClearPendingIRQ(MFT_2_IRQn);
	NVIC_EnableIRQ(MFT_2_IRQn);
	NVIC_SetPriority(MFT_2_IRQn, 15);

    It could be done here or in the main function or in the init of the system
E) How to programa a callback routine is shown at the bottom of the file:
    E1) Callback routine serving FRT Interrupt
    E2) Callback routine serving ICU Interrupt
F)  Management of OCU routine is not yet implemented
*******************************************************************************
VER:01.20: The version 01.20 is done to use the driver with FM3 TYPE 0 board
Modification: In the header file CMIS include file depends on the board type
Bug Fix: icuHAsFRTConnection vector is correctly initialized
*******************************************************************************
*******************************************************************************
VER:01.21:
Bug Fix: The ICU ISR _IRQ_HANDLER(MFT_ICU_IRQ) has been corrected
the define are in board_define.h
i.e.: #define MFT_ICU_1_IRQn          MFT0_2_ICU_IRQn
Corrected the ICU ISR management
*******************************************************************************
*******************************************************************************
VER:01.22:
Bug Fix: Modified methods getIcuICCP and clearICUICP
*******************************************************************************
*******************************************************************************
VER:01.23:
Bug Fix: Modified clearFRTICLR
*******************************************************************************
*/
#include "multiFunctionTimer.h"


//Global Variable to use
uint8_t mftInstalled[MAX_MFT_NUMBER] = {MFT_UNINSTALLED,MFT_UNINSTALLED,MFT_UNINSTALLED};
static void (*IRQ_MFT_FRT_callback_p[MAX_MFT_NUMBER][MAX_FRT_CHANELL_FOR_UNIT])(frtChannel_Type);  //Used for the FRT PEAK interrupt
static void (*IRQ_MFT_FRT_NRZ_callback_p[MAX_MFT_NUMBER][MAX_FRT_CHANELL_FOR_UNIT])(frtChannel_Type);  //Used for the FRT NRZ interrupt
static void (*IRQ_MFT_ICU_callback_p[MAX_MFT_NUMBER][MAX_ICU_UNIT_FOR_MFT_UNIT][MAX_ICU_CHANNEL_FOR_ICU_UNIT])();  //Used for the ICU interrupt

/** Class constructor. */
mftUnit::mftUnit(uint8_t unit)
{
uint8_t i = 0; //iterator
    mftID = MFT_WRONG_ID;
    //We can proceed
    if ((unit > MAX_MFT_NUMBER-1) || (unit < 0))
        {
        //Validating parameter
        //Unit should be included 0 <= Unit <= MAX_MFT_NUMBER-1
        isInstalled = false;
        return;
        }
    else
        {
        //Parameter is validated
        if (mftInstalled[unit] == MFT_INSTALLED)
        {
        //This Unit has been already Installed
        isInstalled = false;
        return;
        }
        else
        {
        //Here we configure
        mftInstalled[unit] = MFT_INSTALLED;
        isInstalled = true;
        mftID = unit;
        switch (mftID)
        {
        case MFT_UNIT_0:
        mftBlock.pmftBaseReference = FM3_MFT0;
        break;
        case MFT_UNIT_1:
        mftBlock.pmftBaseReference = FM3_MFT1;
        break;
        case MFT_UNIT_2:
        mftBlock.pmftBaseReference = FM3_MFT2;
        break;
        default:
        break;
        }
        mftBlock.pmftOcuBaseReference = (FM3_MFT_OCU_TypeDef *)(mftBlock.pmftBaseReference + FM3_MFT_OCU_BASE_OFFSET);  //Set the base addeess for OCU
        mftBlock.pmftWfgBaseReference = (FM3_MFT_WFG_TypeDef *)(mftBlock.pmftBaseReference + FM3_MFT_WFG_BASE_OFFSET);  //Set the base addeess for WFG
        mftBlock.pmftIcuBaseReference = (FM3_MFT_ICU_TypeDef *)(mftBlock.pmftBaseReference + FM3_MFT_ICU_BASE_OFFSET);  //Set the base addeess for ICU
        for (i=0;i<NUMBER_OF_OUTPUT_PIN;i++){
            ocuHasFRTConnection[i] = OCU_ICU_CONNECT_ERROR;
            }
        for (i=0;i<NUMBER_OF_INPUT_PIN;i++){
            icuHasFRTConnection[i] = OCU_ICU_CONNECT_ERROR;
            }
        initOCURegsiter();
        initFRTRegister();
        initWFGRegister();
        initICURegister();
        assignRTPin();
        }
        }
}
/**getID Method*/
uint8_t mftUnit::getID()
{
    return mftID;
}
/**
setCycleRadio Method:
Set the Up/Down counter frequency by using the clock ratio selected (ratioToSet) for the selected
FRT unit (chan)
FRT's count clock cycle is determined based on the PCLK cycle and the clock division ratio set by this register.
ratioToSet setting out of the cycleRatio_Type domain is prohibited
Return retVal = ratioToSet
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setFrtCycleRatio(frtChannel_Type chan, cycleRatio_Type ratioToSet)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->CLK = ratioToSet;
retVal = static_cast<uint8_t>(ratioToSet);
return retVal;
}

/**
setFrtInitRequest Method:
Issue or Cancel an initialization for FRT
Return retVal = request, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setFrtInitRequest(frtChannel_Type chan, frtInitRequest_Type request )
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

tscaSelected->SCLR = request;
retVal = static_cast<uint8_t>(request);
return retVal;
}

/**
setFrtCounterMode Method:
Set the count mode of the selected FRT
Return retVal = mode, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setFrtCounterMode(frtChannel_Type chan, frtCounterMode_Type mode )
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->MODE = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}

/**
setFrtOperMode Method:
Set the operating mode of the selected FRT
Return retVal = mode, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setFrtOperMode(frtChannel_Type chan, frtOperMode_Type mode )
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];
tscaSelected->STOP = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}

/**
setFrtBufferFunction Method:
Set the operating mode of the selected FRT
Return retVal = mode, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setFrtBufferFunction(frtChannel_Type chan, frtBufferFunction_Type mode )
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->BFE = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}

/**
clearFRTIRQZF
Clear the IRQZF Regsiter
*/
void mftUnit::clearFRTIRQZF(frtChannel_Type chan)
{
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->IRQZF = CLEARED_REGISTER;   //Writing Zero cler the register
}

/**
readFRTIRQZF
read the IRQZF Regsiter
Return the value of the register or 0xFF if error
*/
uint8_t mftUnit::readFRTIRQZF(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected != NULL){
    //OK can read the register
    retVal = tscaSelected->IRQZF;//Read the register
    }

    return retVal;
}


/**
clearFRTICLR
Clear the ICLR Regsiter
*/
void mftUnit::clearFRTICLR(frtChannel_Type chan)
{
uint16_t u16TempVal = 0;
uint16_t u16RetVal = 0;
bool bResult = false;
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

u16TempVal = tscaSelected->ALL_BITS;
u16TempVal = u16TempVal & TSCA_ICLR_MASK; //ICLR Mask

/**We want verify if the IRQ has been rised*/
if (u16TempVal != 0){
    //ICLR should be cleared
    u16TempVal = tscaSelected->ALL_BITS;
    u16TempVal = u16TempVal & ~(TSCA_ICLR_MASK);
    u16RetVal = u16TempVal;
    tscaSelected->ALL_BITS = 0xFFFF;
    tscaSelected->ALL_BITS = u16RetVal;

}else{
    bResult = false;
}

return;
}

/**
readFRTICLR
read the ICLR Regsiter
Return the value of the register
*/
uint8_t mftUnit::readFRTICLR(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected != NULL){
    //OK can read the register
    retVal = tscaSelected->ICLR;//Read the register
    }

    return retVal;
}

/**
setFrtIntOnMatch
Enable/Disable the interrupt generation when counter reach the Peak
Return mode if OK
Return 0xFF is some error occurs
*/
uint8_t mftUnit::setFrtIntOnMatch(frtChannel_Type chan,frtEnableIntOnMatch_Type mode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->ICRE = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}

/**
isFrtPeakInterruptEnabled
Verify if the interrupt generation when counter reaches the peak is enabled/disabled
Return 0 if the interrupt is disabled
Return 1 if the interrupt is enabled
Return 0xFF if error occurs
*/
uint8_t mftUnit::isFrtPeakInterruptEnabled(frtChannel_Type chan)
{
uint8_t retVal = 0;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected == NULL)
{
    // Value not plausible
    retVal = 0xFF;
}
else
{
    retVal = tscaSelected->ICRE;
}
return retVal;
}

/**
setFrtIntOnMatch_Z
Enable/Disable the interrupt generation when counter reach the Zero Value
Return mode if OK
Returb 0xFF is some error occurs
*/
uint8_t mftUnit::setFrtIntOnMatch_Z(frtChannel_Type chan,frtEnableIntOnMatch_Type mode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->IRQZE = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}

/**
isFrtZeroInterruptEnabled
Verify if the interrupt generation when counter reaches ZERO is enabled/disabled
Return 0 if the interrupt is disabled
Return 1 if the interrupt is enabled
Return 0xFF if error occurs
*/
uint8_t mftUnit::isFrtZeroInterruptEnabled (frtChannel_Type chan)
{
uint8_t retVal = 0;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected == NULL)
{
    // Value not plausible
    retVal = 0xFF;
}
else
{
    retVal = tscaSelected->IRQZE;
}
return retVal;
}

/**
setFrtClock
Select the type of clock (External or Internal)
Return mode if OK
Return 0xFF is some error occur
*/
uint8_t mftUnit::setFrtClock(frtChannel_Type chan,frtClock_Type mode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected->SCLR == ISSUE_REQUEST)
    tscaSelected->SCLR = CANCEL_REQUEST;   //Rewriting value only if SCLR = 0
while(tscaSelected->SCLR != CANCEL_REQUEST);

tscaSelected->ECKE = mode;
retVal = static_cast<uint8_t>(mode);
return retVal;
}
/**
setTSCARegister
Set all the bit of the TSCA register
*/
uint8_t mftUnit::setTSCARegister(frtChannel_Type chan, cycleRatio_Type ratioToSet, frtInitRequest_Type request,
                            frtCounterMode_Type counterMode, frtOperMode_Type operMode, frtBufferFunction_Type bufferEnable,
                            frtEnableIntOnMatch_Type peakInt, frtEnableIntOnMatch_Type zeroInt, frtClock_Type clockSource )
{
setFrtCycleRatio(chan,ratioToSet);
setFrtInitRequest(chan,request);
setFrtCounterMode(chan,counterMode);
setFrtOperMode(chan,operMode);
setFrtBufferFunction(chan,bufferEnable);
setFrtIntOnMatch(chan,peakInt);
setFrtIntOnMatch_Z(chan,zeroInt);
setFrtClock(chan, clockSource);
clearFRTICLR(chan);
clearFRTIRQZF(chan);
}

/**
setOcuChannelOper
Enable or Disable the OCU channel operation
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::setOcuChannelOper(ocuUnit_Type unit, ocuChannel_Type chan, ocuOperation_Type oper)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
if (ocsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            ocsaSelected->CST0 = oper;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            ocsaSelected->CST1 = oper;
            }
            else{//No else necessary
            }
        }
return retVal;
}

/**
setOcuRegister
Set the OCU register OCSA and OCSB
return 0xFF if error occurs
*/
uint8_t mftUnit::setOcuRegister(ocuUnit_Type unit,ocuChannel_Type chan, ocuOperation_Type oper, ocuBufferState_Type state,
                            ocuGenerateInt_Type genIntState,ocuBufferTransfer_Type transfer,ocuRTOutputLevel_Type level)
{
uint8_t retVal = 0xFF;

retVal = setOcuChannelBufferState(unit, chan, state);
if (retVal != 0xFF){
        retVal = setOcuChannelGenerateInt(unit, chan, genIntState);
        if (retVal != 0xFF){
                retVal = clearOCUIOP(unit,chan);
                if (retVal != 0xFF){
                    retVal = setOcuChannelOper(unit, chan, oper);
                    if (retVal != 0xFF){
                        retVal = setOcuOutputLevel(unit,chan, level);
                        if (retVal !=0xFF){
                            retVal = setOcuBufferTransfer(unit, chan,transfer);
                            }
                        }
                    }
                }
        }
        return retVal;
}

/**
setOcuOutputLevel
Set the output level of the OCU
*/
uint8_t mftUnit::setOcuOutputLevel(ocuUnit_Type unit, ocuChannel_Type chan, ocuRTOutputLevel_Type level)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsb_field_t *ocsbSelected = NULL;
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
ocsbSelected = mftBlock.ocsb_Register[unit];
if ((ocsaSelected == NULL) || (ocsbSelected == NULL)){
    retVal = 0xFF;
    }
    else{
    //The output level of the OCU output pins (RT0 to RT5) can be set by writing to these bits
    // when OCU's operation is disabled
        if (chan == OCU_CHANNEL_0){
            if (ocsaSelected->CST0 == OCU_OPER_ENABLED){
                ocsaSelected->CST0 = OCU_OPER_DISABLED;
                }
                //else not needed
            ocsbSelected->OTD0 = level;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            if (ocsaSelected->CST1 == OCU_OPER_ENABLED){
                ocsaSelected->CST1 = OCU_OPER_DISABLED;
                }
                //else not needed
            ocsbSelected->OTD1 = level;
            }
            else{//No else necessary
            }
        }
return retVal;

}

/**
getOcuOutputLevel
Get the output level of the OCU
Return:
0 if OCU si set at low output level
1 if OCU si set at high output level
0xFF in case of error
*/
uint8_t mftUnit::getOcuOutputLevel(ocuUnit_Type unit, ocuChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_ocu_ocsb_field_t *ocsbSelected = NULL;
ocsbSelected = mftBlock.ocsb_Register[unit];
if (ocsbSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            retVal = ocsbSelected->OTD0;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            retVal = ocsbSelected->OTD1;
            }
            else{//No else necessary
            }
        }
return retVal;

}
/**
Set the OCSC Mode Register
Return the setting requested or 0xFF if error
*/
uint8_t mftUnit::setOCSCModeRegister(uint8_t position, uint8_t value)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsc_field_t *ocscSelected = NULL;
if ((position >= NUMBER_OF_OUTPUT_PIN) ||
    ((value != 0) && (value != 1))){
    retVal = 0xFF; //value not plausible
    }
    else {
        //Value is plausibile
        switch (position){
            case 0:
                ocscSelected->MOD0 = value;
                retVal = value;
                break;
            case 1:
                ocscSelected->MOD1 = value;
                retVal = value;
                break;
            case 2:
                ocscSelected->MOD2 = value;
                retVal = value;
                break;
            case 3:
                ocscSelected->MOD3 = value;
                retVal = value;
                break;
            case 4:
                ocscSelected->MOD4 = value;
                retVal = value;
                break;
            case 5:
                ocscSelected->MOD5 = value;
                retVal = value;
                break;
            default:
                retVal = 0xFF;
                break;
                }
        }
return retVal;
}

/**
setOcuOCCP
Set the OCCP register
Return 0xFF if error occurs
*/
uint8_t mftUnit::setOcuOCCP(ocuUnit_Type unit, ocuChannel_Type chan, uint16_t value)
{
uint8_t retVal = 0;
stc_mft_ocu_occp_field_t *occpSelected = NULL;


if (chan == OCU_CHANNEL_0){
    occpSelected = mftBlock.occp0_Register[unit];
    }
    else {
    occpSelected = mftBlock.occp1_Register[unit];
    }
    occpSelected->REGISTER = value;
return retVal;
}

/**
getOcuOCCP
Get the OCCP register value
Return the register value
*/
uint16_t mftUnit::getOcuOCCP(ocuUnit_Type unit, ocuChannel_Type chan)
{
uint16_t value = 0xEEEE;
stc_mft_ocu_occp_field_t *occpSelected = NULL;

if (chan == OCU_CHANNEL_0){
    occpSelected = mftBlock.occp0_Register[unit];
    }
    else {
    occpSelected = mftBlock.occp1_Register[unit];
    }
    value = occpSelected->REGISTER;
return value;
}


/**
Set the output mode for the OCU channel
Return 0 or 0xFF if error occurs
OCSB_10 is Unit = 0
OCSB_32 is Unit = 1
OCSB_54 is Unit = 2
*/
uint8_t mftUnit::setOcuModeUse(ocuUnit_Type unit, ocuChannel_Type chan, ocuModeUse_Type mode)
{
uint8_t retVal = 0;         //Return value
uint8_t varTraslator = 0;   //Find out the absolute channel number from 0 to 5
frtChannel_Type frtChannelConnected;
stc_mft_ocu_ocsb_field_t *ocsbSelected = NULL;
stc_mft_ocu_ocsc_field_t *ocscSelected = NULL;
stc_mft_frt_tcsa_field_t *tcsaSelected = NULL;

ocscSelected = mftBlock.ocsc_Register;
ocsbSelected = mftBlock.ocsb_Register[unit];
varTraslator =unit*2 + chan;

if ((ocscSelected == NULL) || (ocsbSelected == NULL)){
    retVal = 0xFF;
    }
    else{
    if (ocuHasFRTConnection[varTraslator] == OCU_ICU_CONNECT_ERROR){
        //No FRT has been assigned to this channel
        retVal = 0xFF;
        }
        else{
        frtChannelConnected = static_cast<frtChannel_Type>(ocuHasFRTConnection[varTraslator]);
        switch (mode){
            case UP_COUNT_MODE_CHANGE_1:
            //Setting Valid for both channel
            setFrtCounterMode(frtChannelConnected,UP_MODE);
            //Change the setting of these bits while OCU's operation is disabled.
            setOcuChannelOper(unit,chan,OCU_OPER_DISABLED);
            setOCSCModeRegister(varTraslator, 0); //UP_COUNT_MODE_CHANGE_1 means 0
            ocsbSelected->CMOD = 0; //See Table 4.6 OCU Operation Mode
            break;
            case UP_COUNT_MODE_CHANGE_2:
            if (chan == OCU_CHANNEL_0){
                retVal = 0xFF;
                //Mode not possible if chan 0
                }
                else{
                setFrtCounterMode(frtChannelConnected,UP_MODE);
                //Change the setting of these bits while OCU's operation is disabled.
                setOcuChannelOper(unit,chan,OCU_OPER_DISABLED);
                setOCSCModeRegister(varTraslator,0); //UP_COUNT_MODE_CHANGE_2 means 0
                ocsbSelected->CMOD = 1; //See Table 4.6 OCU Operation Mode
                }
            break;
            case UP_DOWN_COUNT_MODE_ACTIVE_HIGH:
            setFrtCounterMode(frtChannelConnected,UP_DOWN_MODE);
            //Change the setting of these bits while OCU's operation is disabled.
            setOcuChannelOper(unit,chan,OCU_OPER_DISABLED);
            setOCSCModeRegister(varTraslator,1);  //UP_DOWN_COUNT_MODE_ACTIVE_HIGH means 1
            ocsbSelected->CMOD = 0; //See Table 4.6 OCU Operation Mode
            break;
            case UP_DOWN_COUNT_MODE_ACTIVE_LOW:
            setFrtCounterMode(frtChannelConnected,UP_DOWN_MODE);
            //Change the setting of these bits while OCU's operation is disabled.
            setOcuChannelOper(unit,chan,OCU_OPER_DISABLED);
            setOCSCModeRegister(varTraslator,1);  //UP_DOWN_COUNT_MODE_ACTIVE_LOW means 1
            ocsbSelected->CMOD = 0; //See Table 4.6 OCU Operation Mode
            break;
            default:
            retVal = 0xFF; //Should not be here
            break;
                }
        }
    }
return retVal;

}

/**
Set the ADMITTED output mode for the OCU channel
Return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::setOcuAdmittedModeUse(ocuUnit_Type unit, ocuChannel_Type chan, ocuAdmittedMode_Type mode)
{
uint8_t retVal = 0;         //Return value
uint8_t varTraslator = 0;   //Find out the absolute channel number from 0 to 5
/*uint8_t moduloDiv = 0;      //Module Operator
uint8_t increaser = 1;
varTraslator =unit*2 + chan;

moduloDiv = varTraslator % 2;
if (moduloDiv == 0){
 //We have selected 0,2,4
 }else
 {
 //We have selected 1,3,5
 increaser = 0;
 }
*/
switch (mode){
    case OCU_ADMITTED_0:
        // Both channel of the unit has the same settings
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_COUNT_MODE_CHANGE_1);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_COUNT_MODE_CHANGE_1);
        break;
    case OCU_ADMITTED_1:
        // Both channel of the unit has the same settings
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_COUNT_MODE_CHANGE_1);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_COUNT_MODE_CHANGE_2);
        break;
    case OCU_ADMITTED_2:
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_DOWN_COUNT_MODE_ACTIVE_HIGH);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_COUNT_MODE_CHANGE_1);
        break;
    case OCU_ADMITTED_3:
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_COUNT_MODE_CHANGE_1);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_DOWN_COUNT_MODE_ACTIVE_HIGH);
        break;
    case OCU_ADMITTED_4:
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_COUNT_MODE_CHANGE_1);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_DOWN_COUNT_MODE_ACTIVE_LOW);
        break;
    case OCU_ADMITTED_5:
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_DOWN_COUNT_MODE_ACTIVE_HIGH);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_DOWN_COUNT_MODE_ACTIVE_HIGH);
        break;
    case OCU_ADMITTED_6:
        setOcuModeUse(unit,OCU_CHANNEL_0,UP_DOWN_COUNT_MODE_ACTIVE_LOW);
        setOcuModeUse(unit,OCU_CHANNEL_1,UP_DOWN_COUNT_MODE_ACTIVE_LOW);
        break;
    default:
    retVal = 0xFF;
        break;
}
return retVal;

}

/**
setOcuChannelBufferState
Enable or disbale the use of the buffer in the specified channel
Setting of these bits, should be done while OCU's operation is disabled
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::setOcuChannelBufferState(ocuUnit_Type unit, ocuChannel_Type chan, ocuBufferState_Type state)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
if (ocsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            if (ocsaSelected->CST0 == OCU_OPER_ENABLED )
                {
                    ocsaSelected->CST0 = OCU_OPER_DISABLED;
                    while (ocsaSelected->CST0 != OCU_OPER_DISABLED);
                    ocsaSelected->BDIS0 = state;
                    ocsaSelected->CST0 = OCU_OPER_ENABLED;  //Restore to previous state
                }
                else
                { ocsaSelected->BDIS0 = state;
                }
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            if (ocsaSelected->CST1 == OCU_OPER_ENABLED )
                {
                    ocsaSelected->CST1 = OCU_OPER_DISABLED;
                    while (ocsaSelected->CST1 != OCU_OPER_DISABLED);
                    ocsaSelected->BDIS1 = state;
                    ocsaSelected->CST1 = OCU_OPER_ENABLED;  //Restore to previous state
                }
                else
                { ocsaSelected->BDIS1 = state;
                }
            }
            else{//No else necessary
            }
        }
return retVal;
}

/**
setOcuChannelGenerateInt
Enable or disbale the interrupt generation based on the IOP value
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::setOcuChannelGenerateInt(ocuUnit_Type unit, ocuChannel_Type chan, ocuGenerateInt_Type state)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
if (ocsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            ocsaSelected->IOE0 = state;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            ocsaSelected->IOE1 = state;
            }
            else{//No else necessary
            }
        }
return retVal;
}

uint8_t mftUnit::setOcuBufferTransfer(ocuUnit_Type unit, ocuChannel_Type chan, ocuBufferTransfer_Type transfer)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsb_field_t *ocsbSelected = NULL;
ocsbSelected = mftBlock.ocsb_Register[unit];
if (ocsbSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            ocsbSelected->BTS0 = transfer;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            ocsbSelected->BTS1 = transfer;
            }
            else{//No else necessary
            }
        }
return retVal;
}
/**
setOcuModeRegister
set the OCU registers in order to select the operative mode requested
*/
uint8_t mftUnit::setOcuModeRegister(ocuUnit_Type unit, ocuChannel_Type chan, clearedRegister_Type ocsb, clearedRegister_Type ocsc)
{
uint8_t retVal = 0;
stc_mft_ocu_ocsb_field_t *ocsbSelected = NULL;
stc_mft_ocu_ocsc_field_t *ocscSelected = NULL;

ocsbSelected = mftBlock.ocsb_Register[unit];
ocscSelected = mftBlock.ocsc_Register;

if ((ocsbSelected == NULL) || (ocscSelected == NULL)){
    retVal = 0xFF;
    }
    else{
        ocsbSelected->CMOD = ocsb;
        if (chan == OCU_CHANNEL_0){
            ocsbSelected->CMOD = ocsb;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){

            }
            else{//No else necessary
            }
        }
return retVal;

}

/**
clearOCUIOP
Clear the IOP Register
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::clearOCUIOP(ocuUnit_Type unit, ocuChannel_Type chan)
{
uint8_t retVal = 0;         //Return value
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
if (ocsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            ocsaSelected->IOP0 = CLEARED_REGISTER;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            ocsaSelected->IOP1 = CLEARED_REGISTER;
            }
            else{//No else necessary
            }
        }
return retVal;
}

/**
Return the value of the interrupt flag matched
*/
uint8_t mftUnit::readOCUIOP(ocuUnit_Type unit, ocuChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_ocu_ocsa_field_t *ocsaSelected = NULL;
ocsaSelected = mftBlock.ocsa_Register[unit];
if (ocsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == OCU_CHANNEL_0){
            retVal = ocsaSelected->IOP0;
            }
            else{//No else necessary
            }
        if (chan == OCU_CHANNEL_1){
            retVal = ocsaSelected->IOP1;
            }
            else{//No else necessary
            }
        }
return retVal;
}

/**
startsFrtCounting
Starts the FRT Counting for the selected channels
Return retVal = 0, if nothing happens
Return retVal = 0xFF, if some error occurs or FRT already started
*/
uint8_t mftUnit::startsFrtCounting(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];
if (tscaSelected->STOP == STOPPING_MODE)
{
setFrtOperMode(chan, OPERATING_MODE );
setFrtInitRequest(chan, CANCEL_REQUEST);
retVal = 0;
}
else
{
//Already Started
}
return retVal;
}

/**
stopsFrtCounting
Stops teh FRT Counting fr the selected channels
Return retVal = 0, if nothing happens
Return retVal = 0xFF, if some error occurs or FRT already Stopped
*/
uint8_t mftUnit::stopsFrtCounting(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
stc_mft_frt_tcdt_field_t *tcdtSelected = NULL;

tscaSelected = mftBlock.tsca_Register[chan];
tcdtSelected = mftBlock.tcdt_Register[chan];

if (tscaSelected->STOP == OPERATING_MODE)
{
setFrtOperMode(chan, STOPPING_MODE );
setFrtInitRequest(chan, ISSUE_REQUEST);
tcdtSelected->FRT_TCDT = TCDT_CLEARED_VALUE;  //Clear the counter value
retVal = 0;
}
else
{
//Already Stopped
}
return retVal;
}

/**
clearFrtCounting
Clear the FRT Counting for the selected channels
Return retVal = 0, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::clearFrtCounting(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
stc_mft_frt_tcdt_field_t *tcdtSelected = NULL;

tscaSelected = mftBlock.tsca_Register[chan];
tcdtSelected = mftBlock.tcdt_Register[chan];

if ((tscaSelected->STOP == OPERATING_MODE) && (tscaSelected->MODE == UP_MODE))
{
//The claering is possible if the counter si running in UpMode
setFrtInitRequest(chan, ISSUE_REQUEST);
//Wait for the TCDT regsiter is cleared
while (tcdtSelected->FRT_TCDT != TCDT_CLEARED_VALUE);
//Restart the counter
setFrtInitRequest(chan, CANCEL_REQUEST);
retVal = 0;
}
else
{
//Clearing not possible
}
return retVal;
}
/**
getFrtCounterOutput
Return the value of the current free counter for the requested channel
*/

uint16_t mftUnit::getFrtCounterOutput(frtChannel_Type chan)
{
uint16_t retVal = TCDT_CLEARED_VALUE;
stc_mft_frt_tcdt_field_t *tcdtSelected = NULL;
tcdtSelected = mftBlock.tcdt_Register[chan];

if (tcdtSelected == 0)
    {
    //Value not plausible
    retVal = 0xFFFF;
    }
    else
    {
    retVal = tcdtSelected->FRT_TCDT;
    }
return retVal;
}

/**
getFrtCounterMode
Find out the FRT Counter mode
*/
uint8_t mftUnit::getFrtCounterMode(frtChannel_Type chan)
{
uint8_t retVal = 0xFF;

stc_mft_frt_tcsa_field_t *tscaSelected = NULL;
tscaSelected = mftBlock.tsca_Register[chan];

if (tscaSelected == NULL)
    {
    //Value not plausible
    retVal = 0xFF;
    }
    else
    {
    retVal = tscaSelected->MODE;
    }
return retVal;
}

/**
setFrtADCTrigger
Set if the ADC (0,1 or 2) should be connected to FRT channel trigger output (trigger = return to zero)
*/
uint8_t mftUnit::setFrtADCTrigger(frtChannel_Type chan, frtADCTrigger_Type enableTrig, frtADCOutput_Type adcOutput)
{
uint8_t retVal = 0;
stc_mft_frt_tcsb_field_t *tscbSelected = NULL;

tscbSelected = mftBlock.tscb_Register[chan];

//These registers are used to start ADC conversion upon Zero value detection by FRT.
//Each of the AD conversion start signals for the 3 channels of FRT undergoes logic OR by ADC unit to which
//they are to be output
//For the moment we don't put any control or verification about multiple FTR's connected to the same ADC
//If does not seem useful for us now
switch (adcOutput)
{
case FRT_ADC0:
tscbSelected->AD0E = enableTrig;
break;
case FRT_ADC1:
tscbSelected->AD1E = enableTrig;
break;
case FRT_ADC2:
tscbSelected->AD2E = enableTrig;
break;
default:
retVal = 0xFF; //Should not be here
break;
}
return retVal;
}

/**
isFrtADCTriggered
Return 0 if the adcOutput connected to the FRT chan is NOT triggered out
Return 1 if the adcOutput connected to the FRT chan is triggered out
Return 0xFF is error occurs
*/
uint8_t mftUnit::isFrtADCTriggered(frtChannel_Type chan, frtADCOutput_Type adcOutput)
{
uint8_t retVal = 0;
stc_mft_frt_tcsb_field_t *tscbSelected = NULL;

tscbSelected = mftBlock.tscb_Register[chan];

if (tscbSelected == NULL)
{
//Value not plausible
retVal = 0xFF;
}
else
{
switch (adcOutput)
{
case FRT_ADC0:
retVal = tscbSelected->AD0E;
break;
case FRT_ADC1:
retVal = tscbSelected->AD1E;
break;
case FRT_ADC2:
retVal = tscbSelected->AD2E;
break;
default:
retVal = 0xFF; //Should not be here
break;
}
}
return retVal;
}

/**
setFrtPeakRegister
Set the TCCP register of the FRT Channel choosen in order to set the peak value
*/
uint8_t mftUnit::setFrtPeakRegister(frtChannel_Type chan, uint16_t peakValue)
{
uint8_t retVal = 0;
stc_mft_frt_tccp_field_t *tccpSelected = NULL;

tccpSelected = mftBlock.tccp_Register[chan];
tccpSelected->TCCP = peakValue;

return retVal;
}

/**
getFrtPeakRegister
Get the TCCP register value of the FRT Channel choosen
*/
uint16_t mftUnit::getFrtPeakRegister(frtChannel_Type chan)
{
uint16_t retVal = 0;
stc_mft_frt_tccp_field_t *tccpSelected = NULL;

tccpSelected = mftBlock.tccp_Register[chan];
retVal = tccpSelected->TCCP;

return retVal;
}

/**
assignFRTtoOCU
Associate the OCU output to the FRT selected
Return 0 if OK or 0xFF if error occurs
*/
uint8_t mftUnit::assignFRTtoOCU(ocuIcuConnectFrt_Type frtChan, ocuUnit_Type ocuUnitSelected ,ocuChannel_Type ocuChan)
{
uint8_t retVal = 0;
stc_mft_ocu_ocfs_field_t *ocuUnitChoosen = NULL;
ocuUnitChoosen = mftBlock.ocfs_Register[ocuUnitSelected];
if (ocuUnitChoosen == NULL)
{
//Value not pluasible
retVal = 0xFF;
}
else{
if (((MAX_MFT_NUMBER == 1)
    && ((frtChan == OCU_ICU_CONNECT_EXT_MFT_FRT_0) || (frtChan == OCU_ICU_CONNECT_EXT_MFT_FRT_1)))
    ||(frtChan == OCU_ICU_CONNECT_ERROR))
{//Setting is prohibited
retVal = 0xFF;
}
else{
//Now I can connect
if (ocuChan == OCU_CHANNEL_0)
{
// We request the low part of the byte
ocuUnitChoosen->FSO0 = frtChan;
ocuHasFRTConnection[ocuUnitSelected*2 + ocuChan] = frtChan;
}
else{
//Nothing to do
}
if (ocuChan == OCU_CHANNEL_1)
{
// We request the low part of the byte
ocuUnitChoosen->FSO1 = frtChan;
ocuHasFRTConnection[ocuUnitSelected*2 + ocuChan] = frtChan;
}
else{
//Nothing to do
}
}
}
return retVal;
}


/**
whichFRTisAssignedToOCU
Find OUT which FRT is Associate to selected ocu chan
Return the FRT associate or OCU_CONNECT_ERROR if error occurs
*/
ocuIcuConnectFrt_Type mftUnit::whichFRTisAssignedToOCU(ocuUnit_Type ocuUnitSelected ,ocuChannel_Type ocuChan)
{
ocuIcuConnectFrt_Type retVal = OCU_ICU_CONNECT_ERROR;
stc_mft_ocu_ocfs_field_t *ocuUnitChoosen = NULL;
ocuUnitChoosen = mftBlock.ocfs_Register[ocuUnitSelected];
if (ocuUnitChoosen == NULL)
{
//Value not pluasible
retVal = OCU_ICU_CONNECT_ERROR;
}
else{
//Now I can check
if (ocuChan == OCU_CHANNEL_0)
{
// We request the low part of the byte
retVal = static_cast<ocuIcuConnectFrt_Type>(ocuUnitChoosen->FSO0);
}
if (ocuChan == OCU_CHANNEL_1)
{
// We request the high part of the byte
retVal = static_cast<ocuIcuConnectFrt_Type>(ocuUnitChoosen->FSO1);
}
}
return retVal;
}

/**
initFRTRegister(): Initialize all the FRT Register connected to the MFT installed
Return retVal = 0, if nothing happens
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::initFRTRegister()
{
//Init the FRT Register
uint8_t retVal = 0xFF;         //Return value
uint8_t i = 0;                 //Iterator
//TCSA Registers
mftBlock.tsca_Register[FRT_CHANNEL_0] = (stc_mft_frt_tcsa_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSA0_OFFSET/INT_SIZE;
mftBlock.tsca_Register[FRT_CHANNEL_1] = (stc_mft_frt_tcsa_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSA1_OFFSET/INT_SIZE;
mftBlock.tsca_Register[FRT_CHANNEL_2] = (stc_mft_frt_tcsa_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSA2_OFFSET/INT_SIZE;

for (i=0;i<MAX_FRT_CHANELL_FOR_UNIT;i++)
    {
    mftBlock.tsca_Register[i]->ALL_BITS &= TSCA_INIT_VALUES;
    }
//TCSB Register
mftBlock.tscb_Register[FRT_CHANNEL_0] = (stc_mft_frt_tcsb_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSB0_OFFSET/INT_SIZE;
mftBlock.tscb_Register[FRT_CHANNEL_1] = (stc_mft_frt_tcsb_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSB1_OFFSET/INT_SIZE;
mftBlock.tscb_Register[FRT_CHANNEL_2] = (stc_mft_frt_tcsb_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCSB2_OFFSET/INT_SIZE;
for (i=0;i<MAX_FRT_CHANELL_FOR_UNIT;i++)
    {
    mftBlock.tscb_Register[i]->ALL_BITS &= TSCB_INIT_VALUES;
    }
//TCCP Register
mftBlock.tccp_Register[FRT_CHANNEL_0] = (stc_mft_frt_tccp_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCCP0_OFFSET/INT_SIZE;
mftBlock.tccp_Register[FRT_CHANNEL_1] = (stc_mft_frt_tccp_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCCP1_OFFSET/INT_SIZE;
mftBlock.tccp_Register[FRT_CHANNEL_2] = (stc_mft_frt_tccp_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCCP2_OFFSET/INT_SIZE;
for (i=0;i<MAX_FRT_CHANELL_FOR_UNIT;i++)
    {
    mftBlock.tccp_Register[i]->TCCP = TCCP_DEFAULT_VALUE;
    }
//TCDT Register
mftBlock.tcdt_Register[FRT_CHANNEL_0] = (stc_mft_frt_tcdt_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCDT0_OFFSET/INT_SIZE;
mftBlock.tcdt_Register[FRT_CHANNEL_1] = (stc_mft_frt_tcdt_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCDT1_OFFSET/INT_SIZE;
mftBlock.tcdt_Register[FRT_CHANNEL_2] = (stc_mft_frt_tcdt_field_t*)(mftBlock.pmftBaseReference) + FM3_MFT_FRT_TCDT2_OFFSET/INT_SIZE;
for (i=0;i<MAX_FRT_CHANELL_FOR_UNIT;i++)
    {
    mftBlock.tcdt_Register[i]->FRT_TCDT &= TCDT_CLEARED_VALUE;
    }
//Init the function pointer
for (i=0;i<MAX_FRT_CHANELL_FOR_UNIT;i++)
    {
    IRQ_MFT_FRT_callback_p[mftID][i]=NULL;
    IRQ_MFT_FRT_NRZ_callback_p[mftID][i] = NULL;
    mftBlock.IRQ_FRT_callback_p[i]= NULL;
    mftBlock.IRQ_FRT_NRZ_callback_p[i] = NULL;
    }
retVal = 0;
}

/**
Init register for OCU unit
*/
void mftUnit::initOCURegsiter()
{
uint8_t i = 0;                 //Iterator

mftBlock.occp0_Register[OCU_UNIT_0] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP0_OFFSET/INT_SIZE;
mftBlock.occp1_Register[OCU_UNIT_0] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP1_OFFSET/INT_SIZE;
mftBlock.occp0_Register[OCU_UNIT_1] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP2_OFFSET/INT_SIZE;
mftBlock.occp1_Register[OCU_UNIT_1] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP3_OFFSET/INT_SIZE;
mftBlock.occp0_Register[OCU_UNIT_2] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP4_OFFSET/INT_SIZE;
mftBlock.occp1_Register[OCU_UNIT_2] = (stc_mft_ocu_occp_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCCP5_OFFSET/INT_SIZE;

mftBlock.ocsa_Register[OCU_UNIT_0] = (stc_mft_ocu_ocsa_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSA10_OFFSET;
mftBlock.ocsa_Register[OCU_UNIT_1] = (stc_mft_ocu_ocsa_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSA32_OFFSET;
mftBlock.ocsa_Register[OCU_UNIT_2] = (stc_mft_ocu_ocsa_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSA54_OFFSET;
for (i=0;i<MAX_OCU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.ocsa_Register[i]->ALL_BITS = OCU_OCSA_DEFAULT_VALUE;
    }

mftBlock.ocsb_Register[OCU_UNIT_0] = (stc_mft_ocu_ocsb_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSB10_OFFSET;
mftBlock.ocsb_Register[OCU_UNIT_1] = (stc_mft_ocu_ocsb_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSB32_OFFSET;
mftBlock.ocsb_Register[OCU_UNIT_2] = (stc_mft_ocu_ocsb_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSB54_OFFSET;
for (i=0;i<MAX_OCU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.ocsb_Register[i]->ALL_BITS = OCU_OCSB_DEFAULT_VALUE;
    }

mftBlock.ocsc_Register = (stc_mft_ocu_ocsc_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCSC_OFFSET;
mftBlock.ocsc_Register->ALL_BITS = OCU_OCSC_DEFAULT_VALUE;

mftBlock.ocfs_Register[OCU_UNIT_0] = (stc_mft_ocu_ocfs_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCFS10_OFFSET;
mftBlock.ocfs_Register[OCU_UNIT_1] = (stc_mft_ocu_ocfs_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCFS32_OFFSET;
mftBlock.ocfs_Register[OCU_UNIT_2] = (stc_mft_ocu_ocfs_field_t *)(mftBlock.pmftOcuBaseReference) + FM3_MFT_OCU_OCFS54_OFFSET;
for (i=0;i<MAX_OCU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.ocfs_Register[i]->FSO0 = OCU_OCFS_DEFAULT_VALUE;
    mftBlock.ocfs_Register[i]->FSO1 = OCU_OCFS_DEFAULT_VALUE;
    }
mftBlock.OCSC_MODE_TRANSLATOR[i] = 0x00;
}

/**
Init register for WFG unit
*/
void mftUnit::initWFGRegister()
{
uint8_t i = 0;                 //Iterator

mftBlock.wfsa_Regsiter[WFG_UNIT_0] = (stc_mft_wfg_wfsa_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFSA10_OFFSET/INT_SIZE;
mftBlock.wfsa_Regsiter[WFG_UNIT_1] = (stc_mft_wfg_wfsa_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFSA32_OFFSET/INT_SIZE;
mftBlock.wfsa_Regsiter[WFG_UNIT_2] = (stc_mft_wfg_wfsa_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFSA54_OFFSET/INT_SIZE;
for (i=0;i<MAX_WFG_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.wfsa_Regsiter[i]->REGISTER = WFG_WFSA_DEFAULT_VALUE;
    }

mftBlock.wftm_Register[WFG_UNIT_0] = (stc_mft_wfg_wftm_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFTM10_OFFSET/INT_SIZE;
mftBlock.wftm_Register[WFG_UNIT_1] = (stc_mft_wfg_wftm_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFTM32_OFFSET/INT_SIZE;
mftBlock.wftm_Register[WFG_UNIT_2] = (stc_mft_wfg_wftm_field_t *)(mftBlock.pmftWfgBaseReference) + FM3_MFT_WFG_WFTM54_OFFSET/INT_SIZE;
for (i=0;i<MAX_WFG_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.wftm_Register[i]->REGISTER = WFG_WFTM_DEFAULT_VALUE;
    }

mftBlock.wfir_Register = (stc_mft_wfg_wfir_field_t *)(mftBlock.pmftBaseReference) + FM3_MFT_WFG_WFIR_OFFSET/INT_SIZE;
mftBlock.wfir_Register->REGISTER = WFG_WFIR_DEFAULT_VALUE;

//Solo per test ora...Da rimuovere
/*setWfgCycleRatio(WFG_UNIT_0, WFG_CHANNEL_0, CYCLE_RATIO_16);
setWfgModeSetting(WFG_UNIT_0, WFG_CHANNEL_0,WFG_RT_PPG_MODE);
setWfgChGateMode(WFG_UNIT_0, WFG_CHANNEL_0,CH_GATE_ONLY_FIRST_CHAN);
setWfgPPGtoGateMode(WFG_UNIT_0, WFG_CHANNEL_0,USE_PPC_CH_2);
setWfgPPGReflectWfg(WFG_UNIT_0,WFG_CHANNEL_0, TF_REFLECTION_ON_WFG);
setWfgOutputPolarity(WFG_UNIT_0,WFG_CHANNEL_0,POLARITY_ACTIVE_LOW);
setWfgTimerRegister(WFG_UNIT_0, 0xABCD);
uint16_t prova = 0;
uint8_t test = 0xAA;
prova = getWfgTimerRegister(WFG_UNIT_0);
test = isDTIFgenerated(WFG_UNIT_0);
if (test == 1) {
prova = 0xabab;
}else {
prova = 0xaada;
    }*/
}

/**
Init register for ICU unit
*/
void mftUnit::initICURegister()
{
uint8_t i = 0;                 //Iterator
uint8_t j = 0;                 //Iterator

mftBlock.iccp0_Register[ICU_UNIT_0] = (stc_mft_icu_iccp_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICCP0_OFFSET/INT_SIZE;
mftBlock.iccp1_Register[ICU_UNIT_0] = (stc_mft_icu_iccp_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICCP1_OFFSET/INT_SIZE;
mftBlock.iccp0_Register[ICU_UNIT_1] = (stc_mft_icu_iccp_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICCP2_OFFSET/INT_SIZE;
mftBlock.iccp1_Register[ICU_UNIT_1] = (stc_mft_icu_iccp_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICCP3_OFFSET/INT_SIZE;

mftBlock.icsa_Register[ICU_UNIT_0] = (stc_mft_icu_icsa_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICSA10_OFFSET;
mftBlock.icsa_Register[ICU_UNIT_1] = (stc_mft_icu_icsa_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICSA32_OFFSET;

for (i=0;i<MAX_ICU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.icsa_Register[i]->ALL_BITS = ICU_ICSA_DEFAULT_VALUE;
    }

mftBlock.icsb_Register[ICU_UNIT_0] = (stc_mft_icu_icsb_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICSB10_OFFSET;
mftBlock.icsb_Register[ICU_UNIT_1] = (stc_mft_icu_icsb_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICSB32_OFFSET;

for (i=0;i<MAX_ICU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.icsb_Register[i]->ALL_BITS = ICU_ICSB_DEFAULT_VALUE;
    }

mftBlock.icfs_Register[ICU_UNIT_0] = (stc_mft_icu_icfs_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICFS10_OFFSET;
mftBlock.icfs_Register[ICU_UNIT_1] = (stc_mft_icu_icfs_field_t *)(mftBlock.pmftIcuBaseReference) + FM3_MFT_ICU_ICFS32_OFFSET;

for (i=0;i<MAX_ICU_UNIT_FOR_MFT_UNIT;i++)
    {
    mftBlock.icfs_Register[i]->FSO0 = ICU_ICFS_DEFAULT_VALUE;
    mftBlock.icfs_Register[i]->FSO1 = ICU_ICFS_DEFAULT_VALUE;
    }

    //Init the function pointer
for (i=0;i<MAX_ICU_UNIT_FOR_MFT_UNIT;i++)
    {
    for (j=0;j<MAX_ICU_CHANNEL_FOR_ICU_UNIT;j++){
        IRQ_MFT_ICU_callback_p[mftID][i][j]=NULL;
        mftBlock.IRQ_ICU_callback_p[i][j]= NULL;
    }
    }
}




/**
setCycleRadio Method:
Set the WFG clock using the clock ratio selected (ratioToSet) for the selected
WFG unit (chan)
Return retVal = ratioToSet
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setWfgCycleRatio(wfgUnit_Type unit, wfgChannel_Type chan, cycleRatio_Type ratioToSet)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
stc_mft_wfg_wfir_field_t * wfirSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];
wfirSelected = mftBlock.wfir_Register;

if ((wfsaSelected == NULL) || (wfirSelected == NULL)|| (ratioToSet > CYCLE_RATIO_64)){
    retVal = 0xFF; //Value not plausible
    }
    else{
    switch (unit){
        case WFG_UNIT_0:
        //Change the setting of these bits, while the WFG timer is stopping.
            if (wfirSelected->stc_mft_bit_access_t.TMIE10 == WFG_TIMER_IN_OPERATION ){
                wfirSelected->stc_mft_bit_access_t.TMIS10 = 1; //STOP the Timer
                } //Else not needed
            wfsaSelected->DCLOCK = ratioToSet;
            retVal = wfsaSelected->DCLOCK;
            break;
        case WFG_UNIT_1:
        //Change the setting of these bits, while the WFG timer is stopping.
            if (wfirSelected->stc_mft_bit_access_t.TMIE32 == WFG_TIMER_IN_OPERATION ){
                wfirSelected->stc_mft_bit_access_t.TMIS32 = 1; //STOP the Timer
                } //Else not needed
            wfsaSelected->DCLOCK = ratioToSet;
            retVal = wfsaSelected->DCLOCK;
            break;
        case WFG_UNIT_2:
        //Change the setting of these bits, while the WFG timer is stopping.
            if (wfirSelected->stc_mft_bit_access_t.TMIE54 == WFG_TIMER_IN_OPERATION ){
                wfirSelected->stc_mft_bit_access_t.TMIS54 = 1; //STOP the Timer
                } //Else not needed
            wfsaSelected->DCLOCK = ratioToSet;
            retVal = wfsaSelected->DCLOCK;
            break;
        default:
            retVal = 0xFF;
            break;
            }
    }
return retVal;
}

/**
Set the WFG Operation mode
Return retVal = mode
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setWfgModeSetting(wfgUnit_Type unit, wfgChannel_Type chan, wfgOperationMode_t mode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];

//Change the setting of these bits while OCU and PPG timer unit to be connected are stopping
retVal = setOcuChannelOper(static_cast<ocuUnit_Type>(unit), static_cast<ocuChannel_Type>(chan), OCU_OPER_DISABLED);

if ((retVal == 0xFF) || (wfsaSelected == NULL)){
    retVal = 0xFF;
    }
    else {
        wfsaSelected->TMD = mode;
        retVal = wfsaSelected->TMD;
    }
return retVal;
}

/**
Set the WFG Gate Mode
Return retVal = gateMode
Return retVal = 0xFF, if some error occurs
*/

uint8_t mftUnit::setWfgChGateMode(wfgUnit_Type unit, wfgChannel_Type chan, chGateCode_t gateMode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];

if (wfsaSelected == NULL){
    retVal = 0xFF;
    }
    else {
        wfsaSelected->GTEN = gateMode;
        retVal = wfsaSelected->GTEN;
    }
return retVal;
}
/**
Set the PPG channel to associate to the WFG Gate
Return retVal = ppgGateMode
Return retVal = 0xFF, if some error occurs
*/
uint8_t mftUnit::setWfgPPGtoGateMode(wfgUnit_Type unit, wfgChannel_Type chan, ppcToGateMode_t ppgGateMode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];

if (wfsaSelected == NULL){
    retVal = 0xFF;
    }
    else {
        wfsaSelected->PSEL = ppgGateMode;
        retVal = wfsaSelected->PSEL;
    }
return retVal;
}

/**
Set how the WFG reflects the PPG signal in output
Return retVal = refelctionMode
Return retVal = 0xFF, if some error occurs
*/

uint8_t mftUnit::setWfgPPGReflectWfg(wfgUnit_Type unit, wfgChannel_Type chan,  chPPGOnWFGoutput_t reflectionMode)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];

//Change the setting of these bits while OCU and PPG timer unit to be connected are stopping
retVal = setOcuChannelOper(static_cast<ocuUnit_Type>(unit), static_cast<ocuChannel_Type>(chan), OCU_OPER_DISABLED);

if ((retVal == 0xFF) || (wfsaSelected == NULL)){
    retVal = 0xFF;
    }
    else {
        wfsaSelected->PGEN = reflectionMode;
        retVal = wfsaSelected->PGEN;
    }
return retVal;
}

/**
Set how the WFG output polarity
Return retVal = polarity
Return retVal = 0xFF, if some error occurs
*/

uint8_t mftUnit::setWfgOutputPolarity(wfgUnit_Type unit, wfgChannel_Type chan, wfgOutputPolarity_t polarity)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_wfg_wfsa_field_t * wfsaSelected = NULL;
wfsaSelected = mftBlock.wfsa_Regsiter[unit];

//Change the setting of these bits while OCU and PPG timer unit to be connected are stopping
retVal = setOcuChannelOper(static_cast<ocuUnit_Type>(unit), static_cast<ocuChannel_Type>(chan), OCU_OPER_DISABLED);

if ((retVal == 0xFF) || (wfsaSelected == NULL)){
    retVal = 0xFF;
    }
    else {
        wfsaSelected->DMOD = polarity;
        retVal = wfsaSelected->DMOD;
    }
return retVal;
}
/**
Set the value of the timer
Return the value of the timer or 0xFF
*/
uint16_t mftUnit::setWfgTimerRegister(wfgUnit_Type unit, uint16_t value)
{
uint16_t retVal = 0xFFFF;
stc_mft_wfg_wftm_field_t * wftmSelected = NULL;
wftmSelected = mftBlock.wftm_Register[unit];

if (wftmSelected == NULL){
    retVal = 0xFF;
    }
    else {
        wftmSelected->REGISTER = value;
        retVal = wftmSelected->REGISTER;
    }

return retVal;
}

/**
Get the value of the timer
Return the value of the timer or 0xFF
*/
uint16_t mftUnit::getWfgTimerRegister(wfgUnit_Type unit)
{
uint16_t retVal = 0xFFFF;
stc_mft_wfg_wftm_field_t * wftmSelected = NULL;
wftmSelected = mftBlock.wftm_Register[unit];

if (wftmSelected == NULL){
    retVal = 0xFF;
    }
    else {
        retVal = wftmSelected->REGISTER;
    }

return retVal;
}

/**
Return 1 if the interrupt of DTIF has been generated.
0 if not.
0xFF if error
*/
uint8_t mftUnit::isDTIFgenerated(wfgUnit_Type unit)
{
uint8_t retVal = 0xFF;
stc_mft_wfg_wfir_field_t * wfirSelected = NULL;
wfirSelected = mftBlock.wfir_Register;

if (wfirSelected == NULL){
    retVal = 0xFF;
    }
    else {
        if (wfirSelected->stc_mft_bit_access_t.DTIF == WFG_DTIF_NOT_GENERATED) {
            retVal = 0;
            }
            else {
                retVal = 1;
            }
    }
return retVal;
}

/**
Clear the DTIF Signal
Return 0 if OK.
0xFF if error
*/
uint8_t mftUnit::clearDTIF(wfgUnit_Type unit)
{
uint8_t retVal = 0xFF;
stc_mft_wfg_wfir_field_t * wfirSelected = NULL;
wfirSelected = mftBlock.wfir_Register;

if (wfirSelected == NULL){
    retVal = 0xFF;
    }
    else {
        wfirSelected->stc_mft_bit_access_t.DTIF =  1; //Clear the register
        retVal = 0;
    }
return retVal;
}

/**
*/
void mftUnit::assignRTPin()
{
mftBlock.RTO_0[0].ocuUnit = mftBlock.RTO_1[0].ocuUnit = OCU_UNIT_0;
mftBlock.RTO_0[0].ocuChan = mftBlock.RTO_1[0].ocuChan = OCU_CHANNEL_0;
stpncpy(mftBlock.RTO_0[0].rtName,"RTO00_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[0].rtName,"RTO00_1",RT_PIN_STRING_LENGHT);

mftBlock.RTO_0[1].ocuUnit = mftBlock.RTO_1[1].ocuUnit = OCU_UNIT_0;
mftBlock.RTO_0[1].ocuChan = mftBlock.RTO_1[1].ocuChan = OCU_CHANNEL_1;
stpncpy(mftBlock.RTO_0[1].rtName,"RTO01_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[1].rtName,"RTO01_1",RT_PIN_STRING_LENGHT);

mftBlock.RTO_0[2].ocuUnit = mftBlock.RTO_1[2].ocuUnit = OCU_UNIT_1;
mftBlock.RTO_0[2].ocuChan = mftBlock.RTO_1[2].ocuChan = OCU_CHANNEL_0;
stpncpy(mftBlock.RTO_0[2].rtName,"RTO02_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[2].rtName,"RTO02_1",RT_PIN_STRING_LENGHT);

mftBlock.RTO_0[3].ocuUnit = mftBlock.RTO_1[3].ocuUnit = OCU_UNIT_1;
mftBlock.RTO_0[3].ocuChan = mftBlock.RTO_1[3].ocuChan = OCU_CHANNEL_1;
stpncpy(mftBlock.RTO_0[3].rtName,"RTO03_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[3].rtName,"RTO03_1",RT_PIN_STRING_LENGHT);

mftBlock.RTO_0[4].ocuUnit = mftBlock.RTO_1[4].ocuUnit = OCU_UNIT_2;
mftBlock.RTO_0[4].ocuChan = mftBlock.RTO_1[4].ocuChan = OCU_CHANNEL_0;
stpncpy(mftBlock.RTO_0[4].rtName,"RTO04_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[4].rtName,"RTO04_1",RT_PIN_STRING_LENGHT);

mftBlock.RTO_0[5].ocuUnit = mftBlock.RTO_1[5].ocuUnit = OCU_UNIT_2;
mftBlock.RTO_0[5].ocuChan = mftBlock.RTO_1[5].ocuChan = OCU_CHANNEL_1;
stpncpy(mftBlock.RTO_0[5].rtName,"RTIC_1O05_0",RT_PIN_STRING_LENGHT);
stpncpy(mftBlock.RTO_1[5].rtName,"RTO05_1",RT_PIN_STRING_LENGHT);
}

/**
*/
uint8_t mftUnit::findConfiguredOutputPin(char* pinName, ocuUnit_Type &unit, ocuChannel_Type &chan)
{
uint8_t retVal = 0;
uint8_t i = 0; //iterator
bool findOut = false;
uint8_t item = 0xFF;

for (i=0;i<NUMBER_OF_OUTPUT_PIN;i++){
    if (strncmp(mftBlock.RTO_0[i].rtName,pinName,RT_PIN_STRING_LENGHT)== 0){
        findOut = true;
        item = i;
        i= NUMBER_OF_OUTPUT_PIN; //Force the exit
        unit = mftBlock.RTO_0[item].ocuUnit;
        chan = mftBlock.RTO_0[item].ocuChan;
        retVal = 0;
        }
        else{
            findOut = false;
            retVal = 0xFF;
            }
    if (findOut == false){
        if (strncmp(mftBlock.RTO_1[i].rtName,pinName,RT_PIN_STRING_LENGHT)== 0){
            findOut = true;
            item = i;
            i= NUMBER_OF_OUTPUT_PIN; //Force the exit
            unit = mftBlock.RTO_1[item].ocuUnit;
            chan = mftBlock.RTO_1[item].ocuChan;
            retVal = 0;
        }
        else{
            findOut = false;
            retVal = 0xFF;
            }
        }
        //else{do nothing}
        }
return retVal;
}

/**
setIcuChannelGenerateInt
Enable or disbale the interrupt generation based on the ICP value
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::setIcuChannelGenerateInt(icuUnit_Type unit, icuChannel_Type chan, icuGenerateInt_Type state)
{
uint8_t retVal = 0;         //Return value
stc_mft_icu_icsa_field_t *icsaSelected = NULL;
icsaSelected = mftBlock.icsa_Register[unit];
if (icsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == ICU_CHANNEL_0){
            icsaSelected->ICE0 = state;
            }
            else{//No else necessary
            }
        if (chan == ICU_CHANNEL_1){
            icsaSelected->ICE1 = state;
            }
            else{//No else necessary
            }
        }
return retVal;
}
/** setIcuRegister
** General setting for ICU register
** Returns 0xFF if error
**/
uint8_t mftUnit::setIcuRegister(icuUnit_Type unit,icuChannel_Type chan, icuOperation_Type oper, icuGenerateInt_Type genIntState)
{
    uint8_t retVal = 0xFF;

    retVal = setIcuChannelGenerateInt(unit, chan, genIntState);
        if (retVal != 0xFF){
                retVal = clearICUICP(unit,chan);
                if (retVal != 0xFF){
                    retVal = setIcuChannelSelectFrontEnable(unit, chan, oper);
                        }
                    }
    return retVal;

}

/**
clearICUICP
Clear the ICP Register
return 0 or 0xFF if error occurs
*/
uint8_t mftUnit::clearICUICP(icuUnit_Type unit, icuChannel_Type chan)
{
uint8_t retVal = NOT_CLEARED_REGISTER;         //Return value
uint8_t u8ICEx = 0;
uint8_t u8RetVal = 0;
stc_mft_icu_icsa_field_t *icsaSelected = NULL;
icsaSelected = mftBlock.icsa_Register[unit];
if (icsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == ICU_CHANNEL_0){
            if (icsaSelected->ICP0 == NOT_CLEARED_REGISTER){
                u8ICEx = icsaSelected->ALL_BITS;
                u8ICEx = u8ICEx & ICSA_ICP_0_1_RESET_MASK; //All 1 except the ICP0 and ICP1
                u8RetVal = u8ICEx;
                u8ICEx = u8ICEx | ICSA_CLEAR_ICP0_MASK; //1 to ICP1 and 0 to ICP 0
                icsaSelected->ALL_BITS = u8ICEx | ICSA_ICP_0_1_RESET_MASK;
                icsaSelected->ALL_BITS = u8RetVal;
                retVal = CLEARED_REGISTER;
                }
            }
            else{//No else necessary
                //retVal = NOT_CLEARED_REGISTER;
            }
        if (chan == ICU_CHANNEL_1){
            if (icsaSelected->ICP1 == NOT_CLEARED_REGISTER){
                u8ICEx = icsaSelected->ALL_BITS;
                u8ICEx = u8ICEx & ICSA_ICP_0_1_RESET_MASK; //All 1 except the ICP0 and ICP1
                u8RetVal = u8ICEx;
                u8ICEx = u8ICEx | ICSA_CLEAR_ICP1_MASK; //1 to ICP1 and 0 to ICP 0
                icsaSelected->ALL_BITS = u8ICEx | ICSA_ICP_0_1_RESET_MASK;
                icsaSelected->ALL_BITS = u8RetVal;
                retVal = CLEARED_REGISTER;
                }/**else{
                }*/
            }
            else{//No else necessary
                //retVal = NOT_CLEARED_REGISTER;
            }
        }
return retVal;
}/**
Return the value of the interrupt flag matched
*/
uint8_t mftUnit::readICUICP(icuUnit_Type unit, icuChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_icu_icsa_field_t *icsaSelected = NULL;
icsaSelected = mftBlock.icsa_Register[unit];
if (icsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == ICU_CHANNEL_0){
            retVal = icsaSelected->ICP0;
            }
            else{//No else necessary
            }
        if (chan == ICU_CHANNEL_1){
            retVal = icsaSelected->ICP1;
            }
            else{//No else necessary
            }
        }
return retVal;
}
/** setIcuChannelSelectFrontEnable
* Select and enable the kind of front that must activate the counter
*/
uint8_t mftUnit::setIcuChannelSelectFrontEnable(icuUnit_Type unit, icuChannel_Type chan, icuOperation_Type operation)
{
uint8_t retVal = 0;         //Return value
stc_mft_icu_icsa_field_t *icsaSelected = NULL;
icsaSelected = mftBlock.icsa_Register[unit];
if (icsaSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == ICU_CHANNEL_0){
            icsaSelected->EG0 = operation;
            }
            else{//No else necessary
            }
        if (chan == ICU_CHANNEL_1){
            icsaSelected->EG1 = operation;
            }
            else{//No else necessary
            }
        }
return retVal;

}

/**
assignFRTtoICU
Associate the ICU input to the FRT selected
Return 0 if OK or 0xFF if error occurs
*/
uint8_t mftUnit::assignFRTtoICU(ocuIcuConnectFrt_Type frtChan, icuUnit_Type icuUnitSelected ,icuChannel_Type icuChan)
{
uint8_t retVal = 0;
stc_mft_icu_icfs_field_t *icuUnitChoosen = NULL;
icuUnitChoosen = mftBlock.icfs_Register[icuUnitSelected];
if (icuUnitChoosen == NULL)
{
//Value not pluasible
retVal = 0xFF;
}
else{
if (((MAX_MFT_NUMBER == 1)
    && ((frtChan == OCU_ICU_CONNECT_EXT_MFT_FRT_0) || (frtChan == OCU_ICU_CONNECT_EXT_MFT_FRT_1)))
    ||(frtChan == OCU_ICU_CONNECT_ERROR))
{//Setting is prohibited
retVal = 0xFF;
}
else{
//Now I can connect
if (icuChan == ICU_CHANNEL_0)
{
// We request the low part of the byte
icuUnitChoosen->FSO0 = frtChan;
icuHasFRTConnection[icuUnitSelected*2 + icuChan] = frtChan;
}
else{
//Nothing to do
}
if (icuChan == ICU_CHANNEL_1)
{
// We request the low part of the byte
icuUnitChoosen->FSO1 = frtChan;
icuHasFRTConnection[icuUnitSelected*2 + icuChan] = frtChan;
}
else{
//Nothing to do
}
}
}
return retVal;
}


/**
whichFRTisAssignedToICU
Find OUT which FRT is Associate to selected icu chan
Return the FRT associate or OCU_ICU_CONNECT_ERROR if error occurs
*/
ocuIcuConnectFrt_Type mftUnit::whichFRTisAssignedToICU(icuUnit_Type icuUnitSelected ,icuChannel_Type icuChan)
{
ocuIcuConnectFrt_Type retVal = OCU_ICU_CONNECT_ERROR;
stc_mft_icu_icfs_field_t *icuUnitChoosen = NULL;
icuUnitChoosen = mftBlock.icfs_Register[icuUnitSelected];
if (icuUnitChoosen == NULL)
{
//Value not pluasible
retVal = OCU_ICU_CONNECT_ERROR;
}
else{
//Now I can check
if (icuChan == ICU_CHANNEL_0)
{
// We request the low part of the byte
retVal = static_cast<ocuIcuConnectFrt_Type>(icuUnitChoosen->FSO0);
}
if (icuChan == ICU_CHANNEL_1)
{
// We request the high part of the byte
retVal = static_cast<ocuIcuConnectFrt_Type>(icuUnitChoosen->FSO1);
}
}
return retVal;
}

/**
getIcuLastFront
Get the last front intercepted in the input channel
Return:
0 if last was falling
1 if last was rising
0xFF in case of error
*/
uint8_t mftUnit::getIcuLastFront(icuUnit_Type unit, icuChannel_Type chan)
{
uint8_t retVal = 0xFF;         //Return value
stc_mft_icu_icsb_field_t *icsbSelected = NULL;
icsbSelected = mftBlock.icsb_Register[unit];
if (icsbSelected == NULL){
    retVal = 0xFF;
    }
    else{
        if (chan == ICU_CHANNEL_0){
            retVal = icsbSelected->IEI0;
            }
            else{//No else necessary
            }
        if (chan == ICU_CHANNEL_1){
            retVal = icsbSelected->IEI1;
            }
            else{//No else necessary
            }
        }
return retVal;

}

/**
getIcuICCP
Get the ICCP register value
Return the register value if a valid edge has been detected otherways 0xFFFF
*/
uint16_t mftUnit::getIcuICCP(icuUnit_Type unit, icuChannel_Type chan)
{
uint16_t value = 0xFFFF;
stc_mft_icu_iccp_field_t *iccpSelected = NULL;
stc_mft_icu_icsa_field_t *icsaSelected = NULL;
icsaSelected = mftBlock.icsa_Register[unit];

if (chan == ICU_CHANNEL_0){
    if (icsaSelected->ICE0 == 1){
        //Valid edge has been detected
        iccpSelected = mftBlock.iccp0_Register[unit];
        value = iccpSelected->REGISTER;
        }
    }
    else {
        if (icsaSelected->ICE1 == 1){
            //Valid edge has been detected
            iccpSelected = mftBlock.iccp1_Register[unit];
            value = iccpSelected->REGISTER;
            }
    }
return value;
}

/**
attachFrtCallback
Attach the callback to serve peak interrupt of channel "chan"
Return always 0
**/
int mftUnit::attachFrtCallback( void (*_callback_p)(frtChannel_Type),frtChannel_Type chan)
{
    IRQ_MFT_FRT_callback_p[mftID][chan] = _callback_p;
    mftBlock.IRQ_FRT_callback_p[chan] = _callback_p;
	return 0;
}

/**
attachFrtNrzCallback
Attach the callback to serve NRZ interrupt of channel "chan"
Return always 0
**/
int mftUnit::attachFrtNrzCallback( void (*_callback_p)(frtChannel_Type),frtChannel_Type chan)
{
    IRQ_MFT_FRT_NRZ_callback_p[mftID][chan] = _callback_p;
	return 0;
}

/**
attachICUCallback
Attach the callback to serve input detection interrupt of channel "chan"
Return always 0
**/
//int mftUnit::attachICUCallback( void (*_callback_p)(icuUnit_Type icuUnitSelected, icuChannel_Type chan),icuUnit_Type icuUnitSelected, icuChannel_Type chan)
int mftUnit::attachICUCallback( void (*_callback_p)(),icuUnit_Type icuUnitSelected, icuChannel_Type chan)
//attachICUCallback( void (*_callback_p)(icuChannel_Type,icuUnit_Type),icuUnit_Type icuUnitSelected, icuChannel_Type chan)
{
    IRQ_MFT_ICU_callback_p[mftID][icuUnitSelected][chan] = _callback_p;
    mftBlock.IRQ_ICU_callback_p[icuUnitSelected][chan] = _callback_p;
	return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER (MFT_FRT_IRQ)
{
/**Find Out which interrupt has been rised
**Here we could intercept the following interrupt:
** MFT Unit 0
**      ---> FRT CHANNEL 0
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 1
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 2
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
** MFT Unit 1
**      ---> FRT CHANNEL 0
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 1
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 2
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
** MFT Unit 02
**      ---> FRT CHANNEL 0
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 1
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
**      ---> FRT CHANNEL 2
**                  ---> PEAK INTERRUPT
**                  ---> NRZ INTERRUPT
***************************************
*/
    uint8_t i = 0;
    uint8_t j = 0;
    bool found = false;
    uint8_t debugErrorVar = 0;
    frtChannel_Type channel;

    //#define MFT_EN_DIS_IRQ
    #ifdef MFT_EN_DIS_IRQ
    __disable_irq();
    #endif

    //Check which MFT Unit is installed
    for (i=0;i<MAX_MFT_NUMBER;i++){
        if (mftInstalled[i] == MFT_INSTALLED){
            //This MFT has been installed
            found = true;
            for (j=0;j<MAX_FRT_CHANELL_FOR_UNIT;j++){
            //Searching for all the FRT Units
            channel = static_cast<frtChannel_Type>(j);
            if (IRQ_MFT_FRT_callback_p[i][channel]!= NULL){
            // Callback for PEAK Exists.. Launch it
                IRQ_MFT_FRT_callback_p[i][channel](channel);
            }
           if (IRQ_MFT_FRT_NRZ_callback_p[i][channel]!= NULL){
            // Callback fro NRZ Exists.. Launch it
                IRQ_MFT_FRT_NRZ_callback_p[i][channel](channel);
            }
            }
            }
        }
    if (found == false){
        //Interrupt issued but not catched...Problem
        debugErrorVar++;
        }

    #ifdef MFT_EN_DIS_IRQ
    __enable_irq();
    #endif



}//END IRQ_HANDLER
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(MFT_ICU_IRQ)
{
/**Find Out which interrupt has been rised
**Here we could intercept the following interrupt:
** MFT Unit 0
**      ---> ICU CHANNEL 0
**      ---> ICU CHANNEL 1
**      ---> ICU CHANNEL 2
**      ---> ICU CHANNEL 3
** MFT Unit 1
**      ---> ICU CHANNEL 0
**      ---> ICU CHANNEL 1
**      ---> ICU CHANNEL 2
**      ---> ICU CHANNEL 3
** MFT Unit 2
**      ---> ICU CHANNEL 0
**      ---> ICU CHANNEL 1
**      ---> ICU CHANNEL 2
**      ---> ICU CHANNEL 3
***************************************
*/
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    bool found = false;
    uint8_t debugErrorVar = 0;
    icuChannel_Type channel;
    icuUnit_Type icuUnitSelected;


    #ifdef MFT_EN_DIS_IRQ
    __disable_irq();
    #endif



    //Check which MFT Unit is installed
    for (i=0;i<MAX_MFT_NUMBER;i++){
        if (mftInstalled[i] == MFT_INSTALLED){
            //This MFT has been installed
            for (j=0;j<MAX_ICU_UNIT_FOR_MFT_UNIT;j++){
            //Searching for all the ICU Units
                icuUnitSelected = static_cast<icuUnit_Type>(j);
                for (k=0;k<MAX_ICU_CHANNEL_FOR_ICU_UNIT;k++){
                    channel = static_cast<icuChannel_Type>(k);
                    if (IRQ_MFT_ICU_callback_p[i][icuUnitSelected][channel]!= NULL){
                        // Callback for ICU Exists.. Launch it
                        found = true;
                        IRQ_MFT_ICU_callback_p[i][icuUnitSelected][channel]();
                    }
                }//end for cycle - channels for unit
            }//end for cycle - units for MFT
            }//end if - MFT_INSTALLED
        }//end for cycle MFT main
    if (found == false){
        //Interrupt issued but not catched...Problem
        debugErrorVar++;
        }
    #ifdef MFT_EN_DIS_IRQ
    __enable_irq();
    #endif


}//END IRQ_HANDLER
#ifdef __cplusplus
}
#endif



#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(MFT_OCU_IRQ)
{

}//END IRQ_HANDLER
#ifdef __cplusplus
}
#endif

/** Class destructor. */
mftUnit::~mftUnit()
{
    if (mftID != MFT_WRONG_ID)
        mftInstalled[mftID] = MFT_UNINSTALLED;  //Uninstall
}

/**
uint16_t tempDebugRead = 0;
uint16_t tempOcuOCCP = 0;
uint8_t tempDebugQuestion = 0xFF;
ocuConnectFrt_Type tempDebugOCU = OCU_CONNECT_ERROR;
ocuUnit tempOcuUnit;
ocuChannel tempOcuChannel;

//Example 1: Begin
//FRT should be programmed with Interrupt disabled and UP_MODE
mftUnit myUnit(MFT_UNIT_0); //Object creation
myUnit.setTSCARegister(FRT_CHANNEL_0,CYCLE_RATIO_128,CANCEL_REQUEST,UP_MODE,STOPPING_MODE,
                        BUFFER_ENABLE,INT_ON_MATCH_DISABLED,INT_ON_MATCH_DISABLED,INTERNAL_CLOCK);
myUnit.setFrtPeakRegister(FRT_CHANNEL_0,0x5FFF);
tempDebugRead = myUnit.getFrtPeakRegister(FRT_CHANNEL_0);
//OCU : Up-count mode (1-change), with interrupt
myUnit.findConfiguredOutputPin("RTO00_1",tempOcuUnit,tempOcuChannel); //VCIC is RTO00_1
myUnit.assignFRTtoOCU(OCU_CONNECT_FRT_0,tempOcuUnit,tempOcuChannel);
tempDebugOCU = myUnit.whichFRTisAssignedToOCU(tempOcuUnit,tempOcuChannel);
myUnit.setOcuRegister(tempOcuUnit,tempOcuChannel, OCU_OPER_DISABLED, OCU_BUFFER_DISABLED,
                            OCU_GENERATE_INTERRUPT,BUF_TRANSFER_ON_ZERO,OCU_OUTPUT_LOW_LEVEL);
myUnit.setOcuAdmittedModeUse(tempOcuUnit,tempOcuChannel, OCU_ADMITTED_0); //We need UP_COUNT for CH 0 on CH1 we don't have connection so we choose the UP_COUNT for CH1
myUnit.setOcuOCCP(tempOcuUnit,tempOcuChannel,0x1800); //Set the value to match to change the output signal
tempOcuOCCP = myUnit.getOcuOCCP(tempOcuUnit,tempOcuChannel);
myUnit.startsFrtCounting(FRT_CHANNEL_0); //Starts the FRT
myUnit.setOcuChannelOper(tempOcuUnit,tempOcuChannel,OCU_OPER_ENABLED); //Start OCU
int counter = 0;
uint8_t flag = 0xFF;
bool neverchange = false;
while (1)
{
counter++;
flag = myUnit.readOCUIOP(tempOcuUnit,tempOcuChannel);
if (flag == 1){
         tempDebugRead = myUnit.getFrtCounterOutput(FRT_CHANNEL_0);
         counter = 0;
         if (neverchange == false){
            neverchange = true;
            myUnit.setOcuOCCP(tempOcuUnit,tempOcuChannel,0x3800); //Set the value to match to change the output signal
            myUnit.clearOCUIOP(tempOcuUnit,tempOcuChannel);
         } else {
            neverchange = false;
            myUnit.setOcuOCCP(tempOcuUnit,tempOcuChannel,0x1800); //Set the value to match to change the output signal
            myUnit.clearOCUIOP(tempOcuUnit,tempOcuChannel);
         }
}
else tempDebugRead = myUnit.getFrtCounterOutput(FRT_CHANNEL_0);
}
//Example 1: End
*/

/***
CALLBACK ROUTINE SERVED BY FRT INTERRUPT:

//Prepare the pointer to the object
mftUnit *myUnit = NULL;

//Make an instanciation somewhere in your code
myUnit = new mftUnit(MFT_UNIT_0); //Object creation

//Assign the callback where you create the object
//This will call frtWrapperRoutine when a PEAK interrupt occur on FRT_CHANNEL_1
myUnit->attachFrtCallback(frtWrapperRoutine,FRT_CHANNEL_0);
myUnit->setTSCARegister(FRT_CHANNEL_0,CYCLE_RATIO_128,CANCEL_REQUEST,UP_MODE,STOPPING_MODE,
                        BUFFER_ENABLE,INT_ON_MATCH_ENABLED,INT_ON_MATCH_ENABLED,INTERNAL_CLOCK);
myUnit->setFrtPeakRegister(FRT_CHANNEL_0,0x5FFF);
myUnit->startsFrtCounting(FRT_CHANNEL_0); //Starts the FRT



//Define a callback routine
void  frtWrapperRoutine(frtChannel_Type channel));

//Implement the callbackRoutine like here
void  frtWrapperRoutine(frtChannel_Type channel)
{
//A FRT interrupt has been detected for this channel
//Verify if is it plausible for this channel
    bool found = false;
    uint8_t regVal = 0;

    if (myUnit->isFrtPeakInterruptEnabled(channel) == true){
        //Frt Peak is enabled .. let's verify
        //Read the register
        regVal = myUnit->readFRTICLR(channel);
        if (regVal != 0xFF) {
            //Read the value of reg
            if (regVal == NOT_CLEARED_REGISTER){
                // The interrupt has been issued
                found = true;
                // Insert code to serve the peak reached
                //Clear the interrupt
                myUnit->clearFRTICLR(channel);
                }
            }
        }

    regVal = 0;
    if (myUnit->isFrtZeroInterruptEnabled(channel) == true){
        //Frt Zero is enabled .. let's verify
        //Read the register
        regVal = myUnit->readFRTIRQZF(channel);
        if (regVal != 0xFF) {
            //Read the value of reg
            if (regVal == NOT_CLEARED_REGISTER){
                // The interrupt has been issued
                found = true;
                // Insert code to serve the peak reached
                //Clear the interrupt
                myUnit->clearFRTIRQZF(channel);
                }
            }
        }
        if (found == false){
         // Interrupt cathced but not served...error
         regVal = 0xFF;
         }


return;
}
*/
