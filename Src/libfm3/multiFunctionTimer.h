/**
@file		multiFunctionTimer.h
@brief		header File to use multiFunctionTimer in this board
@author		Fernando Morani
@date		06/13/2018
@version	01.23
@source     FM3 Base TYPE 2
*/

#ifndef _MULTIFUNCTIONTIMER_H_
#define _MULTIFUNCTIONTIMER_H_

// include files
#include "board_define.h"  //Ver 01.20 - Use the mbxxx include file selected from the board_define settings
#include "mb9bd1xt.h"
#include "stdio.h"
#include <string>
#include "global.h"   //Used to see _IRQ Macro
#include "alloc.h"
#include "fm3_define.h"  //Ver 01.21


//  define List
// Interrupt vector connected to MFT
#define MFT_FRT_IRQ		VectorB0   //This value has been defined in vectors.c file
#define MFT_ICU_IRQ		VectorB4
#define MFT_OCU_IRQ		VectorB8

#define INT_SIZE                    2 //Size of integer is 2 Bytes
#define MAX_MFT_NUMBER              3 //Max number of multifunction Timer
#define MAX_FRT_CHANELL_FOR_UNIT    3 //Number of FRT channel for unit
#define MAX_OCU_UNIT_FOR_MFT_UNIT   3 //Number of OCU units for MFT
#define MAX_ICU_UNIT_FOR_MFT_UNIT   2 //Number of ICU units for MFT
#define MAX_ICU_CHANNEL_FOR_ICU_UNIT   2 //Number of channels for each ICU units
#define MAX_WFG_UNIT_FOR_MFT_UNIT   3 //Number of WFG units for MFT
#define CHANNEL_FOR_OCU_UNIT        2 //Numebr of channel for OCU unit
#define CHANNEL_FOR_ICU_UNIT        2 //Numebr of channel for ICU unit
#define RT_PIN_STRING_LENGHT        7 //Lenght of RTO00_0
#define IC_PIN_STRING_LENGHT        6 //Lenght of IC10_0
#define NUMBER_OF_OUTPUT_PIN        CHANNEL_FOR_OCU_UNIT*MAX_OCU_UNIT_FOR_MFT_UNIT //OCU PINS
#define NUMBER_OF_INPUT_PIN         CHANNEL_FOR_ICU_UNIT*MAX_ICU_UNIT_FOR_MFT_UNIT //ICU PINS

#define MFT_UNIT_0          0 //MFT Unit 0
#define MFT_UNIT_1          1 //MFT Unit 1
#define MFT_UNIT_2          2 //MFT Unit 2

#define MFT_INSTALLED         1
#define MFT_UNINSTALLED       0

#define MFT_WRONG_ID        0xFF

#define FM3_PCLK            FM3_APB1CLK  //Base clock for MFT is APB1

//FRT Register Offset
#define FM3_MFT_FRT_TCSA0_OFFSET     (0x00000030UL)
#define FM3_MFT_FRT_TCSA1_OFFSET     (0x00000040UL)
#define FM3_MFT_FRT_TCSA2_OFFSET     (0x00000050UL)
#define FM3_MFT_FRT_TCSB0_OFFSET     (0x00000034UL)
#define FM3_MFT_FRT_TCSB1_OFFSET     (0x00000044UL)
#define FM3_MFT_FRT_TCSB2_OFFSET     (0x00000054UL)
#define FM3_MFT_FRT_TCCP0_OFFSET     (0x00000028UL)
#define FM3_MFT_FRT_TCCP1_OFFSET     (0x00000038UL)
#define FM3_MFT_FRT_TCCP2_OFFSET     (0x00000048UL)
#define FM3_MFT_FRT_TCDT0_OFFSET     (0x0000002CUL)
#define FM3_MFT_FRT_TCDT1_OFFSET     (0x0000003CUL)
#define FM3_MFT_FRT_TCDT2_OFFSET     (0x0000004CUL)

//OCU Register Offset
#define FM3_MFT_OCU_BASE_OFFSET       (0x00000000UL) //Offset from FM3_MFTx_BASE
#define FM3_MFT_OCU_OCCP0_OFFSET      (0x00000000UL)
#define FM3_MFT_OCU_OCCP1_OFFSET      (0x00000004UL)
#define FM3_MFT_OCU_OCCP2_OFFSET      (0x00000008UL)
#define FM3_MFT_OCU_OCCP3_OFFSET      (0x0000000CUL)
#define FM3_MFT_OCU_OCCP4_OFFSET      (0x00000010UL)
#define FM3_MFT_OCU_OCCP5_OFFSET      (0x00000014UL)
#define FM3_MFT_OCU_OCSA10_OFFSET     (0x00000018UL)
#define FM3_MFT_OCU_OCSB10_OFFSET     (0x00000019UL)
#define FM3_MFT_OCU_OCSA32_OFFSET     (0x0000001CUL)
#define FM3_MFT_OCU_OCSB32_OFFSET     (0x0000001DUL)
#define FM3_MFT_OCU_OCSA54_OFFSET     (0x00000020UL)
#define FM3_MFT_OCU_OCSB54_OFFSET     (0x00000021UL)
#define FM3_MFT_OCU_OCSC_OFFSET       (0x00000025UL)
#define FM3_MFT_OCU_OCFS10_OFFSET     (0x00000058UL)
#define FM3_MFT_OCU_OCFS32_OFFSET     (0x00000059UL)
#define FM3_MFT_OCU_OCFS54_OFFSET     (0x0000005CUL)

//WFG Regsiter
#define FM3_MFT_WFG_BASE_OFFSET       (0x00000000UL) //Offset from FM3_MFTx_BASE
#define FM3_MFT_WFG_WFTM10_OFFSET     (0x00000080UL)
#define FM3_MFT_WFG_WFTM32_OFFSET     (0x00000084UL)
#define FM3_MFT_WFG_WFTM54_OFFSET     (0x00000088UL)
#define FM3_MFT_WFG_WFSA10_OFFSET     (0x0000008CUL)
#define FM3_MFT_WFG_WFSA32_OFFSET     (0x00000090UL)
#define FM3_MFT_WFG_WFSA54_OFFSET     (0x00000094UL)
#define FM3_MFT_WFG_WFIR_OFFSET       (0x00000098UL)

//ICU Regsiters
#define FM3_MFT_ICU_BASE_OFFSET       (0x00000000UL) //Offset from FM3_MFTx_BASE
#define FM3_MFT_ICU_ICFS10_OFFSET     (0x00000060UL)
#define FM3_MFT_ICU_ICFS32_OFFSET     (0x00000061UL)
#define FM3_MFT_ICU_ICCP0_OFFSET      (0x00000068UL)
#define FM3_MFT_ICU_ICCP1_OFFSET      (0x0000006CUL)
#define FM3_MFT_ICU_ICCP2_OFFSET      (0x00000070UL)
#define FM3_MFT_ICU_ICCP3_OFFSET      (0x00000074UL)
#define FM3_MFT_ICU_ICSA10_OFFSET     (0x00000078UL)
#define FM3_MFT_ICU_ICSB10_OFFSET     (0x00000079UL)
#define FM3_MFT_ICU_ICSA32_OFFSET     (0x0000007CUL)
#define FM3_MFT_ICU_ICSB32_OFFSET     (0x0000007DUL)

#define TSCA_INIT_VALUES      0x0040
#define TSCB_INIT_VALUES      0x0000
#define TCDT_CLEARED_VALUE    0x0000
#define TCCP_DEFAULT_VALUE    0xFFFF
#define OCU_OCFS_DEFAULT_VALUE 0x00
#define OCU_OCSA_DEFAULT_VALUE 0x0C
#define OCU_OCSB_DEFAULT_VALUE 0x60
#define OCU_OCSC_DEFAULT_VALUE 0x00
#define WFG_WFSA_DEFAULT_VALUE 0x0000
#define WFG_WFTM_DEFAULT_VALUE 0x0000
#define WFG_WFIR_DEFAULT_VALUE 0x0000
#define ICU_ICFS_DEFAULT_VALUE 0x00
#define ICU_ICSA_DEFAULT_VALUE 0x00
#define ICU_ICSB_DEFAULT_VALUE 0x00

/**Define Mask to be used */
#define TSCA_ICLR_MASK          0x0200  //Mask to be used to query the ICLR
#define ICSA_ICP_0_1_RESET_MASK 0x3F    //Mask to be used to reset both ICP registers
#define ICSA_CLEAR_ICP0_MASK    0x80;   //1 to ICP1 and 0 to ICP 0
#define ICSA_CLEAR_ICP1_MASK    0x40;   //0 to ICP1 and 1 to ICP 0

// Structure
// The MFT is composed by:
//  Free-run Timer Unit : 3channels
//  Output Compare Unit : 6channels (2channels ×3units)
//  Waveform Generator Unit : 3channels
//  Noise Canceller Unit : 1channel
//  Input Capture Unit : 4channels (2channels ×2units)
//  ADC Start Compare Unit : 3channels
//  ADC Start Trigger Selector Unit : 3channels


typedef struct stc_mft_frt_tcsa_field
{
    union {
        struct {
            __IO  uint16_t  CLK                  :4;
            __IO  uint16_t  SCLR                 :1;
            __IO  uint16_t  MODE                 :1;
            __IO  uint16_t  STOP                 :1;
            __IO  uint16_t  BFE                  :1;
            __IO  uint16_t  ICRE                 :1;
            __IO  uint16_t  ICLR                 :1;
            __IO  uint16_t  RESERVED0            :3;
            __IO  uint16_t  IRQZE                :1;
            __IO  uint16_t  IRQZF                :1;
            __IO  uint16_t  ECKE                 :1;
        };
        struct {
            __IO  uint16_t  CLK0                 :1;
            __IO  uint16_t  CLK1                 :1;
            __IO  uint16_t  CLK2                 :1;
            __IO  uint16_t  CLK3                 :1;
            __IO  uint16_t  RESERVED1            :12;
        };
       __IO  uint16_t ALL_BITS;
    };
} stc_mft_frt_tcsa_field_t;

typedef struct stc_mft_frt_tcsb_field
{
    union
      {
      struct {
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
        };
       __IO  uint16_t ALL_BITS;
      };
} stc_mft_frt_tcsb_field_t;

typedef struct stc_mft_frt_tccp_field
{
    union {
        struct {
            __IO  uint16_t  TCCP                 :16;
        };
        struct {
            __IO  uint16_t  TCCP0                :1;
            __IO  uint16_t  TCCP1                :1;
            __IO  uint16_t  TCCP2                :1;
            __IO  uint16_t  TCCP3                :1;
            __IO  uint16_t  TCCP4                :1;
            __IO  uint16_t  TCCP5                :1;
            __IO  uint16_t  TCCP6                :1;
            __IO  uint16_t  TCCP7                :1;
            __IO  uint16_t  TCCP8                :1;
            __IO  uint16_t  TCCP9                :1;
            __IO  uint16_t  TCCP10               :1;
            __IO  uint16_t  TCCP11               :1;
            __IO  uint16_t  TCCP12               :1;
            __IO  uint16_t  TCCP13               :1;
            __IO  uint16_t  TCCP14               :1;
            __IO  uint16_t  TCCP15               :1;
        };
    };
} stc_mft_frt_tccp_field_t;

typedef struct stc_mft_frt_tcdt_field
{
union {
        __IO uint16_t FRT_TCDT;
        struct {
            __IO  uint8_t FRT_TCDTL;
            __IO  uint8_t FRT_TCDTH;
        };
    };
} stc_mft_frt_tcdt_field_t;

typedef struct stc_mft_ocu_ocsa_field
{
union {
  __IO uint8_t ALL_BITS;
        struct{
  __IO  uint8_t CST0       : 1;
  __IO  uint8_t CST1       : 1;
  __IO  uint8_t BDIS0      : 1;
  __IO  uint8_t BDIS1      : 1;
  __IO  uint8_t IOE0       : 1;
  __IO  uint8_t IOE1       : 1;
  __IO  uint8_t IOP0       : 1;
  __IO  uint8_t IOP1       : 1;
  };
  };
} stc_mft_ocu_ocsa_field_t;

typedef struct stc_mft_ocu_ocsb_field
{
union {
  __IO uint8_t ALL_BITS;
        struct{
  __IO  uint8_t OTD0       : 1;
  __IO  uint8_t OTD1       : 1;
        uint8_t RESERVED1  : 2;
  __IO  uint8_t CMOD       : 1;
  __IO  uint8_t BTS0       : 1;
  __IO  uint8_t BTS1       : 1;
    };
  };
} stc_mft_ocu_ocsb_field_t;

typedef struct stc_mft_ocu_ocfs_field
{
union {
    struct {
            __IO  uint8_t FSO0      :4;
            __IO  uint8_t FSO1      :4;
        };
    struct{
  __IO  uint8_t FSO00      : 1;
  __IO  uint8_t FSO01      : 1;
  __IO  uint8_t FSO02      : 1;
  __IO  uint8_t FSO03      : 1;
  __IO  uint8_t FSO10      : 1;
  __IO  uint8_t FSO11      : 1;
  __IO  uint8_t FSO12      : 1;
  __IO  uint8_t FSO13      : 1;
  };
  };
} stc_mft_ocu_ocfs_field_t;


typedef struct stc_mft_ocu_occp_field
{
  __IO uint16_t REGISTER;
} stc_mft_ocu_occp_field_t;

typedef struct stc_mft_wfg_wfsa_field
{
union {
  __IO uint16_t REGISTER;
  struct {
  __IO uint16_t DCLOCK     : 3;
  __IO uint16_t TMD        : 3;
  __IO uint16_t GTEN       : 2;
  __IO uint16_t PSEL       : 2;
  __IO uint16_t PGEN       : 2;
  __IO uint16_t DMOD       : 1;
       uint16_t RESERVED   : 3;
  };//stc_mft_bit_access_t;
  };
} stc_mft_wfg_wfsa_field_t;

typedef struct stc_mft_wfg_wftm_field
{
  __IO uint16_t REGISTER;
} stc_mft_wfg_wftm_field_t;


typedef struct stc_mft_icu_icsa_field
{
union {
  __IO uint8_t ALL_BITS;
    struct{
  __IO  uint8_t EG0       : 2;
  __IO  uint8_t EG1       : 2;
  __IO  uint8_t ICE0       : 1;
  __IO  uint8_t ICE1       : 1;
  __IO  uint8_t ICP0       : 1;
  __IO  uint8_t ICP1       : 1;
    };
  };
} stc_mft_icu_icsa_field_t;

typedef struct stc_mft_icu_icsb_field
{
union {
  __IO uint8_t ALL_BITS;
        struct{
  __IO  uint8_t IEI0       : 1;
  __IO  uint8_t IEI1       : 1;
  };
  };
} stc_mft_icu_icsb_field_t;

typedef struct stc_mft_icu_icfs_field
{
union {
    struct {
            __IO  uint8_t FSO0      :4;
            __IO  uint8_t FSO1      :4;
        };
    struct{
  __IO  uint8_t FSI00      : 1;
  __IO  uint8_t FSI01      : 1;
  __IO  uint8_t FSI02      : 1;
  __IO  uint8_t FSI03      : 1;
  __IO  uint8_t FSI10      : 1;
  __IO  uint8_t FSI11      : 1;
  __IO  uint8_t FSI12      : 1;
  __IO  uint8_t FSI13      : 1;
  };
  };
} stc_mft_icu_icfs_field_t;

typedef struct stc_mft_icu_iccp_field
{
  __IO uint16_t REGISTER;
} stc_mft_icu_iccp_field_t;


typedef enum frtChannel
{
 FRT_CHANNEL_0 = 0,
 FRT_CHANNEL_1 = 1,
 FRT_CHANNEL_2 = 2
}frtChannel_Type;

typedef enum frtADCOutput
{
FRT_ADC0 = 0,
FRT_ADC1 = 1,
FRT_ADC2 = 2
}frtADCOutput_Type;

typedef enum frtADCTrigger
{
FRT_ADC_TRIG_OUT_DISABLE = 0,
FRT_ADC_TRIG_OUT_ENABLE = 1
}frtADCTrigger_Type;

typedef enum cycleRatio
{
/**
0000 Sets FRT's count clock cycle to the same value as PCLK.
0001 Sets FRT's count clock cycle to PCLK multiplied by 2.
0010 Sets FRT's count clock cycle to PCLK multiplied by 4.
0011 Sets FRT's count clock cycle to PCLK multiplied by 8.
0100 Sets FRT's count clock cycle to PCLK multiplied by 16.
0101 Sets FRT's count clock cycle to PCLK multiplied by 32.
0110 Sets FRT's count clock cycle to PCLK multiplied by 64.
0111 Sets FRT's count clock cycle to PCLK multiplied by 128.
1000 Sets FRT's count clock cycle to PCLK multiplied by 256

000 Sets the count clock cycle of the WFG timer to the same value as PCLK.
001 Sets the count clock cycle of the WFG timer to PCLK multiplied by 2.
010 Sets the count clock cycle of the WFG timer to PCLK multiplied by 4.
011 Sets the count clock cycle of the WFG timer to PCLK multiplied by 8.
100 Sets the count clock cycle of the WFG timer to PCLK multiplied by 16.
101 Sets the count clock cycle of the WFG timer to PCLK multiplied by 32.
110 Sets the count clock cycle of the WFG timer to PCLK multiplied by 64
*/
CYCLE_RATIO_1    = 0,  //Cycle Ratio = 1
CYCLE_RATIO_2    = 1,
CYCLE_RATIO_4    = 2,
CYCLE_RATIO_8    = 3,
CYCLE_RATIO_16   = 4,
CYCLE_RATIO_32   = 5,
CYCLE_RATIO_64   = 6,
CYCLE_RATIO_128  = 7,
CYCLE_RATIO_256  = 8
}cycleRatio_Type;

typedef enum clearedRegister
{
CLEARED_REGISTER = 0,
NOT_CLEARED_REGISTER = 1
}clearedRegister_Type;

typedef enum frtInitRequest
{
CANCEL_REQUEST = 0,
ISSUE_REQUEST = 1
}frtInitRequest_Type;

typedef enum frtCounterMode
{
UP_MODE = 0,
UP_DOWN_MODE = 1
}frtCounterMode_Type;

typedef enum frtOperMode
{
OPERATING_MODE = 0,
STOPPING_MODE = 1
}frtOperMode_Type;

typedef enum frtBufferFunction
{
BUFFER_DISABLE = 0,
BUFFER_ENABLE = 1
}frtBufferFunction_Type;

typedef enum frtEnableIntOnMatch
{
INT_ON_MATCH_DISABLED = 0,
INT_ON_MATCH_ENABLED =1
}frtEnableIntOnMatch_Type;

typedef enum frtClock
{
INTERNAL_CLOCK = 0,
EXTERNAL_CLOCK =1
}frtClock_Type;

typedef enum ocuUnit
{
OCU_UNIT_0 = 0,
OCU_UNIT_1 = 1,
OCU_UNIT_2 = 2
}ocuUnit_Type;

typedef enum ocuChannel
{
OCU_CHANNEL_0 = 0,
OCU_CHANNEL_1 = 1
}ocuChannel_Type;

typedef enum ocuIcuConnectFrt
{
OCU_ICU_CONNECT_FRT_0 = 0,
OCU_ICU_CONNECT_FRT_1 = 1,
OCU_ICU_CONNECT_FRT_2 = 2,
OCU_ICU_CONNECT_EXT_MFT_FRT_0 = 3,
OCU_ICU_CONNECT_EXT_MFT_FRT_1 = 4,
OCU_ICU_CONNECT_ERROR = 8
}ocuIcuConnectFrt_Type;

typedef enum ocuOperation
{
OCU_OPER_DISABLED = 0,
OCU_OPER_ENABLED = 1
}ocuOperation_Type;

typedef enum ocuBufferState{
OCU_BUFFER_ENABLED =0,
OCU_BUFFER_DISABLED = 1
}ocuBufferState_Type;

typedef enum ocuGenerateInt{
OCU_NOT_GENERATE_INTERRUPT = 0,
OCU_GENERATE_INTERRUPT = 1
}ocuGenerateInt_Type;

typedef enum ocuRTOutputLevel{
OCU_OUTPUT_LOW_LEVEL = 0,
OCU_OUTPUT_HIGH_LEVEL = 1
}ocuRTOutputLevel_Type;

typedef enum ocuBufferTransfer{
BUF_TRANSFER_ON_ZERO = 0,
BUF_TRANSFER_ON_PEAK =1
}ocuBufferTransfer_Type;

typedef enum ocuModeUse{
UP_COUNT_MODE_CHANGE_1 = 0,
UP_COUNT_MODE_CHANGE_2 = 1,
UP_DOWN_COUNT_MODE_ACTIVE_HIGH =3,
UP_DOWN_COUNT_MODE_ACTIVE_LOW =4
}ocuModeUse_Type;

typedef enum ocuAdmittedMode{
OCU_ADMITTED_0 = 0, //CH(0): UP_COUNT_MODE_CHANGE_1 - CH(1):UP_COUNT_MODE_CHANGE_1
OCU_ADMITTED_1 = 1, //CH(0): UP_COUNT_MODE_CHANGE_1 - CH(1):UP_COUNT_MODE_CHANGE_2
OCU_ADMITTED_2 = 2, //CH(0): UP_DOWN_COUNT_MODE_ACTIVE_HIGH - CH(1):UP_COUNT_MODE_CHANGE_1
OCU_ADMITTED_3 = 3, //CH(0): UP_COUNT_MODE_CHANGE_1 - CH(1):UP_DOWN_COUNT_MODE_ACTIVE_HIGH
OCU_ADMITTED_4 = 4, //CH(0): UP_COUNT_MODE_CHANGE_1 - CH(1):UP_DOWN_COUNT_MODE_ACTIVE_LOW
OCU_ADMITTED_5 = 5, //CH(0): UP_DOWN_COUNT_MODE_ACTIVE_HIGH - CH(1):UP_DOWN_COUNT_MODE_ACTIVE_HIGH
OCU_ADMITTED_6 = 6  //CH(0): UP_DOWN_COUNT_MODE_ACTIVE_LOW - CH(1):UP_DOWN_COUNT_MODE_ACTIVE_LOW
}ocuAdmittedMode_Type;

typedef enum wfgUnit
{
WFG_UNIT_0 = 0,
WFG_UNIT_1 = 1,
WFG_UNIT_2 = 2
}wfgUnit_Type;

typedef enum wfgChannel
{
WFG_CHANNEL_0 = 0,
WFG_CHANNEL_1 = 1
}wfgChannel_Type;

typedef enum wfgTimerStatus
{
WFG_STOPPED_TIMER = 0,
WFG_TIMER_IN_OPERATION =1
}wfgTimerStatus_t;

typedef enum wfgOperationMode
{
/**
000 Sets WFG's operation mode to Through mode.
001 Sets WFG's operation mode to RT-PPG mode.
010 Sets WFG's operation mode to Timer- PPG mode.
100 Sets WFG's operation mode to RT-dead timer mode.
111 Sets WFG's operation mode to PPG-dead timer mode.
*/
WFG_THROUGH_MODE = 0,
WFG_RT_PPG_MODE  = 1,
WFG_TIMER_PPG_MODE = 2,
WFG_RT_DEAD_TIMER_MODE = 4,
WFG_PPG_DEAD_TIMER_MODE = 7
}wfgOperationMode_t;

typedef enum chGateCode
{
/**
RT-PPG Mode
00 Always outputs Low-level signals
01 Outputs RT(0) signal without change
10 Outputs RT(1) signal without change
11 Outputs High-level signals when either RT(1) signal or RT(0) signal is High-level Outputs Low-level signals when both RT(1) and RT(0) signals are Low-level

Timer-PPG Mode
00 Always outputs Low-level signals
01 Outputs WFG timer active flag0
10 Outputs WFG timer active flag1
11 Outputs High-level signals when either of WFG timer active flags is "1" Outputs Low-level signals when both of WFG timer active flags are "0"

PPG-dead timer mode
00 Always outputs Low-level signals
01 Outputs RT(0) signal without change
10 Outputs RT(1) signal without change
11 Outputs High-level signals when either RT(1) signal or RT(0) signal is High-level Outputs Low-level signals when both RT(1) and RT(0) signals are Low-level
*/
CH_GATE_ALWAYS_LOW = 0,
CH_GATE_ONLY_FIRST_CHAN = 1,
CH_GATE_ONLY_SECOND_CHAN = 2,
CH_GATE_BOTH_CHAN_LOGIC =3
}chGateCode_t;

typedef enum ppcToGateMode
{
/**
00 Sets the output destination of the GATE signal to ch.0 of the PPG timer unit. Sets the input source of the PPG signal to ch.0 of the PPG timer unit.
01 Sets the output destination of the GATE signal to ch.2 of the PPG timer unit. Sets the input source of the PPG signal to ch.2 of the PPG timer unit.
10 Sets the output destination of the GATE signal to ch.4 of the PPG timer unit. Sets the input source of the PPG signal to ch.4 of the PPG timer unit.
11 Setting is prohibited.
*/
USE_PPC_CH_0 = 0,  //Means: CH=0 for MFT Unit 0;  CH=8 for MFT Unit 1;  CH=16 for MFT Unit 2
USE_PPC_CH_2 = 1,  //Means: CH=2 for MFT Unit 0;  CH=10 for MFT Unit 1;  CH=18 for MFT Unit 2
USE_PPC_CH_4 = 2   //Means: CH=4 for MFT Unit 0;  CH=12 for MFT Unit 1;  CH=20 for MFT Unit 2
}ppcToGateMode_t;

typedef enum  chPPGOnWFGoutput
{
// Reflection of CH_PPG signal on WFG output
//Each bit combination has a fixed mening (Table 4.9 of the Manual) based on the WGF Operation Mode
FF_REFLECTION_ON_WFG = 0, //It means False-False (00 Bits) - No reflection
FT_REFLECTION_ON_WFG = 1,
TF_REFLECTION_ON_WFG = 2,
TT_REFLECTION_ON_WFG = 3
} chPPGOnWFGoutput_t;

typedef enum wfgOutputPolarity
{
POLARITY_ACTIVE_HIGH = 0,
POLARITY_ACTIVE_LOW = 1
}wfgOutputPolarity_t;

typedef enum wfgDTIF
{
WFG_DTIF_NOT_GENERATED = 0,
WFG_DTIF_GENERATED = 1
}wfgDTIF_t;

typedef struct rtoPinConversion{
ocuUnit_Type ocuUnit;
ocuChannel_Type ocuChan;
char rtName[RT_PIN_STRING_LENGHT]; //1 is CR
}rtoPinConversion_t;

typedef enum icuUnit
{
ICU_UNIT_0 = 0,
ICU_UNIT_1 = 1
}icuUnit_Type;

typedef enum icuChannel
{
ICU_CHANNEL_0 = 0,
ICU_CHANNEL_1 = 1
}icuChannel_Type;

typedef enum icuOperation
{
ICU_OPER_DISABLED = 0,
ICU_OPER_RISE_ENABLE = 1,
ICU_OPER_FALL_ENABLE = 2,
ICU_OPER_RISE_FALL_ENABLE = 3
}icuOperation_Type;

typedef enum icuGenerateInt{
ICU_NOT_GENERATE_INTERRUPT = 0,
ICU_GENERATE_INTERRUPT = 1
}icuGenerateInt_Type;

typedef enum icuLastInput{
ICU_LAST_WAS_FALLING = 0,
ICU_LAST_WAS_RISING =1
}icuLastInput_Type;

typedef struct icPinConversion{
icuUnit_Type icuUnit;
icuChannel_Type icuChan;
char icName[IC_PIN_STRING_LENGHT]; //1 is CR
}icPinConversion_t;


// MFT UNIT
typedef struct stc_mft_unit
{
    //Struct defined in mb9bd10t.h
    FM3_MFT_TypeDef *pmftBaseReference;
    FM3_MFT_OCU_TypeDef *pmftOcuBaseReference;
    FM3_MFT_WFG_TypeDef *pmftWfgBaseReference;
    FM3_MFT_ICU_TypeDef *pmftIcuBaseReference;

   /* __IO uint16_t *occp0_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    __IO uint16_t *occp1_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];*/
    stc_mft_ocu_occp_field_t *occp0_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    stc_mft_ocu_occp_field_t *occp1_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    stc_mft_ocu_ocfs_field_t *ocfs_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    stc_mft_ocu_ocsc_field_t *ocsc_Register;
    stc_mft_ocu_ocsa_field_t *ocsa_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    stc_mft_ocu_ocsb_field_t *ocsb_Register[MAX_OCU_UNIT_FOR_MFT_UNIT];
    stc_mft_frt_tcsa_field_t  *tsca_Register[MAX_FRT_CHANELL_FOR_UNIT];
    stc_mft_frt_tcsb_field_t  *tscb_Register[MAX_FRT_CHANELL_FOR_UNIT];
    stc_mft_frt_tccp_field_t  *tccp_Register[MAX_FRT_CHANELL_FOR_UNIT];
    stc_mft_frt_tcdt_field_t  *tcdt_Register[MAX_FRT_CHANELL_FOR_UNIT];
    stc_mft_wfg_wfsa_field_t  *wfsa_Regsiter[MAX_WFG_UNIT_FOR_MFT_UNIT];
    stc_mft_wfg_wftm_field_t  *wftm_Register[MAX_WFG_UNIT_FOR_MFT_UNIT];
    stc_mft_wfg_wfir_field_t  *wfir_Register;
    stc_mft_icu_icsa_field_t  *icsa_Register[MAX_ICU_UNIT_FOR_MFT_UNIT];
    stc_mft_icu_icsb_field_t  *icsb_Register[MAX_ICU_UNIT_FOR_MFT_UNIT];
    stc_mft_icu_icfs_field_t  *icfs_Register[MAX_ICU_UNIT_FOR_MFT_UNIT];
    stc_mft_icu_iccp_field_t *iccp0_Register[MAX_ICU_UNIT_FOR_MFT_UNIT];
    stc_mft_icu_iccp_field_t *iccp1_Register[MAX_ICU_UNIT_FOR_MFT_UNIT];

    rtoPinConversion_t RTO_0[NUMBER_OF_OUTPUT_PIN];
    rtoPinConversion_t RTO_1[NUMBER_OF_OUTPUT_PIN];

    /**
    Don't use them for the moment. Rev 01.10 - We will see how to manage it in the future
    icPinConversion_t IC_0[NUMBER_OF_INPUT_PIN];
    icPinConversion_t IC_1[NUMBER_OF_INPUT_PIN];
    */
    uint8_t OCSC_MODE_TRANSLATOR[NUMBER_OF_OUTPUT_PIN];

    void (*IRQ_FRT_callback_p[MAX_FRT_CHANELL_FOR_UNIT])(frtChannel_Type);  //Used for the FRT PEAK interrupt
    void (*IRQ_FRT_NRZ_callback_p[MAX_FRT_CHANELL_FOR_UNIT])(frtChannel_Type);  //Used for the FRT NRZ interrupt
    void (*IRQ_ICU_callback_p[MAX_ICU_UNIT_FOR_MFT_UNIT][MAX_ICU_CHANNEL_FOR_ICU_UNIT])();  //Used for the ICU interrupt

//static void (*IRQ_MFT_ICU_callback_p[MAX_MFT_NUMBER][MAX_ICU_UNIT_FOR_MFT_UNIT][MAX_ICU_CHANNEL_FOR_ICU_UNIT])(icuUnit_Type,icuChannel_Type);  //Used for the ICU interrupt

} stc_mft_unit_t;


class mftUnit{
public:
	mftUnit(uint8_t unit);
	~mftUnit();
	uint8_t getID();
	//TSCA Register of FRT
	uint8_t setTSCARegister(frtChannel_Type, cycleRatio_Type ratioToSet, frtInitRequest_Type request,
                            frtCounterMode_Type counterMode, frtOperMode_Type operMode, frtBufferFunction_Type bufferEnable,
                            frtEnableIntOnMatch_Type peakInt, frtEnableIntOnMatch_Type zeroInt, frtClock_Type clockSource );
	uint8_t startsFrtCounting(frtChannel_Type chan);
	uint8_t stopsFrtCounting(frtChannel_Type chan);
	uint8_t clearFrtCounting(frtChannel_Type chan);
	uint8_t getFrtCounterMode(frtChannel_Type chan);
	uint8_t isFrtPeakInterruptEnabled(frtChannel_Type chan);
	uint8_t isFrtZeroInterruptEnabled(frtChannel_Type chan);
    void clearFRTICLR(frtChannel_Type chan);
    void clearFRTIRQZF(frtChannel_Type chan);
    uint8_t readFRTICLR(frtChannel_Type chan);
    uint8_t readFRTIRQZF(frtChannel_Type chan);


	//TSCB Register of FRT
	uint8_t setFrtADCTrigger(frtChannel_Type chan, frtADCTrigger_Type enableTrig, frtADCOutput_Type adcOutput);
	uint8_t isFrtADCTriggered(frtChannel_Type chan, frtADCOutput_Type adcOutput);
    //TCCP Regsiter of FRT
	uint8_t setFrtPeakRegister(frtChannel_Type chan, uint16_t peakValue);
	uint16_t getFrtPeakRegister(frtChannel_Type chan);
    //TCDT Register of FRT
	uint16_t getFrtCounterOutput(frtChannel_Type chan);
	//Attach interrupt routine
	int attachFrtCallback( void (*_callback_p)(frtChannel_Type chan),frtChannel_Type chan);
	int attachFrtNrzCallback( void (*_callback_p)(frtChannel_Type chan),frtChannel_Type chan);

    //OCU OCFS Register
	uint8_t assignFRTtoOCU(ocuIcuConnectFrt_Type frtChan, ocuUnit_Type ocuUnitSelected ,ocuChannel_Type ocuChan);
	ocuIcuConnectFrt_Type whichFRTisAssignedToOCU(ocuUnit_Type ocuUnitSelected ,ocuChannel_Type ocuChan);
    //OCU OCSA Register
	uint8_t setOcuChannelOper(ocuUnit_Type unit, ocuChannel_Type chan, ocuOperation_Type oper);
	uint8_t clearOCUIOP(ocuUnit_Type unit, ocuChannel_Type chan);
	uint8_t readOCUIOP(ocuUnit_Type unit, ocuChannel_Type chan);
    //OCU OCSB Register
	uint8_t setOcuOutputLevel(ocuUnit_Type unit, ocuChannel_Type chan, ocuRTOutputLevel_Type level);
	uint8_t getOcuOutputLevel(ocuUnit_Type unit, ocuChannel_Type chan);
    //OCU General setting
	uint8_t setOcuRegister(ocuUnit_Type unit,ocuChannel_Type chan, ocuOperation_Type oper, ocuBufferState_Type state,
                            ocuGenerateInt_Type genIntState,ocuBufferTransfer_Type transfer,ocuRTOutputLevel_Type level);
    //OCU OCSB-OCSC regsters
	uint8_t setOcuModeUse(ocuUnit_Type unit, ocuChannel_Type chan, ocuModeUse_Type mode);
	uint8_t setOcuAdmittedModeUse(ocuUnit_Type unit, ocuChannel_Type chan, ocuAdmittedMode_Type mode);
	//OCU OCCP Register
    uint8_t setOcuOCCP(ocuUnit_Type unit, ocuChannel_Type chan, uint16_t value);
    uint16_t getOcuOCCP(ocuUnit_Type unit, ocuChannel_Type chan);
    uint8_t findConfiguredOutputPin(char* pinName, ocuUnit_Type &unit, ocuChannel_Type &chan);

    //WFG WFSA Register
    uint8_t setWfgModeSetting(wfgUnit_Type unit, wfgChannel_Type chan, wfgOperationMode_t mode);
    //WFG WFTM Regsiter
    uint16_t setWfgTimerRegister(wfgUnit_Type unit, uint16_t value);
    uint16_t getWfgTimerRegister(wfgUnit_Type unit);
    //WFG WFIR Register
    uint8_t isDTIFgenerated(wfgUnit_Type unit);
    uint8_t clearDTIF(wfgUnit_Type unit);

    //ICU ICSA Register
    uint8_t clearICUICP(icuUnit_Type unit, icuChannel_Type chan);
    uint8_t readICUICP(icuUnit_Type unit, icuChannel_Type chan);
    //ICU ICFS Register
	uint8_t assignFRTtoICU(ocuIcuConnectFrt_Type frtChan, icuUnit_Type icuUnitSelected ,icuChannel_Type icuChan);
	ocuIcuConnectFrt_Type whichFRTisAssignedToICU(icuUnit_Type icuUnitSelected ,icuChannel_Type icuChan);
	//ICU ICSB Register
	uint8_t getIcuLastFront(icuUnit_Type unit, icuChannel_Type chan);
	//ICU ICCP Regsiter
	uint16_t getIcuICCP(icuUnit_Type unit, icuChannel_Type chan);
    //ICU General setting
	uint8_t setIcuRegister(icuUnit_Type unit,icuChannel_Type chan, icuOperation_Type oper, icuGenerateInt_Type genIntState);
    //Attach int routine
    int attachICUCallback( void (*_callback_p)(),icuUnit_Type icuUnitSelected, icuChannel_Type chan);

    //Structures, arrays and var
	stc_mft_unit_t mftBlock;
    ocuIcuConnectFrt_Type ocuHasFRTConnection[NUMBER_OF_OUTPUT_PIN];
    ocuIcuConnectFrt_Type icuHasFRTConnection[NUMBER_OF_INPUT_PIN];
	uint8_t mftID;
	bool isInstalled;

private:
    //initialization
    uint8_t initFRTRegister();
    void initOCURegsiter();
    void initWFGRegister();
    void initICURegister();
    void assignRTPin();
    void assignICPin();
    //FRT TSCA Register settings
	uint8_t setFrtCycleRatio(frtChannel_Type chan, cycleRatio_Type ratioToSet);
	uint8_t setFrtInitRequest(frtChannel_Type chan, frtInitRequest_Type request );
	uint8_t setFrtCounterMode(frtChannel_Type chan, frtCounterMode_Type mode );
	uint8_t setFrtOperMode(frtChannel_Type chan, frtOperMode_Type mode );
	uint8_t setFrtBufferFunction(frtChannel_Type chan, frtBufferFunction_Type mode );
	uint8_t setFrtIntOnMatch(frtChannel_Type chan, frtEnableIntOnMatch_Type mode );
	uint8_t setFrtIntOnMatch_Z(frtChannel_Type chan, frtEnableIntOnMatch_Type mode );
	uint8_t setFrtClock(frtChannel_Type chan, frtClock_Type mode );
    // OCU OCSA Register
	uint8_t setOcuChannelBufferState(ocuUnit_Type unit, ocuChannel_Type chan, ocuBufferState_Type state);
	uint8_t setOcuChannelGenerateInt(ocuUnit_Type unit, ocuChannel_Type chan, ocuGenerateInt_Type state);
	//OCU OCSB Register
	uint8_t setOcuBufferTransfer(ocuUnit_Type unit, ocuChannel_Type chan, ocuBufferTransfer_Type transfer);
	//OCU OCSB-OCSC Register
    uint8_t setOcuModeRegister(ocuUnit_Type unit, ocuChannel_Type chan, clearedRegister_Type ocsb, clearedRegister_Type ocsc);
    uint8_t setOCSCModeRegister(uint8_t position, uint8_t value);
    //WFG WFSA Register
    uint8_t setWfgCycleRatio(wfgUnit_Type unit, wfgChannel_Type chan, cycleRatio_Type ratioToSet);
    uint8_t setWfgChGateMode(wfgUnit_Type unit, wfgChannel_Type chan, chGateCode_t gateMode);
    uint8_t setWfgPPGtoGateMode(wfgUnit_Type unit, wfgChannel_Type chan, ppcToGateMode_t ppcGateMode);
    uint8_t setWfgPPGReflectWfg(wfgUnit_Type unit, wfgChannel_Type chan,  chPPGOnWFGoutput_t reflectionMode);
    uint8_t setWfgOutputPolarity(wfgUnit_Type unit, wfgChannel_Type chan, wfgOutputPolarity_t polarity);
    //ICU ICSA Register
    uint8_t setIcuChannelGenerateInt(icuUnit_Type unit, icuChannel_Type chan, icuGenerateInt_Type state);
    uint8_t setIcuChannelSelectFrontEnable(icuUnit_Type unit, icuChannel_Type chan, icuOperation_Type operation);
};



#endif //_MULTIFUNCTIONTIMER_H_
