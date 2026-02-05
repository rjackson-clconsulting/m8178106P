/**
@file		fm3_sysclk.c
@brief		fm3 system clock setup

@author		Enrico
@date		18/01/2011
@version	01.1
*/
#include "fm3_sysclk.h"

#define	APB1_ENABLE	0x80
#define	APB2_ENABLE	0x80

static sysCLKErrors_t sysclkError;

/**
 * @brief   system clock initalisation
 * @details
 *
 *
 * @note
 */
sysCLKErrors_t fm3_sysclock_init(uint8_t __pllM, uint8_t __pllK, uint8_t __pllN)
{
long clkstab;
uint8_t prescal;
uint8_t prescaler_r;

	sysclkError = ESYSCLK_ERR_NONE;

	FM3_CRG->SCM_CTL  = 0;       					/* Set Master Clock switch to internal RC oscillator */

	FM3_CRG->BSC_PSR   = 0x00;               	/* set System Clock presacaler */
	FM3_CRG->APBC0_PSR = 0x00;              	/* set APB0 presacaler */
	FM3_CRG->APBC1_PSR = 0x80;              	/* set APB1 presacaler */
	FM3_CRG->APBC2_PSR = 0x80;              	/* set APB2 presacaler */
	FM3_CRG->SWC_PSR   = 0x80;   				/* set SW Watchdog presacaler */
	FM3_CRG->TTC_PSR   = 0;                		/* set Trace Clock presacaler */

    /* CR Trimming Data  */
    if( (FM3_FLASH_IF->CRTRMM & 0x000003FF) != 0x000003FF )
    {
	    /* UnLock (MCR_FTRM) */
        FM3_CRTRIM->MCR_RLR = 0x1ACCE554;
	    /* Set MCR_FTRM */
        FM3_CRTRIM->MCR_FTRM = FM3_FLASH_IF->CRTRMM;
	    /* Lock (MCR_FTRM) */
        FM3_CRTRIM->MCR_RLR = 0x00000000;
    }

    //----- Base Clock Prescaler -----
	prescal = (FM3_PLLCLK/FM3_SYSCLK);
	switch(prescal)
	{
		case 1:
			prescaler_r = 0;
			break;
		case 2:
			prescaler_r = 1;
			break;
		case 3:
			prescaler_r = 2;
			break;
		case 4:
			prescaler_r = 3;
			break;
		case 6:
			prescaler_r = 4;
			break;
		case 8:
			prescaler_r = 5;
			break;
		case 16:
			prescaler_r = 6;
			break;
		default:
			sysclkError = ESYSCLK_ERR_Base_Clock_Prescaler;
			return sysclkError;
			break;
	}
	FM3_CRG->BSC_PSR   = prescaler_r;      				// set System Clock presacaler

	//----- APB0 Prescaler -----
	prescal = FM3_SYSCLK/FM3_APB0CLK;
	switch(prescal)
	{
		case 1:
			prescaler_r = 0;
			break;
		case 2:
			prescaler_r = 1;
			break;
		case 4:
			prescaler_r = 2;
			break;
		case 8:
			prescaler_r = 3;
			break;
		default:
			sysclkError = ESYSCLK_ERR_APB0_Prescaler;
			return sysclkError;
			break;
	}
	FM3_CRG->APBC0_PSR = prescaler_r;       // set APB0 prescaler

	//----- APB1 Prescaler -----
	prescal = FM3_SYSCLK/FM3_APB1CLK;
	switch(prescal)
	{
		case 1:
			prescaler_r = 0;
			break;
		case 2:
			prescaler_r = 1;
			break;
		case 4:
			prescaler_r = 2;
			break;
		case 8:
			prescaler_r = 3;
			break;
		default:
			sysclkError = ESYSCLK_ERR_APB1_Prescaler;
			return sysclkError;
			break;
	}
	FM3_CRG->APBC1_PSR = prescaler_r | APB1_ENABLE;       // set APB1 prescaler

	//----- APB2 Prescaler -----
	prescal = FM3_SYSCLK/FM3_APB2CLK;
	switch(prescal)
	{
		case 1:
			prescaler_r = 0;
			break;
		case 2:
			prescaler_r = 1;
			break;
		case 4:
			prescaler_r = 2;
			break;
		case 8:
			prescaler_r = 3;
			break;
		default:
			sysclkError = ESYSCLK_ERR_APB2_Prescaler;
			return sysclkError;
			break;
	}
	FM3_CRG->APBC2_PSR = prescaler_r | APB2_ENABLE;       // set APB2 prescaler

	//----- ASoftware Watchdog Clock Prescaler -----
	prescal = FM3_APB0CLK/FM3_SWDOGCLK;
	switch(prescal)
	{
		case 1:
			prescaler_r = 0;
			break;
		case 2:
			prescaler_r = 1;
			break;
		case 4:
			prescaler_r = 2;
			break;
		case 8:
			prescaler_r = 3;
			break;
		default:
			sysclkError = ESYSCLK_ERR_SoftwareWatchdogClockPrescaler;
			return sysclkError;
			break;
	}
	FM3_CRG->SWC_PSR   = prescaler_r | 0x80;   // set SW Watchdog prescaler

	//----- Trace Clock Prescaler ----------------
	FM3_CRG->TTC_PSR   = 0;             // set Trace Clock prescaler

	//------------- Clock Stabilization Wait Time ----------------
	FM3_CRG->CSW_TMR   = 0x5C;          // set oscillation stabilization wait time
										// circa 500msec per sub clock
										// circa 33msec per main clock


#if (FM3_ENABLE_MAINOSC)		/* Main clock oscillator enabled ? */
	bFM3_CRG_SCM_CTL_MOSCE = 1;                			/* enable main oscillator */

	for(clkstab=0; ; clkstab++)
	{
		if( bFM3_CRG_SCM_STR_MORDY )
			break;      								/* wait for Main clock oscillation stable */

		if( clkstab >= 100000 )
		{
			sysclkError = ESYSCLK_ERR_MAINCLK;
			return sysclkError;
		}
	}
#endif

#if (FM3_ENABLE_SUBOSC)					// Sub clock oscillator enabled ?
	bFM3_CRG_SCM_CTL_SOSCE = 1;                		// enable sub oscillator

	for(clkstab=0; ; clkstab++)
	{
		if( bFM3_CRG_SCM_STR_SORDY )
			break;      							// wait for Sub clock oscillation stable

		if( clkstab >= 100000 )
		{
			sysclkError = ESYSCLK_ERR_SUBCLK;
			return sysclkError;
		}
	}
#endif

#if (FM3_ENABLE_PLL)					// Sub clock oscillator enabled ?

	#if (FM3_ENABLE_MAINOSC)
		FM3_CRG->PSW_TMR   = 0x00;            // set PLL stabilization wait time + select pll in = main osc
	#else
		FM3_CRG->PSW_TMR   = 0x10;            // set PLL stabilization wait time + select pll in = hs cr osc
	#endif

	__pllM &= 0x0F;				// PLLM: PLL VCO clock frequency division ratio setting bits
	__pllK <<= 4;
	__pllK &= 0xF0;
	__pllN &= 0x1F;				// se qualcuno ci prova ... gli tagliamo le zampine ...


	FM3_CRG->PLL_CTL1  = __pllK | __pllM;        		// set PLLM and PLLK
	FM3_CRG->PLL_CTL2  = __pllN;        				// set PLLN

	bFM3_CRG_SCM_CTL_PLLE = 1;							// enable PLL

	for(clkstab=0; ; clkstab++)
	{
		if( bFM3_CRG_SCM_STR_PLRDY )
			break;      							// wait for PLL stable

		if( clkstab >= 100000 )
		{
			sysclkError = ESYSCLK_ERR_PLLCLK;
			return sysclkError;
		}
	}

	bFM3_CRG_SCM_CTL_RCS1 = 1;       				/* Set Master Clock switch */

#elif (FM3_ENABLE_MAINOSC)
	bFM3_CRG_SCM_CTL_RCS0 = 1;       				/* Set Master Clock switch */
#endif

	return sysclkError;
}

/**
 * The MCR_PSR register sets the frequency division ratio of high-speed CR oscillation.
 * A divided clock can be input into input capture.
 * The functions of this register vary depending on the product TYPE
 */
int fm3_HSCR_setPrescalerHSCRcnt(fm3_HSCR_prescaler_t __prescaler)
{
	if(__prescaler >= EHSCR_MCR_PSR_NUM)
		return 1;

	return fm3_HSCR_writePrescalerHSCRcnt(__prescaler);
}

int fm3_HSCR_writePrescalerHSCRcnt(fm3_HSCR_prescaler_t __prescaler)
{
	switch(__prescaler)
	{
		case EHSCR_MCR_PSR_clk_4:
			FM3_CRTRIM->MCR_PSR &= ~( BIT1 +  BIT0);
			break;
		case EHSCR_MCR_PSR_clk_8:
			FM3_CRTRIM->MCR_PSR &= ~( BIT1        );
			FM3_CRTRIM->MCR_PSR |=            BIT0 ;
			break;
		case EHSCR_MCR_PSR_clk_16:
			FM3_CRTRIM->MCR_PSR &= ~(         BIT0);
			FM3_CRTRIM->MCR_PSR |=    BIT1         ;
			break;
		case EHSCR_MCR_PSR_clk_32:
			FM3_CRTRIM->MCR_PSR |=    BIT1 +  BIT0 ;
			break;
        case EHSCR_MCR_PSR_NUM:
            //Nothing to be done??
            break;
		default:
			break;
	}
	return 0;
}


