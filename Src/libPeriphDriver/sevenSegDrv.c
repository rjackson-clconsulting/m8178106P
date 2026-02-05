/**
@file		sevenSegDrv.c
@brief		Created to drive 7 segment display on FM3
@author		Fernando Morani (Tecnoideal)
@date		07/04/2017
@version	01.00
@source     Flexiper - Protection Micro
*/

#include "sevenSegDrv.h"

/**
Write number zero with the display. Dot is used
*/
void writeZERO(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number one with the display. Dot is used
*/
void writeONE(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number two with the display. Dot is used
*/
void writeTWO(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number three with the display. Dot is used
*/
void writeTHREE(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number four with the display. Dot is used
*/
void writeFOUR(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number five with the display. Dot is used
*/
void writeFIVE(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number six with the display. Dot is used
*/
void writeSIX(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number seven with the display. Dot is used
*/
void writeSEVEN(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number eight with the display. Dot is used
*/
void writeEIGHT(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write number nine with the display. Dot is used
*/
void writeNINE(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = USE_SEGMENT;
}
/**
Write letter A with the display. Dot is not used
*/
void writeA(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter C with the display. Dot is not used
*/
void writeC(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter E with the display. Dot is not used
*/
void writeE(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter F with the display. Dot is not used
*/
void writeF(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter H with the display. Dot is not used
*/
void writeH(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter I with the display. Dot is not used
*/
void writeI(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter L with the display. Dot is not used
*/
void writeL(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter O with the display. Dot is not used
*/
void writeO(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter P with the display. Dot is not used
*/
void writeP(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = DONT_USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = DONT_USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter S with the display. Dot is not used
*/
void writeS(void)
{
    SEG_RS_1   = DONT_USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = DONT_USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = USE_SEGMENT;
    SEG_ML     = USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter U with the display. Dot is not used
*/
void writeU(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
/**
Write letter J with the display. Dot is not used
*/
void writeJ(void)
{
    SEG_RS_1   = USE_SEGMENT;
    SEG_RS_2   = USE_SEGMENT;
    SEG_LS_1   = DONT_USE_SEGMENT;
    SEG_LS_2   = USE_SEGMENT;
    SEG_BL     = USE_SEGMENT;
    SEG_UL     = DONT_USE_SEGMENT;
    SEG_ML     = DONT_USE_SEGMENT;
    SEG_DOT_P  = DONT_USE_SEGMENT;
}
