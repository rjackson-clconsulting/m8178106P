/**
@file		Cd_Air.h
@brief		Module to implement Air Drivers - Former "CDSC_DR1.H"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/


#ifndef _CD_AIR_H_
#define _CD_AIR_H_

#include "global.h"
#include "board_define.h"   //Pin Definition
#include "Cd_ALA.H"         //This is a Dependency

#define PinTEST_ARIA_P          PinTestAir
#define PinAIR_IN_P             PinAirIn
#define AIR_TIME_NARROW_TSHLD   500     //Portato a 500 per evitare allarmi "TEST ARIA FALLITO" indesiderati
#define AIR_TIME_LARGE_TSHLD    2000    //Portato a 2000 per evitare allarmi "TEST ARIA FALLITO" indesiderati

/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
struct AIR
   {
	UBYTE ala;
    UBYTE state;
	UBYTE ini;
	UBYTE test;
    uint32_t time;
   };


/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void DrvAIR_Ini(void);
void DrvAIR_Ges(void);
#ifdef __cplusplus
}
#endif

#endif

