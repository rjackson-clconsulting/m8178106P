#ifndef FM3_DEFINE_H_INCLUDED
#define FM3_DEFINE_H_INCLUDED


								// costanti di base dei vari moduli
#define BASEMOD_SIO		100		// prima i moduli HW
#define BASEMOD_DMA		200		// modulo dma
#define BASEMOD_IRQ		300		// modulo irq & vectors
#define BASEMOD_WTN		400		// Watch Timer
#define BASEMOD_WDT		500		// Watch Dog Timer
#define BASEMOD_TIM		600		// modulo timers
#define BASEMOD_ADC		700		// modulo ad conveters
#define BASEMOD_CAN		800		// modulo can
#define SYSCLK_BASE_ERR	900

			// costanti di base dei vari moduli
#define BASEMOD_SIOMA1	100		// prima i moduli HW
#define BASEMOD_DMAMA1	200		// modulo dma
#define BASEMOD_IRQMA1	300		// modulo irq & vectors
#define BASEMOD_WTNMA1	400		// Watch Timer
#define BASEMOD_WDTMA1	500		// Watch Dog Timer
#define BASEMOD_TIMMA1	600		// modulo timers
#define BASEMOD_ADCMA1	700		// modulo ad conveters


#define BASEMOD_COMBUF	1100	// poi i moduli SW
#define BASEMOD_CSI		1200
#define BASEMOD_I2C		1300
#define BASEMOD_UART	1400
#define BASEMOD_ALLOC	1500
#define BASEMOD_MMCARD	1600
#define BASEMOD_FILSYS	1700
#define BASEMOD_CHRSET	1800
#define BASEMOD_RTCDRV	1900
#define BASEMOD_NVRDRV	2000
#define BASEMOD_EEPDRV	2100
#define BASEMOD_DATAFLASH	2200

//Board frequencies
#define FM3_SUBOSC            32768		// freq. oscillatore subclock
#define FM3_MAINOSC           4000000	// freq. oscillatore quarzato
#define FM3_PLLCLK            120000000 //96000000	// freq. desiderata all'uscita dal PLL
										// nota: valori compresi tra 60Mhz e 120Mhz
#define FM3_SYSCLK            60000000  //48000000	// cpu + AHB base clock, Prescaler validi rispetto a FM3_PLLCLK: 1, 2, 3, 4, 6, 8, 16
#define FM3_APB0CLK           30000000  //24000000	// freq. desiderata per APB0
										// nota: max 40Mhz, Prescaler validi rispetto a FM3_SYSCLK: 1, 2, 4, 8
#define FM3_APB1CLK           30000000  //24000000	// freq. desiderata per APB1
										// nota: max 40Mhz, Prescaler validi rispetto a FM3_SYSCLK: 1, 2, 4, 8
#define FM3_APB2CLK           30000000 //24000000	// freq. desiderata per APB2
										// nota: max 40Mhz, Prescaler validi rispetto a FM3_SYSCLK: 1, 2, 4, 8
#define FM3_SWDOGCLK          7500000	// freq. desiderata per SOFT-WATCHDOG
										// nota: max 40Mhz, Prescaler validi rispetto a FM3_SYSCLK: 1, 2, 4, 8
#define FM3_ADCCLK			  FM3_APB1CLK// frq. A/D converter
#define FM3_BTCLK			  FM3_APB1CLK// frq. Base Timer
#define FM3_UARTCLK			  FM3_APB2CLK// frq. Uart

#define FM3_ENABLE_SUBOSC     FALSE
#define FM3_ENABLE_MAINOSC    TRUE
#define FM3_ENABLE_PLL    	  TRUE
#define FM3_SPICLK		FM3_APB2CLK	// frq. SPI


#endif // FM3_DEFINE_H_INCLUDED
