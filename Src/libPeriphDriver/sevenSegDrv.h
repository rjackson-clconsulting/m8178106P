/**
@file		sevenSegDrv.h
@brief		Created to drive 7 segment display on FM3
@author		Fernando Morani (Tecnoideal)
@date		07/04/2017
@version	01.00
@source     Flexiper - Protection Micro
*/

#ifndef SEVENSEGDRV_H_INCLUDED
#define SEVENSEGDRV_H_INCLUDED

#include "board_define.h"

/**
Define Section
*/
#define SEG_RS_1                Pin7Seg_B   //Segment right side number 1
#define SEG_RS_2                Pin7Seg_C   //Segment right side number 2
#define SEG_LS_1                Pin7Seg_F   //Segment left side number 1
#define SEG_LS_2                Pin7Seg_E   //Segment left side number 2
#define SEG_BL                  Pin7Seg_D   //Segment bottom line
#define SEG_UL                  Pin7Seg_A   //Segment upper line
#define SEG_ML                  Pin7Seg_G   //Segment middle line
#define SEG_DOT_P               Pin7Seg_DP  //Segment dot point

#define USE_SEGMENT              1
#define DONT_USE_SEGMENT         0

/**Declaration Section
*/
#ifdef __cplusplus
extern "C" {
#endif
void writeZERO(void);
void writeONE(void);
void writeTWO(void);
void writeTHREE(void);
void writeFOUR(void);
void writeFIVE(void);
void writeSIX(void);
void writeSEVEN(void);
void writeEIGHT(void);
void writeNINE(void);
void writeA(void);
void writeC(void);
void writeE(void);
void writeF(void);
void writeH(void);
void writeI(void);
void writeL(void);
void writeO(void);
void writeP(void);
void writeS(void);
void writeU(void);
void writeJ(void);
#ifdef __cplusplus
}
#endif

#endif // SEVENSEGDRV_H_INCLUDED
