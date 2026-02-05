/**
@file		globalAppDefine.h
@brief		Only define for the application
@author		Fernando Morani
@date		03/04/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _GLOBALAPPDEFINE_H_
#define _GLOBALAPPDEFINE_H_

#include "fm3_adc.h"

//Define to be used as anologue input using ADC.cpp
#define adcGauge1       Adc1    // AN00
#define adcGauge2      Adc2    // AN01
#define adcPressArt     Adc3    // AN02
#define adcPressFil     Adc4    // AN03
#define adcPressVen     Adc5    // AN04
#define adcPressArt2    Adc6    // AN05
#define adcBldRx        Adc7    // AN06
#define vTest1          Adc8    // AN07
#define vTest2          Adc9    // AN08
#define monAvrefp  Adc10
#define monAvrefc  Adc11
#define mon5V      Adc12
#define mon33Vp    Adc13
#define mon33V     Adc14

#define RELE_ON     0
#define RELE_OFF    1

#define ERROR_NONE          0
#define ERROR_INIT          1
#define ERROR_RUN           2


#define BLINK_LED_SWAP_TIME     1000

#ifdef LOADER_ENGINE
#define LOADER_PROTODATA_BUF_LEN 	 1050  //Ver. 01.01
#endif //LOADER_ENGINE


#define	Buzzer			    PinBUZZER

//Fatal_Error
#define FATAL_ERROR_ALA      1         // Errore switc allarmi
#define FATAL_ERROR_AIR     10         // Errore allarme aria
#define FATAL_ERROR_CLA     20         // Errore clamp
#define FATAL_ERROR_BLD     30         // Errore bld
#define FATAL_ERROR_PRE     40         // Errore bld
#define FATAL_ERROR_GES_XFR 50         // Errore irreversibile nella comunicazione

/***************************************************************************************
*  Alarms  *
*  Control Alarms   la numerazione e l'ordine degli allarmi e de warning non può essere
*  modificato l'ordine è strettamente legato all'ordine nel file messaggi  stbu.msh
****************************************************************************************/
#define ALA_NUL              0            /* No Alarms                      */

#define ALC_FAT_ERR          1            /* T0   : Fatal error             */
#define ALC_T0T_PRO_RUN      2            /* T0   : Protec. Running         */
#define ALC_T0T_DIF_ART      3            /* T0   : Different C-P Pre. Art. */
#define ALC_T0T_DIF_FIL      4            /* T0   : Different C-P Pre. Fil. */
#define ALC_T0T_DIF_VEN      5            /* T0   : Different C-P Pre. Ven. */
#define ALC_T0T_DIF_DRA      6            /* T0   : Different C-P Wei. Uf   */
#define ALC_T0T_AIR_TST      7            /* T0   : Air Test                */
#define ALC_T0T_BLD_TST      8            /* T0   : Bld Test                */
/* ... */
#define ALC_T1S_PRE_ART      9            /* T1-S : Arterial  Pressure      */
#define ALC_T1S_PRE_FIL     10            /* T1-S : Filter    Pressure      */
#define ALC_T1S_PRE_VEN     11            /* T1-S : Venous    Pressure      */
#define ALC_T1S_WEI_DRA     12            /* T1-S : Weight Uf non zero      */
#define ALC_T1S_AIR_ALA     13            /* T1-S : Air Alarm               */
#define ALC_T1S_BLD_ALA     14            /* T1-S : Bld Alarm               */
#define ALC_T1S_ERR_DAT     15            /* T1-S : Dati                    */
#define ALC_T1S_ERR_CAL     16            /* T1-S : Calibrazioni            */




/* ... */
#define ALC_T1D_REL_CLA     17            /* T1-D : Relee Clamp             */
#define ALC_T1D_REL_BLO     18            /* T1-D : Relee Blood     Pump    */
#define ALC_T1D_REL_DRA     19            /* T1-D : Relee Dra       Pump    */
#define ALC_T1D_PRE_PRZ     20            /* T1-D : Pressurization  Failed  */
#define ALC_T1D_PRE_VEN     21            /* T1-D : Venous Pressure Miscal. */
#define ALC_T1D_WS2_TIM     22            /* T1-D : Priming Timeout         */
/* ... */
#define ALC_AIR_VEN_LIN     23            /* Air  : Air in Venous Line      */
/* ... */
#define ALC_BLD_BLO_LEA     24            /* Bld  : Blood Leakage           */
#define ALC_BLD_TUB_INS     25            /* Bld  : Tube not Inserted       */
#define ALC_BLD_AMB_LIG     26            /* Bld  : Ambient Light too High  */
/* ... */
#define ALC_T0T_DIF_PLA     27            /* T0   : Different C-P Pre. Pla. */
#define ALC_FLW_UF_MIN      28            /* Flw  : UF minima               */
#define ALC_FLW_PLA_MIN     29            /* Flw  : Linea Plasma bloccata   */
/* ... */
#define ALC_PRE_ART_MIN     30            /* Pre  : Minimum Arterial Press. */
#define ALC_PRE_ART_MAX     31            /* Pre  : Maximum Arterial Press. */
#define ALC_PRE_FIL_MIN     32            /* Pre  : Minimum Filter   Press. */
#define ALC_PRE_FIL_MAX     33            /* Pre  : Maximum Filter   Press. */
#define ALC_PRE_VEN_MIN     34            /* Pre  : Minimum Venous   Press. */
#define ALC_PRE_VEN_MAX     35            /* Pre  : Maximum Venous   Press. */
#define ALC_PRE_ART_MOD     36            /* Pre  : Modif.  Arterial Press. */
#define ALC_PRE_FIL_MOD     37            /* Pre  : Modif.  Arterial Press. */
#define ALC_PRE_VEN_MOD     38            /* Pre  : Modif.  Arterial Press. */
#define ALC_PRE_DRO_MAX     39            /* Pre  : Maximum Drop     Press. */
#define ALC_PRE_TMP_MAX     40            /* Pre  : Maximum Tmp      Press. */
/* ... */
#define ALC_WEI_DRA_EXC     41            /* Wei  : UF        Excessive     */
#define ALC_WEI_DRA_VAR     42            /* Wei  : UF        Variation     */
/* ... */
#define ALC_CLA_SWI_CLO     43            /* Cla  : Switch not Closed       */
#define ALC_CLA_SWI_OPE     44            /* Cla  : Switch not Opened       */
/* ... */
#define ALC_PMP_BLO_COV     45            /* Blo  : Cover Opened            */
#define ALC_PMP_BLO_STO     46            /* Blo  : not Stopped             */
#define ALC_PMP_BLO_RUN     47            /* Blo  : not Running             */
#define ALC_PMP_BLO_SPE     48            /* Blo  : Speed Error             */
/* ... */
#define ALC_PMP_DRA_COV     49            /* Uf   : Cover Opened            */
#define ALC_PMP_DRA_STO     50            /* Uf   : not Stopped             */
#define ALC_PMP_DRA_RUN     51            /* Uf   : not Running             */
#define ALC_PMP_DRA_SPE     52            /* Uf   : Speed Error             */
/* ... */
#define ALC_STE_HEP_OVE     53            /* Ste  : Heparin Overload        */
#define ALC_STE_HEP_BLO     54            /* Ste  : Heparin   Motor Blocked */
/* ... */
#define ALC_APP_BLO_2MS     55            /* App  : Blood Stop for 2 Minutes*/
#define ALC_APP_HEP_SYR     56            /* Ste  : Heparin Syringe Absent  */
#define ALC_APP_HEP_END     57            /* App  : Heparin End             */
#define ALC_APP_BAG_DRA     58            /* App  : Bag Drain     Full      */

/* ... */
#define ALC_PRE_PLA_MAX     59            /* Pre  : Pressione Plasma Max    */
#define ALC_PRE_PLA_MIN     60            /* Pre  : Pressione Plasma Max    */
#define ALC_PRE_UF_MAX      61            /* Pre  : Pressione Plasma Max    */
#define ALC_PRE_UF_MIN      62

#define ALC_LIBERO          63            /* LIBERO                         */

   /*
      Protective Alarms
   */
#define ALP_FAT_ERR		    64            /* Fatal error protective         */
#define ALP_T1S_ERR_CAL     65            /* T1-S : Calibrazioni 1          */
#define ALP_T0T_CON_RUN     66            /* T0   : Controllo Running       */
#define ALP_T0T_DIF_BLD     67            /* T0   : Different C-P BLD Signal*/
#define ALP_TENSIONI        68            /* T0   : Monitor Tensioni        */
#define ALP_CALO_POND       69            /* Dra  : Calo Ponderale          */

   /*
      Warnings
   */
#define WRN_TEMP_NOT_WRK    70            /* Scheda Temperatura non comunica */
#define WRN_TEMP_PROBE      71            /* Temperatura overrange low      */
#define WRN_NEG_PRE         72                /* Negative Pressure       */
#define WRN_PMP1_STO        73                /* Pompa 1 basso flusso           */ //MEDIA
#define WRN_PMP2_STO        74                /* Pompa 2 basso flusso           */ //MEDIA
#define WRN_TEMP_MAX        75                /* Temperatura massima            */ //MEDIA
#define WRN_TEMP_MIN        76                /* Temperatura minima             */ //MEDIA


#define WRN_SERVICE_DUE     77                /* Fine Sacca LavAggio            */ //BASSA
#define WRN_SERVICE_OVERDUE 78                /* Override pressione venosa      */ //BASSA
#define WRN_MIN_FLW_PMP2    79                /*                                */ //BASSA
#define WRN_CHA_SET         80                /* Change Set                     */ //BASSA


#define WRN_COL_CIR         81                /* Collegare circuito             */ //BASSA
#define WRN_PRI_TST         82                /* Test in corso                  */ //BASSA
#define WRN_OFF_PRE         83                /* Offset pressioni assente       */ //BASSA
#define WRN_TAG_END         84                /* TAG esaurito                   */ //BASSA
#define WRN_TAG_MIS         85                /* TAG non corretto               */ //BASSA
#define WRN_TAG_NOT         86                /* Manca TAG                      */ //BASSA
#define WRN_MIN_FLW_PMP1    87                /* Venosa minima	                */ //BASSA
#define WRN_PRI_PRE         88                /* Priming Trasduttore Delivery   */ //BASSA
#define WRN_BAT_VOL         89                /* Voltage Battery                */ //BASSA
#define WRN_CODE_ERR        90                /* Code Error                     */ //BASSA
#define WRN_SERVICE_T_OUT   91                /* Timeout Service                */ //BASSA

#define ALA_MAX             92                /* Maximum Number of Alarms       */

/* ... */

/*Alarm Acknoledges*/
#define ALA_ACK_AUT          0               /* Ack. Automatic              */
#define ALA_ACK_AQ1          1               /* Ack. After 1 push of 'AQ'   */
#define ALA_ACK_AQ2          2               /* Ack. After 2 push of 'AQ'   */

/*Alarm Lock-Unlocks*/
#define ALA_LOC_NUL       0x00               /* Lock-Unlock Nul             */
#define ALA_LOC_BLO       0x01               /* Lock-Unlock Blood           */
#define ALA_LOC_FLU       0x02               /* Lock-Unlock Fluid           */
#define ALA_LOC_ALL       0x03               /* Lock-Unlock All (Blo.+Flu.) */

/*Alarm Handler States*/
#define ALA_STA_ABS          0               /* Absence  of Alarms          */
#define ALA_STA_PRE          1               /* Presence of Alarms          */
#define ALA_STA_SIL          2               /* Silence  of Present Alarms  */

/*Single Alarm States*/
#define ALA_ALA_STA_ABS      0               /* Absent                      */
#define ALA_ALA_STA_PRE      1               /* Present                     */
#define ALA_ALA_STA_MEM      2               /* Memorized                   */

/*Alarms timeout*/
#define ALA_DRV_GES_AIR_WAIT_TIME      5000   /*Do not start DrvAIR_Ges() before this time - Milliseconds*/

   /*************************************
      Comandi dal master
   *************************************/
#define SLV_BASE_CMD            0
#define	CAL_ART_ZERO	  		 1
#define	CAL_ART_GAIN	      	 2
#define	CAL_FIL_ZERO	  		 3
#define	CAL_FIL_GAIN	      	 4
#define	CAL_VEN_ZERO	  		 5
#define	CAL_VEN_GAIN	      	 6
#define	CAL_GAUGE_ZERO	  		 7
#define	CAL_GAUGE_ZERO_GAIN		 8
#define	CAL_GAUGE_GAIN	  		 9
#define	START_TRATT		    	10
#define	TARA_TRATT	    	  	11
#define RESET_CSI_SLAVE         12
#define RESET_ALA_SLAVE         13
#define	CAL_UF_ZERO	  			14
#define	CAL_UF_GAIN	    	  	15
#define OFFSET_PRESSIONI        16
#define RESET_OFFSET_PRESSIONI  17


#define	OFS_POINTS_CH0  		0x0100	// calibrazioni canale 0 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH1   		0x0140	// calibrazioni canale 5 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH2  		0x0180	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH3  		0x01C0	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH4  		0x0200	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH5  		0x0240	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH6  		0x0280	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH7  		0x02C0	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH8  		0x0300	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH9  		0x0340	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH10  		0x0380	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH11 		0x03C0	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define	OFS_POINTS_CH12 		0x0400	//
#define	OFS_POINTS_CH13 		0x0440	//

#define	OFS_POINTS_DAT  		0x0500	// calibrazioni canale 6 (6*8=48bytes usati + 2 per crc)
#define ALARM_DELAY_TIMER       10000    // Waiting start timer before setting alarms - Milliseconds
#define GESXFR_MAX_PKT_LOSS     30
#define GES_ALA_WAIT_CYCLE      20      // Nummber of main cycle to wait before setting the alarms - It depends on the boot speed

#endif
