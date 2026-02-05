/**
@file		main.h
@brief		main file.

@author		Fernando Morani
@date		09/01/2018
@version	01.00.00
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include "board.h"              //Call to boardInit()
#include "board_define.h"
#include "globalApplication.h"
#include "sevenSegDrv.h"
#include "mainTest.h"

/**
Macro to be used only in main.cpp
**/
#ifdef LOADER_ENGINE
#define  WAITING_FOR_CYCLE_TO_SHOW_FAIL         1000000000
#endif //LOADER_ENGINE

#endif
