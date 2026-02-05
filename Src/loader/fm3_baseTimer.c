/**
@file		fm3_baseTimer.cpp
@brief		fm3 base timer setup

@author		Nicola
@date		17/06/2012
@version	01.00.00
*/

#include "fm3_baseTimer.h"

/**
 * Base Timers (ch0... ch7) Interrupt
 */
#define BaseTimers_Ch0toCh7_IRQ	VectorBC

/**
 * Base Timer struct
 */
static fm3_BT_t FM3_BT[FM3_BT_AVAILABLE];

/**
 *Base Timer register's pointer
 */
static FM3_BT_TypeDef *FM3_BT_R[FM3_BT_AVAILABLE];

/**
 * Underflow/Overflow Interrupt request callback
 * PWM: underflow IRQ; PPG: underflow IRQ; RT: underflow IRQ; PWC: overflow IRQ
 */
static void (*IRQ_BT_under_over_flow_callback_p[FM3_BT_AVAILABLE])(void);

/**
 * Duty match Interrupt request callback
 * PWM: duty match IRQ; PPG: undefined IRQ; RT: undefined IRQ; PWC: undefined IRQ
 */
static void (*IRQ_BT_dutyMatch_callback_p[FM3_BT_AVAILABLE])(void);

/**
 * Trigger input Interrupt/Measurement completion request callback
 * PWM: trigger input IRQ; PPG: trigger input IRQ; RT: trigger input IRQ; PWC: measurement completion IRQ
 */
static void (*IRQ_BT_TrigIn_MeasureComplete_callback_p[FM3_BT_AVAILABLE])(void);


int fm3_BT_writeEnable(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_writeTimerFunction(fm3_BT_available_t __BT, fm3_BT_functions_t __function);
int fm3_BT_writeOperationMode(fm3_BT_available_t __BT, fm3_BT_operationMode_t	__operationMode);
int fm3_BT_writeOutputPolarity(fm3_BT_available_t __BT, fm3_BT_outputPolarity_t __outputPolarity);
int fm3_BT_writeTriggerRestart(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_writeCountClock(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS);
int fm3_BT_write_PCSR(fm3_BT_available_t __BT, uint32_t __PRCS);
int fm3_BT_write_PDUT(fm3_BT_available_t __BT, uint16_t __PDUT);
int fm3_BT_write_PRLL_PRLH(fm3_BT_available_t __BT, uint16_t __PRLL, uint16_t __PRLH);
int fm3_BT_writeTriggerInputEdges(fm3_BT_available_t __BT, fm3_BT_triggerInputEdges_t __triggerInputEdge);
int fm3_BT_writeMeasureEdges(fm3_BT_available_t __BT, fm3_BT_measureEdges_t __measureEdge);
int fm3_BT_writePulseMask(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_writePWMinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __dutyMatchIRQen, bool __underflowIRQen);
int fm3_BT_writePPGinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen);
int fm3_BT_writeRTinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen);
int fm3_BT_writePWCinterrupts(fm3_BT_available_t __BT, bool __measureCompletionIRQen, bool __overflowIRQen);

int calculate_prescaler_cnt_uint16(uint32_t __frq, fm3_BT_prescaler_t *__prescaler, uint16_t *__cnt);
int calculate_prescaler_cnt_uint32(uint32_t __frq, fm3_BT_prescaler_t *__prescaler, uint32_t *__cnt);
int calculate_prescaler(uint32_t __frq, fm3_BT_prescaler_t *__prescaler);
fm3_BT_CCS_t prescaler2CCS(fm3_BT_prescaler_t __prescaler);
fm3_BT_prescaler_t CCS2perscaler(fm3_BT_CCS_t __CCS);

/**
 * BT pointers initialization
 */
int fm3_BT_init()
{
	int i;

	FM3_BT_R[EBT0] = FM3_BT0;
	FM3_BT_R[EBT1] = FM3_BT1;
	FM3_BT_R[EBT2] = FM3_BT2;
	FM3_BT_R[EBT3] = FM3_BT3;
	FM3_BT_R[EBT4] = FM3_BT4;
	FM3_BT_R[EBT5] = FM3_BT5;
	FM3_BT_R[EBT6] = FM3_BT6;
	FM3_BT_R[EBT7] = FM3_BT7;

	for(i = 0; i < FM3_BT_AVAILABLE; i++)
	{
		IRQ_BT_under_over_flow_callback_p[i] = NULL;
		IRQ_BT_dutyMatch_callback_p[i] = NULL;
		IRQ_BT_TrigIn_MeasureComplete_callback_p[i] = NULL;

		fm3_BT_setTimerFunction((fm3_BT_available_t)i, EBT_FNC_resetMode);
		FM3_BT[i].interruptsAreEnabled = False;
		FM3_BT[i].isEnable = False;
	}
	return 0;
}

/**
 * Enable/Disable the base timer
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_setEnable(fm3_BT_available_t __BT, bool __enable)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;

	FM3_BT[__BT].isEnable = __enable;
	return fm3_BT_writeEnable(__BT,__enable);
}
/**
 * Software trigger
 * When the CTEN bit is "1", writing "1" to the STRG bit enables software triggering.
 * The read value of the STRG bit is always "0".
 * Software triggering is also enabled when "1" is written to the CTEN and STRG bits simultaneously.
 * If the STRG bit is set to "1", software triggering is enabled regardless of the EGS1 and EGS0 settings.
 */
int fm3_BT_start(fm3_BT_available_t __BT)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;

	FM3_BT_R[__BT]->RT.TMCR |= BIT0;

	return 0;
}
/**
 * Set the base timer function: if 32bit function mode selected the timer's number must be even.
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_setTimerFunction(fm3_BT_available_t __BT, fm3_BT_functions_t __function)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM || __function >= EBT_FNC_NUM)
		return 1;
	if((__function == EBT_FNC_reload_32bit || __function == EBT_FNC_PWC_32bit) && (__BT % 2 != 0))
		return 1;

	FM3_BT[__BT].function = __function;
	return fm3_BT_writeTimerFunction(__BT, __function);
}

/**
 * Set the timer operation mode: one shot or continuous mode
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_setOperationMode(fm3_BT_available_t __BT, fm3_BT_operationMode_t	__operationMode)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM || __operationMode >= EBT_OM_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	FM3_BT[__BT].operationMode = __operationMode;
	return fm3_BT_writeOperationMode(__BT, __operationMode);
}

/**
 * Set the timer output polarity: not use this setting with PWC function
 * This function can be used in: PWM, PPG, RTF mode
 */
int fm3_BT_setOutputPolarity(fm3_BT_available_t __BT, fm3_BT_outputPolarity_t __outputPolarity)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM || __outputPolarity >= EBT_OUTP_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode || FM3_BT[__BT].function == EBT_FNC_PWC_32bit || FM3_BT[__BT].function == EBT_FNC_PWC_16bit)
		return 1;
	FM3_BT[__BT].outputPolarity = __outputPolarity;
	return fm3_BT_writeOutputPolarity(__BT, __outputPolarity);
}

/**
 * Set the restart trigger: use this setting only with PWM and PPG function
 * This function can be used in: PWM, PPG mode
 */
int fm3_BT_setTriggerRestart(fm3_BT_available_t __BT, bool __enable)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit && FM3_BT[__BT].function != EBT_FNC_PPG_16bit)
		return 1;

	FM3_BT[__BT].triggerRestartIsEnabled = __enable;
	return fm3_BT_writeTriggerRestart(__BT, __enable);
}

/**
 * Set the count clock
 * This function can be used in: RTF mode
 */
int fm3_BT_setCountClockCnt(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS, uint32_t __cnt)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	//if(FM3_BT[__BT].function != EBT_FNC_reload_16bit && FM3_BT[__BT].function != EBT_FNC_reload_32bit)
			//return 1;

	FM3_BT[__BT].CCS = __CCS;
	FM3_BT[__BT].cnt = __cnt;
	FM3_BT[__BT].prescaler = CCS2perscaler(__CCS);

	return (fm3_BT_write_PCSR(__BT, __cnt) || fm3_BT_writeCountClock(__BT, __CCS));
}

/**
 * Set the prescaler and count value.
 * This function can be used in: RTF mode
 */
int fm3_BT_setPrescalerCnt(fm3_BT_available_t __BT, fm3_BT_prescaler_t __prescaler, uint32_t __cnt)
{
	fm3_BT_CCS_t CCS;
	if(__BT >= EFM3_BT_AVAILABLE_NUM || __prescaler >= EBT_PRSC_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	//if(FM3_BT[__BT].function != EBT_FNC_reload_16bit && FM3_BT[__BT].function != EBT_FNC_reload_32bit)
		//return 1;
	CCS = prescaler2CCS(__prescaler);
	return fm3_BT_setCountClockCnt(__BT, CCS, __cnt);
}

/**
 * Set the count clock: do not use EBT_CCS_extClokRisingEdge, EBT_CCS_extClkFallingEdge, EBT_CCS_extClkBothEdges in PWC function mode.
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_setCountClock(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	if(  (FM3_BT[__BT].function == EBT_FNC_PWC_16bit || FM3_BT[__BT].function == EBT_FNC_PWC_32bit)
	   &&(__CCS == EBT_CCS_extClokRisingEdge || __CCS == EBT_CCS_extClkFallingEdge || __CCS == EBT_CCS_extClkBothEdges) )
		return 1;

	FM3_BT[__BT].CCS = __CCS;
	FM3_BT[__BT].prescaler = CCS2perscaler(__CCS);

	return  fm3_BT_writeCountClock(__BT, __CCS);
}

/**
 * Set the prescaler.
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_setPrescaler(fm3_BT_available_t __BT, fm3_BT_prescaler_t __prescaler)
{
	fm3_BT_CCS_t CCS;
	if(__BT >= EFM3_BT_AVAILABLE_NUM || __prescaler >= EBT_PRSC_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	CCS = prescaler2CCS(__prescaler);
	return fm3_BT_setCountClock(__BT, CCS);
}

/**
 * Set the PCRS & PDUT registers.
 * This function can be used in: PWM mode
 */
int fm3_BT_set_PCSR_PDUT(fm3_BT_available_t __BT, uint16_t __PCSR, uint16_t __PDUT)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_PWM_16bit)
		return 1;
	if(__PDUT > __PCSR)
		__PCSR = __PDUT;
	return (fm3_BT_write_PCSR(__BT, __PDUT) || fm3_BT_write_PDUT(__BT, __PDUT));
}

/**
 * Set the PRLL & PRLH registers.
 * This function can be used in: PPG mode
 */
int fm3_BT_set_PRLL_PRLH(fm3_BT_available_t __BT, uint16_t __PRLL, uint16_t __PRLH)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_PPG_16bit)
		return 1;
	return fm3_BT_write_PRLL_PRLH(__BT, __PRLL, __PRLH);
}

/**
 * Set the pulse mask: use this setting ONLY in PWM e PPG mode
 * When __enable = False: PWM waveforms are output as they are.
 * When __enable = True: the PWM output is masked with LOW output regardless of the cycle and duty set values.
 * This function can be used in: PWM, PPG mode
 */
int fm3_BT_setPulseMask(fm3_BT_available_t __BT, bool __enable)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit && FM3_BT[__BT].function != EBT_FNC_PPG_16bit)
		return 1;

	FM3_BT[__BT].pulseMaskIsEnabled = __enable;
	return fm3_BT_writePulseMask(__BT, __enable);
}

/**
 * Set the trigger input edges: do NOT use this setting in PWC mode
 * This function can be used in: PWM, PPG, RTF mode
 */
int fm3_BT_setTriggerInputEdges(fm3_BT_available_t __BT, fm3_BT_triggerInputEdges_t __triggerInputEdge)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_PWC_16bit || FM3_BT[__BT].function == EBT_FNC_PWC_32bit)
		return 1;

	FM3_BT[__BT].triggerInputEdge = __triggerInputEdge;
	return fm3_BT_writeTriggerInputEdges(__BT, __triggerInputEdge);
}

/**
 * Set the measure edges: use this setting ONLY in PWC mode
 * This function can be used in: PWC mode
 */
int fm3_BT_setMeasureEdges(fm3_BT_available_t __BT, fm3_BT_measureEdges_t __measureEdge)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWC_16bit && FM3_BT[__BT].function != EBT_FNC_PWC_32bit)
		return 1;

	FM3_BT[__BT].measureEdge = __measureEdge;
	return fm3_BT_writeMeasureEdges(__BT, __measureEdge);
}

/**
 * Enable PWM function mode IRQ. Use this settings ONLY when PWM mode is selected
 * This function can be used in: PWM mode
 */
int fm3_BT_setPWMinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __dutyMatchIRQen, bool __underflowIRQen)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;

	FM3_BT[__BT].PWM_interrupts.triggerInterruptRequestIsEnabled = __triggerIRQen;
	FM3_BT[__BT].PWM_interrupts.dutyMatchInterruptRequestIsEnabled = __dutyMatchIRQen;
	FM3_BT[__BT].PWM_interrupts.underflowInterruptRequestIsEnabled = __underflowIRQen;
	if(__triggerIRQen || __dutyMatchIRQen || __underflowIRQen)
		FM3_BT[__BT].interruptsAreEnabled = True;
	else
		FM3_BT[__BT].interruptsAreEnabled = False;
	return fm3_BT_writePWMinterrupts(__BT, __triggerIRQen, __dutyMatchIRQen, __underflowIRQen);
}

/**
 * Enable PPG function mode IRQ. Use this settings ONLY when PPG mode is selected
 * This function can be used in: PPG mode
 */
int fm3_BT_setPPGinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PPG_16bit)
		return 1;

	FM3_BT[__BT].PPG_interrupts.triggerInterruptRequestIsEnabled = __triggerIRQen;
	FM3_BT[__BT].PPG_interrupts.underflowInterruptRequestIsEnabled = __underflowIRQen;
	if(__triggerIRQen || __underflowIRQen)
		FM3_BT[__BT].interruptsAreEnabled = True;
	else
		FM3_BT[__BT].interruptsAreEnabled = False;
	return fm3_BT_writePPGinterrupts(__BT, __triggerIRQen, __underflowIRQen);
}
/**
 * Enable RTF(Reload timer function) function mode IRQ. Use this settings ONLY when RTF mode is selected
 * This function can be used in: RTF mode
 */
int fm3_BT_setRTinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_reload_16bit && FM3_BT[__BT].function != EBT_FNC_reload_32bit)
		return 1;
	if(__triggerIRQen || __underflowIRQen)
		FM3_BT[__BT].interruptsAreEnabled = True;
	else
		FM3_BT[__BT].interruptsAreEnabled = False;
	FM3_BT[__BT].RT_interrupts.triggerInterruptRequestIsEnabled = __triggerIRQen;
	FM3_BT[__BT].RT_interrupts.underflowInterruptRequestIsEnabled = __underflowIRQen;
	return fm3_BT_writeRTinterrupts(__BT, __triggerIRQen, __underflowIRQen);
}

/**
 * Enable PWC function mode IRQ. Use this settings ONLY when PWC mode is selected
 * This function can be used in: PWC mode
 */
int fm3_BT_setPWCinterrupts(fm3_BT_available_t __BT, bool __measureCompletionIRQen, bool __overflowIRQen)
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWC_16bit && FM3_BT[__BT].function != EBT_FNC_PWC_32bit)
		return 1;
	if(__measureCompletionIRQen || __overflowIRQen)
		FM3_BT[__BT].interruptsAreEnabled = True;
	else
		FM3_BT[__BT].interruptsAreEnabled = False;
	FM3_BT[__BT].PWC_interrupts.measurementCompletionInterruptRequestIsEnabled = __measureCompletionIRQen;
	FM3_BT[__BT].PWC_interrupts.overflowInterruptRequestIsEnabled = __overflowIRQen;
	return fm3_BT_writePWCinterrupts(__BT, __measureCompletionIRQen, __overflowIRQen);
}

/**
 * Set the Base Timer IRQ callback.
 * PWM: underflow IRQ; PPG: underflow IRQ; RT: underflow IRQ; PWC: overflow IRQ
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_set_IRQ_BT_under_over_flow_callback(fm3_BT_available_t __BT, void (*_callback_p)(void))
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	IRQ_BT_under_over_flow_callback_p[__BT] = _callback_p;
	return 0;
}

/**
 * Set the Base Timer IRQ callback.
 * PWM: duty match IRQ; PPG: undefined IRQ; RT: undefined IRQ; PWC: undefined IRQ
 * This function can be used in: PWM mode
 */
int fm3_BT_set_IRQ_dutyMatch_callback(fm3_BT_available_t __BT, void (*_callback_p)(void))
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;
	IRQ_BT_dutyMatch_callback_p[__BT] = _callback_p;
	return 0;
}

/**
 * Set the Base Timer IRQ callback.
 * PWM: trigger input IRQ; PPG: trigger input IRQ; RT: trigger input IRQ; PWC: measurement completion IRQ
 * This function can be used in: PWM, PPG, RTF, PWC mode
 */
int fm3_BT_set_IRQ_TrigIn_MeasureComplete_callback(fm3_BT_available_t __BT, void (*_callback_p)(void))
{
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function == EBT_FNC_resetMode)
		return 1;
	IRQ_BT_TrigIn_MeasureComplete_callback_p[__BT] = _callback_p;
	return 0;
}

/**
 * Set the PWM frequency and duty cycle.
 * This function can be used in: PWM mode only.
 */
int fm3_BT_set_PWM_frq_duty(fm3_BT_available_t __BT, uint32_t __frq, float __duty)
{
	uint16_t cnt;
	uint16_t dut;
	fm3_BT_prescaler_t prescaler;

	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;
	if(__frq <= 0 || __duty < 0)
		return 1;
	FM3_BT[__BT].PWM.frq = __frq;
	FM3_BT[__BT].PWM.T = 1.0/(float)__frq;
	FM3_BT[__BT].PWM.dutyON = __duty;
	FM3_BT[__BT].PWM.dutyOFF = 100.0 - __duty;
	FM3_BT[__BT].PWM.Ton = FM3_BT[__BT].PWM.T * FM3_BT[__BT].PWM.dutyON / 100.0;
	FM3_BT[__BT].PWM.Toff = FM3_BT[__BT].PWM.T * FM3_BT[__BT].PWM.dutyOFF / 100.0;

	calculate_prescaler_cnt_uint16(__frq, &prescaler, &cnt);
	dut = round(FM3_BT[__BT].PWM.dutyON/100.0 * cnt);
	FM3_BT[__BT].prescaler = prescaler;
	FM3_BT[__BT].cnt = cnt;
	if(dut > cnt)
		dut = cnt;
	return ( fm3_BT_setPrescalerCnt(__BT, prescaler, cnt) || fm3_BT_write_PDUT(__BT, dut));
}

int fm3_BT_set_PWM_duty(fm3_BT_available_t __BT, float __duty)
{
	uint16_t dut;
	uint16_t cnt;
	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;

	FM3_BT[__BT].PWM.dutyON = __duty;
	FM3_BT[__BT].PWM.dutyOFF = 100.0 - __duty;
	FM3_BT[__BT].PWM.Ton = FM3_BT[__BT].PWM.T * FM3_BT[__BT].PWM.dutyON / 100.0;
	FM3_BT[__BT].PWM.Toff = FM3_BT[__BT].PWM.T * FM3_BT[__BT].PWM.dutyOFF / 100.0;

	cnt = (uint16_t)FM3_BT[__BT].cnt;
	dut = round(FM3_BT[__BT].PWM.dutyON/100.0 * cnt);
	if(dut > cnt)
		dut = cnt;
	//return ( fm3_BT_setPrescalerCnt(__BT, FM3_BT[__BT].prescaler, cnt) || fm3_BT_write_PDUT(__BT, dut));
	return fm3_BT_write_PDUT(__BT, dut);
}

int fm3_BT_set_PPG_Ton_Toff(fm3_BT_available_t __BT, float __Ton, float __Toff)
{
	uint32_t cnt;
	fm3_BT_prescaler_t prescaler;

	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;
	if(__Ton <= 0 || __Toff <= 0)
		return 1;

	FM3_BT[__BT].PPG.Ton = __Ton;
	FM3_BT[__BT].PPG.Toff = __Toff;
	FM3_BT[__BT].PPG.T = __Ton + __Toff;
	FM3_BT[__BT].PPG.dutyON = (FM3_BT[__BT].PPG.Ton *100.0) / FM3_BT[__BT].PPG.T;
	FM3_BT[__BT].PPG.dutyOFF = (FM3_BT[__BT].PPG.Toff *100.0) / FM3_BT[__BT].PPG.T;
	FM3_BT[__BT].PPG.frq = round(1.0/FM3_BT[__BT].PWM.T);

	calculate_prescaler_cnt_uint32(FM3_BT[__BT].PPG.frq, &prescaler, &cnt);
	FM3_BT[__BT].prescaler = prescaler;
	FM3_BT[__BT].cnt = cnt;
	return (fm3_BT_setPrescaler(__BT, prescaler) || fm3_BT_write_PRLL_PRLH(__BT, round(cnt*FM3_BT[__BT].PPG.dutyOFF), round(cnt*FM3_BT[__BT].PPG.dutyON)));
}

int fm3_BT_set_RT_frq(fm3_BT_available_t __BT, uint32_t __frq)
{
	uint32_t cnt_32;
	uint16_t cnt_16;
	fm3_BT_prescaler_t prescaler;

	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_reload_32bit && FM3_BT[__BT].function != EBT_FNC_reload_16bit)
		return 1;
	if(__frq <= 0)
		return 1;

	if(FM3_BT[__BT].function == EBT_FNC_reload_16bit)
	{
		calculate_prescaler_cnt_uint16(__frq, &prescaler, &cnt_16);
		cnt_32 = cnt_16;
	}else if(FM3_BT[__BT].function == EBT_FNC_reload_32bit)
	{
		calculate_prescaler_cnt_uint32(__frq, &prescaler, &cnt_32);
	}else
	{
		return 1;
	}
	FM3_BT[__BT].RT.frq = __frq;
	FM3_BT[__BT].RT.T = 1.0/(float)__frq;
	FM3_BT[__BT].prescaler = prescaler;
	FM3_BT[__BT].cnt = cnt_32;
	return fm3_BT_setPrescalerCnt(__BT, prescaler, cnt_32);
}

int fm3_BT_set_PWC_T(fm3_BT_available_t __BT, float __T)
{
	fm3_BT_prescaler_t prescaler;

	if(__BT >= EFM3_BT_AVAILABLE_NUM)
		return 1;
	if(FM3_BT[__BT].function != EBT_FNC_PWC_32bit && FM3_BT[__BT].function != EBT_FNC_PWC_16bit)
		return 1;
	if(__T <= 0)
		return 1;


	FM3_BT[__BT].PWC.T = __T;
	FM3_BT[__BT].PWC.frq = round(1.0/__T);
	calculate_prescaler(FM3_BT[__BT].PWC.frq, &prescaler);
	FM3_BT[__BT].prescaler = prescaler;
	return fm3_BT_setPrescaler(__BT, prescaler);
}

FM3_BT_TypeDef *getBT(fm3_BT_available_t __BT)
{
	return FM3_BT_R[__BT];
}
//------------------------------------------------------------------------------------------
//------------------------------------------ ISR -------------------------------------------
//------------------------------------------------------------------------------------------
_IRQ_HANDLER(BaseTimers_Ch0toCh7_IRQ)
{
	//---------------------------------------------------------------
	//--- BASE TIMER 0 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT0->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[0] != NULL)
			IRQ_BT_under_over_flow_callback_p[0]();
		FM3_BT0->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT0->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[0] != NULL)
			IRQ_BT_dutyMatch_callback_p[0]();
		FM3_BT0->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT0->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[0] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[0]();
		FM3_BT0->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 1 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT1->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[1] != NULL)
			IRQ_BT_under_over_flow_callback_p[1]();
		FM3_BT1->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT1->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[1] != NULL)
			IRQ_BT_dutyMatch_callback_p[1]();
		FM3_BT1->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT1->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[1] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[1]();
		FM3_BT1->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 2 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT0->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_under_over_flow_callback_p[2] != NULL)
			IRQ_BT_under_over_flow_callback_p[2]();
		FM3_BT2->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT2->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[2] != NULL)
			IRQ_BT_dutyMatch_callback_p[2]();
		FM3_BT2->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT2->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[2] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[2]();
		FM3_BT2->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 3 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT3->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[3] != NULL)
			IRQ_BT_under_over_flow_callback_p[3]();
		FM3_BT3->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT3->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[3] != NULL)
			IRQ_BT_dutyMatch_callback_p[3]();
		FM3_BT3->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT3->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[3] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[3]();
		FM3_BT3->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 4 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT4->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[4] != NULL)
			IRQ_BT_under_over_flow_callback_p[4]();
		FM3_BT4->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT4->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[4] != NULL)
			IRQ_BT_dutyMatch_callback_p[4]();
		FM3_BT4->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT4->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[4] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[4]();
		FM3_BT4->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 5 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT5->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[5] != NULL)
			IRQ_BT_under_over_flow_callback_p[5]();
		FM3_BT5->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT5->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[5] != NULL)
			IRQ_BT_dutyMatch_callback_p[5]();
		FM3_BT5->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT5->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[5] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[5]();
		FM3_BT5->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 6 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT6->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[6] != NULL)
			IRQ_BT_under_over_flow_callback_p[6]();
		FM3_BT6->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT6->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[6] != NULL)
			IRQ_BT_dutyMatch_callback_p[6]();
		FM3_BT6->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT6->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[6] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[6]();
		FM3_BT6->RT.STC &= ~BIT2;
	}
	//---------------------------------------------------------------
	//--- BASE TIMER 7 IRQ ------------------------------------------
	//---------------------------------------------------------------
	//PWM: underflow IRQ; PPG: underflow IRQ
	//RT:  underflow IRQ; PWC: overflow IRQ
	if((FM3_BT7->RT.STC & BIT0) == BIT0 )
	{
		if(IRQ_BT_under_over_flow_callback_p[7] != NULL)
			IRQ_BT_under_over_flow_callback_p[7]();
		FM3_BT7->RT.STC &= ~BIT0;
	}
	//PWM: duty match IRQ; PPG: undefined IRQ
	//RT:  undefined IRQ; PWC: undefined IRQ
	if((FM3_BT7->PWM.STC & BIT1) == BIT1 )
	{
		if(IRQ_BT_dutyMatch_callback_p[7] != NULL)
			IRQ_BT_dutyMatch_callback_p[7]();
		FM3_BT7->PWM.STC &= ~BIT1;
	}
	//PWM: trigger input IRQ; PPG: trigger input IRQ
	//RT:  trigger input IRQ; PWC: measurement completion IRQ
	if((FM3_BT7->RT.STC & BIT2) == BIT2 )
	{
		if(IRQ_BT_TrigIn_MeasureComplete_callback_p[7] != NULL)
			IRQ_BT_TrigIn_MeasureComplete_callback_p[7]();
		FM3_BT7->RT.STC &= ~BIT2;
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

int fm3_BT_writeEnable(fm3_BT_available_t __BT, bool __enable)
{
	if(__enable)
		FM3_BT_R[__BT]->RT.TMCR |= (BIT1);
	else
		FM3_BT_R[__BT]->RT.TMCR &= ~(BIT1);
	return 0;
}

int fm3_BT_writeTimerFunction(fm3_BT_available_t __BT, fm3_BT_functions_t __function)
{
	switch (__function)
	{
		case EBT_FNC_resetMode:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT7);
			break;
		case EBT_FNC_PWM_16bit:
			FM3_BT_R[__BT]->PWM.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->PWM.TMCR |= (BIT4);
			break;
		case EBT_FNC_PPG_16bit:
			FM3_BT_R[__BT]->PPG.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->PPG.TMCR |= (BIT5);
			break;
		case EBT_FNC_reload_16bit:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT4 + BIT5);
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT7);
			break;
		case EBT_FNC_PWC_16bit:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT6);
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT7);
			break;
		case EBT_FNC_reload_32bit:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT4 + BIT5);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT7);
			break;
		case EBT_FNC_PWC_32bit:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT6);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT7);
			break;
		default:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT4 + BIT5 + BIT6);
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT7);
			break;
	};
	return 0;
}

int fm3_BT_writeOperationMode(fm3_BT_available_t __BT, fm3_BT_operationMode_t	__operationMode)
{
	switch(__operationMode)
	{
		case EBT_OM_continuous:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT2);
			break;
		case EBT_OM_oneShot:
			FM3_BT_R[__BT]->RT.TMCR |= (BIT2);
			break;
		default:
			break;
	}
	return 0;
}

int fm3_BT_writeOutputPolarity(fm3_BT_available_t __BT, fm3_BT_outputPolarity_t __outputPolarity)
{
	switch(__outputPolarity)
	{
		case EBT_OUTP_normal:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT3);
			break;
		case EBT_OUTP_inverted:
			FM3_BT_R[__BT]->RT.TMCR |= (BIT3);
			break;
		default:
			break;
	};
	return 0;
}

int fm3_BT_writePulseMask(fm3_BT_available_t __BT, bool __enable)
{
	if(__enable)
		FM3_BT_R[__BT]->PWM.TMCR |= BIT10;
	else
		FM3_BT_R[__BT]->PWM.TMCR &= ~BIT10;
	return 0;
}

int fm3_BT_writeTriggerRestart(fm3_BT_available_t __BT, bool __enable)
{
	if(__enable)
		FM3_BT_R[__BT]->RT.TMCR |= (BIT11);
	else
		FM3_BT_R[__BT]->RT.TMCR &= ~(BIT11);

	return 0;
}

int fm3_BT_writeCountClock(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS)
{
	switch(__CCS)
	{
		case EBT_CCS_clk:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			break;
		case EBT_CCS_clk_4:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT12);
			break;
		case EBT_CCS_clk_16:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT13);
			break;
		case EBT_CCS_clk_128:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT12 + BIT13);
			break;
		case EBT_CCS_clk_256:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT14);
			break;
		case EBT_CCS_clk_512:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR2 |= (BIT1);
			break;
		case EBT_CCS_clk_1024:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR2 |= (BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT12);
			break;
		case EBT_CCS_clk_2048:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR2 |= (BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT13);
			break;
		case EBT_CCS_extClokRisingEdge:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT12 + BIT14);
			break;
		case EBT_CCS_extClkFallingEdge:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT13 + BIT14);
			break;
		case EBT_CCS_extClkBothEdges:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT12 + BIT13 + BIT14);
			break;
		default:
			FM3_BT_R[__BT]->RT.TMCR  &= ~(BIT12 + BIT13 + BIT14);
			FM3_BT_R[__BT]->RT.TMCR2 &= ~(BIT1);
			break;
	};
	return 0;
}

int fm3_BT_write_PCSR(fm3_BT_available_t __BT, uint32_t __PRCS)
{
	switch (FM3_BT[__BT].function)
	{
		case EBT_FNC_PWM_16bit:
			FM3_BT_R[__BT]->PWM.PCSR = (uint16_t)__PRCS;
			break;
		case EBT_FNC_reload_16bit:
			FM3_BT_R[__BT]->RT.PCSR = (uint16_t)__PRCS;
			break;
		case EBT_FNC_reload_32bit:
			FM3_BT_R[__BT+1]->RT.PCSR = (uint16_t)((__PRCS >> 16) & 0x0000FFFF);
			FM3_BT_R[__BT]->RT.PCSR = (uint16_t)(__PRCS & 0x0000FFFF);
			break;
		default:
			return 1;
			break;
	};
	return 0;
}

int fm3_BT_write_PDUT(fm3_BT_available_t __BT, uint16_t __PDUT)
{
	if(FM3_BT[__BT].function != EBT_FNC_PWM_16bit)
		return 1;
	FM3_BT_R[__BT]->PWM.PDUT = __PDUT;
	return 0;
}

int fm3_BT_write_PRLL_PRLH(fm3_BT_available_t __BT, uint16_t __PRLL, uint16_t __PRLH)
{
	if(FM3_BT[__BT].function != EBT_FNC_PPG_16bit)
		return 1;
	FM3_BT_R[__BT]->PPG.PRLH = __PRLH;
	FM3_BT_R[__BT]->PPG.PRLL = __PRLL;
	return 0;
}

int fm3_BT_writeTriggerInputEdges(fm3_BT_available_t __BT, fm3_BT_triggerInputEdges_t __triggerInputEdge)
{
	switch(__triggerInputEdge)
	{
		case EBT_TIE_disbaled:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT8 + BIT9);
			break;
		case EBT_TIE_risingEdge:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT8 + BIT9);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT8);
			break;
		case EBT_TIE_fallingEdge:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT8 + BIT9);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT9);
			break;
		case EBT_TIE_bothEdges:
			FM3_BT_R[__BT]->RT.TMCR &= ~(BIT8 + BIT9);
			FM3_BT_R[__BT]->RT.TMCR |= (BIT8 +  BIT9);
			break;
		default:
			break;
	};
	return 0;
}

int fm3_BT_writeMeasureEdges(fm3_BT_available_t __BT, fm3_BT_measureEdges_t __measureEdge)
{
	switch(__measureEdge)
	{
		case EBT_PWCME_highPulseMeasure:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT8 + BIT9 + BIT10);
			break;
		case EBT_PWCME_cycleMeasureBetweenRisingEdges:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT8 + BIT9 + BIT10);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT8);
			break;
		case EBT_PWCME_cycleMeasureBetweenFallingEdges:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT8 + BIT9 + BIT10);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT9);
			break;
		case EBT_PWCME_pulseMeasureBetweenAllEdges:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT8 + BIT9 + BIT10);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT8 +  BIT9);
			break;
		case EBT_PWCME_lowPulseMeasure:
			FM3_BT_R[__BT]->PWC.TMCR &= ~(BIT8 + BIT9 + BIT10);
			FM3_BT_R[__BT]->PWC.TMCR |= (BIT10);
			break;
		default:
			break;
	};
	return 0;
}

int fm3_BT_writePWMinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __dutyMatchIRQen, bool __underflowIRQen)
{
	if(__triggerIRQen)
		FM3_BT_R[__BT]->PWM.STC |= BIT6;
	else
		FM3_BT_R[__BT]->PWM.STC &= ~BIT6;

	if(__dutyMatchIRQen)
		FM3_BT_R[__BT]->PWM.STC |= BIT5;
	else
		FM3_BT_R[__BT]->PWM.STC &= ~BIT5;

	if(__underflowIRQen)
		FM3_BT_R[__BT]->PWM.STC |= BIT4;
	else
		FM3_BT_R[__BT]->PWM.STC &= ~BIT4;

	return 0;
}

int fm3_BT_writePPGinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen)
{
	if(__triggerIRQen)
		FM3_BT_R[__BT]->PPG.STC |= BIT6;
	else
		FM3_BT_R[__BT]->PPG.STC &= ~BIT6;

	if(__underflowIRQen)
		FM3_BT_R[__BT]->PPG.STC |= BIT4;
	else
		FM3_BT_R[__BT]->PPG.STC &= ~BIT4;

	return 0;
}

int fm3_BT_writeRTinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen)
{
	if(__triggerIRQen)
		FM3_BT_R[__BT]->RT.STC |= BIT6;
	else
		FM3_BT_R[__BT]->RT.STC &= ~BIT6;

	if(__underflowIRQen)
		FM3_BT_R[__BT]->RT.STC |= BIT4;
	else
		FM3_BT_R[__BT]->RT.STC &= ~BIT4;

	return 0;
}

int fm3_BT_writePWCinterrupts(fm3_BT_available_t __BT, bool __measureCompletionIRQen, bool __overflowIRQen)
{
	if(__measureCompletionIRQen)
		FM3_BT_R[__BT]->PWC.STC |= BIT6;
	else
		FM3_BT_R[__BT]->PWC.STC &= ~BIT6;

	if(__overflowIRQen)
		FM3_BT_R[__BT]->PWC.STC |= BIT4;
	else
		FM3_BT_R[__BT]->PWC.STC &= ~BIT4;

	return 0;
}

/**
 * __frq = (FM3_BTCLK)/(__prescaler * (__cnt + 1))
 */
int calculate_prescaler_cnt_uint16(uint32_t __frq, fm3_BT_prescaler_t *__prescaler, uint16_t *__cnt)
{
	int i;
	uint32_t frq_diff[EBT_PRSC_NUM];
	uint32_t frq_min = 1000000000;
	uint16_t cnt;

	for(i = 0; i < EBT_PRSC_NUM; i++)
	{
		cnt = round((float)FM3_BTCLK/(float)(FM3_BT_PRSC_VALUE_ARRAY[i] * __frq));
		frq_diff[i]  = abs(__frq - ((FM3_BTCLK)/(FM3_BT_PRSC_VALUE_ARRAY[i] * (cnt))));
		if(frq_diff[i] < frq_min)
		{
			frq_min = frq_diff[i];
			*__prescaler = (fm3_BT_prescaler_t)i;
			*__cnt = cnt--;
		}
	}
	return 0;
}

/**
 * __frq = (FM3_BTCLK)/(__prescaler * (__cnt + 1))
 */
int calculate_prescaler_cnt_uint32(uint32_t __frq, fm3_BT_prescaler_t *__prescaler, uint32_t *__cnt)
{
	int i;
	uint32_t frq_diff[EBT_PRSC_NUM];
	uint32_t frq_min = 100000000;
	uint32_t cnt;

	for(i = 0; i < EBT_PRSC_NUM; i++)
	{
		cnt = ((float)FM3_BTCLK/(float)(FM3_BT_PRSC_VALUE_ARRAY[i] * __frq));
		frq_diff[i]  = abs(__frq - ((FM3_BTCLK)/(FM3_BT_PRSC_VALUE_ARRAY[i] * (cnt))));
		if(frq_diff[i] < frq_min)
		{
			frq_min = frq_diff[i];
			*__prescaler = (fm3_BT_prescaler_t)i;
			*__cnt = cnt--;
		}
	}
	return 0;
}

int calculate_prescaler(uint32_t __frq, fm3_BT_prescaler_t *__prescaler)
{
	int i;
	uint32_t frq_diff[EBT_PRSC_NUM];
	uint32_t frq_min = 100000000;

	for(i = 0; i < EBT_PRSC_NUM; i++)
	{
		frq_diff[i]  = abs(__frq - ((FM3_BTCLK)/(FM3_BT_PRSC_VALUE_ARRAY[i])) );
		if(frq_diff[i] < frq_min)
		{
			frq_min = frq_diff[i];
			*__prescaler = (fm3_BT_prescaler_t)i;
		}
	}
	return 0;
}
fm3_BT_CCS_t prescaler2CCS(fm3_BT_prescaler_t __prescaler)
{
	switch(__prescaler)
	{
		case EBT_PRSC_clk:
			return EBT_CCS_clk;
		case EBT_PRSC_clk_4:
			return EBT_CCS_clk_4;
		case EBT_PRSC_clk_16:
			return EBT_CCS_clk_16;
		case EBT_PRSC_clk_128:
			return EBT_CCS_clk_128;
		case EBT_PRSC_clk_256:
			return EBT_CCS_clk_256;
		case EBT_PRSC_clk_512:
			return  EBT_CCS_clk_512;
		case EBT_PRSC_clk_1024:
			return EBT_CCS_clk_1024;
		case EBT_PRSC_clk_2048:
			return EBT_CCS_clk_2048;
		default:
			return EBT_CCS_NUM;
	}
	return EBT_CCS_NUM;
}

fm3_BT_prescaler_t CCS2perscaler(fm3_BT_CCS_t __CCS)
{
	switch(__CCS)
	{
		case EBT_CCS_clk:
			return EBT_PRSC_clk;
		case EBT_CCS_clk_4:
			return EBT_PRSC_clk_4;
		case EBT_CCS_clk_16:
			return EBT_PRSC_clk_16;
		case EBT_CCS_clk_128:
			return EBT_PRSC_clk_128;
		case EBT_CCS_clk_256:
			return EBT_PRSC_clk_256;
		case EBT_CCS_clk_512:
			return  EBT_PRSC_clk_512;
		case EBT_CCS_clk_1024:
			return EBT_PRSC_clk_1024;
		case EBT_CCS_clk_2048:
			return EBT_PRSC_clk_2048;
		default:
			return EBT_PRSC_NUM;
	}
	return EBT_PRSC_NUM;
}

