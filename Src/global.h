/**
@file		global.h
@brief		global defines.

@author		Nicola
@date		10/06/2012
@version	01.00.00
*/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "stdDataTypes.h"
#include "alloc.h"
#include "ch.h"
/**
 * Scales' fifos depth
 */
#define SCALES_FIFO_DEPTH	2

/**
 * Pressures' fifos depth
 */
#define PRS_FIFO_DEPTH		2

/**
 * Temperatures' fifos depth
 */
#define TEMPERATURE_FIFO_DEPTH		2

/**
 * Power supplies' fifos depth
 */
#define POWER_SUPPLY_FIFO_DEPTH		2

/**
 * BLD' fifos depth
 */
#define BLD_FIFO_DEPTH		2

/**
 * ISR define
 */
#define _IRQ_HANDLER(id) void id(void)

typedef const char* textp;			// serve per mettere in un segmento const
									// gli array di puntatori a stringhe

typedef void (*funp)();				// serve per mettere in un segmento const
									// gli array di puntatori a funzioni

typedef void (*funpv)(void);		// serve per mettere in un segmento const
									// gli array di puntatori a funzioni

typedef void (*funpi)(int);			// serve per mettere in un segmento const
									// gli array di puntatori a funzioni

typedef void (*funpb)(byte);

typedef void (*funprb)(byte*);

#define Disable	     0
#define Enable		 1
#define OFF			 0
#define ON			 1
#define SET_BIT_LOW  0
#define SET_BIT_HIGH 1


// Bit Definitions
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80
#define BIT8  0x100
#define BIT9  0x200
#define BIT10 0x400
#define BIT11 0x800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000
#define BIT16 0x10000
#define BIT17 0x20000
#define BIT18 0x40000
#define BIT19 0x80000
#define BIT20 0x100000
#define BIT21 0x200000
#define BIT22 0x400000
#define BIT23 0x800000
#define BIT24 0x1000000
#define BIT25 0x2000000
#define BIT26 0x4000000
#define BIT27 0x8000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000
#endif



