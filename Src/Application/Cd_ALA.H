/**
@file		Cd_Ala.h
@brief		Module to implement Air Drivers - Former "CDSC_ALA.H"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _CD_ALA_H_
#define _CD_ALA_H_

#include "global.h"            //Standard Types
#include "board_define.h"      //Include Pin define
#include "gesXfr_p.h"          //SPI intercommunication structure
#include "globalApplication.h" //Stazione State

/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/
struct ALA
   {
    UBYTE sta;                       /* State                                    */
    uint32_t tim;                       /* Time                                     */
    UBYTE act;                       /* First Active Alarm                       */
    UBYTE unl;                       /* Unlock                                   */
    UBYTE loc_blo;                   /* Number of Lock of Blood                  */
    UBYTE loc_flu;                   /* Number of Lock of Fluid                  */
    UBYTE ala_ack[ALA_MAX];          /* Reset Type                               */
    UBYTE ala_loc[ALA_MAX];          /* Type of Lock                             */
    UBYTE ala_unl[ALA_MAX];          /* Type of Unlock                           */
    UBYTE ala_sta[ALA_MAX];          /* State                                    */
    UBYTE ala_sil[ALA_MAX];          /* Silence                                  */
    UBYTE ala_ena[ALA_MAX];          /* Enabled                                  */
    UWORD fatal_num;                 /* Number fatal error                       */
    UWORD cal_num_pro;               /* Number error cal protective              */
    //UWORD tim_cic;                   /* Timer cicalino                           */
    UBYTE air;                       /* Allarme aria                             */
    UBYTE air_test;                  /* Allarme test aria                        */
	word  old_flw;					 /*	Flusso sangue precedente    			 */
};

/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void  Ala_Ini           (void);
void  Ala_Ges           (void);
void  Ala_Pre           (UBYTE all, UBYTE pres);
void  Ala_Kal           (void);
void  Ala_Res           (void);
void  Fatal_Error       (int errore);
void  Ges_luci_cicalino (void);
#ifdef __cplusplus
}
#endif
/* Internal Functions Declaration   */
void Ala_Loc     (UBYTE all);
void Ala_Unl_Ala (UBYTE all);
void Ala_Unl_Tot (void);

#endif
