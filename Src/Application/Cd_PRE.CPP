/**
@file		Cd_Air.cpp
@brief		Module to implement preparation - Former "CDSC_DR1.C"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#include "irqManager.h"
#include "cd_pre.h"

/*****************************************************
** GLOBAL VAR SECTION
*****************************************************/
UWORD tim_art_min;
UBYTE ven_lim_sta;         /* Venous Limits : State          */
uint32_t ven_lim_tim;         /* Venous Limits : Time           */
PRE   pre[NUM_PRE];

/*****************************************************
** EXTERN VAR SECTION
*****************************************************/
//extern volatile uint32_t 	g_time_msec;          //globalApplication.cpp
extern ALA      ala;                  //Cd_Ala.cpp
extern CsiXfrBlockRx chkBlock;        //gesXfr_p.cpp
extern ADChan adcSet[MAX_AD];

/****************************************************************************
void AlaPRE_Ini(void)
** \brief PRE Drivers 1 Initialization
** \return void
****************************************************************************/
void AlaPRE_Ini(void)
{
UBYTE ub;
   for (ub=0; ub<NUM_PRE; ub++)
      pre[ub].dat=0;

	tim_art_min=0;
	ven_lim_sta=0;
	ala.old_flw=0;

	Pre_Lim(PRE_ART,  100, -200);
	Pre_Lim(PRE_VEN,  350,  -50);
}
/****************************************************************************
AlaPRE_Ges(void)
** \brief PRE Drivers 1 Handling
** \return void
****************************************************************************/
void AlaPRE_Ges(void)
{
UBYTE ub6,ub7;

	pre[PRE_ART].dat = (WORD)((adcPressArt.conv.value/5)*5);
	pre[PRE_VEN].dat = (WORD)((adcPressVen.conv.value/5)*5);

   for (ub6=PRE_ART; ub6<=PRE_VEN; ub6++)
      {
		 if( ub6==PRE_FIL )
			 continue;
         ub7=(UBYTE)(ALC_PRE_ART_MIN+2*ub6);

		 if(pre[ub6].dat < pre[ub6].dat_min)
	     {
   	            Ala_Pre(ub7,1);
         }
         ub7++;
		 if(pre[ub6].dat > pre[ub6].dat_max)
	        Ala_Pre(ub7,1);
      }
                                                                   /* memorizza allarme per 2" perchè autoresettante e se scatta per un attimo non lo vedo */

      if(pre[PRE_ART].dat < pre[PRE_ART].dat_min)  tim_art_min=8;  /* se in allarme carica timer */
      if(tim_art_min) tim_art_min--;                               /* timer decrementa */
      if(tim_art_min) Ala_Pre(ALC_PRE_ART_MIN,1);                  /* se timer caricato mantiene allarme */
                                                                   /* che viene tolto il giro successivo dalla routine sopra */
}
/****************************************************************************
Pre_Lim(UBYTE press, WORD vmax, WORD vmin)
** \brief Updates "pre[].dat_min" and "pre[].dat_max"
** \return void
****************************************************************************/
void Pre_Lim(UBYTE press, WORD vmax, WORD vmin)
{
   switch(press)
      {
       case PRE_ART:
		    pre[PRE_ART].dat_max=vmax;
            pre[PRE_ART].dat_min=vmin;
            break;
       case PRE_FIL:
            pre[PRE_FIL].dat_max=vmax;
            pre[PRE_FIL].dat_min=vmin;
            break;
       case PRE_VEN:
            pre[PRE_VEN].dat_max=vmax;
            pre[PRE_VEN].dat_min=vmin;
            break;
       case PRE_DRO:
            pre[PRE_DRO].dat_max=vmax;
            pre[PRE_DRO].dat_min=vmin;
            break;
        default:
            Fatal_Error(FATAL_ERROR_PRE);
            break;
       }
}








