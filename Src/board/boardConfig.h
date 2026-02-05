/**
@file		boardConfig.h
@brief		header file for board configuration

@author		Fernando Morani
@date		21/11/2016
@version	01.00.00
*/

#ifndef _BOARDCONFIG_H_
#define _BOARDCONFIG_H_

//Define to be used for setting the Port Function Setting Register
#define GPIO_SET_00  0x00000001
#define GPIO_SET_01  0x00000002
#define GPIO_SET_02  0x00000004
#define GPIO_SET_03  0x00000008
#define GPIO_SET_04  0x00000010
#define GPIO_SET_05  0x00000020
#define GPIO_SET_06  0x00000040
#define GPIO_SET_07  0x00000080
#define GPIO_SET_08  0x00000100
#define GPIO_SET_09  0x00000200
#define GPIO_SET_10  0x00000400
#define GPIO_SET_11  0x00000800
#define GPIO_SET_12  0x00001000
#define GPIO_SET_13  0x00002000
#define GPIO_SET_14  0x00004000
#define GPIO_SET_15  0x00008000
#define GPIO_SET_16  0x00010000
#define GPIO_SET_17  0x00020000
#define GPIO_SET_18  0x00040000
#define GPIO_SET_19  0x00080000
#define GPIO_SET_20  0x00100000
#define GPIO_SET_21  0x00200000
#define GPIO_SET_22  0x00400000
#define GPIO_SET_23  0x00800000
#define GPIO_SET_24  0x01000000
#define GPIO_SET_25  0x02000000
#define GPIO_SET_26  0x04000000
#define GPIO_SET_27  0x08000000
#define GPIO_SET_28  0x10000000
#define GPIO_SET_29  0x20000000
#define GPIO_SET_30  0x40000000
#define GPIO_SET_31  0x80000000

#define GPIO_ALL_SET 	0xFFFF
#define GPIO_ALL_UNSET 	0x0000

#endif //_BOARDCONFIG_H_
