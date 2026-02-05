/**
@file		dataTypes.h
@brief		standard data types defines.
@author		Andrea Pellegrini
@date		21/12/2015
@version	00.00
@source     Ashai_SPS_CTRL_CPU
*/

#ifndef _STD_DATA_TYPES_H_
#define _STD_DATA_TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

typedef char             BYTE;
typedef unsigned char   UBYTE;
typedef short            WORD;
typedef unsigned short  UWORD;
typedef long             LONG;
typedef unsigned long   ULONG;

typedef	unsigned short	word;
typedef unsigned char	byte;
typedef unsigned long	dword;

typedef unsigned short  wchar;

/** bool type 					*/
#ifdef __cplusplus
#else
typedef uint8_t	bool;
#endif

enum Bool{ False = 0, True };
#ifdef __cplusplus
#else
typedef enum Bool Bool;
#endif

#endif
