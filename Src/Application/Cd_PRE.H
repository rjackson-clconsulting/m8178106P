/**
@file		Cd_Air.h
@brief		Module to implement preparation - Former "CDSC.H"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _CD_PRE_H_
#define _CD_PRE_H_

#include "global.h"
#include "cd_ala.h"
#include "swtim.h"

/*Pressures    puntatori al verrore dati pressioni*/
#define NUM_PRE              4            /* Number of Pressures            */
#define PRE_ART              0            /* Arterial Pressure              */
#define PRE_FIL              1            /* Filter   Pressure              */
#define PRE_VEN              2            /* Venous   Pressure              */
#define PRE_DRO              3            /* Drop     Pressure              */

/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
struct PRE
   {
    WORD  dat;                         /* Dat                                 */
    WORD  dat_min;                     /* Dat Minimum                         */
    WORD  dat_max;                     /* Dat Maximum                         */
   } ;
typedef PRE PRE;

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void AlaPRE_Ini(void);
void AlaPRE_Ges(void);
void Pre_Lim(UBYTE press, WORD vmax, WORD vmin);
#ifdef __cplusplus
}
#endif

#endif

