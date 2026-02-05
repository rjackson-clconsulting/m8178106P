/**
@file		gesXfr_p.h
@brief		Module to implement SPI communication with controller micro (used in the protective one)
@author		Fernando Morani
@date		10/04/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _GESXFR_P_H
#define _GESXFR_P_H

#include "stdDataTypes.h"
#include "globalAppDefine.h"
#include "COMBUF.H"
#include "SIOFM3.H"
#include "CSI.H"
#include "ADC.h"
#include "DECODE.H"
#include "sevenSegDrv.h"    //Tu use the 7Seg Display as visual debug
#include "Cd_ALA.H"


#define INTERCOM_STATE_SEND_BLOCK  0
#define INTERCOM_STATE_READY_TO_RX 1
#define INTERCOM_STATE_DATA_CHK    2
#define INTERCOM_STATE_WAITING     3
#define INTERCOM_STATE_TMT_EXPIRED 4
#define INTERCOM_STATE_NOT_DEFINE  5
#define BLOCK_START_VAL            0x81
#define BLOCK_END_VAL              0x18
#define PAK_NULL_DEFAULT           0
#define PAK_NULL_MAX_NUM           1000
#define SPI_SEND_SCHEDULED_MS      100 // ms every transmission. It should take between 72 and 80 ms for the complete tranfer

#define CSI_TX_BUF_SIZE             100
#define CSI_RX_BUF_SIZE             100
#define CSI_TX_TRESHOLD             1
#define CSI_RX_TRESHOLD             100

#define WAIT_TIME_BEFORE_START      500
#define WAIT_TIME_BEFORE_ENABLE     5000
#define WAIT_TIME_BASE              10


/**********************************************************
**     DEFINE STRUCTURE SECTION
***********************************************************/

struct CsiXfrBlockRx{
	byte	 	codeSttBlock;			// dato di avvio del blocco
	byte	 	cntBlockTop;			// contatore dei blocchi lato start

	byte        cmd;					// comando
	byte	    ala_act;				// allarme attivo

	byte	    ala_bit[(ALA_MAX/8)+1];			// allarme attivo a bit

	byte        bld_tst;				// bld test
	byte		bld_state;				// bld state
	byte		bld_cal;				// bld cal

    word    	PsFlow;
    word	    UfFlow;
    word    	Wct;
    long	    Wcr;

	long	    P_Art_Min;
	long	    F_Venosa;

	long		dat1;
	long		dat2;
	long		dat3;
	long		dat4;
	long		dat5;
	long		dat6;

	long		Durata_Tratt;
	long		Lim_Ven_Min;

    byte		Trattamento;

	byte		disable_pa;
	byte		disable_cp;
	byte		disable_bld;
	byte		disable_air;
	byte		disable_pv;
	byte		disable_pf;
	byte		disable_scale;

	byte		ind_dat;				// indice per i 6 long sotto
	byte		ala_loc_flu;
	byte		bld_cal_sta;
	byte		hiflow;

    byte		Stato;
    byte		Phase;


	byte 	cntBlockBot;			// contatore dei blocchi lato end
	byte 	codeEndBlock;			// dato di fine del blocco
};
extern CsiXfrBlockRx chkBlock;		// bloccco dati ricevuti dal micro di controllo per confronto


struct CsiXfrBlockTx{
	byte	 	codeSttBlock;				// dato di avvio del blocco
	byte	 	cntBlockTop;				// contatore dei blocchi lato start

	byte        l1;
	byte	    ala_act;			   		// allarme attivo
	byte	    ala_bit[(ALA_MAX/8)+1];		// allarme attivo a bit
	byte        l2;
	byte		l3;
	byte		l4;

    word    	l5;
    word	    l6;
    word    	l7;
    long	    l8;

	long	    teoric_calop;
	long	    totale_calop;


	long		dat1;
	long		dat2;
	long		dat3;
	long		dat4;
	long		dat5;
	long		dat6;

	long		durata_tratt;
	long		b0;

	byte		b1;
	byte		b2;
	byte		b3;
	byte		b4;
	byte		b5;
	byte		b6;
	byte		b7;
	byte		b8;

	byte		ind_dat;				// indice per i 6 long
	byte		fatal_num;
	byte		cal_num_pro;
	byte		cla;

    byte		l12;
    byte		l13;


	byte 	cntBlockBot;			// contatore dei blocchi lato end
	byte 	codeEndBlock;			// dato di fine del blocco
};
extern CsiXfrBlockTx bigBlock;		// blocco dati personale




/**********************************************************
**     FUNCTION DECLARATION SECTION
***********************************************************/
uint8_t GesXfr_P();
uint8_t IniXfr_P(void);

#endif
