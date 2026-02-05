/**
@file		board_define.h
@brief		board definition
@author		Fernando Morani
@date		23/02/2017
@version	01.00
@source     Flexiper
*/
#ifndef _BOARD_DEFINE_H_
#define _BOARD_DEFINE_H_

/**
 * CPUs Types
 */
//SW TYPES
#define APPLICATION_SW              1
#define BOOTLOADER_FW               2
#define THIS_SW APPLICATION_SW

//BOARD TYPES
#define FLEXIPER_CONTROLLER         0
#define FLEXIPER_PROTECTION         1
#define THIS_BOARD FLEXIPER_PROTECTION

#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #include "mb9bd10t.h"
#else
    #include "mb9bf50x.h"
#endif

#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #define FM3_TYPE_BOARD      TYPE_2
#else
    #if (THIS_BOARD == FLEXIPER_PROTECTION)
         #define FM3_TYPE_BOARD      TYPE_0
    #endif
#endif

//Here we have the board configuration
#if (THIS_SW  == BOOTLOADER_FW)
    #undef USE_ETH0
    #undef USE_ETH1
#else
    #if (THIS_BOARD == FLEXIPER_CONTROLLER)
        #define USE_ETH0
        #undef USE_ETH1
    #else
        #if (THIS_BOARD == FLEXIPER_PROTECTION)
            #undef USE_ETH0
            #undef USE_ETH1
        #endif
    #endif
#endif

#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #define FM3_SUBOSC         	32768	// subclock (32768 Hz) It is not used in the project. Verify if we need or delete
    #define FM3_MAINOSC         4000000	// External   4MHz Crystal
    #define FM3_HSINTERNALOSC   4000000	// High-speed Internal CR
    #define FM3_LSINTERNALOSC   100000  // Low-speed Internal CR

    #define FM3_PLLCLK        120000000	// PLL Exit (60-144MHz)
    #define FM3_SYSCLK        60000000	// cpu + AHB base clock, 	 Prescaler validi rispetto a FM3_PLLCLK:  1, 2, 3, 4, 6, 8, 16
    #define FM3_APB0CLK       30000000	// APB0  	  	 (max 72MHz) Prescaler validi rispetto a FM3_SYSCLK:  1, 2,    4, 8
    #define FM3_APB1CLK       30000000	// APB1  	 	 (max 72MHz) Prescaler validi rispetto a FM3_SYSCLK:  1, 2,    4, 8
    #define FM3_APB2CLK       30000000	// APB2  	 	 (max 72MHz) Prescaler validi rispetto a FM3_SYSCLK:  1, 2,    4, 8
    #define FM3_SWDOGCLK    	7500000	// SOFT-WATCHDOG (max 72Mhz) Prescaler validi rispetto a FM3_APB0CLK: 1, 2,    4, 8
    #define FM3_ADCCLK		FM3_APB1CLK	// frq. A/D converter
    #define FM3_BTCLK		FM3_APB1CLK	// frq. Base Timer
    #define FM3_UARTCLK		FM3_APB2CLK	// frq. Uart
    #define FM3_SPICLK		FM3_APB2CLK	// frq. SPI
    #define FM3_USBCLK		FM3_APB2CLK	// frq. USB ?? o AHB
    #define FM3_DTCLK       FM3_APB0CLK
    #define EXT_BUSCLK      30000000 // external bus clock frequency

    #define FM3_ENABLE_SUBOSC     FALSE
    #define FM3_ENABLE_MAINOSC     TRUE
    #define FM3_ENABLE_PLL    	   TRUE

    #define CYCLE_DELAY_CLKSTAB_MAINOSC 100000	// cycle for iteraction "timeout" stabilization clock
    #define CYCLE_DELAY_CLKSTAB_SUBOSC  100000	// cycle for iteraction "timeout" stabilization clock
    #define CYCLE_DELAY_CLKSTAB_PLL     100000	// cycle for iteraction "timeout" stabilization clock
#if (THIS_SW  == APPLICATION_SW)
    #define DUAL_TIMER_DEFAULT_VALUE_MS     1 //One millisecond
    #define DUAL_TIMER_LAUNCH_EMAC_TIMEOUT  5 //5 ms
    #define DUAL_TIMER_LAUNCH_PMP_GES_TIMEOUT 5 //5 ms
    #define DUAL_TIMER_AIR_GES_TIMEOUT      1 //1 ms
    #define DUAL_TIMER_MOR_GES_TIMEOUT      1 //1 ms
    #define DUAL_TIMER_SEND_MSG_TIMEOUT     1000 //One second
    #define DUAL_TIMER_IRQ		Vector58  //The same IRQ for both Timer
#endif
#else
    // #warning clock define are moved in fm3_define.h
#endif

#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #if (THIS_SW == APPLICATION_SW)
        #define IRQ_DMA_CHA_LEVEL       IRQ_PRIORITY_LEV_15
        #define IRQ_BT07_LEVEL          IRQ_PRIORITY_LEV_15
        #define IRQ_BT815_LEVEL         IRQ_PRIORITY_LEV_15
        #define IRQ_DT_LEVEL            IRQ_PRIORITY_LEV_15
        #define IRQ_EXTINT_07_LEVEL     IRQ_PRIORITY_LEV_15
        #define IRQ_EXTINT_831_LEVEL    IRQ_PRIORITY_LEV_15
        #define IRQ_MFT0_FRT2_LEVEL     IRQ_PRIORITY_LEV_15
        #define IRQ_MFT0_ICU2_LEVEL     IRQ_PRIORITY_LEV_15
        #define IRQ_MFT0_OCU2_LEVEL     IRQ_PRIORITY_LEV_15
        #define IRQ_MFS0_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS0_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS1_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS1_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS3_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS3_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS4_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS4_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS6_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS6_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS7_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS7_TX_LEVEL       IRQ_PRIORITY_LEV_14
    #else
        #warning Using the bootloader. Please fill here if needed
    #endif
#else
    #if (THIS_SW == APPLICATION_SW)
        #define IRQ_MFS4_RX_LEVEL       IRQ_PRIORITY_LEV_13
        #define IRQ_MFS4_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFT_ICU_LEVEL       IRQ_PRIORITY_LEV_15
        #define IRQ_MFT_FRT_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS3_TX_LEVEL       IRQ_PRIORITY_LEV_14
        #define IRQ_MFS3_RX_LEVEL       IRQ_PRIORITY_LEV_13
    #else
        #warning Using the bootloader for protective. Please fill here if needed
    #endif
#endif

/**
 * IO pins assignments.
 * Pin name in according with DSN scheme; in the function file (Eparin for example) redefine by using the name used here:
 * #define SEGNALE_EPA_AVANTI-INDIETRO		PinEPA_A-I
 */
#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #if (THIS_SW == APPLICATION_SW)
// **** 				PORT 0							****
        #define PinSAFETY_BUSY				bFM3_GPIO_PDIR0_P8	// Input for the application
        #define PinTASTO_8					bFM3_GPIO_PDOR0_P9	// output
// **** 				PORT 1							****
// **** 				PORT 2							****
        #define PinV_CIC                    bFM3_GPIO_PDOR2_P3  //Output, no more used
// **** 				PORT 3							****
        #define PinCS_FRAM_n                bFM3_GPIO_PDOR3_P0  // output, SPI_C chip select
        #define PinCS_RTC_n                 bFM3_GPIO_PDOR3_P4  // output, SPI_C chip select
        #define PinCS_BT_n                  bFM3_GPIO_PDOR3_P5  // output, SPI_C chip select
        #define PinCOP_PS                   bFM3_GPIO_PDIR3_P9  // input
        #define PinRIF_PS                   bFM3_GPIO_PDIR3_PA  // input
        #define PinCOP_UF                   bFM3_GPIO_PDIR3_PB  // input
        #define PinRIF_UF                   bFM3_GPIO_PDIR3_PC  // input
        #define PinLUCE_V                   bFM3_GPIO_PDOR3_PD  // output
        #define PinLUCE_G                   bFM3_GPIO_PDOR3_PE  // output
        #define PinLUCE_R                   bFM3_GPIO_PDOR3_PF  // output
// **** 				PORT 4							****
        #define PinEPA_CLK	                bFM3_GPIO_PDOR4_P0  // output
        #define PinEPA_REF_IN               bFM3_GPIO_PDOR4_P1  // output
        #define PinEPA_LIM		            PinEPA_REF_IN  // output per compatibilita con vecchio s/w
        #define PinEPA_M1                   bFM3_GPIO_PDOR4_P2  // output
        #define PinEPA_M2                   bFM3_GPIO_PDOR4_P3  // output
        #define PinEPA_AI                   bFM3_GPIO_PDOR4_P4  // output
        #define PinEPA_EN                   bFM3_GPIO_PDOR4_P5  // output
        #define PinAIR_IN                   bFM3_GPIO_PDIR4_P6  // input
        #define PinRESET_CPLD_n             bFM3_GPIO_PDOR4_P7  // output
// **** 				PORT 5							****
        #define PinLDAC_n                   bFM3_GPIO_PDOR5_P4	// output
        #define PinRS485_DE_RE_n            bFM3_GPIO_PDOR5_P8	// output
        #define PinBUZ_WEAK                 bFM3_GPIO_PDOR5_PD	// output
// **** 				PORT 6							****
        #define PinBATT_TEST                bFM3_GPIO_PDOR6_P0	// output
        #define PinSW_PINZA                 bFM3_GPIO_PDIR6_P2	// input
// **** 				PORT 7							****
        #define PinLED_LIFE_2               bFM3_GPIO_DDR7_PB	// output in Pseudo open Drain
        #define PinLED_LIFE_1               bFM3_GPIO_DDR7_PC   // output in Pseudo open Drain
        #define PinCS_SPI_CPLD_n            bFM3_GPIO_PDOR7_PD	// output, SPI_C chip select
// **** 				PORT 8							****
// **** 				PORT 9							****
        #define PinUSBHOST_OVER_n           bFM3_GPIO_PDIR9_P4  // input
        #define PinUSBHOST_EN_n             bFM3_GPIO_PDOR9_P5  // output
// **** 				PORT A							****
        #define PinTEST_ARIA                bFM3_GPIO_PDORA_P0  // output
        #define PinEPA_CHB                  bFM3_GPIO_PDIRA_P1  // input set but not used
        #define PinCS_ADEXT                 bFM3_GPIO_PDORA_P2  // output set but not used
        #define PinEPA_CHA                  bFM3_GPIO_PDIRA_P3  // input set but not used
        #define PinBLD_TUBO                 bFM3_GPIO_PDIRA_P5  // input
// **** 				PORT B							****
// **** 				PORT C							****
        #define PinTASTO_1                  bFM3_GPIO_PDIRC_P0  // input
        #define PinTASTO_2                  bFM3_GPIO_PDIRC_P1  // input
        #define PinTASTO_3                  bFM3_GPIO_PDIRC_P2  // input
        #define PinETH_RESET                bFM3_GPIO_PDORC_P9  // output
        #define PinEPA_SW                   bFM3_GPIO_PDIRC_PA  // input
        #define PinEPA_FC2                  bFM3_GPIO_PDIRC_PB  // input
        #define PinEPA_FC1                  bFM3_GPIO_PDIRC_PC  // input
        #define PinEPA_PRE                  bFM3_GPIO_PDIRC_PD  // input
        #define PinSEL_SW                   bFM3_GPIO_PDIRC_PE  // input
        #define PinSEL_CHB                  bFM3_GPIO_PDIRC_PF  // input
// **** 				PORT D							****
        #define PinSEL_CHA                  bFM3_GPIO_PDIRD_P0  // input
// **** 				PORT E							****
// **** 				PORT F							****
        #define PinIO_MISO_DRV              bFM3_GPIO_PDIRF_P3  // input - DRIVER SPI often used like simple GPIO
        #define PinIO_MOSI_DRV              bFM3_GPIO_PDORF_P4  // output
        #define PinIO_SCLK_DRV              bFM3_GPIO_PDORF_P5  // output
        #define PinIO_CSSPI_DRV             bFM3_GPIO_PDORF_P6  // output
    #else
        #warning Please define pin for BOOTLOADER CONTROLLER
    #endif
#else
	// ****************  PORT 3  ***********************
        //FRAM Chip Select
        #define PinCS_FRAM_n                bFM3_GPIO_PDOR3_P0  // output, SPI_C chip select
        //7 Segment Display
        #define Pin7Seg_D                   bFM3_GPIO_PDOR3_P8 //output
        #define Pin7Seg_E                   bFM3_GPIO_PDOR3_P9 //output
        #define Pin7Seg_G                   bFM3_GPIO_PDOR3_PA //output
        #define Pin7Seg_F                   bFM3_GPIO_PDOR3_PB //output
        #define Pin7Seg_A                   bFM3_GPIO_PDOR3_PC //output
        #define Pin7Seg_B                   bFM3_GPIO_PDOR3_PD //output
        #define Pin7Seg_C                   bFM3_GPIO_PDOR3_PE //output
        #define Pin7Seg_DP                  bFM3_GPIO_PDOR3_PF //output
	// ****************  PORT 4  ***********************
        #define PinLedLife_1                bFM3_GPIO_PDOR4_P4  //Output
        #define PinLedLife_2                bFM3_GPIO_PDOR4_P5  //Output
	// ****************  PORT 6  ***********************
        #define PinProgRunMode              bFM3_GPIO_PDIR6_P0  //Output

    //Define only for application
    #if (THIS_SW == APPLICATION_SW)
// **** 				PORT 0							****
        #define PinSAFETY_BUSY      bFM3_GPIO_PDOR0_P8 //output
        //Macro for P09 to P0F has not been defined for the moment - 03/04/2017
// **** 				PORT 3							****
        //Pin for keyboard selector
        #define PinSEL_SW                   bFM3_GPIO_PDIR3_P4  // input
        #define PinSEL_CHB                  bFM3_GPIO_PDIR3_P5  // input
        #define PinSEL_CHA                  bFM3_GPIO_PDIR3_P6  // input
        //RTC Input
        #define PinRTC_VCC                  bFM3_GPIO_PDIR3_P7  // input
// **** 				PORT 4							****
        #define PinBUZZER 			        bFM3_GPIO_PDOR4_P0  //output
        #define PinRelePS                   bFM3_GPIO_PDOR4_P1  //output
        #define PinReleUF 			        bFM3_GPIO_PDOR4_P2  //output
        #define PinRelePinza                bFM3_GPIO_PDOR4_P3  //output
        #define PinBldTubo                  bFM3_GPIO_PDIR4_PD  // input
        #define PinSwPinza                  bFM3_GPIO_PDIR4_PE  // input
// **** 				PORT 5							****
        #define PinTASTO_1                  bFM3_GPIO_PDIR5_P0  // input
        #define PinTASTO_2                  bFM3_GPIO_PDIR5_P1  // input
        #define PinTASTO_3                  bFM3_GPIO_PDIR5_P2  // input
// **** 				PORT 7							****
        #define PinAirIn                    bFM3_GPIO_PDIR7_P0  // input
        #define PinTestAir                  bFM3_GPIO_PDIR7_P1  // input

    #else
        //Define only for bootloader
        #warning Please define pin for BOOTLOADER PROTECTIVE
    #endif
#endif

//PFRX Init Value. Valid for all the FM Processor
#define PFR0_INIT_VALUE 0x001F
#define PFRx_INIT_VALUE 0x0000  //Init value for register from 1 to F

//PCRX Init Value. Valid for all the FM Processor
#define PCR0_INIT_VALUE 0x001F
#define PCRx_INIT_VALUE 0x0000  //Init value for register from 1 to F. PCR8 does not exist

//DDRX Init Value. Valid for all the FM Processor
#define DDRx_INIT_VALUE 0x0000  //Init value for register from 0 to F.

//PDIRX Init Value. Valid for all the FM Processor
#define PDIRx_INIT_VALUE 0x0000  //Init value for register from 0 to F.

//PDORX Init Value. Valid for all the FM Processor
#define PDORx_INIT_VALUE 0x0000  //Init value for register from 0 to F.

//PZRX Init Value. Valid for all the FM Processor
#define PZRx_INIT_VALUE 0x0000  //Init value for register from 0 to F.

//SPSR Init Value.
#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #define SPSR_INIT_VAL  0x00000005 //TYPE2 Board
#else
    #define SPSR_INIT_VAL  0x00000001  //TYPE0 Board
#endif

//ADE Init Value. Valid for all the FM Processor
#define ADE_INIT_VAL  0xFFFFFFFF
#define ADE_RESET_VAL 0x00000000

//EPFR Default Value. Valid for all the FM Processor
#define EPFR00DEFVALUE  0x00030000
#define EPFR01DEFVALUE  0x00000000
#define EPFR02DEFVALUE  0x00000000
#define EPFR03DEFVALUE  0x00000000
#define EPFR04DEFVALUE  0x00000000
#define EPFR05DEFVALUE  0x00000000
#define EPFR06DEFVALUE  0x00000000
#define EPFR07DEFVALUE  0x00000000
#define EPFR08DEFVALUE  0x00000000
#define EPFR09DEFVALUE  0x00000000
#define EPFR10DEFVALUE  0x00000000
#define EPFR11DEFVALUE  0x00000000
#define EPFR12DEFVALUE  0x00000000
#define EPFR13DEFVALUE  0x00000000
#define EPFR14DEFVALUE  0x00000000
#define EPFR15DEFVALUE  0x00000000
#define EPFR16DEFVALUE  0x00000000
#define EPFR17DEFVALUE  0x00000000
#define EPFR18DEFVALUE  0x00000000

#ifndef MFT0_2_ICU_IRQn
#define MFT0_2_ICU_IRQn INCAP_IRQn
#endif

#ifndef MFT0_2_FRT_IRQn
#define MFT0_2_FRT_IRQn FRTIM_IRQn
#endif

#endif /* _BOARD_H_ */
