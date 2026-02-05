/**
@file		fm3_baseTimer.h
@brief		fm3 base timer setup

@author		Nicola
@date		17/06/2012
@version	01.00.00
*/

#ifndef _FM3_BASE_TIMER_H_
#define _FM3_BASE_TIMER_H_

#include "global.h"
#include "mb9bf50x.h"
#include "arith.h"
#include "fm3_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Hardware description
 */
typedef struct
{
	union
	{
		FM3_BT_PPG_TypeDef PPG; /* Base Timer PPG registers */
		FM3_BT_PWM_TypeDef PWM; /* Base Timer PWM registers */
		FM3_BT_RT_TypeDef RT; /* Base Timer  RT registers */
		FM3_BT_PWC_TypeDef PWC; /* Base Timer PWC registers */
	};
}FM3_BT_TypeDef;

#define FM3_BT0      ((FM3_BT_TypeDef *)FM3_BT0_RT_BASE)
#define FM3_BT1      ((FM3_BT_TypeDef *)FM3_BT1_RT_BASE)
#define FM3_BT2      ((FM3_BT_TypeDef *)FM3_BT2_RT_BASE)
#define FM3_BT3      ((FM3_BT_TypeDef *)FM3_BT3_RT_BASE)
#define FM3_BT4      ((FM3_BT_TypeDef *)FM3_BT4_RT_BASE)
#define FM3_BT5      ((FM3_BT_TypeDef *)FM3_BT5_RT_BASE)
#define FM3_BT6      ((FM3_BT_TypeDef *)FM3_BT6_RT_BASE)
#define FM3_BT7      ((FM3_BT_TypeDef *)FM3_BT7_RT_BASE)

enum fm3_BT_available_t
{
	EBT0 = 0,
	EBT1,
	EBT2,
	EBT3,
	EBT4,
	EBT5,
	EBT6,
	EBT7,
	EFM3_BT_AVAILABLE_NUM
};

#define FM3_BT_AVAILABLE 	8
#define FM3_BT_AVAILABLE_MAX	16
#if (FM3_BT_AVAILABLE > FM3_BT_AVAILABLE_MAX)
	#error "BT: FM3_BT_AVAILABLE > 16!! Not supported!!!!"
#endif

#ifdef __cplusplus
#else
typedef enum fm3_BT_available_t 		fm3_BT_available_t;
#endif
/**
 * timer functions
 */
enum fm3_BT_functions_t
{
	EBT_FNC_resetMode = 0, /*< The reset mode is a status where the base timer macros are reset
							 (with each register set to the initial value).*/
	EBT_FNC_PWM_16bit, /*<	This timer consists of a 16-bit down counter, a 16-bit data register with a cycle set buffer,
					a 16-bit compare register with a duty set buffer, and a pin controller*/
	EBT_FNC_PPG_16bit, /*< This timer consists of a 16-bit down counter, a 16-bit data register for setting the HIGH width,
					a 16-bit data register for setting the LOW width, and a pin controller.*/
	EBT_FNC_reload_16bit, /*< This timer consists of a 16-bit down counter, a 16-bit reload register, and a pin controller*/
	EBT_FNC_PWC_16bit, /*< This timer consists of a 16-bit up counter, a measurement input pin, and a control register.*/
	EBT_FNC_reload_32bit, /*< This timer consists of a 32-bit down counter, a 32-bit reload register, and a pin controller
	 	 	 	 	 	 	 It uses two channels, even and odd, to operate as a 32-bit reload timer.*/
	EBT_FNC_PWC_32bit, /*< This timer consists of a 16-bit up counter, a measurement input pin, and a control register.
	 	 	 	 	 	  It uses two channels, even and odd, to operate as a 32-bit reload timer.*/
	EBT_FNC_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_functions_t 		fm3_BT_functions_t;
#endif

/**
 * Base Timer operation Mode: selects continuous pulse output or one-shot pulse output
 */
enum fm3_BT_operationMode_t
{
	EBT_OM_continuous = 0,
	EBT_OM_oneShot,
	EBT_OM_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_operationMode_t 		fm3_BT_operationMode_t;
#endif

/**
 * Base timer Output polarity: sets the polarity of the PWM output.
 */
enum fm3_BT_outputPolarity_t
{
	EBT_OUTP_normal = 0, /*< After reset: LOW output, Match in duty: LOW to HIGH, Underflow: HIGH to LOW*/
	EBT_OUTP_inverted, /*< After reset: HIGH output, Match in duty: HIGH to LOW, Underflow: LOW to HIGH*/
	EBT_OUTP_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_outputPolarity_t 		fm3_BT_outputPolarity_t;
#endif

/**
 * Base timer trigger input edges: select a valid edge for input waveforms as an external start cause and set the trigger condition
 */
enum fm3_BT_triggerInputEdges_t
{
	EBT_TIE_disbaled = 0,
	EBT_TIE_risingEdge,
	EBT_TIE_fallingEdge,
	EBT_TIE_bothEdges,
	EBT_TIE_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_triggerInputEdges_t 		fm3_BT_triggerInputEdges_t;
#endif

/**
 * Base Timer Measurement edge detection: set measurement edge conditions in PWC mode
 */
enum fm3_BT_measureEdges_t
{
	EBT_PWCME_highPulseMeasure = 0,
	EBT_PWCME_cycleMeasureBetweenRisingEdges,
	EBT_PWCME_cycleMeasureBetweenFallingEdges,
	EBT_PWCME_pulseMeasureBetweenAllEdges,
	EBT_PWCME_lowPulseMeasure,
	EBT_PWCME_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_measureEdges_t 		fm3_BT_measureEdges_t;
#endif

/**
 * Baset timer Count clock selection: Select the count clock
 */
enum fm3_BT_CCS_t
{
	EBT_CCS_clk,
	EBT_CCS_clk_4,
	EBT_CCS_clk_16,
	EBT_CCS_clk_128,
	EBT_CCS_clk_256,
	EBT_CCS_clk_512,
	EBT_CCS_clk_1024,
	EBT_CCS_clk_2048,
	EBT_CCS_extClokRisingEdge,
	EBT_CCS_extClkFallingEdge,
	EBT_CCS_extClkBothEdges,
	EBT_CCS_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_CCS_t 		fm3_BT_CCS_t;
#endif

/**
 * Base Timer: prescaler
 */
enum fm3_BT_prescaler_t
{
	EBT_PRSC_clk,
	EBT_PRSC_clk_4,
	EBT_PRSC_clk_16,
	EBT_PRSC_clk_128,
	EBT_PRSC_clk_256,
	EBT_PRSC_clk_512,
	EBT_PRSC_clk_1024,
	EBT_PRSC_clk_2048,
	EBT_PRSC_NUM
};
#ifdef __cplusplus
#else
typedef enum fm3_BT_prescaler_t 	fm3_BT_prescaler_t;
#endif
#define FM3_BT_PRSC_NUM_MAX 	8
static const uint32_t FM3_BT_PRSC_VALUE_ARRAY[FM3_BT_PRSC_NUM_MAX] ={1, 4, 16, 128, 256, 512, 1024, 2048};

/**
 * Base Timer PWM Interrupts
 */
struct fm3_BT_PWM_interrupts_t
{
	bool triggerInterruptRequestIsEnabled;
	bool dutyMatchInterruptRequestIsEnabled;
	bool underflowInterruptRequestIsEnabled;
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PWM_interrupts_t 	fm3_BT_PWM_interrupts_t;
#endif

/**
 * Base Timer PPG Interrupts
 */
struct fm3_BT_PPG_interrupts_t
{
	bool triggerInterruptRequestIsEnabled;
	bool reserved;
	bool underflowInterruptRequestIsEnabled;
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PPG_interrupts_t 	fm3_BT_PPG_interrupts_t;
#endif

/**
 * Base Timer Reload Function Timer Interrupts
 */
struct fm3_BT_RT_interrupts_t
{
	bool triggerInterruptRequestIsEnabled;
	bool reserved;
	bool underflowInterruptRequestIsEnabled;
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_RT_interrupts_t 	fm3_BT_RT_interrupts_t;
#endif

/**
 * Base Pulse Width Count (PWC) Interrupts
 */
struct fm3_BT_PWC_interrupts_t
{
	bool measurementCompletionInterruptRequestIsEnabled;
	bool reserved;
	bool overflowInterruptRequestIsEnabled;
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PWC_interrupts_t 	fm3_BT_PWC_interrupts_t;
#endif
/**
 * Base Timer PWM type
 */
struct fm3_BT_PWM_t
{
	uint32_t frq;	/*< pwm frequency (Hz)*/
	float dutyON;	/*< pwm duty cycle ON percent (%)*/
	float dutyOFF;	/*< pwm duty cycle OFF percent (%)*/
	float T	;		/*< pwm period (sec)*/
	float Ton;		/*< pwm on time (sec)*/
	float Toff;		/*< pwm off time (sec)*/
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PWM_t 	fm3_BT_PWM_t;
#endif

/**
 * Base Timer PPG type
 */
struct fm3_BT_PPG_t
{
	uint32_t frq;		/*< ppg frequency (Hz)*/
	float dutyON;		/*< ppg duty cycle ON percent (%)*/
	float dutyOFF;		/*< ppg duty cycle OFF percent (%)*/
	float T;			/*< ppg period (sec)*/
	float Ton;		/*< ppg on time (sec)*/
	float Toff;		/*< ppg off time (sec)*/
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PPG_t 	fm3_BT_PPG_t;
#endif

/**
 * Base Timer Reload Function Timer type
 */
struct fm3_BT_RTF_t
{
	uint32_t frq;		/*< frequency (Hz)*/
	float reserved_1[2];
	float T	;		/*< period (sec)*/
	float reserved_2[2];
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_RTF_t 	fm3_BT_RTF_t;
#endif

/**
 * Base Timer PWC type
 */
struct fm3_BT_PWC_t
{
	uint32_t frq;		/*< frequency (Hz)*/
	float reserved_1[2];
	float T;			/*< period (sec)*/
	float reserved_2[2];
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_PWC_t 	fm3_BT_PWC_t;
#endif

/**
 * Base Timer
 */
struct fm3_BT_t
{
	bool isEnable;
	fm3_BT_functions_t function;
	fm3_BT_operationMode_t	operationMode;
	fm3_BT_outputPolarity_t outputPolarity;
	bool triggerRestartIsEnabled; /*<  Base timer restart: enables restart by a software trigger or trigger input*/
	fm3_BT_CCS_t CCS; // count clock selection
	bool pulseMaskIsEnabled; //controls the output level of PWM output waveforms.
							 // When False: PWM waveforms are output as they are.
							 // When True: the PWM output is masked with LOW output regardless of the cycle and duty set values.
	uint32_t 		cnt;
	fm3_BT_prescaler_t  prescaler;
	union
	{
		fm3_BT_triggerInputEdges_t triggerInputEdge;
		fm3_BT_measureEdges_t measureEdge;
	};
	union
	{
		fm3_BT_PWM_interrupts_t PWM_interrupts;
		fm3_BT_PPG_interrupts_t PPG_interrupts;
		fm3_BT_RT_interrupts_t RT_interrupts;
		fm3_BT_PWC_interrupts_t PWC_interrupts;
	};
	bool interruptsAreEnabled; //at least one interrupt was enabled;
	union
	{
		fm3_BT_PWM_t	PWM;
		fm3_BT_PPG_t	PPG;
		fm3_BT_RTF_t	RT;
		fm3_BT_PWC_t	PWC;
	};
};
#ifdef __cplusplus
#else
typedef struct fm3_BT_t 		fm3_BT_t;
#endif

int fm3_BT_init();
int fm3_BT_setEnable(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_start(fm3_BT_available_t __BT);
int fm3_BT_setTimerFunction(fm3_BT_available_t __BT, fm3_BT_functions_t __function);
int fm3_BT_setOperationMode(fm3_BT_available_t __BT, fm3_BT_operationMode_t	__operationMode);
int fm3_BT_setOutputPolarity(fm3_BT_available_t __BT, fm3_BT_outputPolarity_t __outputPolarity);
int fm3_BT_setTriggerRestart(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_setCountClockCnt(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS, uint32_t __cnt);
int fm3_BT_setPrescalerCnt(fm3_BT_available_t __BT, fm3_BT_prescaler_t __prescaler, uint32_t __cnt);
int fm3_BT_setCountClock(fm3_BT_available_t __BT, fm3_BT_CCS_t __CCS);
int fm3_BT_setPrescaler(fm3_BT_available_t __BT, fm3_BT_prescaler_t __prescaler);
int fm3_BT_set_PCSR_PDUT(fm3_BT_available_t __BT, uint16_t __PCSR, uint16_t __PDUT);
int fm3_BT_set_PRLL_PRLH(fm3_BT_available_t __BT, uint16_t __PRLL, uint16_t __PRLH);
int fm3_BT_setPulseMask(fm3_BT_available_t __BT, bool __enable);
int fm3_BT_setTriggerInputEdges(fm3_BT_available_t __BT, fm3_BT_triggerInputEdges_t __triggerInputEdge);
int fm3_BT_setMeasureEdges(fm3_BT_available_t __BT, fm3_BT_measureEdges_t __measureEdge);
int fm3_BT_setPWMinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __dutyMatchIRQen, bool __underflowIRQen);
int fm3_BT_setPPGinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen);
int fm3_BT_setRTinterrupts(fm3_BT_available_t __BT, bool __triggerIRQen, bool __underflowIRQen);
int fm3_BT_setPWCinterrupts(fm3_BT_available_t __BT, bool __measureCompletionIRQen, bool __overflowIRQen);

int fm3_BT_set_IRQ_BT_under_over_flow_callback(fm3_BT_available_t __BT, void (*__pfCallback)(void));
int fm3_BT_set_IRQ_dutyMatch_callback(fm3_BT_available_t __BT, void (*__pfCallback)(void));
int fm3_BT_set_IRQ_TrigIn_MeasureComplete_callback(fm3_BT_available_t __BT, void (*__pfCallback)(void));

int fm3_BT_set_PWM_frq_duty(fm3_BT_available_t __BT, uint32_t __frq, float __duty);
int fm3_BT_set_PWM_duty(fm3_BT_available_t __BT, float __duty);

int fm3_BT_set_PPG_Ton_Toff(fm3_BT_available_t __BT, float __Ton, float __Toff);

int fm3_BT_set_RT_frq(fm3_BT_available_t __BT, uint32_t __frq);

int fm3_BT_set_PWC_T(fm3_BT_available_t __BT, float __T);

FM3_BT_TypeDef *getBT(fm3_BT_available_t __BT)							__attribute__ (( section(".ramcode") ));

#ifdef __cplusplus
}
#endif

#endif
