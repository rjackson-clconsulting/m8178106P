/**
@file		mb9bd1xt.h
@brief		header File to add some structure and define for peripheral device of MF3
@author		Fernando Morani
@date		12/12/2016
@version	00.00
@source     M75-CPU Controller
*/

/**
* This File has the same name of the one downloaded from Fujitsu Site.
* This is an extract of that file in order to fill the missing declaration
* in mb9md10t.h file avoiding duplicates.
**/

#ifndef _MB9BD1XT_H_
#define _MB9BD1XT_H_

#define FM3_MFT0_BASE                             (0x40020000UL) /* MFT0 Base Address */
#define FM3_MFT1_BASE                             (0x40021000UL) /* MFT1 Base Address */
#define FM3_MFT2_BASE                             (0x40022000UL) /* MFT2 Base Address */

#define FM3_MFT0                                  ((FM_MFT_TypeDef *)FM3_MFT0_BASE)
#define FM3_MFT1                                  ((FM_MFT_TypeDef *)FM3_MFT1_BASE)
#define FM3_MFT2                                  ((FM_MFT_TypeDef *)FM3_MFT2_BASE)


/******************************************************************************
 ** MFT_MODULE
 **
 ******************************************************************************/
typedef struct stc_mft_ocsa10_field
{
        __IO   uint8_t  CST0                     :1;
        __IO   uint8_t  CST1                     :1;
        __IO   uint8_t  BDIS0                    :1;
        __IO   uint8_t  BDIS1                    :1;
        __IO   uint8_t  IOE0                     :1;
        __IO   uint8_t  IOE1                     :1;
        __IO   uint8_t  IOP0                     :1;
        __IO   uint8_t  IOP1                     :1;
} stc_mft_ocsa10_field_t;

typedef struct stc_mft_ocsb10_field
{
        __IO   uint8_t  OTD0                     :1;
        __IO   uint8_t  OTD1                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS0                     :1;
        __IO   uint8_t  BTS1                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocsb10_field_t;

typedef struct stc_mft_ocsa32_field
{
        __IO   uint8_t  CST2                     :1;
        __IO   uint8_t  CST3                     :1;
        __IO   uint8_t  BDIS2                    :1;
        __IO   uint8_t  BDIS3                    :1;
        __IO   uint8_t  IOE2                     :1;
        __IO   uint8_t  IOE3                     :1;
        __IO   uint8_t  IOP2                     :1;
        __IO   uint8_t  IOP3                     :1;
} stc_mft_ocsa32_field_t;

typedef struct stc_mft_ocsb32_field
{
        __IO   uint8_t  OTD2                     :1;
        __IO   uint8_t  OTD3                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS2                     :1;
        __IO   uint8_t  BTS3                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocsb32_field_t;

typedef struct stc_mft_ocsa54_field
{
        __IO   uint8_t  CST4                     :1;
        __IO   uint8_t  CST5                     :1;
        __IO   uint8_t  BDIS4                    :1;
        __IO   uint8_t  BDIS5                    :1;
        __IO   uint8_t  IOE4                     :1;
        __IO   uint8_t  IOE5                     :1;
        __IO   uint8_t  IOP4                     :1;
        __IO   uint8_t  IOP5                     :1;
} stc_mft_ocsa54_field_t;

typedef struct stc_mft_ocsb54_field
{
        __IO   uint8_t  OTD4                     :1;
        __IO   uint8_t  OTD5                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS4                     :1;
        __IO   uint8_t  BTS5                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocsb54_field_t;

typedef struct stc_mft_ocsc_field
{
        __IO   uint8_t  MOD0                     :1;
        __IO   uint8_t  MOD1                     :1;
        __IO   uint8_t  MOD2                     :1;
        __IO   uint8_t  MOD3                     :1;
        __IO   uint8_t  MOD4                     :1;
        __IO   uint8_t  MOD5                     :1;
        __IO   uint8_t  RESERVED0                :2;
} stc_mft_ocsc_field_t;

typedef struct stc_mft_tccp0_field
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
} stc_mft_tccp0_field_t;

typedef struct stc_mft_tcsa0_field
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
    };
} stc_mft_tcsa0_field_t;

typedef struct stc_mft_tcsb0_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_tcsb0_field_t;

typedef struct stc_mft_tccp1_field
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
} stc_mft_tccp1_field_t;

typedef struct stc_mft_tcsa1_field
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
    };
} stc_mft_tcsa1_field_t;

typedef struct stc_mft_tcsb1_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_tcsb1_field_t;

typedef struct stc_mft_tccp2_field
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
} stc_mft_tccp2_field_t;

typedef struct stc_mft_tcsa2_field
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
    };
} stc_mft_tcsa2_field_t;

typedef struct stc_mft_tcsb2_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_tcsb2_field_t;

typedef struct stc_mft_ocfs10_field
{
    union {
        struct {
            __IO   uint8_t  FSO0                 :4;
            __IO   uint8_t  FSO1                 :4;
        };
        struct {
            __IO   uint8_t  FSO00                :1;
            __IO   uint8_t  FSO01                :1;
            __IO   uint8_t  FSO02                :1;
            __IO   uint8_t  FSO03                :1;
            __IO   uint8_t  FSO10                :1;
            __IO   uint8_t  FSO11                :1;
            __IO   uint8_t  FSO12                :1;
            __IO   uint8_t  FSO13                :1;
        };
    };
} stc_mft_ocfs10_field_t;

typedef struct stc_mft_ocfs32_field
{
    union {
        struct {
            __IO   uint8_t  FSO2                 :4;
            __IO   uint8_t  FSO3                 :4;
        };
        struct {
            __IO   uint8_t  FSO20                :1;
            __IO   uint8_t  FSO21                :1;
            __IO   uint8_t  FSO22                :1;
            __IO   uint8_t  FSO23                :1;
            __IO   uint8_t  FSO30                :1;
            __IO   uint8_t  FSO31                :1;
            __IO   uint8_t  FSO32                :1;
            __IO   uint8_t  FSO33                :1;
        };
    };
} stc_mft_ocfs32_field_t;

typedef struct stc_mft_ocfs54_field
{
    union {
        struct {
            __IO   uint8_t  FSO4                 :4;
            __IO   uint8_t  FSO5                 :4;
        };
        struct {
            __IO   uint8_t  FSO40                :1;
            __IO   uint8_t  FSO41                :1;
            __IO   uint8_t  FSO42                :1;
            __IO   uint8_t  FSO43                :1;
            __IO   uint8_t  FSO50                :1;
            __IO   uint8_t  FSO51                :1;
            __IO   uint8_t  FSO52                :1;
            __IO   uint8_t  FSO53                :1;
        };
    };
} stc_mft_ocfs54_field_t;

typedef struct stc_mft_icfs10_field
{
    union {
        struct {
            __IO   uint8_t  FSI0                 :4;
            __IO   uint8_t  FSI1                 :4;
        };
        struct {
            __IO   uint8_t  FSI00                :1;
            __IO   uint8_t  FSI01                :1;
            __IO   uint8_t  FSI02                :1;
            __IO   uint8_t  FSI03                :1;
            __IO   uint8_t  FSI10                :1;
            __IO   uint8_t  FSI11                :1;
            __IO   uint8_t  FSI12                :1;
            __IO   uint8_t  FSI13                :1;
        };
    };
} stc_mft_icfs10_field_t;

typedef struct stc_mft_icfs32_field
{
    union {
        struct {
            __IO   uint8_t  FSI2                 :4;
            __IO   uint8_t  FSI3                 :4;
        };
        struct {
            __IO   uint8_t  FSI20                :1;
            __IO   uint8_t  FSI21                :1;
            __IO   uint8_t  FSI22                :1;
            __IO   uint8_t  FSI23                :1;
            __IO   uint8_t  FSI30                :1;
            __IO   uint8_t  FSI31                :1;
            __IO   uint8_t  FSI32                :1;
            __IO   uint8_t  FSI33                :1;
        };
    };
} stc_mft_icfs32_field_t;

typedef struct stc_mft_icsa10_field
{
    union {
        struct {
            __IO   uint8_t  EG0                  :2;
            __IO   uint8_t  EG1                  :2;
            __IO   uint8_t  ICE0                 :1;
            __IO   uint8_t  ICE1                 :1;
            __IO   uint8_t  ICP0                 :1;
            __IO   uint8_t  ICP1                 :1;
        };
        struct {
            __IO   uint8_t  EG00                 :1;
            __IO   uint8_t  EG01                 :1;
            __IO   uint8_t  EG10                 :1;
            __IO   uint8_t  EG11                 :1;
            __IO   uint8_t  RESERVED0            :4;
        };
    };
} stc_mft_icsa10_field_t;

typedef struct stc_mft_icsb10_field
{
        __IO   uint8_t  IEI0                     :1;
        __IO   uint8_t  IEI1                     :1;
        __IO   uint8_t  RESERVED0                :6;
} stc_mft_icsb10_field_t;

typedef struct stc_mft_icsa32_field
{
    union {
        struct {
            __IO   uint8_t  EG2                  :2;
            __IO   uint8_t  EG3                  :2;
            __IO   uint8_t  ICE2                 :1;
            __IO   uint8_t  ICE3                 :1;
            __IO   uint8_t  ICP2                 :1;
            __IO   uint8_t  ICP3                 :1;
        };
        struct {
            __IO   uint8_t  EG20                 :1;
            __IO   uint8_t  EG21                 :1;
            __IO   uint8_t  EG30                 :1;
            __IO   uint8_t  EG31                 :1;
            __IO   uint8_t  RESERVED0            :4;
        };
    };
} stc_mft_icsa32_field_t;

typedef struct stc_mft_icsb32_field
{
        __IO   uint8_t  IEI2                     :1;
        __IO   uint8_t  IEI3                     :1;
        __IO   uint8_t  RESERVED0                :6;
} stc_mft_icsb32_field_t;

typedef struct stc_mft_wfsa10_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfsa10_field_t;

typedef struct stc_mft_wfsa32_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfsa32_field_t;

typedef struct stc_mft_wfsa54_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfsa54_field_t;

typedef struct stc_mft_wfir_field
{
        __IO  uint16_t  DTIFA                    :1;
        __IO  uint16_t  DTICA                    :1;
        __IO  uint16_t  RESERVED0                :2;
        __IO  uint16_t  TMIF10                   :1;
        __IO  uint16_t  TMIC10                   :1;
        __IO  uint16_t  TMIE10                   :1;
        __IO  uint16_t  TMIS10                   :1;
        __IO  uint16_t  TMIF32                   :1;
        __IO  uint16_t  TMIC32                   :1;
        __IO  uint16_t  TMIE32                   :1;
        __IO  uint16_t  TMIS32                   :1;
        __IO  uint16_t  TMIF54                   :1;
        __IO  uint16_t  TMIC54                   :1;
        __IO  uint16_t  TMIE54                   :1;
        __IO  uint16_t  TMIS54                   :1;
} stc_mft_wfir_field_t;

typedef struct stc_mft_nzcl_field
{
    union {
        struct {
            __IO  uint16_t  DTIEA                :1;
            __IO  uint16_t  NWS                  :3;
            __IO  uint16_t  SDTI                 :1;
            __IO  uint16_t  RESERVED1            :11;
        };
        struct {
            __IO  uint16_t  RESERVED0            :1;
            __IO  uint16_t  NWS0                 :1;
            __IO  uint16_t  NWS1                 :1;
            __IO  uint16_t  NWS2                 :1;
            __IO  uint16_t  RESERVED2            :12;
        };
    };
} stc_mft_nzcl_field_t;

typedef struct stc_mft_acsb_field
{
        __IO   uint8_t  BDIS0                    :1;
        __IO   uint8_t  BDIS1                    :1;
        __IO   uint8_t  BDIS2                    :1;
        __IO   uint8_t  RESERVED0                :1;
        __IO   uint8_t  BTS0                     :1;
        __IO   uint8_t  BTS1                     :1;
        __IO   uint8_t  BTS2                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_acsb_field_t;

typedef struct stc_mft_acsa_field
{
    union {
        struct {
            __IO  uint16_t  CE0                  :2;
            __IO  uint16_t  CE1                  :2;
            __IO  uint16_t  CE2                  :2;
            __IO  uint16_t  RESERVED0            :2;
            __IO  uint16_t  SEL0                 :2;
            __IO  uint16_t  SEL1                 :2;
            __IO  uint16_t  SEL2                 :2;
            __IO  uint16_t  RESERVED2            :2;
        };
        struct {
            __IO  uint16_t  CE00                 :1;
            __IO  uint16_t  CE01                 :1;
            __IO  uint16_t  CE10                 :1;
            __IO  uint16_t  CE11                 :1;
            __IO  uint16_t  CE20                 :1;
            __IO  uint16_t  CE21                 :1;
            __IO  uint16_t  RESERVED1            :2;
            __IO  uint16_t  SEL00                :1;
            __IO  uint16_t  SEL01                :1;
            __IO  uint16_t  SEL10                :1;
            __IO  uint16_t  SEL11                :1;
            __IO  uint16_t  SEL20                :1;
            __IO  uint16_t  SEL21                :1;
            __IO  uint16_t  RESERVED3            :2;
        };
    };
} stc_mft_acsa_field_t;

typedef struct stc_mft_atsa_field
{
    union {
        struct {
            __IO  uint16_t  AD0S                 :2;
            __IO  uint16_t  AD1S                 :2;
            __IO  uint16_t  AD2S                 :2;
            __IO  uint16_t  RESERVED0            :2;
            __IO  uint16_t  AD0P                 :2;
            __IO  uint16_t  AD1P                 :2;
            __IO  uint16_t  AD2P                 :2;
            __IO  uint16_t  RESERVED2            :2;
        };
        struct {
            __IO  uint16_t  AD0S0                :1;
            __IO  uint16_t  AD0S1                :1;
            __IO  uint16_t  AD1S0                :1;
            __IO  uint16_t  AD1S1                :1;
            __IO  uint16_t  AD2S0                :1;
            __IO  uint16_t  AD2S1                :1;
            __IO  uint16_t  RESERVED1            :2;
            __IO  uint16_t  AD0P0                :1;
            __IO  uint16_t  AD0P1                :1;
            __IO  uint16_t  AD1P0                :1;
            __IO  uint16_t  AD1P1                :1;
            __IO  uint16_t  AD2P0                :1;
            __IO  uint16_t  AD2P1                :1;
            __IO  uint16_t  RESERVED3            :2;
        };
    };
} stc_mft_atsa_field_t;

typedef struct stc_mft_ocu_ocsa10_field
{
        __IO   uint8_t  CST0                     :1;
        __IO   uint8_t  CST1                     :1;
        __IO   uint8_t  BDIS0                    :1;
        __IO   uint8_t  BDIS1                    :1;
        __IO   uint8_t  IOE0                     :1;
        __IO   uint8_t  IOE1                     :1;
        __IO   uint8_t  IOP0                     :1;
        __IO   uint8_t  IOP1                     :1;
} stc_mft_ocu_ocsa10_field_t;

typedef struct stc_mft_ocu_ocsb10_field
{
        __IO   uint8_t  OTD0                     :1;
        __IO   uint8_t  OTD1                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS0                     :1;
        __IO   uint8_t  BTS1                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocu_ocsb10_field_t;

typedef struct stc_mft_ocu_ocsa32_field
{
        __IO   uint8_t  CST2                     :1;
        __IO   uint8_t  CST3                     :1;
        __IO   uint8_t  BDIS2                    :1;
        __IO   uint8_t  BDIS3                    :1;
        __IO   uint8_t  IOE2                     :1;
        __IO   uint8_t  IOE3                     :1;
        __IO   uint8_t  IOP2                     :1;
        __IO   uint8_t  IOP3                     :1;
} stc_mft_ocu_ocsa32_field_t;

typedef struct stc_mft_ocu_ocsb32_field
{
        __IO   uint8_t  OTD2                     :1;
        __IO   uint8_t  OTD3                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS2                     :1;
        __IO   uint8_t  BTS3                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocu_ocsb32_field_t;

typedef struct stc_mft_ocu_ocsa54_field
{
        __IO   uint8_t  CST4                     :1;
        __IO   uint8_t  CST5                     :1;
        __IO   uint8_t  BDIS4                    :1;
        __IO   uint8_t  BDIS5                    :1;
        __IO   uint8_t  IOE4                     :1;
        __IO   uint8_t  IOE5                     :1;
        __IO   uint8_t  IOP4                     :1;
        __IO   uint8_t  IOP5                     :1;
} stc_mft_ocu_ocsa54_field_t;

typedef struct stc_mft_ocu_ocsb54_field
{
        __IO   uint8_t  OTD4                     :1;
        __IO   uint8_t  OTD5                     :1;
        __IO   uint8_t  RESERVED0                :2;
        __IO   uint8_t  CMOD                     :1;
        __IO   uint8_t  BTS4                     :1;
        __IO   uint8_t  BTS5                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_ocu_ocsb54_field_t;

typedef struct stc_mft_ocu_ocsc_field
{
 union {
  __IO uint8_t ALL_BITS;
        struct{
  __IO  uint8_t MOD0       : 1;
  __IO  uint8_t MOD1       : 1;
  __IO  uint8_t MOD2       : 1;
  __IO  uint8_t MOD3       : 1;
  __IO  uint8_t MOD4       : 1;
  __IO  uint8_t MOD5       : 1;
  };
  };
} stc_mft_ocu_ocsc_field_t;


typedef struct stc_mft_frt_tccp0_field
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
} stc_mft_frt_tccp0_field_t;

typedef struct stc_mft_frt_tcsa0_field
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
    };
} stc_mft_frt_tcsa0_field_t;

typedef struct stc_mft_frt_tcsb0_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_frt_tcsb0_field_t;

typedef struct stc_mft_frt_tccp1_field
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
} stc_mft_frt_tccp1_field_t;

typedef struct stc_mft_frt_tcsa1_field
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
    };
} stc_mft_frt_tcsa1_field_t;

typedef struct stc_mft_frt_tcsb1_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_frt_tcsb1_field_t;

typedef struct stc_mft_frt_tccp2_field
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
} stc_mft_frt_tccp2_field_t;

typedef struct stc_mft_frt_tcsa2_field
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
    };
} stc_mft_frt_tcsa2_field_t;

typedef struct stc_mft_frt_tcsb2_field
{
        __IO  uint16_t  AD0E                     :1;
        __IO  uint16_t  AD1E                     :1;
        __IO  uint16_t  AD2E                     :1;
        __IO  uint16_t  RESERVED0                :13;
} stc_mft_frt_tcsb2_field_t;

typedef struct stc_mft_ocu_ocfs10_field
{
    union {
        struct {
            __IO   uint8_t  FSO0                 :4;
            __IO   uint8_t  FSO1                 :4;
        };
        struct {
            __IO   uint8_t  FSO00                :1;
            __IO   uint8_t  FSO01                :1;
            __IO   uint8_t  FSO02                :1;
            __IO   uint8_t  FSO03                :1;
            __IO   uint8_t  FSO10                :1;
            __IO   uint8_t  FSO11                :1;
            __IO   uint8_t  FSO12                :1;
            __IO   uint8_t  FSO13                :1;
        };
    };
} stc_mft_ocu_ocfs10_field_t;

typedef struct stc_mft_ocu_ocfs32_field
{
    union {
        struct {
            __IO   uint8_t  FSO2                 :4;
            __IO   uint8_t  FSO3                 :4;
        };
        struct {
            __IO   uint8_t  FSO20                :1;
            __IO   uint8_t  FSO21                :1;
            __IO   uint8_t  FSO22                :1;
            __IO   uint8_t  FSO23                :1;
            __IO   uint8_t  FSO30                :1;
            __IO   uint8_t  FSO31                :1;
            __IO   uint8_t  FSO32                :1;
            __IO   uint8_t  FSO33                :1;
        };
    };
} stc_mft_ocu_ocfs32_field_t;

typedef struct stc_mft_ocu_ocfs54_field
{
    union {
        struct {
            __IO   uint8_t  FSO4                 :4;
            __IO   uint8_t  FSO5                 :4;
        };
        struct {
            __IO   uint8_t  FSO40                :1;
            __IO   uint8_t  FSO41                :1;
            __IO   uint8_t  FSO42                :1;
            __IO   uint8_t  FSO43                :1;
            __IO   uint8_t  FSO50                :1;
            __IO   uint8_t  FSO51                :1;
            __IO   uint8_t  FSO52                :1;
            __IO   uint8_t  FSO53                :1;
        };
    };
} stc_mft_ocu_ocfs54_field_t;

typedef struct stc_mft_icu_icfs10_field
{
    union {
        struct {
            __IO   uint8_t  FSI0                 :4;
            __IO   uint8_t  FSI1                 :4;
        };
        struct {
            __IO   uint8_t  FSI00                :1;
            __IO   uint8_t  FSI01                :1;
            __IO   uint8_t  FSI02                :1;
            __IO   uint8_t  FSI03                :1;
            __IO   uint8_t  FSI10                :1;
            __IO   uint8_t  FSI11                :1;
            __IO   uint8_t  FSI12                :1;
            __IO   uint8_t  FSI13                :1;
        };
    };
} stc_mft_icu_icfs10_field_t;

typedef struct stc_mft_icu_icfs32_field
{
    union {
        struct {
            __IO   uint8_t  FSI2                 :4;
            __IO   uint8_t  FSI3                 :4;
        };
        struct {
            __IO   uint8_t  FSI20                :1;
            __IO   uint8_t  FSI21                :1;
            __IO   uint8_t  FSI22                :1;
            __IO   uint8_t  FSI23                :1;
            __IO   uint8_t  FSI30                :1;
            __IO   uint8_t  FSI31                :1;
            __IO   uint8_t  FSI32                :1;
            __IO   uint8_t  FSI33                :1;
        };
    };
} stc_mft_icu_icfs32_field_t;

typedef struct stc_mft_icu_icsa10_field
{
    union {
        struct {
            __IO   uint8_t  EG0                  :2;
            __IO   uint8_t  EG1                  :2;
            __IO   uint8_t  ICE0                 :1;
            __IO   uint8_t  ICE1                 :1;
            __IO   uint8_t  ICP0                 :1;
            __IO   uint8_t  ICP1                 :1;
        };
        struct {
            __IO   uint8_t  EG00                 :1;
            __IO   uint8_t  EG01                 :1;
            __IO   uint8_t  EG10                 :1;
            __IO   uint8_t  EG11                 :1;
            __IO   uint8_t  RESERVED0            :4;
        };
    };
} stc_mft_icu_icsa10_field_t;

typedef struct stc_mft_icu_icsb10_field
{
        __IO   uint8_t  IEI0                     :1;
        __IO   uint8_t  IEI1                     :1;
        __IO   uint8_t  RESERVED0                :6;
} stc_mft_icu_icsb10_field_t;

typedef struct stc_mft_icu_icsa32_field
{
    union {
        struct {
            __IO   uint8_t  EG2                  :2;
            __IO   uint8_t  EG3                  :2;
            __IO   uint8_t  ICE2                 :1;
            __IO   uint8_t  ICE3                 :1;
            __IO   uint8_t  ICP2                 :1;
            __IO   uint8_t  ICP3                 :1;
        };
        struct {
            __IO   uint8_t  EG20                 :1;
            __IO   uint8_t  EG21                 :1;
            __IO   uint8_t  EG30                 :1;
            __IO   uint8_t  EG31                 :1;
            __IO   uint8_t  RESERVED0            :4;
        };
    };
} stc_mft_icu_icsa32_field_t;

typedef struct stc_mft_icu_icsb32_field
{
        __IO   uint8_t  IEI2                     :1;
        __IO   uint8_t  IEI3                     :1;
        __IO   uint8_t  RESERVED0                :6;
} stc_mft_icu_icsb32_field_t;

typedef struct stc_mft_wfg_wfsa10_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfg_wfsa10_field_t;

typedef struct stc_mft_wfg_wfsa32_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfg_wfsa32_field_t;

typedef struct stc_mft_wfg_wfsa54_field
{
    union {
        struct {
            __IO  uint16_t  DCK                  :3;
            __IO  uint16_t  TMD                  :3;
            __IO  uint16_t  GTEN                 :2;
            __IO  uint16_t  PSEL                 :2;
            __IO  uint16_t  PGEN                 :2;
            __IO  uint16_t  DMOD                 :1;
            __IO  uint16_t  RESERVED0            :3;
        };
        struct {
            __IO  uint16_t  DCK0                 :1;
            __IO  uint16_t  DCK1                 :1;
            __IO  uint16_t  DCK2                 :1;
            __IO  uint16_t  TMD0                 :1;
            __IO  uint16_t  TMD1                 :1;
            __IO  uint16_t  TMD2                 :1;
            __IO  uint16_t  GTEN0                :1;
            __IO  uint16_t  GTEN1                :1;
            __IO  uint16_t  PSEL0                :1;
            __IO  uint16_t  PSEL1                :1;
            __IO  uint16_t  PGEN0                :1;
            __IO  uint16_t  PGEN1                :1;
            __IO  uint16_t  RESERVED1            :4;
        };
    };
} stc_mft_wfg_wfsa54_field_t;

typedef struct stc_mft_wfg_wfir_field
{
union {
    struct {
  __IO uint16_t DTIF       : 1;
  __IO uint16_t DTIC       : 1;
       uint16_t RESERVED1  : 2;
  __IO uint16_t TMIF10     : 1;
  __IO uint16_t TMIC10     : 1;
  __IO uint16_t TMIE10     : 1;
  __IO uint16_t TMIS10     : 1;
  __IO uint16_t TMIF32     : 1;
  __IO uint16_t TMIC32     : 1;
  __IO uint16_t TMIE32     : 1;
  __IO uint16_t TMIS32     : 1;
  __IO uint16_t TMIF54     : 1;
  __IO uint16_t TMIC54     : 1;
  __IO uint16_t TMIE54     : 1;
  __IO uint16_t TMIS54     : 1;
} stc_mft_bit_access_t;
__IO  uint16_t REGISTER;
};
} stc_mft_wfg_wfir_field_t;


typedef struct stc_mft_wfg_nzcl_field
{
    union {
        struct {
            __IO  uint16_t  DTIEA                :1;
            __IO  uint16_t  NWS                  :3;
            __IO  uint16_t  SDTI                 :1;
            __IO  uint16_t  RESERVED1            :11;
        };
        struct {
            __IO  uint16_t  RESERVED0            :1;
            __IO  uint16_t  NWS0                 :1;
            __IO  uint16_t  NWS1                 :1;
            __IO  uint16_t  NWS2                 :1;
            __IO  uint16_t  RESERVED2            :12;
        };
    };
} stc_mft_wfg_nzcl_field_t;

typedef struct stc_mft_adcmp_acsb_field
{
        __IO   uint8_t  BDIS0                    :1;
        __IO   uint8_t  BDIS1                    :1;
        __IO   uint8_t  BDIS2                    :1;
        __IO   uint8_t  RESERVED0                :1;
        __IO   uint8_t  BTS0                     :1;
        __IO   uint8_t  BTS1                     :1;
        __IO   uint8_t  BTS2                     :1;
        __IO   uint8_t  RESERVED1                :1;
} stc_mft_adcmp_acsb_field_t;

typedef struct stc_mft_adcmp_acsa_field
{
    union {
        struct {
            __IO  uint16_t  CE0                  :2;
            __IO  uint16_t  CE1                  :2;
            __IO  uint16_t  CE2                  :2;
            __IO  uint16_t  RESERVED0            :2;
            __IO  uint16_t  SEL0                 :2;
            __IO  uint16_t  SEL1                 :2;
            __IO  uint16_t  SEL2                 :2;
            __IO  uint16_t  RESERVED2            :2;
        };
        struct {
            __IO  uint16_t  CE00                 :1;
            __IO  uint16_t  CE01                 :1;
            __IO  uint16_t  CE10                 :1;
            __IO  uint16_t  CE11                 :1;
            __IO  uint16_t  CE20                 :1;
            __IO  uint16_t  CE21                 :1;
            __IO  uint16_t  RESERVED1            :2;
            __IO  uint16_t  SEL00                :1;
            __IO  uint16_t  SEL01                :1;
            __IO  uint16_t  SEL10                :1;
            __IO  uint16_t  SEL11                :1;
            __IO  uint16_t  SEL20                :1;
            __IO  uint16_t  SEL21                :1;
            __IO  uint16_t  RESERVED3            :2;
        };
    };
} stc_mft_adcmp_acsa_field_t;

typedef struct stc_mft_adcmp_atsa_field
{
    union {
        struct {
            __IO  uint16_t  AD0S                 :2;
            __IO  uint16_t  AD1S                 :2;
            __IO  uint16_t  AD2S                 :2;
            __IO  uint16_t  RESERVED0            :2;
            __IO  uint16_t  AD0P                 :2;
            __IO  uint16_t  AD1P                 :2;
            __IO  uint16_t  AD2P                 :2;
            __IO  uint16_t  RESERVED2            :2;
        };
        struct {
            __IO  uint16_t  AD0S0                :1;
            __IO  uint16_t  AD0S1                :1;
            __IO  uint16_t  AD1S0                :1;
            __IO  uint16_t  AD1S1                :1;
            __IO  uint16_t  AD2S0                :1;
            __IO  uint16_t  AD2S1                :1;
            __IO  uint16_t  RESERVED1            :2;
            __IO  uint16_t  AD0P0                :1;
            __IO  uint16_t  AD0P1                :1;
            __IO  uint16_t  AD1P0                :1;
            __IO  uint16_t  AD1P1                :1;
            __IO  uint16_t  AD2P0                :1;
            __IO  uint16_t  AD2P1                :1;
            __IO  uint16_t  RESERVED3            :2;
        };
    };
} stc_mft_adcmp_atsa_field_t;

/******************************************************************************
 ** MFT_MODULE
 **
 **   register structur
 ******************************************************************************/
typedef struct
{
    union {
        __IO uint16_t OCCP0;
        struct {
            __IO  uint8_t OCCP0L;
            __IO  uint8_t OCCP0H;
        };
        __IO uint16_t OCU_OCCP0;
        struct {
            __IO  uint8_t OCU_OCCP0L;
            __IO  uint8_t OCU_OCCP0H;
        };
    };
        __IO  uint8_t RESERVED0[2];
    union {
        __IO uint16_t OCCP1;
        struct {
            __IO  uint8_t OCCP1L;
            __IO  uint8_t OCCP1H;
        };
        __IO uint16_t OCU_OCCP1;
        struct {
            __IO  uint8_t OCU_OCCP1L;
            __IO  uint8_t OCU_OCCP1H;
        };
    };
        __IO  uint8_t RESERVED1[2];
    union {
        __IO uint16_t OCCP2;
        struct {
            __IO  uint8_t OCCP2L;
            __IO  uint8_t OCCP2H;
        };
        __IO uint16_t OCU_OCCP2;
        struct {
            __IO  uint8_t OCU_OCCP2L;
            __IO  uint8_t OCU_OCCP2H;
        };
    };
        __IO  uint8_t RESERVED2[2];
    union {
        __IO uint16_t OCCP3;
        struct {
            __IO  uint8_t OCCP3L;
            __IO  uint8_t OCCP3H;
        };
        __IO uint16_t OCU_OCCP3;
        struct {
            __IO  uint8_t OCU_OCCP3L;
            __IO  uint8_t OCU_OCCP3H;
        };
    };
        __IO  uint8_t RESERVED3[2];
    union {
        __IO uint16_t OCCP4;
        struct {
            __IO  uint8_t OCCP4L;
            __IO  uint8_t OCCP4H;
        };
        __IO uint16_t OCU_OCCP4;
        struct {
            __IO  uint8_t OCU_OCCP4L;
            __IO  uint8_t OCU_OCCP4H;
        };
    };
        __IO  uint8_t RESERVED4[2];
    union {
        __IO uint16_t OCCP5;
        struct {
            __IO  uint8_t OCCP5L;
            __IO  uint8_t OCCP5H;
        };
        __IO uint16_t OCU_OCCP5;
        struct {
            __IO  uint8_t OCU_OCCP5L;
            __IO  uint8_t OCU_OCCP5H;
        };
    };
        __IO  uint8_t RESERVED5[2];
    union {
        __IO  uint8_t OCSA10;
        stc_mft_ocsa10_field_t OCSA10_f;
        __IO  uint8_t OCU_OCSA10;
        stc_mft_ocu_ocsa10_field_t OCU_OCSA10_f;
    };
    union {
        __IO  uint8_t OCSB10;
        stc_mft_ocsb10_field_t OCSB10_f;
        __IO  uint8_t OCU_OCSB10;
        stc_mft_ocu_ocsb10_field_t OCU_OCSB10_f;
    };
        __IO  uint8_t RESERVED6[2];
    union {
        __IO  uint8_t OCSA32;
        stc_mft_ocsa32_field_t OCSA32_f;
        __IO  uint8_t OCU_OCSA32;
        stc_mft_ocu_ocsa32_field_t OCU_OCSA32_f;
    };
    union {
        __IO  uint8_t OCSB32;
        stc_mft_ocsb32_field_t OCSB32_f;
        __IO  uint8_t OCU_OCSB32;
        stc_mft_ocu_ocsb32_field_t OCU_OCSB32_f;
    };
        __IO  uint8_t RESERVED7[2];
    union {
        __IO  uint8_t OCSA54;
        stc_mft_ocsa54_field_t OCSA54_f;
        __IO  uint8_t OCU_OCSA54;
        stc_mft_ocu_ocsa54_field_t OCU_OCSA54_f;
    };
    union {
        __IO  uint8_t OCSB54;
        stc_mft_ocsb54_field_t OCSB54_f;
        __IO  uint8_t OCU_OCSB54;
        stc_mft_ocu_ocsb54_field_t OCU_OCSB54_f;
    };
        __IO  uint8_t RESERVED8[3];
    union {
        __IO  uint8_t OCSC;
        stc_mft_ocsc_field_t OCSC_f;
        __IO  uint8_t OCU_OCSC;
        stc_mft_ocu_ocsc_field_t OCU_OCSC_f;
    };
        __IO  uint8_t RESERVED9[2];
    union {
        __IO uint16_t TCCP0;
        stc_mft_tccp0_field_t TCCP0_f;
        struct {
            __IO  uint8_t TCCP0L;
            __IO  uint8_t TCCP0H;
        };
        __IO uint16_t FRT_TCCP0;
        stc_mft_frt_tccp0_field_t FRT_TCCP0_f;
        struct {
            __IO  uint8_t FRT_TCCP0L;
            __IO  uint8_t FRT_TCCP0H;
        };
    };
        __IO  uint8_t RESERVED10[2];
    union {
        __IO uint16_t TCDT0;
        struct {
            __IO  uint8_t TCDT0L;
            __IO  uint8_t TCDT0H;
        };
        __IO uint16_t FRT_TCDT0;
        struct {
            __IO  uint8_t FRT_TCDT0L;
            __IO  uint8_t FRT_TCDT0H;
        };
    };
        __IO  uint8_t RESERVED11[2];
    union {
        __IO uint16_t TCSA0;
        stc_mft_tcsa0_field_t TCSA0_f;
        struct {
            __IO  uint8_t TCSA0L;
            __IO  uint8_t TCSA0H;
        };
        __IO uint16_t FRT_TCSA0;
        stc_mft_frt_tcsa0_field_t FRT_TCSA0_f;
        struct {
            __IO  uint8_t FRT_TCSA0L;
            __IO  uint8_t FRT_TCSA0H;
        };
    };
        __IO  uint8_t RESERVED12[2];
    union {
        __IO uint16_t TCSB0;
        stc_mft_tcsb0_field_t TCSB0_f;
        struct {
            __IO  uint8_t TCSB0L;
            __IO  uint8_t TCSB0H;
        };
        __IO uint16_t FRT_TCSB0;
        stc_mft_frt_tcsb0_field_t FRT_TCSB0_f;
        struct {
            __IO  uint8_t FRT_TCSB0L;
            __IO  uint8_t FRT_TCSB0H;
        };
    };
        __IO  uint8_t RESERVED13[2];
    union {
        __IO uint16_t TCCP1;
        stc_mft_tccp1_field_t TCCP1_f;
        struct {
            __IO  uint8_t TCCP1L;
            __IO  uint8_t TCCP1H;
        };
        __IO uint16_t FRT_TCCP1;
        stc_mft_frt_tccp1_field_t FRT_TCCP1_f;
        struct {
            __IO  uint8_t FRT_TCCP1L;
            __IO  uint8_t FRT_TCCP1H;
        };
    };
        __IO  uint8_t RESERVED14[2];
    union {
        __IO uint16_t TCDT1;
        struct {
            __IO  uint8_t TCDT1L;
            __IO  uint8_t TCDT1H;
        };
        __IO uint16_t FRT_TCDT1;
        struct {
            __IO  uint8_t FRT_TCDT1L;
            __IO  uint8_t FRT_TCDT1H;
        };
    };
        __IO  uint8_t RESERVED15[2];
    union {
        __IO uint16_t TCSA1;
        stc_mft_tcsa1_field_t TCSA1_f;
        struct {
            __IO  uint8_t TCSA1L;
            __IO  uint8_t TCSA1H;
        };
        __IO uint16_t FRT_TCSA1;
        stc_mft_frt_tcsa1_field_t FRT_TCSA1_f;
        struct {
            __IO  uint8_t FRT_TCSA1L;
            __IO  uint8_t FRT_TCSA1H;
        };
    };
        __IO  uint8_t RESERVED16[2];
    union {
        __IO uint16_t TCSB1;
        stc_mft_tcsb1_field_t TCSB1_f;
        struct {
            __IO  uint8_t TCSB1L;
            __IO  uint8_t TCSB1H;
        };
        __IO uint16_t FRT_TCSB1;
        stc_mft_frt_tcsb1_field_t FRT_TCSB1_f;
        struct {
            __IO  uint8_t FRT_TCSB1L;
            __IO  uint8_t FRT_TCSB1H;
        };
    };
        __IO  uint8_t RESERVED17[2];
    union {
        __IO uint16_t TCCP2;
        stc_mft_tccp2_field_t TCCP2_f;
        struct {
            __IO  uint8_t TCCP2L;
            __IO  uint8_t TCCP2H;
        };
        __IO uint16_t FRT_TCCP2;
        stc_mft_frt_tccp2_field_t FRT_TCCP2_f;
        struct {
            __IO  uint8_t FRT_TCCP2L;
            __IO  uint8_t FRT_TCCP2H;
        };
    };
        __IO  uint8_t RESERVED18[2];
    union {
        __IO uint16_t TCDT2;
        struct {
            __IO  uint8_t TCDT2L;
            __IO  uint8_t TCDT2H;
        };
        __IO uint16_t FRT_TCDT2;
        struct {
            __IO  uint8_t FRT_TCDT2L;
            __IO  uint8_t FRT_TCDT2H;
        };
    };
        __IO  uint8_t RESERVED19[2];
    union {
        __IO uint16_t TCSA2;
        stc_mft_tcsa2_field_t TCSA2_f;
        struct {
            __IO  uint8_t TCSA2L;
            __IO  uint8_t TCSA2H;
        };
        __IO uint16_t FRT_TCSA2;
        stc_mft_frt_tcsa2_field_t FRT_TCSA2_f;
        struct {
            __IO  uint8_t FRT_TCSA2L;
            __IO  uint8_t FRT_TCSA2H;
        };
    };
        __IO  uint8_t RESERVED20[2];
    union {
        __IO uint16_t TCSB2;
        stc_mft_tcsb2_field_t TCSB2_f;
        struct {
            __IO  uint8_t TCSB2L;
            __IO  uint8_t TCSB2H;
        };
        __IO uint16_t FRT_TCSB2;
        stc_mft_frt_tcsb2_field_t FRT_TCSB2_f;
        struct {
            __IO  uint8_t FRT_TCSB2L;
            __IO  uint8_t FRT_TCSB2H;
        };
    };
        __IO  uint8_t RESERVED21[2];
    union {
        __IO  uint8_t OCFS10;
        stc_mft_ocfs10_field_t OCFS10_f;
        __IO  uint8_t OCU_OCFS10;
        stc_mft_ocu_ocfs10_field_t OCU_OCFS10_f;
    };
    union {
        __IO  uint8_t OCFS32;
        stc_mft_ocfs32_field_t OCFS32_f;
        __IO  uint8_t OCU_OCFS32;
        stc_mft_ocu_ocfs32_field_t OCU_OCFS32_f;
    };
        __IO  uint8_t RESERVED22[2];
    union {
        __IO  uint8_t OCFS54;
        stc_mft_ocfs54_field_t OCFS54_f;
        __IO  uint8_t OCU_OCFS54;
        stc_mft_ocu_ocfs54_field_t OCU_OCFS54_f;
    };
        __IO  uint8_t RESERVED23[3];
    union {
        __IO  uint8_t ICFS10;
        stc_mft_icfs10_field_t ICFS10_f;
        __IO  uint8_t ICU_ICFS10;
        stc_mft_icu_icfs10_field_t ICU_ICFS10_f;
    };
    union {
        __IO  uint8_t ICFS32;
        stc_mft_icfs32_field_t ICFS32_f;
        __IO  uint8_t ICU_ICFS32;
        stc_mft_icu_icfs32_field_t ICU_ICFS32_f;
    };
        __IO  uint8_t RESERVED24[6];
    union {
        __IO uint16_t ICCP0;
        struct {
            __IO  uint8_t ICCP0L;
            __IO  uint8_t ICCP0H;
        };
        __IO uint16_t ICU_ICCP0;
        struct {
            __IO  uint8_t ICU_ICCP0L;
            __IO  uint8_t ICU_ICCP0H;
        };
    };
        __IO  uint8_t RESERVED25[2];
    union {
        __IO uint16_t ICCP1;
        struct {
            __IO  uint8_t ICCP1L;
            __IO  uint8_t ICCP1H;
        };
        __IO uint16_t ICU_ICCP1;
        struct {
            __IO  uint8_t ICU_ICCP1L;
            __IO  uint8_t ICU_ICCP1H;
        };
    };
        __IO  uint8_t RESERVED26[2];
    union {
        __IO uint16_t ICCP2;
        struct {
            __IO  uint8_t ICCP2L;
            __IO  uint8_t ICCP2H;
        };
        __IO uint16_t ICU_ICCP2;
        struct {
            __IO  uint8_t ICU_ICCP2L;
            __IO  uint8_t ICU_ICCP2H;
        };
    };
        __IO  uint8_t RESERVED27[2];
    union {
        __IO uint16_t ICCP3;
        struct {
            __IO  uint8_t ICCP3L;
            __IO  uint8_t ICCP3H;
        };
        __IO uint16_t ICU_ICCP3;
        struct {
            __IO  uint8_t ICU_ICCP3L;
            __IO  uint8_t ICU_ICCP3H;
        };
    };
        __IO  uint8_t RESERVED28[2];
    union {
        __IO  uint8_t ICSA10;
        stc_mft_icsa10_field_t ICSA10_f;
        __IO  uint8_t ICU_ICSA10;
        stc_mft_icu_icsa10_field_t ICU_ICSA10_f;
    };
    union {
        __IO  uint8_t ICSB10;
        stc_mft_icsb10_field_t ICSB10_f;
        __IO  uint8_t ICU_ICSB10;
        stc_mft_icu_icsb10_field_t ICU_ICSB10_f;
    };
        __IO  uint8_t RESERVED29[2];
    union {
        __IO  uint8_t ICSA32;
        stc_mft_icsa32_field_t ICSA32_f;
        __IO  uint8_t ICU_ICSA32;
        stc_mft_icu_icsa32_field_t ICU_ICSA32_f;
    };
    union {
        __IO  uint8_t ICSB32;
        stc_mft_icsb32_field_t ICSB32_f;
        __IO  uint8_t ICU_ICSB32;
        stc_mft_icu_icsb32_field_t ICU_ICSB32_f;
    };
        __IO  uint8_t RESERVED30[2];
    union {
        __IO uint16_t WFTM10;
        struct {
            __IO  uint8_t WFTM10L;
            __IO  uint8_t WFTM10H;
        };
        __IO uint16_t WFG_WFTM10;
        struct {
            __IO  uint8_t WFG_WFTM10L;
            __IO  uint8_t WFG_WFTM10H;
        };
    };
        __IO  uint8_t RESERVED31[2];
    union {
        __IO uint16_t WFTM32;
        struct {
            __IO  uint8_t WFTM32L;
            __IO  uint8_t WFTM32H;
        };
        __IO uint16_t WFG_WFTM32;
        struct {
            __IO  uint8_t WFG_WFTM32L;
            __IO  uint8_t WFG_WFTM32H;
        };
    };
        __IO  uint8_t RESERVED32[2];
    union {
        __IO uint16_t WFTM54;
        struct {
            __IO  uint8_t WFTM54L;
            __IO  uint8_t WFTM54H;
        };
        __IO uint16_t WFG_WFTM54;
        struct {
            __IO  uint8_t WFG_WFTM54L;
            __IO  uint8_t WFG_WFTM54H;
        };
    };
        __IO  uint8_t RESERVED33[2];
    union {
        __IO uint16_t WFSA10;
        stc_mft_wfsa10_field_t WFSA10_f;
        struct {
            __IO  uint8_t WFSA10L;
            __IO  uint8_t WFSA10H;
        };
        __IO uint16_t WFG_WFSA10;
        stc_mft_wfg_wfsa10_field_t WFG_WFSA10_f;
        struct {
            __IO  uint8_t WFG_WFSA10L;
            __IO  uint8_t WFG_WFSA10H;
        };
    };
        __IO  uint8_t RESERVED34[2];
    union {
        __IO uint16_t WFSA32;
        stc_mft_wfsa32_field_t WFSA32_f;
        struct {
            __IO  uint8_t WFSA32L;
            __IO  uint8_t WFSA32H;
        };
        __IO uint16_t WFG_WFSA32;
        stc_mft_wfg_wfsa32_field_t WFG_WFSA32_f;
        struct {
            __IO  uint8_t WFG_WFSA32L;
            __IO  uint8_t WFG_WFSA32H;
        };
    };
        __IO  uint8_t RESERVED35[2];
    union {
        __IO uint16_t WFSA54;
        stc_mft_wfsa54_field_t WFSA54_f;
        struct {
            __IO  uint8_t WFSA54L;
            __IO  uint8_t WFSA54H;
        };
        __IO uint16_t WFG_WFSA54;
        stc_mft_wfg_wfsa54_field_t WFG_WFSA54_f;
        struct {
            __IO  uint8_t WFG_WFSA54L;
            __IO  uint8_t WFG_WFSA54H;
        };
    };
        __IO  uint8_t RESERVED36[2];
    union {
        __IO uint16_t WFIR;
        stc_mft_wfir_field_t WFIR_f;
        struct {
            __IO  uint8_t WFIRL;
            __IO  uint8_t WFIRH;
        };
        __IO uint16_t WFG_WFIR;
        stc_mft_wfg_wfir_field_t WFG_WFIR_f;
        struct {
            __IO  uint8_t WFG_WFIRL;
            __IO  uint8_t WFG_WFIRH;
        };
    };
        __IO  uint8_t RESERVED37[2];
    union {
        __IO uint16_t NZCL;
        stc_mft_nzcl_field_t NZCL_f;
        struct {
            __IO  uint8_t NZCLL;
            __IO  uint8_t NZCLH;
        };
        __IO uint16_t WFG_NZCL;
        stc_mft_wfg_nzcl_field_t WFG_NZCL_f;
        struct {
            __IO  uint8_t WFG_NZCLL;
            __IO  uint8_t WFG_NZCLH;
        };
    };
        __IO  uint8_t RESERVED38[2];
    union {
        __IO uint16_t ACCP0;
        struct {
            __IO  uint8_t ACCP0L;
            __IO  uint8_t ACCP0H;
        };
        __IO uint16_t ADCMP_ACCP0;
        struct {
            __IO  uint8_t ADCMP_ACCP0L;
            __IO  uint8_t ADCMP_ACCP0H;
        };
    };
        __IO  uint8_t RESERVED39[2];
    union {
        __IO uint16_t ACCPDN0;
        struct {
            __IO  uint8_t ACCPDN0L;
            __IO  uint8_t ACCPDN0H;
        };
        __IO uint16_t ADCMP_ACCPDN0;
        struct {
            __IO  uint8_t ADCMP_ACCPDN0L;
            __IO  uint8_t ADCMP_ACCPDN0H;
        };
    };
        __IO  uint8_t RESERVED40[2];
    union {
        __IO uint16_t ACCP1;
        struct {
            __IO  uint8_t ACCP1L;
            __IO  uint8_t ACCP1H;
        };
        __IO uint16_t ADCMP_ACCP1;
        struct {
            __IO  uint8_t ADCMP_ACCP1L;
            __IO  uint8_t ADCMP_ACCP1H;
        };
    };
        __IO  uint8_t RESERVED41[2];
    union {
        __IO uint16_t ACCPDN1;
        struct {
            __IO  uint8_t ACCPDN1L;
            __IO  uint8_t ACCPDN1H;
        };
        __IO uint16_t ADCMP_ACCPDN1;
        struct {
            __IO  uint8_t ADCMP_ACCPDN1L;
            __IO  uint8_t ADCMP_ACCPDN1H;
        };
    };
        __IO  uint8_t RESERVED42[2];
    union {
        __IO uint16_t ACCP2;
        struct {
            __IO  uint8_t ACCP2L;
            __IO  uint8_t ACCP2H;
        };
        __IO uint16_t ADCMP_ACCP2;
        struct {
            __IO  uint8_t ADCMP_ACCP2L;
            __IO  uint8_t ADCMP_ACCP2H;
        };
    };
        __IO  uint8_t RESERVED43[2];
    union {
        __IO uint16_t ACCPDN2;
        struct {
            __IO  uint8_t ACCPDN2L;
            __IO  uint8_t ACCPDN2H;
        };
        __IO uint16_t ADCMP_ACCPDN2;
        struct {
            __IO  uint8_t ADCMP_ACCPDN2L;
            __IO  uint8_t ADCMP_ACCPDN2H;
        };
    };
        __IO  uint8_t RESERVED44[2];
    union {
        __IO  uint8_t ACSB;
        stc_mft_acsb_field_t ACSB_f;
        __IO  uint8_t ADCMP_ACSB;
        stc_mft_adcmp_acsb_field_t ADCMP_ACSB_f;
    };
        __IO  uint8_t RESERVED45[3];
    union {
        __IO uint16_t ACSA;
        stc_mft_acsa_field_t ACSA_f;
        struct {
            __IO  uint8_t ACSAL;
            __IO  uint8_t ACSAH;
        };
        __IO uint16_t ADCMP_ACSA;
        stc_mft_adcmp_acsa_field_t ADCMP_ACSA_f;
        struct {
            __IO  uint8_t ADCMP_ACSAL;
            __IO  uint8_t ADCMP_ACSAH;
        };
    };
        __IO  uint8_t RESERVED46[2];
    union {
        __IO uint16_t ATSA;
        stc_mft_atsa_field_t ATSA_f;
        struct {
            __IO  uint8_t ATSAL;
            __IO  uint8_t ATSAH;
        };
        __IO uint16_t ADCMP_ATSA;
        stc_mft_adcmp_atsa_field_t ADCMP_ATSA_f;
        struct {
            __IO  uint8_t ADCMP_ATSAL;
            __IO  uint8_t ADCMP_ATSAH;
        };
    };
} FM_MFT_TypeDef, FM3_MFT_TypeDef;


#endif //_MB9BD1XT_H_
