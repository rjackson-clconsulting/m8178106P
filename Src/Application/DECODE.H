//---------------------------------------------------------------------------//
//                                                                           //
/**
@file		decode.h
@brief		Manage the decode-encode serial protocol (former release from COMETA)
@author		Fernando Morani
@date		16/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#include "global.h"
//#include "iodef.h"
#include "error.h"
#include "combuf.h"
#include "uart.h"
#include "alloc.h"
//#include "adcsb1.h"
#include "swtim.h"
//#include "cd_ala.h"
//#include "main.h"
//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define NUMBER_ID	4
                               /* codici di controllo seriale */
#define STX         0x02
#define ETX         0x03
#define ACK         0x06
#define NAK         0x15

                           		    /* macro-definizioni dei dati nei buffer rx/tx */
#define ma_gr_id    rx_buffer[0]    /* Identificativo Gruppo Master */
#define ma_no_id    rx_buffer[1]    /* Identificativo Numero Master */
#define sl_gr_id    rx_buffer[2]    /* Identificativo Gruppo Slave  */
#define sl_no_id    rx_buffer[3]    /* Identificativo Numero Slave */
#define com_n_ser   rx_buffer[4]    /* Numero di Comunicazione */
#define len_l_pack  rx_buffer[5]    /* lunghezza pacco */
#define len_h_pack  rx_buffer[6]    /* lunghezza pacco  */
#define cmd_rx      rx_buffer[7]    /* Comando in ricezione */
#define cmd_tx      tx_buffer[7]    /* Comando in trasmissione */
#define drx_ser     (rx_buffer+8)   /* inizio parametri ricevuti */
#define dtx_ser     (tx_buffer+8)   /* inizio parametri da trasm. */

#define	FLASH_KEEP_LOAD	(dword *) 0x000008		// indirizzo attuale salto sosta nel loader

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

/* Stati del protocollo seriale
*/
enum STATIProt{
        PR_RX_AND_DECODE,
        PR_EXEC_SPEC_CMD,
        PR_EXEC_ILLEGAL_CMD,
        PR_WAIT_TX_END,
        PR_TX_ERR
	};



/* Codici di comando del protocollo seriale
*/
enum SERCMDCodes {
		SERCMD_ACK=6,            		// Codice del Comando ACK
		SERCMD_SET_ID=11,            	// Codice del Comando SET_ID
		SERCMD_GET_ID=12,            	// Codice del Comando GET_ID

		SERCMD_TST_COM=16,          	// test comunicazione

		SERCMD_CH_BAUD=0x16,            // Cambia il Baud Rate
		SERCMD_WR_CODE=0x17,            // scrittura codice apparecchio

								// comandi gruppo 1: trasferimento dati
								////////////////////////////////////////////////////////////
		SERCMD_WR_CICLO=32,
		SERCMD_RD_CICLO,
		SERCMD_FLASH_CICLO,

		SERCMD_WR_STEP,
		SERCMD_RD_STEP,

		SERCMD_WR_STEP_TAB,
		SERCMD_RD_STEP_TAB,

		SERCMD_RD_RESULT,

								// comandi gruppo 2: controllo esecuzione
								////////////////////////////////////////////////////////////
		SERCMD_SET_FASE=60,        	/* Codice del Comando SET_FASE    */
		SERCMD_SET_PMP,            	/* Codice del Comando SET_PMP     */
		SERCMD_SET_STE,            	/* Codice del Comando SET_STE     */
		SERCMD_SET_TMP,            	/* Codice del Comando SET_TMP     */
		SERCMD_SET_DAT,           	/* Codice del Comando SET_DAT     */
		SERCMD_GET_ACT,            	/* Codice del Comando GET_ACT     */
		SERCMD_SEL_CICLO,
		SERCMD_STT_CICLO,
		SERCMD_STT_CICTAB,
		SERCMD_STP_CICLO,
		SERCMD_RES_CICLO,
		SERCMD_TST_CICLO,

		SERCMD_SET_LAN,           	/* Codice del Comando SET_LAN     */
		SERCMD_PRI_REP,     		/* Codice del Comando PRI_REP     */
		SERCMD_SET_FLG,             /* Codice del Comando SET_FLG     */
		SERCMD_SET_SET,             /* Codice del Comando SET_SET     */
		SERCMD_SET_STA,             /* Codice del Comando SET_STA     */
		SERCMD_SET_PHA,             /* Codice del Comando SET_PHA     */
		SERCMD_GET_DEBUG_PAR,       /* Codice del Comando DEB_STS     */
		SERCMD_DEB_COP,             /* Codice del Comando DEB_COP     */
		SERCMD_DEB_ERR,             /* Codice del Comando DEB_ERR     */
		SERCMD_SET_ALA,             /* Codice del Comando SET_ALA     */
		SERCMD_SET_OUT,             /* Codice del Comando SET_OUT     */
		SERCMD_SET_CAL,             /* Codice del Comando SET_CAL     */

								// comandi gruppo 3: controllo (manuale/diretto) dispositivi
								////////////////////////////////////////////////////////////
		SERCMD_RD_INP=100,
		SERCMD_RD_OUT,
		SERCMD_WR_OUT,
//		SERCMD_SET_OUT,
		SERCMD_RES_OUT,
		SERCMD_RD_ADC,
		SERCMD_RD_DAC,
		SERCMD_WR_DAC,

								// comandi gruppo 4: calibrazioni e par. di calibrazione
								////////////////////////////////////////////////////////////
		SERCMD_RD_PAR_ADC=140,
		SERCMD_WR_PAR_ADC,
		SERCMD_RD_PAR_CMP,
		SERCMD_WR_PAR_CMP,
		SERCMD_RD_PAR_DAC,
		SERCMD_WR_PAR_DAC,
		SERCMD_RD_PAR_STEP,
		SERCMD_WR_PAR_STEP,
		SERCMD_RD_PAR_DCMOT,
		SERCMD_WR_PAR_DCMOT,
		SERCMD_RD_PAR_STAB,
		SERCMD_WR_PAR_STAB,
		SERCMD_SET_TIME,           			/* Codice del Comando SET_TIM: imposta l'orologio/datario del microbo */
		SERCMD_GET_TIME,           			/* Codice del Comando GET_TIM: legge l'orologio/datario del microbo */

		SERCMD_ADC_ZERO,
		SERCMD_ADC_SPAN,

								// comandi gruppo 6: comandi speciali di base del gestore protocollo seriale
								////////////////////////////////////////////////////////////
		SERCMD_RD_BYTE=201,	                /* lettura di uno o piu` bytes ad indirizzi diversi */
		SERCMD_WR_BYTE,		                /* scrittura di uno o piu` bytes ad indirizzi diversi */
		SERCMD_RD_BLOCK,		            /* lettura di un blocco di bytes contigui */
		SERCMD_WR_BLOCK,		            /* scrittura di un blocco di bytes contigui */

		SERCMD_CHK_ST=209,         	        /* richiesta di alcuni bytes di stato */
		SERCMD_CHK_FULL_ST=210,         	/* richiesta del pacco completo delle info di stato */
		SERCMD_DEB_STS,            			/* Codice del Comando DEB_STS: ritorna il numero di revisione e lo stato della macchina     */
  											/* Dati aggiuntivi: nessuno				*/
											/* Dati restituiti: 				*/
											/* 	-xxxxx-  xxxxxx 	-  struttura di stato */
		SERCMD_DO_RESET,            		/* Codice del Comando RESET       */
		SERCMD_RESET,        	           	/* Codice del Comando RESET : esegue un salto all'indirizzo 0x00000 eseguendo un reset sw   */
  											/* Dati aggiuntivi: nessuno				*/

								// comandi gruppo 5: selfprogramming NEC
								////////////////////////////////////////////////////////////

                                    /* comandi speciali per prog. micro NEC */
        SERCMD_SELF_BLKCHK = 220,           /* Codice del Comando "blank_check" : verifica che tutte le celle siano cancellate = 0xFF */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il check */
											/* 	-dword- StartAddr	-  indirizzo di partenza controllo blankchk */
											/* 	-dword- EndAddr		-  indirizzo di arrivo controllo blankchk  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_PREWRITE,               /* Codice del Comando "prewrite" : mette a zero tutte le celle di flash per poter cancellare */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il pre-write */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_WRITEBACK,              /* Codice del Comando "writeback" : prova ad "equalizzare" le celle */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il write back */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

											/* !!!!! Comando principale da utilizzare per la cancellazione !!!!!*/
        SERCMD_SELF_CHIPERASE,	            /* Codice del Comando "chipErase" : cancella tutta la flash + riscrive il loader */
											/* Dati aggiuntivi: 	nessuno	*/
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_WRITEBLOCK,             /* Codice del Comando "writeBlock" : scrive un blocco di dati in flash */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati da scrivere */
											/* 	-dword-  Size    	-  numero di bytes da scrivere eg. dimensione del blocco trasferito */
											/* 	-byte[]- DataBlock 	-  blocco di dati da scrivere a seguire  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_READBLOCK,              /* Codice del Comando "readBlock" : legge un blocco di dati dalla */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati da leggere */
											/* 	-dword-  Size    	-  numero di bytes da leggere eg. dimensione del blocco da trasferire */
											/* Dati restituiti: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati letti */
											/* 	-dword-  Size    	-  numero di bytes in transfer */
											/* 	-byte[]- DataBlock 	-  dati a seguire  */

        SERCMD_SELF_VERIFBLOCK,             /* Codice del Comando "verifBlock" : verifica un blocco di dati in flash con i dati passati */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati da scrivere */
											/* 	-dword-  Size    	-  numero di bytes da scrivere eg. dimensione del blocco trasferito */
											/* 	-byte[]- DataBlock 	-  blocco di dati da scrivere a seguire  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_SIGNREAD,           	/* Codice del Comando "signatureRead" : legge il codice interno del dispositivo */
											/* Dati aggiuntivi: 	nessuno	*/
											/* Dati restituiti: 				*/
											/* 	-xxxx-  code 		-  codice del dispositivo e altri ciappini */

        SERCMD_SELF_CTRLVPP,                /* Codice del Comando "setVPP" : esegue il controllo dell'alimentazione per la prog. */
											/* Dati aggiuntivi: 				*/
											/* 	-byte-  Enable -  se =0 disattiva VPP, se <> 0 attiva VPP */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_AREAERASE,              /* Codice del Comando "areaErase" : esegue l'erase di un'area con il prewrite implicito */
											/* Dati aggiuntivi: 				*/
											/* 	-byte-  Area -  numero di area su cui eseguire l'erase con prewrite automatico */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_FLASHERASE,             /* Codice del Comando "flashErase" : esegue il solo comando di erase (da usare con molta delicatezza) */
											/* Dati aggiuntivi: 				*/
											/* 	-byte-  Area -  numero di area su cui eseguire l'erase brutale */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_REWRLOADER,             /* Codice del Comando "rewriteLoaqder": forza la copiatura del loader dalla ram alla flash */
											/* Dati aggiuntivi: 	nessuno	*/
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_INTERNVRF,              /* Codice del Comando "internalVerify": comando internal verify */
											/* Dati aggiuntivi: 				*/
											/* 	-byte-  Area -  numero di area su cui eseguire l'erase brutale */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_SETPROGPAR,             /* Codice del Comando "setparam": imposta i parametri di programmazione */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Freq 	   		-  frequenza di clock del micretto (in Hz) */
											/* 	-dword-  WriteTime 		-  tempo di scrittura del micretto (in uSec)  */
											/* 	-dword-  EraseTime 		-  tempo di cancellazione del micretto (in uSec)  */
											/* 	-dword-  WriteBackTime 	-  tempo di write-back del micretto (in uSec)  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */

        SERCMD_SELF_JMPTOAPP,               /* Codice del Comando JMPTOAPP: forza il salto dal loader all'applicazione all'indirizzo richiesto */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address   		-  indirizzo salto nell'app utente: se = 0xFFFF.FFFF -> salta all'indirizzo di default dell'app.  */
											/* Dati restituiti: nessuno				*/

        SERCMD_SELF_JMPTOLOAD,              /* Codice del Comando JMPTOLOAD: forza il salto dall'applicazione al loader */
											/* Dati aggiuntivi: nessuno (usa indirizzo loader di default) */
											/* Dati restituiti: nessuno				*/

		SERCMD_LOAD,		        		/* Codice del Comando LOAD        */

        SERCMD_WR_FLASH=250,               	/* scrittura di un blocco di bytes */
        SERCMD_JP_RAM=251,         	        /* Copia e salta in Ram */

		SERCMD_JP_LOAD=252,    	            /* Salta al Load */
		SERCMD_COP_RST=254,	                /* test di reset del COP */
		SERCMD_SYS_RST=255,	                /* test di reset del systema */

		END_CMD
	};




struct TestComInfo{
    byte    test;              	/* contatore prove eseguite */
    long    Count1;             /* conteggio stepper 1 */
    long    Count2;             /* conteggio stepper 2 */
    float   Count3;             /* conteggio stepper 2 */
    float  	Count4;             /* conteggio stepper 2 */
	};

typedef struct TestComInfo TestComInfo;


struct SystemStatus{
        dword   ChkProg;				// chk programmato in memoria
        dword   ChkCalc;				// chk calcolato
        dword	ChkLoader;				// chk calcolato del loader

        long   	MaxScanMain;			// tempi di scansione del main e dintorni
        long   	ActScanMain;
        long   	MaxScanComm;
        long   	ActScanComm;
        word   	MaxScanTick;
        word   	ActScanTick;

        word	LoaderPlease;			// flag per richiesta stop nel loader
        byte	ErrorLevel;				// errore di sistema
		byte 	ResetCause;				// flags riepilogo condizioni di reset

		byte 	InRam;					// flag controllo trasferito al loader in ram
		byte	JpDefault;				// flag controllo reinit parametri al valore di default
		word 	sector_erased_flags;	// flags riepilogo settori cancellati
    };

typedef struct SystemStatus SystemStatus;



struct FullSystemStatus{
        char    SwVersLoader[16];		// numero di revisione del loader
        char    SwNameLoader[32];		// nome del softw. di loading

        char    SwVersApp[16];			// numero di revisione del softw.
        char    SwNameApp[32];			// nome del softw.

        dword   ChkProg;				// chk applicativo programmato in memoria
        dword   ChkCalc;				// chk applicativo calcolato
        dword	ChkLoader;				// chk calcolato del loader

        long   	MaxScanMain;			// tempi di scansione del main e dintorni
        long   	ActScanMain;
        long   	MaxScanComm;
        long   	ActScanComm;
        word   	MaxScanTick;
        word   	ActScanTick;
        long   	SystemTickCount;

        word	LoaderPlease;			// flag per richiesta stop nel loader
        byte	ErrorLevel;				// errore di sistema
        byte	ResetCause;				// causa del reset di sistema

		word 	InRam;					// flag controllo trasferito al loader in ram
		byte	JpDefault;				// flag controllo reinit parametri al valore di default
		word	sector_erased_flags;	// flags riepilogo settori cancellati
    };

typedef struct FullSystemStatus FullSystemStatus;



//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

void proto_init(UARTHandle hUartDevB) 	;
void protoEntry(void)					;
bool protoRxAndDecode(void);
bool protoExecSpecCmd(void)				;
bool protoExecIllegalCmd(void)			;
void err_protoc(void)					;
bool ExecAckCmd(void)					;
bool ExecChgBaudCmd(void)				;
bool ExecWrCodeCmd(void)				;
bool ExecRdByteCmd(void)				;
bool ExecWrByteCmd(void)				;
bool ExecRdBlockCmd(void)				;
bool ExecWrBlockCmd(void)				;
bool ExecChkStatusCmd(void)				;
bool ExecChkFullStatusCmd(void)			;
bool ExecJmpToLoaderCmd(void)			;
bool ExecCopResetCmd(void)				;
bool ExecSysResetCmd(void)				;

//////////////////////////////////////////////////
// Comandi speciali per caricamento su microbo NEC
//////////////////////////////////////////////////

bool ExecSelfBlkChk(void)				;
bool ExecSelfPreWrite(void)				;
bool ExecSelfWriteBack(void)			;
bool ExecSelfChipErase(void)			;
bool ExecSelfWriteBlock(void)			;
bool ExecSelfReadBlock(void)			;
bool ExecSelfVerifBlock(void)			;
bool ExecSelfSignature(void)			;
bool ExecSelfCtrlVpp(void)				;
bool ExecSelfAreaEr(void)				;
bool ExecSelfFlashEr(void)				;
bool ExecSelfRewrLoader(void)			;
bool ExecSelfInternVerif(void)			;
bool ExecSelfParam(void)				;
bool ExecSelfJmpToApp(void)				;
bool ExecSelfJmpToLoader(void)			;


bool protoEncodeAndTx(word dataSize)	;

bool protoExecUserCmd(void)				;

bool UserTestCom(void)					;
bool UserWrCicloCmd(void)				;
bool UserRdCicloCmd(void)				;
bool UserFlashCicloCmd(void)			;
bool UserWrStepCmd(void)				;
bool UserRdStepCmd(void)				;
bool UserWrStepTabCmd(void)				;
bool UserRdStepTabCmd(void)				;
bool UserRdResultCmd(void)				;
bool UserSelCicloCmd(void)				;
bool UserStartCicloCmd(void)			;
bool UserStartCicTabCmd(void)			;
bool UserStopCicloCmd(void)				;
bool UserResetCicloCmd(void)			;
bool UserTestCicloCmd(void)				;
bool UserRdInputCmd(void)				;
bool UserRdOutputCmd(void)				;
bool UserWrOutputCmd(void)				;
bool UserSetOutCmd(void)				;
bool UserResOutCmd(void)				;
bool UserRdAdcCmd(void)					;
bool UserRdDacCmd(void)					;
bool UserWrDacCmd(void)					;
bool UserRdParAdcCmd(void)				;
bool UserWrParAdcCmd(void)				;
bool UserRdParCmpCmd(void)				;
bool UserWrParCmpCmd(void)				;
bool UserRdParDacCmd(void)				;
bool UserWrParDacCmd(void)				;
bool UserRdParStepCmd(void)				;
bool UserWrParStepCmd(void)				;
bool UserRdParDcMotCmd(void)			;
bool UserWrParDcMotCmd(void)			;
bool UserRdParStabCmd(void)				;
bool UserWrParStabCmd(void)				;
bool UserAdcZeroCmd(void)				;
bool UserAdcSpanCmd(void)				;

//////////////////////////////////////////////////
// aux
dword xctLong(byte *pdata)								 ;
void  ramcpy(void *psrc, void *pdst, int size)			 ;
void romstrcpy(char *pfDst, const char *pfSrc, int size) ;

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern SystemStatus SysStatus;                 /* stato di funzionamento */
extern UARTHandle hUartDevA;



