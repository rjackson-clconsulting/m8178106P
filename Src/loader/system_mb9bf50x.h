/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/

#ifndef _SYSTEM_MB9BF50X_H_
#define _SYSTEM_MB9BF50X_H_

  
typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define IWDG_BASE       0x40003000
#define FLASH_BASE      0x40022000

#define IWDG            ((IWDG_TypeDef *) IWDG_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)

// Independent WATCHDOG
typedef struct {
  vu32 KR;
  vu32 PR;
  vu32 RLR;
  vu32 SR;
} IWDG_TypeDef;

// Flash Registers
typedef struct {
  vu32 ACR;
  vu32 KEYR;
  vu32 OPTKEYR;
  vu32 SR;
  vu32 CR;
  vu32 AR;
  vu32 RESERVED;
  vu32 OBR;
  vu32 WRPR;
} FLASH_TypeDef;


// Flash Keys
#define RDPRT_KEY       0x5AA5
#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

// Flash Control Register definitions
#define FLASH_PG        0x00000001
#define FLASH_PER       0x00000002
#define FLASH_MER       0x00000004
#define FLASH_OPTPG     0x00000010
#define FLASH_OPTER     0x00000020
#define FLASH_STRT      0x00000040
#define FLASH_LOCK      0x00000080
#define FLASH_OPTWRE    0x00000100

// Flash Status Register definitions
#define FLASH_BSY       0x00000001
#define FLASH_PGERR     0x00000004
#define FLASH_WRPRTERR  0x00000010
#define FLASH_EOP       0x00000020

/* Fujitsu Flash Status Register definitions
*/

#define FLASH_SEQ_1550 ((volatile uint16_t*) 0x00001550) // sequence address 1
#define FLASH_SEQ_0AA8 ((volatile uint16_t*) 0x00000AA8) // sequence address 2

#define FLASH_CHIP_ERASE_1   0x00AA   // chip erase commands
#define FLASH_CHIP_ERASE_2   0x0055
#define FLASH_CHIP_ERASE_3   0x0080
#define FLASH_CHIP_ERASE_4   0x00AA
#define FLASH_CHIP_ERASE_5   0x0055
#define FLASH_CHIP_ERASE_6   0x0010

#define FLASH_SECTOR_ERASE_1 0x00AA   // sector erase commands
#define FLASH_SECTOR_ERASE_2 0x0055
#define FLASH_SECTOR_ERASE_3 0x0080
#define FLASH_SECTOR_ERASE_4 0x00AA
#define FLASH_SECTOR_ERASE_5 0x0055
#define FLASH_SECTOR_ERASE_6 0x0030

#define FLASH_WRITE_1        0x00AA   // flash write commands
#define FLASH_WRITE_2        0x0055 
#define FLASH_WRITE_3        0x00A0

#define FLASH_SECTOR_ERASE_SUSPEND 0x00B0 // Sector erase suspend command
#define FLASH_SECTOR_ERASE_RESTART 0x0030 // Sector erase restart (resume) command

#define FLASH_ADDRESS_SA4  0x00000000     // address of sector SA4
#define FLASH_ADDRESS_SA5  0x00000004     // address of sector SA5
#define FLASH_ADDRESS_SA6  0x00004000     // address of sector SA6
#define FLASH_ADDRESS_SA7  0x00004004     // address of sector SA7
#define FLASH_ADDRESS_SA8  0x00008000     // address of sector SA8
#define FLASH_ADDRESS_SA9  0x00008004     // address of sector SA9
#define FLASH_ADDRESS_SA10 0x00020000     // address of sector SA10
#define FLASH_ADDRESS_SA11 0x00020004     // address of sector SA11
#define FLASH_ADDRESS_SA12 0x00040000     // address of sector SA12
#define FLASH_ADDRESS_SA13 0x00040004     // address of sector SA13
#define FLASH_ADDRESS_SA14 0x00060000     // address of sector SA14
#define FLASH_ADDRESS_SA15 0x00060004     // address of sector SA15

#define FLASH_DQ7 0x0080    // data polling flag bit (DPOL) position
#define FLASH_DQ6 0x0040    // data toggle flag bit (TOGG) position
#define FLASH_DQ5 0x0020    // time limit exceeding flag bit (TLOV) position
#define FLASH_DQ3 0x0008    // sector erase timer flag bit (SETI) position

#define FLASH_TIMEOUT_ERROR   -1
#define FLASH_OK              1




// System config


#define CLOCK_SETUP           1
#define SCM_CTL_Val           0x00000052
#define CSW_TMR_Val           0x0000005C
#define PSW_TMR_Val           0x00000000
#define PLL_CTL1_Val          0x00000000
#define PLL_CTL2_Val          0x00000013
#define BSC_PSR_Val           0x00000000
#define APBC0_PSR_Val         0x00000001
#define APBC1_PSR_Val         0x00000081
#define APBC2_PSR_Val         0x00000081
#define SWC_PSR_Val           0x00000003
#define TTC_PSR_Val           0x00000000

/*--------------------- WatchDog Configuration -------------------------------*/
//
// <o0.0>  HW Watchdog disable 

#define HWWD_DISABLE          0x00000001


/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Clock Configuration -------------------------------------------------------*/
#if (CHECK_RSVD((SCM_CTL_Val),    ~0x000000FA))
   #error "SCM_CTL: Invalid values of reserved bits!"
#endif

#if ((SCM_CTL_Val & 0xE0) == 0x40) && ((SCM_CTL_Val & 0x10) != 0x10)
   #error "SCM_CTL: CLKPLL is selected but PLL is not enabled!"
#endif

#if (CHECK_RSVD((CSW_TMR_Val),    ~0x0000007F))
   #error "CSW_TMR: Invalid values of reserved bits!"
#endif

#if ((SCM_CTL_Val & 0x10))       /* if PLL is used */
  #if (CHECK_RSVD((PSW_TMR_val),  ~0x00000007))
     #error "PSW_TMR: Invalid values of reserved bits!"
  #endif

  #if (CHECK_RSVD((PLL_CTL1_Val), ~0x000000FF))
     #error "PLL_CTL1: Invalid values of reserved bits!"
  #endif

  #if (CHECK_RSVD((PLL_CTL2_Val), ~0x0000001F))
     #error "PLL_CTL2: Invalid values of reserved bits!"
  #endif
#endif

#if (CHECK_RSVD((BSC_PSR_Val),    ~0x00000007))
   #error "BSC_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC0_PSR_Val),  ~0x00000003))
   #error "APBC0_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC1_PSR_Val),  ~0x00000083))
   #error "APBC1_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC2_PSR_Val),  ~0x00000083))
   #error "APBC2_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((SWC_PSR_Val),    ~0x00000003))
   #error "SWC_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((TTC_PSR_Val),    ~0x00000001))
   #error "TTC_PSR: Invalid values of reserved bits!"
#endif


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __CLKMO        ( 4000000UL)         /* External   4MHz Crystal        */
#define __CLKSO        (   32768UL)         /* External  32KHz Crystal        */
#define __CLKHC        ( 4000000UL)         /* Internal   4MHz RC Oscillator  */
#define __CLKLC        (  100000UL)         /* Internal 100KHz RC Oscillator  */


/* CLKPLL = (CLKMO / PLLK) * PLLN  */
#define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
#define __PLLN         (((PLL_CTL2_Val     ) & 0x1F) + 1)
#define __PLLCLK       ((__CLKMO  * __PLLN) / __PLLK)

/* Determine core clock frequency according to settings */
#if   (((SCM_CTL_Val >> 5) & 0x07) == 0)
    #define __MASTERCLK     (__CLKHC)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 1)
    #define __MASTERCLK     (__CLKMO)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 2)
    #define __MASTERCLK     (__PLLCLK)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 4)
    #define __MASTERCLK     (__CLKLC)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 5)
    #define __MASTERCLK     (__CLKSO)
#else
    #define __MASTERCLK     (0UL)
#endif

#if   ((BSC_PSR_Val & 0x07) == 0)
    #define __HCLK         (__MASTERCLK / 1)
#elif ((BSC_PSR_Val & 0x07) == 1)
    #define __HCLK         (__MASTERCLK / 2)
#elif ((BSC_PSR_Val & 0x07) == 2)
    #define __HCLK         (__MASTERCLK / 3)
#elif ((BSC_PSR_Val & 0x07) == 3)
    #define __HCLK         (__MASTERCLK / 4)
#elif ((BSC_PSR_Val & 0x07) == 4)
    #define __HCLK         (__MASTERCLK / 6)
#elif ((BSC_PSR_Val & 0x07) == 5)
    #define __HCLK         (__MASTERCLK / 8)
#elif ((BSC_PSR_Val & 0x07) == 6)
    #define __HCLK         (__MASTERCLK /16)
#else
    #define __HCLK         (0UL)
#endif



/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)*/




#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */
 
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_MB9BF50X_H */
