/**
@file		board.h
@brief		board definition and setup
@author		Fernando Morani (Tecnoideal) - From Enrico
@date		07/03/2017
@version	01.2
@source     Flexiper - Protection Micro
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fm3_sysclk.h"
//#include  "fm3.h"   //Usato per le definizioni flash
//#include "alloc.h"
#include "boardConfig.h"
#include "board_functDeclare.h"
#include "board_define.h"
#include "global.h"


typedef enum boardType
{
    TYPE_0 = 0,
    TYPE_1,
    TYPE_2,
    TYPE_3,
    TYPE_4,
    TYPE_5,
    TYPE_6,
    TYPE_7,
    TYPE_8,
    TYPE_9,
    TYPE_10,
    TYPE_11,
    TYPE_12
}boardType_Type;

typedef enum IRQPriorityBoard{
    NO_PRIORITY_SET = 0,
    IRQ_PRIORITY_LEV_1 = 1,
    IRQ_PRIORITY_LEV_2 = 2,
    IRQ_PRIORITY_LEV_3 = 3,
    IRQ_PRIORITY_LEV_4 = 4,
    IRQ_PRIORITY_LEV_5 = 5,
    IRQ_PRIORITY_LEV_6 = 6,
    IRQ_PRIORITY_LEV_7 = 7,
    IRQ_PRIORITY_LEV_8 = 8,
    IRQ_PRIORITY_LEV_9 = 9,
    IRQ_PRIORITY_LEV_10 = 10,
    IRQ_PRIORITY_LEV_11 = 11,
    IRQ_PRIORITY_LEV_12 = 12,
    IRQ_PRIORITY_LEV_13 = 13,
    IRQ_PRIORITY_LEV_14 = 14,
    IRQ_PRIORITY_LEV_15 = 15
}IRQPriorityBoard_Type;

typedef enum boardError{
    NO_ERROR                = 0x00000000,
    ETH_DEVICE_OUT_OF_ORDER = 0x00000001
}boardError_Type;

typedef enum relocateNumber
{
    RELOC_NUMBER_0 = 0,
    RELOC_NUMBER_1 = 1
}relocateNumber_Type;

typedef enum produceOutputExtBus
{
    EXT_BUS_DO_NOT_PRODUCE_OUTPUT = 0,
    EXT_BUS_PRODUCE_OUTPUT = 1,
}produceOutputExtBus_Type;

typedef enum extBusDeviceConfigured
{
    EXT_BUS_CS_MCSX_0 =0, // MCSX0 -> not Used
    EXT_BUS_CS_MCSX_1 = 1, // MCSX1 -> CPLD
    EXT_BUS_CS_MCSX_2 = 2, 	// MCSX2 -> FLASH
    EXT_BUS_CS_MCSX_3 = 3, // MCSX3 -> DAC
    EXT_BUS_CS_MCSX_4 = 4,	// MCSX4 -> RAM
    EXT_BUS_CS_MCSX_5 = 5,	// MCSX5 -> not used
    EXT_BUS_CS_MCSX_6 = 6, 	// MCSX6 -> not used
    EXT_BUS_CS_MCSX_7 = 7 	// MCSX7 -> RAM PROT
}extBusDeviceConfigured_Type;

typedef enum oscillatorConfig
{
    DO_NOT_USE_AS_CLOCK = 0, //Valid for main oscillator and sub oscillator
    USE_AS_CLOCK = 1 //Valid for main oscillator and sub oscillator
}oscillatorConfig_Type;

typedef enum extIntSelection
{
    USE_PIN_0_FUNCTION = 0,
    USE_PIN_INT_SPARE  = 1,
    USE_PIN_1_FUNCTION = 2,
    USE_PIN_2_FUNCTION = 3
}extIntSelection_Type;

typedef enum serialClockSelection
{
    DO_NOT_USE_AS_SCKL = 0,
    USE_SCLK_FUN_0     = 1,
    USE_SCLK_FUN_1     = 2,
    USE_SCLK_FUN_2     = 3
}serialClockSelection_Type;

typedef enum serialInputSelection
{
    DO_NOT_USE_AS_SIN = 0,
    USE_SIN_FUN_0     = 1,
    USE_SIN_FUN_1     = 2,
    USE_SIN_FUN_2     = 3
}serialInputSelection_Type;

typedef enum serialOutputSelection
{
    DO_NOT_USE_AS_SOUT = 0,
    USE_SOUT_FUN_0     = 1,
    USE_SOUT_FUN_1     = 2,
    USE_SOUT_FUN_2     = 3
}serialOutputSelection_Type;

typedef enum analogInputSelection{
    DON_NOT_USE_AS_ANALOGUE_INPUT = 0,
    USE_AS_ANALOGUE_INPUT =1
}analogInputSelection_Type;

typedef enum mfsIcuFunctionSelection{
    USE_ICU_INIT_VAL = 0,
    USE_ICU_FUN_0_AS_INPUT = 1,
    USE_ICU_FUN_1_AS_INPUT = 2,
    USE_INTERN_MFS_3_AS_INPUT = 4,
    USE_INTERN_MFS_7_AS_INPUT =5
    //3,6,7 setting are prohibited
}mfsIcuFunctionSelection_Type;

typedef enum baseTimerSelection
{
    //Valid for TIOB and TIOA
    INIT_VAL_FOR_BT  = 0,
    USE_BT_FUN_0     = 1,
    USE_BT_FUN_1     = 2,
    USE_BT_FUN_2     = 3
}baseTimerSelection_Type;

typedef struct{
    __IO uint32_t TRMM0       : 1;
    __IO uint32_t TRMM1       : 1;
    __IO uint32_t TRMM2       : 1;
    __IO uint32_t TRMM3       : 1;
    __IO uint32_t TRMM4       : 1;
    __IO uint32_t TRMM5       : 1;
    __IO uint32_t TRMM6       : 1;
    __IO uint32_t TRMM7       : 1;
    __IO uint32_t TRMM8       : 1;
    __IO uint32_t TRMM9       : 1;
}FM3_FLASH_TRM_IF_Type;

#ifdef __cplusplus
extern "C" {
#endif

void boardInit(void);


#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H_ */
