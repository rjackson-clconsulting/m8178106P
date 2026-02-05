/**
@file		fm3_sysclk.h
@brief		system clock setup

@author		Enrico
@date		18/01/2011
@version	01.1
*/

#ifndef _SYSCLK_H_
#define _SYSCLK_H_

#include <ch.h>
#include "fm3_define.h"
#include "mb9bf50x.h"
#include "global.h"

enum sysCLKErrors_t{ ESYSCLK_ERR_NONE,
				ESYSCLK_ERR_MAINCLK = SYSCLK_BASE_ERR,	// errore su main osc

				ESYSCLK_ERR_Base_Clock_Prescaler,
				ESYSCLK_ERR_APB0_Prescaler,
				ESYSCLK_ERR_APB1_Prescaler,
				ESYSCLK_ERR_APB2_Prescaler,
				ESYSCLK_ERR_SoftwareWatchdogClockPrescaler,
				ESYSCLK_ERR_SUBCLK,				// errore subosc
				ESYSCLK_ERR_PLLCLK,				// errore pll

 			};

/**
 * High Speed CR oscillation Frequency Division
 */
enum fm3_HSCR_prescaler_t
{
	EHSCR_MCR_PSR_clk_4,
	EHSCR_MCR_PSR_clk_8,
	EHSCR_MCR_PSR_clk_16,
	EHSCR_MCR_PSR_clk_32,
	EHSCR_MCR_PSR_NUM,
};
typedef enum fm3_HSCR_prescaler_t 		fm3_HSCR_prescaler_t;
#ifndef __cplusplus
typedef enum sysCLKErrors_t sysCLKErrors_t;
#endif




#ifdef __cplusplus
extern "C" {
#endif

sysCLKErrors_t fm3_sysclock_init(uint8_t __pllM, uint8_t __pllK, uint8_t __pllN);

int fm3_HSCR_writePrescalerHSCRcnt(fm3_HSCR_prescaler_t __prescaler);
int fm3_HSCR_setPrescalerHSCRcnt(fm3_HSCR_prescaler_t __prescaler);



#ifdef __cplusplus
}
#endif

#endif /* _SYSCLK_H_ */
