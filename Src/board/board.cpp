/**
@file		board.c
@brief		board definition and setup

@author		Enrico
@date		18/01/2011
@version	01.1
*/

#include "board.h"


boardType workingCPU;
int       g_SwType;

/**Global var used only in this module*/
static bool g_irqUartMFS3Rx;
static bool g_irqUartMFS3Tx;



static void pin_init();
/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
#ifdef __cplusplus
extern "C" {
#endif
void __early_init(void)
{
    if (g_SwType == APPLICATION_SW){
        FM3_GPIO->PDOR4 = BIT1 | BIT2 | BIT3; //Rele are initialized OFF (it means ouptut 1)
            }


	FM3_HWWDT->WDG_LCK = 0x1ACCE551;                     /* HW Watchdog Unlock */
	FM3_HWWDT->WDG_LCK = 0xE5331AAE;
	FM3_HWWDT->WDG_CTL = 0;                              /* HW Watchdog stop */

	FM3_CRG->SCM_CTL  = 0;       						/* Set Master Clock switch to internal RC oscillator */

	FM3_CRG->BSC_PSR   = 0x00;               			/* set System Clock presacaler */
	FM3_CRG->APBC0_PSR = 0x00;              			/* set APB0 presacaler */
	FM3_CRG->APBC1_PSR = 0x80;              			/* set APB1 presacaler */
	FM3_CRG->APBC2_PSR = 0x80;              			/* set APB2 presacaler */
	FM3_CRG->SWC_PSR   = 0x80;   						/* set SW Watchdog presacaler */
	FM3_CRG->TTC_PSR   = 0;                				/* set Trace Clock presacaler */

    if (workingCPU == TYPE_2)
    {
        if( (FM3_FLASH_IF->CRTRMM & 0x000003FF) != 0x000003FF )
        {
            /* UnLock (MCR_FTRM) */
            FM3_CRTRIM->MCR_RLR = 0x1ACCE554;
            /* Set MCR_FTRM */
            FM3_CRTRIM->MCR_FTRM = FM3_FLASH_IF->CRTRMM;
            /* Lock (MCR_FTRM) */
            FM3_CRTRIM->MCR_RLR = 0x00000000;
        }
        else {
        }
    }
        else {
            //Here is TYPE_0
            //Flash doesn't have CRTRMM
            /* UnLock (MCR_FTRM) */
            FM3_CRTRIM->MCR_RLR = 0x1ACCE554;
            /* Set MCR_FTRM */
            fm3_HSCR_setPrescalerHSCRcnt(EHSCR_MCR_PSR_clk_8);
            FM3_CRTRIM->MCR_FTRM = 0x180;  //Default Value
            /* Lock (MCR_FTRM) */
            FM3_CRTRIM->MCR_RLR = 0x00000000;

        }
    }

#ifdef __cplusplus
}
#endif

/**
 * @brief   Late initialization.
 * @details This hook is invoked after the DATA and BSS segments
 *          initialization and before any static constructor. The
 *          default behavior is to do nothing.
 * @note    This function is a weak symbol.
 */
#ifdef __cplusplus
extern "C" {
#endif
void __late_init(void)
{
uint8_t pllM, pllK, pllN;

	pllM = 0;
	pllK = 0;		// non dividiamo la freq. di ingresso..

					// dividiamo solo la freq. sul feedback
					// per arrivare al valore richiesto
	pllN = (FM3_PLLCLK/FM3_MAINOSC)-1;

	if(fm3_sysclock_init(pllM, pllK, pllN) != ESYSCLK_ERR_NONE)
	{
		while(1);
	}
}
#ifdef __cplusplus
}
#endif
/*
 * Board-specific initialization code.
 */
void boardInit(void)
{
    workingCPU =  FM3_TYPE_BOARD;
    g_SwType = THIS_SW;

	pin_init();
	mem_init();

	// setup SysTick
	SysTick_Config((FM3_SYSCLK / CH_FREQUENCY)-1);

    nvic_init();

}

/**
 * PIN initialization
 */
static void pin_init()
{
    pin_port0_Set();
    pin_port1_Set();
    pin_port2_Set();
    pin_port3_Set();
    pin_port4_Set();
    pin_port5_Set();
    pin_port6_Set();
    pin_port7_Set();


	// ****************  PORT 8  ***********************
	/**
	 * PORT 8 Settings: Flexiper
	 * P80 -> AIR_IN_P
	 * P81 -> TEST_ARIA_P
	 * Others P8x x=2 to F NOT CONNECTED
	 */
	//Port Function Setting Register
	FM3_GPIO->PFR8 = GPIO_ALL_UNSET;
	FM3_GPIO->PFR8 = GPIO_SET_00 | GPIO_SET_01;

	//No Pull up Register available for this port

	// Direction Setting Register
	FM3_GPIO->DDR8 = GPIO_ALL_UNSET;

	// Port Output Data register
	FM3_GPIO->PDOR8 = GPIO_ALL_UNSET;


/***************************************************/
//Setting the Analogue Input Setting Register ADE

  pin_SpecAde_Set();

/*****************************************************/
// Setting the Extended Pin Function Setting Register

	// EPRFO0:
	FM3_GPIO->EPFR00 = GPIO_ALL_UNSET;
	FM3_GPIO->EPFR00 = GPIO_SET_16 | GPIO_SET_17;

	// EPRFO1:
	FM3_GPIO->EPFR01 = GPIO_ALL_UNSET;

	// EPRFO2:
	setEPFR02Reg();

	// EPRFO3 - Doesn't Exist whit this uP

	// EPRFO4:
	FM3_GPIO->EPFR04 = GPIO_ALL_UNSET;

	// EPRFO5:
	FM3_GPIO->EPFR05 = GPIO_ALL_UNSET;

	// EPRFO6:
	FM3_GPIO->EPFR06 = GPIO_ALL_UNSET;

	// EPRO7:
	setEPFR07Reg();
	// EPRFO8:
    setEPFR08Reg();

		// EPRFO9:
		FM3_GPIO->EPFR09 = GPIO_ALL_UNSET;

		// EPRF10:
		FM3_GPIO->EPFR10 = GPIO_ALL_UNSET;

		//EPFR11,12,13,14,15,16,17,18 don't Exist whit this uP.
/*****************************************************************/
//Special Port Setting Register
//		FM3_GPIO->SPSR = GPIO_ALL_UNSET;
//		FM3_GPIO->SPSR = GPIO_SET_04;

/******************************************************************/
		// Port Pseudo Open Drain Setting Register (PZRx)
		// There is no sw structure done for this
}
// ****************  PORT 0  ***********************
void pin_port0_Set()
{
	/** TYPE 2
	    P00 	-> JTRST_R		(TRSTX)  		(JTAG)
	 	P01 	-> JTCK_R		(TCK)			(JTAG)
	 	P02 	-> JTDI_R		(TDI)			(JTAG)
	 	P03 	-> JTMS_R 		(TMS)			(JTAG)
	 	P04 	-> JTDO_R 		(TDO)			(JTAG)
		P05 	-> SAFETY_MISO	(SIN4_2)		(SPI_SAFETY_C-P)
		P06 	-> SAFETY_MOSI 	(SOT4_2)		(SPI_SAFETY_C-P)
		P07 	-> SAFETY_SCLK	(SCK4_2)		(SPI_SAFETY_C-P)
		P08 	-> SAFETY_BUSY	(I)			    (SPI_SAFETY_C-P)
		P09		-> TASTO_8 		(O)
		P0A..P0F-> does not exist

        TYPE 0

	  	P00	-> TRST  	---> JTAG
		P01 -> TCK	 	---> JTAG
		P02 -> TDI	 	---> JTAG
		P03 -> TMS	 	---> JTAG
		P04 -> TDO	 	---> JTAG
		P05 -> SIN4_2	---> SPI C-P SAFE
		P06 -> SOT4_2   ---> SPI C-P SAFE
		P07 -> SCK4_2	---> SPI C-P SAFE
		P08 -> CTS4_2 	---> SPI C-P SAFE (P08 - GPIO)
		P09 -> P09 		---> STRIP EXP
		P0A -> P0A		---> STRIP EXP
		P0B -> P0B		---> STRIP EXP
		P0C -> P0C		---> STRIP EXP
		P0D -> P0D		---> STRIP EXP
		P0E -> P0E		---> STRIP EXP
		P0F -> P0F		---> STRIP EXP
	*/

    if (workingCPU == TYPE_2){
        FM3_GPIO->PFR0  = PFR0_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR0  = BIT0 | BIT1 | BIT2 | BIT3 |
                          BIT4 | BIT5 | BIT6 | BIT7;
        FM3_GPIO->DDR0  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->DDR0  = BIT9;  //TASTO_8 is output
        FM3_GPIO->PCR0  = PCR0_INIT_VALUE; // Pull-up register init val
        FM3_GPIO->PZR0  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR0 = PDORx_INIT_VALUE;// Output Data register init val
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR0  = PFR0_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR0  = BIT0 | BIT1 | BIT2 | BIT3 |
                          BIT4 | BIT5 | BIT6 | BIT7;
        FM3_GPIO->DDR0  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->DDR0  = BIT8;  //Safety GPIO for SPI ued
        FM3_GPIO->PCR0  = PCR0_INIT_VALUE; // Pull-up register init val
        FM3_GPIO->PZR0  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR0 = PDORx_INIT_VALUE;// Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}

}
/**
****************  PORT 1  ***********************
*/
void pin_port1_Set()
{
	/**
	 * FlexiPer:
	 * P10 -> GAUGE_1P
	 * P11 -> P_P2_P
	 * P12 -> P_ART_P
	 * P13 -> P_FIL_P
	 * P14 -> P_VEN_P
	 * P15 -> P_UF_P
	 * P16 -> BLD_RX_P
	 * P17 -> V_TEST_1
	 * P18 -> V_TEST_2
	 * P19 -> MON_AVREF_P
	 * P1A -> MON_AVREF_C
	 * P1B -> MON_+5V
	 * P1C -> MON_+3.3V_P
	 * P1D -> MON_+3.3V
	 * P1E -> Free Up
	 * P1F -> Free Up
	 */

if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR1  = PFRx_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR1  = BIT0 | BIT1 | BIT2 | BIT3 |
                          BIT4 | BIT5 | BIT6 | BIT7 |
                          BIT8 | BIT9 | BIT10 | BIT12 |
                          BIT13 | BIT14;    //All the analogue port
        FM3_GPIO->DDR1  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->PCR1  = PCRx_INIT_VALUE; // Pull-up register init val
        FM3_GPIO->PCR1  = BIT0 | BIT1 | BIT2 | BIT3 |
                          BIT4 | BIT5 | BIT6 | BIT7 |
                          BIT8 | BIT9 | BIT10 | BIT12 |
                          BIT13 | BIT14;    //All the analogue port
        FM3_GPIO->PZR1  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR1 = PDORx_INIT_VALUE;// Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}
}

/**
** Setting pin function of port 2
*/
void pin_port2_Set()
{
	/**
	 * P20 -> FREE UP
	 * P21 -> P_UART_DBG_RX
	 * P22 -> P_UART_DBG_TX
	 * P23 -> FREE UP
	 * P24 -> FREE UP
	 * P25 -> FREE UP
	 * P26 -> FREE UP
	 * P27 -> FREE UP
	 * P28 -> FREE UP
	 * P29 -> NOT CONNECTED
	 * P2A -> NOT CONNECTED
	 * P2B -> NOT CONNECTED
	 * P2C -> NOT CONNECTED
	 * P2D -> NOT CONNECTED
	 * P2E -> NOT CONNECTED
	 * P2F -> NOT CONNECTED
	 */


	if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR2  = PFRx_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR2  = BIT1 | BIT2;     //Debug Serial Port
        FM3_GPIO->DDR2  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->PCR2  = PCRx_INIT_VALUE; // Pull-up register init val
        FM3_GPIO->PZR2  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR2 = PDORx_INIT_VALUE;// Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}


}

/********************************
** Setting pin function of port 3
*********************************/
void pin_port3_Set()
{
	/**
	 * PORT 3 Settings: Flexiper
	 * P30 -> CS_FRAM_P#
	 * P31 -> SCKL_P
	 * P32 -> MOSI_P
	 * P33 -> MISO_P
	 * P34 -> TASTO_SEL_SW_P
	 * P35 -> TASTO_SEL_CHA_P
	 * P36 -> TASTO_SEL_CHB_P
	 * P37 -> RTC_VCC_C_KO\PRO
	 * P38 -> 7SEG D
	 * P39 -> 7SEG E
	 * P3A -> 7SEG G
	 * P3B -> 7SEG F
	 * P3C -> 7SEG A
	 * P3D -> 7SEG B
	 * P3E -> 7SEG C
	 * P3F -> 7SEG DP
	 */

    if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR3  = PFRx_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR3  = BIT1 | BIT2 | BIT3;     //SPI for FRAM
        FM3_GPIO->DDR3  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->DDR3 = BIT0 |
			BIT8 | BIT9 | BIT10 | BIT11 |
			BIT12 | BIT13 | BIT14 | BIT15;  //CS for FRAM and 7-Segment Display

        FM3_GPIO->PCR3  = PCRx_INIT_VALUE; // Pull-up register init val

        FM3_GPIO->PZR3  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR3 = PDORx_INIT_VALUE;// Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}
}

/********************************
** Setting pin function of port 4
*********************************/
void pin_port4_Set()
{
	/**
	 * PORT 4 Settings: Flexiper
	 * P40 -> BUZ_P
	 * P41 -> RELE_PS
	 * P42 -> RELE_UF
	 * P43 -> RELE_PINZA
	 * P44 -> LED_LIFE_1P
	 * P45 -> LED_LIFE_2P
	 * P46 -> FREE
	 * P47 -> FREE
	 * P48 -> P_UART_UPFWP_RX
	 * P49 -> P_UART_UPFWP_TX
	 * P4A -> UF_CH_B
	 * P4B -> PS_CH_B
	 * P4C -> FREE
	 * P4D -> BLD_TUBO_P
	 * P4E -> SW_PINZA_P
	 * P4F -> NOT CONNECTED
	 */
    if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR4  = PFRx_INIT_VALUE; //Function set register init Val
        FM3_GPIO->PFR4  = BIT8 | BIT9 | BIT10 | BIT11;     //UART Update Firmware + IC connected to encoder PS and UF
        FM3_GPIO->DDR4  = DDRx_INIT_VALUE; // Direction register init val
        FM3_GPIO->DDR4  = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;  //Buzzer, various relé and leds
        FM3_GPIO->PCR4  = PCRx_INIT_VALUE; // Pull-up register init val
        FM3_GPIO->PZR4  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR4 = PDORx_INIT_VALUE;// Output Data register init val
        if (g_SwType == APPLICATION_SW){
            FM3_GPIO->PDOR4 = BIT1 | BIT2 | BIT3; //Rele are initialized OFF (it means ouptut 1)
            }
       } else {
            //Here we have an error
            while (1);
       }
	}
}

/********************************
** Setting pin function of port 5
*********************************/
void pin_port5_Set()
{
	/**
	 * PORT 5 Settings: Flexiper
	 * P50 -> TASTO1_P
	 * P51 -> TASTO2_P
	 * P52 -> TASTO3_P
	 * P53 -> FREE
	 * P54 -> FREE
	 * P55 -> FREE
	 * P56 -> FREE
	 * P57 -> FREE
	 * P58 -> FREE
	 * P59 -> P_UART_EXP_RX
	 * P5A -> P_UART_EXP_TX
	 * P5B -> FREE
	 * P5C -> NOT CONNECTED
	 * P5D -> NOT CONNECTED
	 * P5E -> NOT CONNECTED
	 * P5F -> NOT CONNECTED
	 */

    if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR5  = PFRx_INIT_VALUE;  // Function set register init Val
        FM3_GPIO->PFR5  = BIT9 | BIT10;     // Uart EXP
        FM3_GPIO->DDR5  = DDRx_INIT_VALUE;  // Direction register init val
        FM3_GPIO->PCR5  = PCRx_INIT_VALUE;  // Pull-up register init val
        FM3_GPIO->PZR5  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR5 = PDORx_INIT_VALUE; // Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}
}
/********************************
** Setting pin function of port 6
*********************************/
void pin_port6_Set()
{
	/**
	 * PORT 6 Settings: Flexiper
	 * P60 -> Prog/Run Mode -Switch
	 * P61 -> Free
	 * P62 -> Free
	 * P63 -> Free
	 * P64 -> Free
	 * P65 -> Free
	 * P66 -> Free
	 * P67 -> Free
	 * P68 -> STRIP - EXP
	 * P69 -> NOT CONNECTED
	 * P6A -> NOT CONNECTED
	 * P6B -> NOT CONNECTED
	 * P6C -> NOT CONNECTED
	 * P6D -> NOT CONNECTED
	 * P6E -> NOT CONNECTED
	 * P6F -> NOT CONNECTED
	 */

    if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR6  = PFRx_INIT_VALUE;  // Function set register init Val
        FM3_GPIO->DDR6  = DDRx_INIT_VALUE;  // Direction register init val
        FM3_GPIO->PCR6  = PCRx_INIT_VALUE;  // Pull-up register init val
        FM3_GPIO->PZR6  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR6 = PDORx_INIT_VALUE; // Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}
}
/********************************
** Setting pin function of port 7
*********************************/
void pin_port7_Set()
{
	/**
	 * PORT 7 Settings: Flexiper
	 * P70 -> AIR_IN_P
	 * P71 -> TEST_ARIA_P
	 * P72 -> P_USBHOST_EN
	 * P73 -> P_USBHOST_OVER
	 * P74 -> Free
	 * Others P7x x=5 to F NOT CONNECTED
	 */
    if (workingCPU == TYPE_2){
    //If you want copy here the setting of the Flexiper Controller
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->PFR7  = PFRx_INIT_VALUE;  // Function set register init Val
        FM3_GPIO->DDR7  = DDRx_INIT_VALUE;  // Direction register init val
        FM3_GPIO->PCR7  = PCRx_INIT_VALUE;  // Pull-up register init val
        FM3_GPIO->PZR7  = PZRx_INIT_VALUE;	// Pseudo OpenDrain register init val
        FM3_GPIO->PDOR7 = PDORx_INIT_VALUE; // Output Data register init val
       } else {
            //Here we have an error
            while (1);
       }
	}
}

/**
*  Special Function PIN
**/
void pin_SpecAde_Set()
{
	FM3_GPIO->ADE = ADE_INIT_VAL;
    if (workingCPU == TYPE_2){
        /*Fill when use Flexiper Controller*/
	} else {
       if (workingCPU == TYPE_0){
            FM3_GPIO->ADE = ADE_RESET_VAL;  //Reset means no Analog Input at all
            FM3_GPIO->ADE_f.AN00 = USE_AS_ANALOGUE_INPUT;  //GAUGE_1P
            FM3_GPIO->ADE_f.AN01 = USE_AS_ANALOGUE_INPUT;  //GAUGE_2P
            FM3_GPIO->ADE_f.AN02 = USE_AS_ANALOGUE_INPUT;  //P_ART_P
            FM3_GPIO->ADE_f.AN03 = USE_AS_ANALOGUE_INPUT;  //P_FIL_P
            FM3_GPIO->ADE_f.AN04 = USE_AS_ANALOGUE_INPUT;  //P_VEN_P
            FM3_GPIO->ADE_f.AN05 = USE_AS_ANALOGUE_INPUT;  //P_UF_P
            FM3_GPIO->ADE_f.AN06 = USE_AS_ANALOGUE_INPUT;  //BLD_RX_P
            FM3_GPIO->ADE_f.AN07 = USE_AS_ANALOGUE_INPUT;  //V_TEST_1
            FM3_GPIO->ADE_f.AN08 = USE_AS_ANALOGUE_INPUT;  //V_TEST_2
            FM3_GPIO->ADE_f.AN09 = USE_AS_ANALOGUE_INPUT;  //MON_AVREF_P
            FM3_GPIO->ADE_f.AN10 = USE_AS_ANALOGUE_INPUT;  //MON_AVREF_C
            FM3_GPIO->ADE_f.AN11 = USE_AS_ANALOGUE_INPUT;  //MON_+5V
            FM3_GPIO->ADE_f.AN12 = USE_AS_ANALOGUE_INPUT;  //MON_+3.3V_P
            FM3_GPIO->ADE_f.AN13 = USE_AS_ANALOGUE_INPUT;  //MON_+3.3V
       } else {
            //Here we have an error
            while (1);
       }
	}

}
/**********************************************************************************
** The EPFR02 register assigns functions to a pin of the multifunction timer Unit1.
***********************************************************************************/
void setEPFR02Reg()
{
	FM3_GPIO->EPFR02 = EPFR02DEFVALUE;
    if (workingCPU == TYPE_2){
        /*Fill when use Flexiper Controller*/
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->EPFR02_f.IC11 = USE_ICU_FUN_1_AS_INPUT; //Uses IC11_1 at the input pin of the input capture IC11. - UF_CHB
        FM3_GPIO->EPFR02_f.IC12 = USE_ICU_FUN_1_AS_INPUT; //Uses IC12_1 at the input pin of the input capture IC12. - PS_CHB
       } else {
            //Here we have an error
            while (1);
       }
	}


}

/*******************************************************************************
** The EPFR07 register assigns functions of multi-function serial ch.0 to ch.3.
********************************************************************************/
void setEPFR07Reg()
{
	FM3_GPIO->EPFR07 = EPFR07DEFVALUE;
    if (workingCPU == TYPE_2){
        /*Fill when use Flexiper Controller*/
	} else {
       if (workingCPU == TYPE_0){
        FM3_GPIO->EPFR07_f.SIN0 = USE_SIN_FUN_0;   //DEBUG UART
        FM3_GPIO->EPFR07_f.SOT0 = USE_SOUT_FUN_0;  //DEBUG UART
        FM3_GPIO->EPFR07_f.SIN3 = USE_SIN_FUN_2;   //FW UPDATE UART
        FM3_GPIO->EPFR07_f.SOT3 = USE_SOUT_FUN_2;  //FW UPDATE UART
       } else {
            //Here we have an error
            while (1);
       }
	}


}

/*******************************************************************************
** The EPFR08 register assigns functions of multi-function serial ch.4 to ch.7.
********************************************************************************/
void setEPFR08Reg()
{
	FM3_GPIO->EPFR08 = EPFR08DEFVALUE;
    if (workingCPU == TYPE_2){
        //Fill if you want setting for TYPE_2
	} else {
       if (workingCPU == TYPE_0){
        //SPI -4 -  Communication with Micro C
        //SPI -6 -  Communication with FRAM
        //UART -7 - EXP UART RS232
        FM3_GPIO->EPFR08_f.SCK4 = USE_SCLK_FUN_2;
        FM3_GPIO->EPFR08_f.SIN4 = USE_SIN_FUN_2;
        FM3_GPIO->EPFR08_f.SOT4 = USE_SOUT_FUN_2;
        FM3_GPIO->EPFR08_f.SCK6 = USE_SCLK_FUN_1;
        FM3_GPIO->EPFR08_f.SIN6 = USE_SIN_FUN_1;
        FM3_GPIO->EPFR08_f.SOT6 = USE_SOUT_FUN_1;
        FM3_GPIO->EPFR08_f.SIN7 = USE_SIN_FUN_0;
        FM3_GPIO->EPFR08_f.SOT7 = USE_SOUT_FUN_0;
       } else {
            //Here we have an error
            while (1);
       }
	}
}

/**
* Nested Vectored Interrupt Controller (NVIC) Initialization
*/
void nvic_init()
{
    //Not all the peripheral interrupt are set here.
    //---- MFS 4 RX------------------------------------
	NVIC_ClearPendingIRQ(MFS4RX_IRQn);
	NVIC_EnableIRQ(MFS4RX_IRQn);
	NVIC_SetPriority(MFS4RX_IRQn, IRQ_MFS4_RX_LEVEL);

	//---- MFS 4 TX------------------------------------
	NVIC_ClearPendingIRQ(MFS4TX_IRQn);
	NVIC_EnableIRQ(MFS4TX_IRQn);
	NVIC_SetPriority(MFS4TX_IRQn, IRQ_MFS4_TX_LEVEL);

    //---- MFT 1 ------------------------------------
    NVIC_ClearPendingIRQ(MFT0_2_ICU_IRQn);
	NVIC_EnableIRQ(MFT0_2_ICU_IRQn);
	NVIC_SetPriority(MFT0_2_ICU_IRQn,IRQ_MFT_ICU_LEVEL);

	NVIC_ClearPendingIRQ(MFT0_2_FRT_IRQn);
	NVIC_EnableIRQ(MFT0_2_FRT_IRQn);
	NVIC_SetPriority(MFT0_2_FRT_IRQn,IRQ_MFT_FRT_LEVEL);

	irqSelect();

    #ifdef LOADER_ENGINE_INT
	//---- MFS 3 RX------------------------------------
	if (g_irqUartMFS3Rx == true){
        NVIC_ClearPendingIRQ(MFS3RX_IRQn);
        NVIC_EnableIRQ(MFS3RX_IRQn);
        NVIC_SetPriority(MFS3RX_IRQn, IRQ_MFS3_RX_LEVEL);
	}else{
        NVIC_ClearPendingIRQ(MFS3RX_IRQn);
        NVIC_DisableIRQ(MFS3RX_IRQn);
	}

	//---- MFS 3 TX------------------------------------
	if (g_irqUartMFS3Tx == true){
        NVIC_ClearPendingIRQ(MFS3TX_IRQn);
        NVIC_EnableIRQ(MFS3TX_IRQn);
        NVIC_SetPriority(MFS3TX_IRQn, IRQ_MFS3_TX_LEVEL);
	}else{
        NVIC_ClearPendingIRQ(MFS3TX_IRQn);
        NVIC_EnableIRQ(MFS3TX_IRQn);
	}
    #endif
}

/**Select the irq resource you want to enable**/
void irqSelect()
{
    g_irqUartMFS3Rx = true;
    g_irqUartMFS3Tx = false;

}











