/**
@file		Cd_Air.cpp
@brief		Module to implement Air Drivers - Former "CDSC_ALA.C"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#include "irqManager.h"
#include "cd_ala.h"

/*****************************************************
** EXTERN VAR SECTION
*****************************************************/
extern DecTimer timLoad;	          //globalApplication.cpp
extern CsiXfrBlockRx chkBlock;        //gesXfr_p.cpp

/*****************************************************
** GLOBAL VAR SECTION
*****************************************************/
ALA   ala;

/****************************************************************************
void Ala_Ini(void)
** \brief Alarms initialization
** \return void
****************************************************************************/
void Ala_Ini(void)
{
UBYTE ub;

   ala.sta=ALA_STA_ABS;
   ala.act=ALA_NUL;
   ala.unl=ALA_LOC_NUL;
   ala.loc_blo=ala.loc_flu=0;

   /*
      "ala.ala_ack[]"
   */
   for (ub=0; ub<ALA_MAX; ub++)
      ala.ala_ack[ub]=ALA_ACK_AQ1;

   ala.ala_ack[ALC_PMP_BLO_COV]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PMP_DRA_COV]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_TST_REL]=ALA_ACK_AUT;		// Warning
//   ala.ala_ack[WRN_SAC_LAV]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_OVE_VEN]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_TST_PRE]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_PRI_COM]=ALA_ACK_AUT;
   ala.ala_ack[WRN_CHA_SET]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_TRE_STO]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_TRE_END]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_CHA_SIR]=ALA_ACK_AUT;
   ala.ala_ack[WRN_COL_CIR]=ALA_ACK_AUT;
//   ala.ala_ack[WRN_CHA_SAC]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_ART_MIN]=ALA_ACK_AUT;   /* arteriosa minima autoresettante */
   ala.ala_ack[ALC_PRE_UF_MIN]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_ART_MAX]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_ART_MOD]=ALA_ACK_AUT;
   ala.ala_ack[ALC_APP_BLO_2MS]=ALA_ACK_AUT;
   ala.ala_ack[WRN_BAT_VOL]=ALA_ACK_AUT;


//   ala.ala_ack[ALC_T0T_DIF_ART]=ALA_ACK_AUT;


   ala.ala_ack[ALC_BLD_TUB_INS]=ALA_ACK_AUT;
   ala.ala_ack[ALC_BLD_AMB_LIG]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_FIL_MAX]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_VEN_MAX]=ALA_ACK_AUT;
   ala.ala_ack[ALC_APP_HEP_END]=ALA_ACK_AUT;
   ala.ala_ack[ALC_APP_HEP_SYR]=ALA_ACK_AUT;


   ala.ala_ack[ALC_APP_BAG_DRA]=ALA_ACK_AUT;
   ala.ala_ack[ALC_PRE_PLA_MAX]=ALA_ACK_AUT;




   /*
      "ala.ala_loc[]"
   */
   for (ub=0; ub<ALA_MAX; ub++)
      ala.ala_loc[ub]=ALA_LOC_ALL;

   ala.ala_loc[ALC_PMP_DRA_COV]=ALA_LOC_FLU;

   ala.ala_loc[ALC_T0T_DIF_ART]=ALA_LOC_FLU;
   ala.ala_loc[ALC_T0T_DIF_FIL]=ALA_LOC_FLU;
   ala.ala_loc[ALC_T0T_DIF_VEN]=ALA_LOC_FLU;
   ala.ala_loc[ALC_T1D_PRE_VEN]=ALA_LOC_FLU;
   ala.ala_loc[ALC_WEI_DRA_VAR]=ALA_LOC_FLU;
   ala.ala_loc[ALC_STE_HEP_OVE]=ALA_LOC_NUL;
   ala.ala_loc[ALC_APP_HEP_END]=ALA_LOC_NUL;
   ala.ala_loc[ALC_APP_HEP_SYR]=ALA_LOC_NUL;
   ala.ala_loc[ALC_APP_BAG_DRA]=ALA_LOC_FLU;
   //ala.ala_loc[ALC_APP_VAR_WEI]=ALA_LOC_FLU;
   ala.ala_loc[ALC_STE_HEP_BLO]=ALA_LOC_NUL;  /* Allarme di pompa eparina non ferma il trattamento */
   ala.ala_loc[ALC_STE_HEP_OVE]=ALA_LOC_NUL;  /* Allarme di pompa eparina non ferma il trattamento */

//   ala.ala_loc[WRN_SAC_LAV]=ALA_LOC_NUL;		// Warning non fermano il trattamento
//   ala.ala_loc[WRN_OVE_VEN]=ALA_LOC_NUL;
//   ala.ala_loc[WRN_PRI_COM]=ALA_LOC_NUL;
   ala.ala_loc[WRN_CHA_SET]=ALA_LOC_NUL;
//   ala.ala_loc[WRN_PRI_AIR]=ALA_LOC_NUL;
//   ala.ala_loc[WRN_TRE_END]=ALA_LOC_FLU;
//   ala.ala_loc[WRN_CHA_SIR]=ALA_LOC_NUL;
   ala.ala_loc[WRN_COL_CIR]=ALA_LOC_NUL;
   ala.ala_loc[WRN_PRI_TST]=ALA_LOC_NUL;
//   ala.ala_loc[WRN_TST_PRE]=ALA_LOC_NUL;
//   ala.ala_loc[WRN_CHA_SAC]=ALA_LOC_FLU;
//   ala.ala_loc[WRN_INI_TAR]=ALA_LOC_NUL;

   ala.ala_loc[ALC_FLW_PLA_MIN]=ALA_LOC_FLU;

   ala.ala_loc[ALC_PRE_ART_MIN]=ALA_LOC_NUL;
   ala.ala_loc[ALC_PRE_UF_MIN]=ALA_LOC_NUL;


   /*
      "ala.ala_unl[]"
   */
   for (ub=0; ub<ALA_MAX; ub++)
      ala.ala_unl[ub]=ala.ala_loc[ub];

   ala.ala_unl[ALC_AIR_VEN_LIN]=ALA_LOC_NUL;
   ala.ala_unl[ALC_PRE_VEN_MIN]=ALA_LOC_NUL;
//   ala.ala_unl[ALC_APP_BAG_DRA]=ALA_LOC_NUL;

   /*
      "ala.ala_sta[]"
   */
   for (ub=0; ub<ALA_MAX; ub++)
      ala.ala_sta[ub]=ALA_ALA_STA_ABS;

   for (ub=0; ub<ALA_MAX; ub++)					// Abilita tutti gli allarmi
      ala.ala_ena[ub]=Enable;

	Buzzer = 1;	// cicalino off e luce rossa spenta

	timLoad.Preset(4000);



}

/****************************************************************************
void Ala_Ges(void)
** \brief Alarms Handling
** \return void
****************************************************************************/
void Ala_Ges(void)
{
UBYTE ub,ub2;
/*Computation of "ala.act"*/
   ala.act=ALA_NUL;

   if (ala.ala_sta[ALP_T0T_CON_RUN])                               /* se errore di run protective abilita solo lui */
      ala.act=ALP_T0T_CON_RUN;
   else                                                            /* altrimenti fa scansione degli altri allarmi */
      {
       for (ub=0; ub<ALA_MAX; ub++)
          {
           if (ala.ala_sta[ub])                                    /* se ala_sta di allarme X è attivo */
              {
               ala.act=ub;                                         /* mette su ala.act allarme attivo */
               break;
              }
          }
      }

   /*
      Handling of "ala.sta"
   */
   switch(ala.sta)                                             /* stato allarme */
      {
       case ALA_STA_ABS :
            /*   Alarms Absent */
            if (ala.act)                                       /* se vede un allarme manda in allarme presente */
               ala.sta=ALA_STA_PRE;
            break;
       case ALA_STA_PRE :
            /* Alarms Present*/
            if (ala.act == ALA_NUL)                            /* se manca allarme manda in allarme assente */
               {
                ala.sta=ALA_STA_ABS;
                break;
               }
            ub2=0;
            for (ub=0; ub<ALA_MAX; ub++)                       /* scansione tutti gli allarmi */
               {
                if (ala.ala_sta[ub] == ALA_STA_ABS)            /* se allarme non presente toglie silenziamento */
                   ala.ala_sil[ub]=0;
                else
                   {
                    if (ala.ala_sil[ub] == 0)      /* se allarme presente con richiesta silenziato abilita per silenziare */
                       ub2++;                      /* incrementa ub2 se non deve essere silenziato */
                   }
               }
            if (ub2 == 0)                                      /* se ub2 = 0 salta */
               {
                ala.sta=ALA_STA_SIL;
                ala.tim=g_time_msec;
               }
            break;
       case ALA_STA_SIL :
            /* Alarms Silenced */
            if (ala.act == ALA_NUL)                            /* se manca allarme manda in allarme assente */
               {
                ala.sta=ALA_STA_ABS;
                break;
               }
            ub2=0;
            if ((g_time_msec-ala.tim) > 60000)            /* dopo 60 secondi toglie silenziamento */
              ub2++;
            for (ub=0; ub<ALA_MAX; ub++)                       /* scansione di tutti gli allarmi */
               {
                if (ala.ala_sta[ub] && (ala.ala_sil[ub] == 0)) /* in allarme e senza richiesta di silenziamento torna in presente */
                   {
                    //if (ub!=ALC_WEI_DRA_VAR)
                       ub2++;
                   }
               }
            if (ub2)
               {
                ala.sta=ALA_STA_PRE;                           /* mette la presenza allarme */
                for (ub=0; ub<ALA_MAX; ub++)                   /* toglie silenz. a tutti gli all. */
                   ala.ala_sil[ub]=0;
               }
            break;
       default :
            Fatal_Error(FATAL_ERROR_ALA);
            break;
      }
        //Ges_luci_cicalino();
}

/****************************************************************************
void Ala_Pre(UBYTE all, UBYTE pres)
** \brief Handles presence "pres" of alarm "all"
** \return void
****************************************************************************/
void Ala_Pre(UBYTE all, UBYTE pres)
{
UBYTE ub5;
   ub5=0;
   /*
      "ala.ala_sta[all]"
   */

   switch(ala.ala_sta[all])
      {
       case ALA_ALA_STA_ABS :
            if (pres)
               {
                ala.ala_sta[all]=ALA_ALA_STA_PRE;             /* mette allarme in presenza */
                ala.ala_sil[all]=0;                           /* toglie silenziamento */
                Ala_Loc(all);                                 /* esegue blocchi a seconda del tipo di allarme */
               }
            break;

       case ALA_ALA_STA_PRE :
            if (pres)
               break;
            switch(ala.ala_ack[all])
               {
                case ALA_ACK_AUT :
                     ala.ala_sta[all]=ALA_ALA_STA_ABS;
                     break;

                case ALA_ACK_AQ1 :
                     ala.ala_sta[all]=ala.ala_sil[all] ? ALA_ALA_STA_ABS : ALA_ALA_STA_MEM;
                     break;

                case ALA_ACK_AQ2 :
                     ala.ala_sta[all]=ALA_ALA_STA_MEM;
                     break;

                default :
                     Fatal_Error(FATAL_ERROR_ALA+1);
                     break;
               }
            if (ala.ala_sta[all] == ALA_ALA_STA_ABS)
               ub5++;
            break;

       case ALA_ALA_STA_MEM :
            if (pres)
               {
                ala.ala_sta[all]=ALA_ALA_STA_PRE;
                ala.ala_sil[all]=0;
                break;
               }
            if ((ala.ala_ack[all] == ALA_ACK_AQ1) && ala.ala_sil[all])
               ala.ala_sta[all]=ALA_ALA_STA_ABS;
            if (ala.ala_sta[all] == ALA_ALA_STA_ABS)
               ub5++;
            break;

       default :
            Fatal_Error(FATAL_ERROR_ALA+2);
            break;
      }

   if (ub5){
    Ala_Unl_Ala(all);
   }

}

/****************************************************************************
void Ala_Kal(void)
** \brief Handling of Push of KEY_ALA
** \return void
****************************************************************************/
void Ala_Kal(void)
{
UBYTE ub;

	ala.air=0;
	ala.air_test=0;
	PinBUZZER = 1;

   switch(ala.sta)
      {
       case ALA_STA_ABS :
            break;

       case ALA_STA_PRE :
			ub=ala.act;

            for (ub=0; ub<ALA_MAX; ub++)
               {
                if ((ala.ala_sta[ub] == ALA_ALA_STA_MEM) && (ala.ala_ack[ub] != ALA_ACK_AQ2))
                   {
                    ala.ala_sta[ub]=ALA_ALA_STA_ABS;
                    Ala_Unl_Ala(ub);
                   }
                ala.ala_sil[ub]=ala.ala_sta[ub];
               }
            Ala_Res();
            break;

       case ALA_STA_SIL :
            Ala_Res();
            break;

       default :
            Fatal_Error(FATAL_ERROR_ALA+3);
            break;
      }
}

/****************************************************************************
void Ala_Res(void)
** \brief Resets all Alarms
** \return void
****************************************************************************/
void Ala_Res(void)
   /*
      Resets all Alarms
   */
{
UBYTE ub;
   for (ub=0; ub<ALA_MAX; ub++)
      ala.ala_sta[ub]=ALA_ALA_STA_ABS;
   ala.sta=ALA_STA_ABS;
   ala.act=ALA_NUL;
   ala.loc_blo=ala.loc_flu=0;
   Ala_Unl_Tot();
}

/****************************************************************************
void Ala_Loc(UBYTE all)
** \brief Locks Alarm "all"
** \return void
****************************************************************************/
void Ala_Loc(UBYTE all)
{
UBYTE ub,ub2;
   ub=ub2=0;
   if (ala.ala_loc[all] & ALA_LOC_BLO)
      {
       ala.loc_blo++;
      }
   if (ala.ala_loc[all] & ALA_LOC_FLU)
      {
       ala.loc_flu++;
      }

   switch(ala.ala_unl[all])

      {
       case ALA_LOC_NUL :
            ala.unl=ALA_LOC_NUL;
            break;

       case ALA_LOC_BLO :
            if (ub)
               {
                if (ala.unl == ALA_LOC_FLU)
                   ala.unl=ALA_LOC_ALL;
                else
                   ala.unl=ALA_LOC_BLO;
               }
            break;

       case ALA_LOC_FLU :
            if (ub2)
               {
                if (ala.unl == ALA_LOC_BLO)
                   ala.unl=ALA_LOC_ALL;
                else
                   ala.unl=ALA_LOC_FLU;
               }
            break;

       case ALA_LOC_ALL :
            if ((ub == 0) && (ub2 == 0))
               break;
            if (ub)
               {
                if (ala.unl == ALA_LOC_FLU)
                   ala.unl=ALA_LOC_ALL;
                else
                   ala.unl=ALA_LOC_BLO;
               }
            if (ub2)
               {
                if (ala.unl == ALA_LOC_BLO)
                   ala.unl=ALA_LOC_ALL;
                else
                   ala.unl=ALA_LOC_FLU;
               }
            break;

       default :
            Fatal_Error(FATAL_ERROR_ALA+4);
            break;
      }
}

/****************************************************************************
void Ala_Unl_Ala(UBYTE all)
** \brief Unlocks Alarm "all"
** \return void
****************************************************************************/
void Ala_Unl_Ala(UBYTE all)
   /*
      Unlocks Alarm "all"
   */
{
   if (ala.ala_unl[all] & ALA_LOC_BLO)
      {
       if (ala.loc_blo)
          ala.loc_blo--;
      }
   if (ala.ala_unl[all] & ALA_LOC_FLU)
      {
       if (ala.loc_flu)
          ala.loc_flu--;
      }
   Ala_Unl_Tot();

   if (ala.loc_blo)
      {
       if ((ala.ala_loc[all] & ALA_LOC_BLO) && ((ala.ala_unl[all] & ALA_LOC_BLO) == 0))
          ala.loc_blo--;
      }
   if (ala.loc_flu)
      {
       if ((ala.ala_loc[all] & ALA_LOC_FLU) && ((ala.ala_unl[all] & ALA_LOC_FLU) == 0))
          ala.loc_flu--;
      }
}

/****************************************************************************
void Ala_Unl_Tot
** \brief Unlocks Total
** \return void
****************************************************************************/
void Ala_Unl_Tot(void)
{
   if ((ala.unl & ALA_LOC_BLO) && (ala.loc_blo == 0))
      {
       ala.unl &= ~ALA_LOC_BLO;
      }
   if ((ala.unl & ALA_LOC_FLU) && (ala.loc_flu == 0))
      {
       ala.unl &= ~ALA_LOC_FLU;
      }
}

/****************************************************************************
void Fatal_Error  (int errore)
** \brief Manage Fatal Error
** \return void
****************************************************************************/
void  Fatal_Error  (int errore)
{
	Ala_Pre(ALP_FAT_ERR,1);
	ala.fatal_num=errore;
}

/****************************************************************************
void Ges_luci_cicalino  (void)
** \brief Light of the buzzer management
** \return void
****************************************************************************/
void  Ges_luci_cicalino  (void)
{
byte ub;
		//ala.tim_cic=g_time_msec;
		ub=1;
		if( chkBlock.Stato==STATE_IDL && !timLoad.Match() )
			ub=0;

		if ( ala.act == ALP_T0T_CON_RUN && ub )
		{
			Buzzer = 0;
		}
		else
		{
			Buzzer = 1;
		}
}




