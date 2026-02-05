/**
@file		globalApplication.h
@brief		Module to implement global routine to be used from application
@author		Fernando Morani
@date		03/04/2017
@version	01.00.01
@source     VetSmart Protective Application
Notes on version 01.00.01
    Add support for loader s/w engine
*/

#ifndef _GLOBALAPPLICATION_H_
#define _GLOBALAPPLICATION_H_

#include "mb9bf50x.h" //FM3 definitions (CMIS layer)
#include "adcMapFlexiP.h"
#include "UART.H"
#include "SwTim.h"
#include "gesXfr_p.h"
#include "mftEncoderPSUF.h"
#include "cd_air.h"
#include "cd_pre.h"
#include "cd_ala.h"
#include "key.h"
#include "adc.h"
#include "eeprom.h"

#ifdef LOADER_ENGINE
#include "COMPROTO_LOADER.h"    //Ver. 01.00.01
#endif //LOADER_ENGINE


/**********GLOBAL STRUCT SECTION ******
***************************************/
enum StatoStaz{
				STATE_NUL,                 	// Null > visualizza logo
				STATE_IDL,                 	// Idle
				STATE_MON_LIN,             	// Montaggio linee
				STATE_PRI,					// Priming
				STATE_TEST, 				// Test
				STATE_TRE,					// Trattamento
				STATE_STACCO				// Stacco
			  };

enum PhaseStaz {
                PHASE_IDL      =0,
				PHASE_TST_REL_STO=10,
				PHASE_TST_REL_RES,
				PHASE_TST_REL_STA,

				PHASE_PRI_STO_1=20,
				PHASE_PRI_RES_1,
				PHASE_PRI_STA_1,

				PHASE_PRI_STO_1_TPE=30,
				PHASE_PRI_RES_1_TPE,
				PHASE_PRI_STA_1_TPE,

				PHASE_PRI_STO_2=40,
				PHASE_PRI_RES_2,
				PHASE_PRI_STA_2,

				PHASE_PRI_STO_2_TPE=50,
				PHASE_PRI_RES_2_TPE,
				PHASE_PRI_STA_2_TPE,

				PHASE_PRI_STO_3=60,
				PHASE_PRI_RES_3,
				PHASE_PRI_STA_3_1,
				PHASE_PRI_STA_3_2,
				PHASE_PRI_STA_3_3,
				PHASE_PRI_STA_3_4,

				PHASE_PRI_STO_4=70,
				PHASE_PRI_STA_4,

				PHASE_PRI_END=80,

				PHASE_TEST_MSG_1=90,
				PHASE_TEST_MSG_2=100,
				PHASE_TEST_MSG_3=110,
				PHASE_TEST_TRASDUTTORE_PORTA=120,

				PHASE_TEST_TRASDUTTORE_ARTERIA=130,

				PHASE_TRE_ATT_STO=140,
				PHASE_TRE_ATT_RES,
				PHASE_TRE_ATT_STA,

				PHASE_TRE_STO=150,
				PHASE_TRE_RES,
				PHASE_TRE_STA,

				PHASE_STACCO_STO=160,
				PHASE_STACCO_RES,
				PHASE_STACCO_STA
			  };


struct dat{
    long    Gauge1_ini;
    long    Offset_P2;
    long    durata_tratt;
	long	cumu_calop;
    long    Offset_Art;
    long    Offset_Fil;
    long    Offset_Ven;
    long    Offset_Art2;
};

/**********DEFINE SECTION ******
***************************************/


/**********FUNCTION DECLARATION SECTION
 *******************************************/
void global_var_init();
void hwAppInit();
uint8_t mainAppInit();
uint8_t mainAppRun();
uint8_t blinkLedLife();
void Ges_rele(void);
void Ges_app(void);
void Ges_ala_adc(void);
void Ges_key(void);
void Ges_ala_pmp(void);
uint8_t Ges_cmd(void);
bool WriteCalPressione(CalPoint *CalRun1,CalPoint *CalRun2,UBYTE qpres);
bool WriteCalBilancia(CalPoint *CalRun1,UBYTE num_point1,CalPoint *CalRun2,UBYTE num_point2);
void WriteDati(void);
void SetupDati(void);
void Treat_Time_Ges(void);
void Offset_Pressioni(void);
void Reset_Offset_Pressioni(void);

#ifdef LOADER_ENGINE
uint8_t loaderInit();   //File Ver. 01.01
void protocol_init();   //File Ver. 01.01
#ifdef LOADER_ENGINE_INT
void uartRxFromControllerIrq();
#endif
#endif //LOADER_ENGINE


#endif
