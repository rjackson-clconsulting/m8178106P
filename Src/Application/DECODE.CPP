/**
@file		decode.cpp
@brief		Manage the decode-encode serial protocol (former release from COMETA)
@author		Fernando Morani
@date		16/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#include "decode.h"
/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/
extern DecTimer timLoad;	             // timer override vrain pressure
extern char sAppRevision[];
extern char sAppDate[];
extern char sAppName[];

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------
word lung_rx;				// conteggio lunghezza in ricezione
word tot_rx;				// conteggio num cacatteri ricevuti
word lung_tx;              	// lunghezza della trasmissione
word rx_mem;
word rx_tot;
word tx_mem;
byte rx_etx;				// flag fine ricezione con ETX incamerato

uint16_t NEW_BAUD;
byte GROUP_ID;
byte stato_rx;				// stato esecuzione ricezione
byte err_ges;				// stato errore gestore protocollo seriale

SystemStatus SysStatus;		// stato di funzionamento

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

#define MAGIC_JUMP		0x55AABBCC
#define DIM_BUF_TX		256
#define DIM_BUF_RX		256

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------

STATIProt statoProto;       // stato esecuzione gestore protocollo seriale

int (*fpstart)();			// puntatore alla funzione di servizio
long fpjump;				// flag chiamata alla funzione di jump

word chk_sum;               // Somma per controllo

byte tx_prec;				// carattere precedente	inviato
byte rx_prec;				// carattere precedente	ricevuto

byte *tx_buffer;
byte *rx_buffer;



// Inizializzazione

void proto_init(UARTHandle hUartDevA)
{
	statoProto = PR_RX_AND_DECODE;

    stato_rx = 0;
    fpjump = 0;

	mem_alloc(DIM_BUF_TX, (void **) &tx_buffer);
	mem_alloc(DIM_BUF_RX, (void **) &rx_buffer);

}




// Entry point del gestore per il protocollo seriale

void protoEntry(void)
{

    switch( statoProto )
    {
        case    PR_RX_AND_DECODE:
//            if(BAUD!=NEW_BAUD && !uart_getRxCount(hUartDevA))			// cambio baudrate alla fine del pacco prec.
//                BAUD=NEW_BAUD;

			if( protoRxAndDecode() == True )
				statoProto = PR_EXEC_SPEC_CMD;				// salto con l'asta
	        break;

        case    PR_EXEC_SPEC_CMD:
				if( protoExecSpecCmd() == True )			// se è un comando speciale
				{
					statoProto = PR_WAIT_TX_END;			// aspetta che tutto venga inviato
				}
				else
				{
					if( protoExecUserCmd() == True )		// se è un comando utente
					{
						statoProto = PR_WAIT_TX_END;		// aspetta che tutto venga inviato
					}
					else
					{
						statoProto = PR_EXEC_ILLEGAL_CMD;	// prova con i comandi illegali
					}
				}
	            break;

        case    PR_EXEC_ILLEGAL_CMD:
        	protoExecIllegalCmd();							// prepara la risposta
			statoProto = PR_WAIT_TX_END;					// aspetta che tutto venga inviato
            break;

        case    PR_WAIT_TX_END:
            if( !uart_getTxCount(hUartDevA) )				// se tutto è stato inviato...
            {
            	statoProto = PR_RX_AND_DECODE;				// riparte da capo

            	if( fpjump == MAGIC_JUMP )					// magic number ?
            	{
            		fpjump = 0;								// reset jumping flag
				   	(*fpstart)();							// jump !!
            	}
            }
            break;

        case    PR_TX_ERR:
            err_protoc();
            break;
    }

}


// Ricezione e decodifica del pacco in ricezione

bool protoRxAndDecode(void)
{
byte c=0,chr1;
word chkCalc, chkRx;
int i;

    while( uart_getRxCount(hUartDevA) )
    {

		if( uart_getc(hUartDevA, &chr1) )
		{
			c = chr1;
		}
		else
			break;

		if(	rx_prec	== 0x02	)							// codifica	?
		{
			rx_prec	= 0xFF;								// rx prec.	non	valido

			if(	c == 0x02 )								// stx ? reset comunicazione!!
			{
//				rx_buffer[0] = 0x02;					// ultimi due bytes	ricevuti

				stato_rx  = 1;							// start macchinetta
				rx_mem = 0;								// Azzera il Puntatore
				rx_tot = 0;								// Bytes Ricevuti =	1
				rx_etx = 0;								// reset sequenza di etx ricevuta

				timLoad.Preset(4000);

			}
			else
			{
				if(	c == 0x03 && stato_rx == 1 )		// etx ? fine pacco
				{
					rx_buffer[rx_mem]	= c;			// avanti il prossimo
					rx_tot++;							// Incrementa i	Bytes Ricevuti
					rx_etx = 1;							// una sequenza di etx ricevuta

	                if( rx_buffer[2] == GROUP_ID || rx_buffer[2] == 0  )
					{

		            	if( rx_buffer[3] == NUMBER_ID || rx_buffer[3] == 0  )
						{
							chkCalc = 0xFFFF;

						    for(i=0; i < rx_tot - 3; i++)				// calcolo chk basale...saltando l'stx
        						chkCalc += rx_buffer[i];

							chkRx = rx_buffer[rx_mem-2];
							chkRx |= ((word) (rx_buffer[rx_mem-1])) << 8;

			                lung_rx = len_l_pack;
			                lung_rx |= ((word )len_h_pack) << 8;

							if( chkRx  == chkCalc )
							{
			                    stato_rx = 0x51;
						        err_ges = 0;
								return( True );
							}

						}

						else
						{
							rx_mem = 0;								// Azzera il Puntatore
							rx_tot = 0;								// Bytes Ricevuti =	1
							stato_rx  = 0;							// start macchinetta
						}
					}
					else
					{
						stato_rx  = 0;							// start macchinetta
						rx_mem = 0;								// Azzera il Puntatore
						rx_tot = 0;								// Bytes Ricevuti =	1
					}

				}
				else
				{
					if(	c == 0x00 )						// 0 padding ? reset comunicazione!!
					{
						if( rx_mem < DIM_BUF_RX - 10 )
						{
							rx_buffer[rx_mem++]	= 0x02;		// avanti il prossimo
							rx_tot++;						// Incrementa i	Bytes Ricevuti
						}
					}
				}
			}
														// carattere ignorato (scasinerà tutto...)
		}
		else
		{
			rx_prec	= c;								// tiene traccia del dato prec.

			if(	c != 0x02 )								// se non è	un sincro
			{
				if( rx_mem < DIM_BUF_RX - 10 )
				{
					rx_buffer[rx_mem++]	= c;			// avanti il prossimo
					rx_tot++;								// Incrementa i	Bytes Ricevuti
				}
			}
		}
	}


	return( False );
}

bool protoExecIllegalCmd(void)
{
	return(True);
}


/* Esecuzione comandi speciali
*/
bool protoExecSpecCmd(void)
{

    if( cmd_rx <= 200 )
    {
        return( False );
    }

    switch( cmd_rx )                /* Risposta a Comando */
    {
        case    SERCMD_ACK:  			return( ExecAckCmd() );   			/* Richiesta di Comunicazione */
        case    SERCMD_CH_BAUD:			return( ExecChgBaudCmd() );
        case    SERCMD_WR_CODE:    		return( ExecWrCodeCmd() );			/* scrittura codice apparecchio */
        case    SERCMD_RD_BYTE:			return( ExecRdByteCmd() );
        case    SERCMD_WR_BYTE:			return( ExecWrByteCmd() );
        case    SERCMD_RD_BLOCK:		return( ExecRdBlockCmd() );
        case    SERCMD_WR_BLOCK:		return( ExecWrBlockCmd() );
        case    SERCMD_CHK_ST: 			return( ExecChkStatusCmd() );		/* Richiesta di Stato */
        case    SERCMD_CHK_FULL_ST:		return( ExecChkFullStatusCmd() );	/* Richiesta di Stato maggiorata */
        case    SERCMD_JP_LOAD:			return( ExecJmpToLoaderCmd() );		/* Salta al Loader */
        case    SERCMD_COP_RST:			return( ExecCopResetCmd() );		/* Richiesta di Reset */
        case    SERCMD_SYS_RST:    		return( ExecSysResetCmd() );		/* Richiesta di Reset */

                                    /* comandi speciali per prog. micro NEC */
        case    SERCMD_SELF_BLKCHK:		return( ExecSelfBlkChk() );
        case    SERCMD_SELF_PREWRITE:	return( ExecSelfPreWrite() );
        case    SERCMD_SELF_WRITEBACK:	return( ExecSelfWriteBack() );
        case    SERCMD_SELF_CHIPERASE:	return( ExecSelfChipErase() );
        case    SERCMD_SELF_WRITEBLOCK:	return( ExecSelfWriteBlock() );
        case    SERCMD_SELF_READBLOCK:	return( ExecSelfReadBlock() );
        case    SERCMD_SELF_VERIFBLOCK:	return( ExecSelfVerifBlock() );
        case    SERCMD_SELF_SIGNREAD:	return( ExecSelfSignature() );
        case    SERCMD_SELF_CTRLVPP:	return( ExecSelfCtrlVpp() );
        case    SERCMD_SELF_AREAERASE:	return( ExecSelfAreaEr() );
        case    SERCMD_SELF_FLASHERASE:	return( ExecSelfFlashEr() );
        case    SERCMD_SELF_REWRLOADER:	return( ExecSelfRewrLoader() );
        case    SERCMD_SELF_INTERNVRF:	return( ExecSelfInternVerif() );
        case    SERCMD_SELF_SETPROGPAR:	return( ExecSelfParam() );
        case    SERCMD_SELF_JMPTOAPP:	return( ExecSelfJmpToApp() );
        case    SERCMD_SELF_JMPTOLOAD: 	return( ExecSelfJmpToLoader() );

        default:			/* Comando Sconosciuto */
          return( False );
    }

	return( True );
}


void err_protoc(void)
{
    lung_tx=2;
    dtx_ser[0]=err_ges;

}


bool ExecAckCmd(void)
{
	return(True);
}

bool ExecChgBaudCmd(void)
{
    switch(((word)drx_ser[1]<<8) + (word)drx_ser[0])
    {
/*              case 1152:
        NEW_BAUD=B_115200;
        break;*/
/*              case 768:
        NEW_BAUD=B_76800;
        break;*/
/*              case 576:
        NEW_BAUD=B_57600;
        break;*/
/*              case 384:
        NEW_BAUD=B_38400;
        break;*/
/*              case 288:
        NEW_BAUD=B_28800;
        break;*/
/*              case 192:
        NEW_BAUD=B_19200;
        break;*/
/*              case 14:
        NEW_BAUD=B_14400;
        break;*/
      case 96:
        NEW_BAUD=9600;
        break;
      case 48:
        NEW_BAUD=4800;
        break;
      case 24:
        NEW_BAUD=2400;
        break;
      case 12:
        NEW_BAUD=1200;
        break;
      case 6:
        NEW_BAUD=600;
        break;
      case 3:
        NEW_BAUD=300;
        break;
      default:
//        stato_ges = TX_ERR;
        err_ges = 0x00;
    }

    lung_tx = 1;
    return(True);
}


bool ExecWrCodeCmd(void)
{
	if( drx_ser[0] == 0xa7 && drx_ser[1] == 0x57 )
	{
		/* fare verifica su config e se abilitata la EEPROM fare scrittura */
		if((!NUMBER_ID && drx_ser[2]) || (NUMBER_ID && !drx_ser[2]))
		{
//			NUMBER_ID = drx_ser[2];
//			disable_rx();               /* disabilita la ricezione */
//			init_ser();                 /* inizializza la seriale */
//			enable_rx();                /* abilita la ricezione */
		}
//		else
//			NUMBER_ID = drx_ser[2];

		lung_tx = 1;                    /* gia` fatto */
	}
	else
	{
//		stato_ges=TX_ERR;
		err_ges=0xfe;
	}
	return(True);
}


bool ExecRdByteCmd(void)
{
int i;
byte *pd;

	for(i=1,lung_tx=0; i<lung_rx; i+=2,lung_tx++)
	{
		pd = (byte *) ((drx_ser[i+1]<<8) + drx_ser[i]);
		dtx_ser[lung_tx] = *pd;
	}

	return( True );
}


bool ExecWrByteCmd(void)
{
int i;
byte *pd;

	for(i=lung_tx=1; i<lung_rx; i+=3)
	{
		pd = (byte *)((drx_ser[i+1]<<8)+drx_ser[i]);

		*pd = drx_ser[i+2];
	}

	return( True );
}


bool ExecRdBlockCmd(void)
{
int i;
byte *pd;

    pd = (byte *) ((drx_ser[2]<<8) + drx_ser[1]);  /* indirizzo di partenza */
    i = ((drx_ser[4]<<8) + drx_ser[3]);

    if( i > DIM_BUF_TX - 20 )
        i = DIM_BUF_TX - 20;

    dtx_ser[0] = i & 0xFF;
    dtx_ser[1] = i >> 8;

    ramcpy(&dtx_ser[2], pd, i);     /* copia il copiabile */
    lung_tx = i+3;

	return( True );
}



bool ExecWrBlockCmd(void)
{
int i;
byte *pd;

	pd = (byte *) ((drx_ser[2]<<8) + drx_ser[1]);  /* indirizzo di partenza */
	i = ((drx_ser[4]<<8) + drx_ser[3]);

	ramcpy(pd, &drx_ser[5], i);     /* copia il copiabile */
	lung_tx = 1;

	return( True );
}



/* Ritorna lo stato corrente del sistema
*/
bool ExecChkStatusCmd(void)
{
	ramcpy(dtx_ser, &SysStatus, sizeof(SystemStatus));		// copia pure i dati di sistema

	return( protoEncodeAndTx(sizeof(SystemStatus)) );	// spedisce .... come richiesto
}


/* Ritorna lo stato corrente del sistema
*/
bool ExecChkFullStatusCmd(void)
{
register FullSystemStatus *ps;
const char **pVersApp;
const char **pNameApp;
const long *pmagic;
const long *pChkCalc;


    ps = (FullSystemStatus *) dtx_ser;  				/* Punta a dtx_ser di tipo SysStat */

//	romstrcpy(ps->SwVersLoader, sLoadRevision, sizeof(ps->SwVersLoader));		// imposta la revisione e il nome del sw
//	romstrcpy(ps->SwNameLoader, sLoadName, sizeof(ps->SwNameLoader));			// nome del softw. di loading

	romstrcpy(ps->SwVersApp, sAppRevision, sizeof(ps->SwVersApp));		// imposta la revisione e il nome del sw
	romstrcpy(ps->SwNameApp, sAppName, sizeof(ps->SwNameApp));			// nome del softw. di loading

	pVersApp = (const char **) 0x40C;									// punta alla tabellina dell'app.
	pNameApp = (const char **) 0x410;
	pmagic   = (const long *) 0x400;
	pChkCalc = (const long *) 0x20408;

	if( *pmagic == 0x55667788 )													// se l'app. è stato caricato
	{
		romstrcpy(ps->SwVersLoader, *pVersApp, sizeof(ps->SwVersLoader));		// numero di revisione dell'app
		romstrcpy(ps->SwNameLoader, *pNameApp, sizeof(ps->SwNameLoader));		// nome del softw. di app
	}
	else
	{
		romstrcpy(ps->SwVersLoader, "V0000", sizeof(ps->SwVersLoader));		// numero di revisione dell'app
		romstrcpy(ps->SwNameLoader, "00000", sizeof(ps->SwNameLoader));		// nome del softw. di app
	}

    ps->ChkProg = SysStatus.ChkProg;
    ps->ChkCalc = *pChkCalc;
//  ps->ChkCalc = SysStatus.ChkCalc;
    ps->ChkLoader = SysStatus.ChkLoader;
    ps->MaxScanMain = SysStatus.MaxScanMain;
    ps->ActScanMain = SysStatus.ActScanMain;
    ps->MaxScanComm = SysStatus.MaxScanComm;
    ps->ActScanComm = SysStatus.ActScanComm;
    ps->MaxScanTick = SysStatus.MaxScanTick;
    ps->ActScanTick = SysStatus.ActScanTick;

    ps->LoaderPlease = SysStatus.LoaderPlease;
    ps->ErrorLevel = SysStatus.ErrorLevel;
    ps->ResetCause = SysStatus.ResetCause;

    ps->InRam = SysStatus.InRam;
    ps->sector_erased_flags = SysStatus.sector_erased_flags;

	return( protoEncodeAndTx(sizeof(FullSystemStatus)) );			// spedisce .... come richiesto
}



bool ExecJmpToLoaderCmd(void)
{
    if( drx_ser[1] == 0xaa && drx_ser[2] == 0x55 )
    {
//??      disable_interrupt();
      *(word *)0x0000=0xaa55;
      while(1);					// autodistruzione
    }
    else
    {
//      stato_ges = TX_ERR;
      err_ges = 0xfe;
    }

	return( True );
}


/* Richiesta di Reset */
bool ExecCopResetCmd(void)
{
	if( drx_ser[1] == 0xaa && drx_ser[2] == 0x55 )
	{
//??	    disable_interrupt();
	    while(1);       /* Blocco */
	}
	else
	{
//	    stato_ges = TX_ERR;
	    err_ges = 0xfe;
	}
	return( True );
}


/* Richiesta di Reset */
bool ExecSysResetCmd(void)
{
//    if( drx_ser[1] == 0xaa && drx_ser[2] == 0x55 )
//??        SW_RESET();       /* Reset */
//   else
//    {
//        stato_ges=TX_ERR;
        err_ges=0xfe;
//    }
	return( True );
}

void romstrcpy(char *pfDst, const char *pfSrc, int size)
{
int i;

	for(i=0;i<size; i++)	// numero di revisione del loader
		*pfDst++ = *pfSrc++;

	pfDst -= size;

	if( pfDst[0] == '\xFF' )
		pfDst[0] = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Comandi speciali per caricamento su microbo NEC
/////////////////////////////////////////////////////////////////////////////////////////

bool ExecSelfBlkChk(void)
{
int result;
dword address;
											/* Codice del Comando "blank_check" : verifica che tutte le celle siano cancellate = 0xFF */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il check */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);

//	result = selfprog_blankCheck(address);

//	result = 0;
	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfPreWrite(void)
{
int result;
dword address;

								           	/* Codice del Comando "prewrite" : mette a zero tutte le celle di flash per poter cancellare */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il pre-write */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);

//	result = selfprog_preWrite(address);
	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfWriteBack(void)
{
int result;
dword address;
								           	/* Codice del Comando "writeback" : prova ad "equalizzare" le celle */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il write back */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);

//	result = selfprog_writeBack(address);
	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfChipErase(void)
{
int result;
											/* !!!!! Comando principale da utilizzare per la cancellazione !!!!!*/
								           	/* Codice del Comando "chipErase" : cancella tutta la flash + riscrive il loader */
											/* Dati aggiuntivi: 	nessuno	*/
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
//	result = selfprog_chipErase();
//	result = selfprog_areaErase(1);

//	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfWriteBlock(void)
{
int result;
byte *pWrite;
byte *pData;
//word fData;
long count;
//int sector=0;
								           	/* Codice del Comando "writeBlock" : scrive un blocco di dati in flash */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati da scrivere */
											/* 	-dword-  Size    	-  numero di bytes da scrivere eg. dimensione del blocco trasferito */
											/* 	-byte[]- DataBlock 	-  blocco di dati da scrivere a seguire  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	pWrite = (byte *) xctLong(&drx_ser[0]);
	count = xctLong(&drx_ser[4]);
	pData = &drx_ser[8];

	if( count > DIM_BUF_RX - 24 )			// piccolo controllo limiti anti-autodistruzione
		count = DIM_BUF_RX - 24;

//	if( pWrite >= FLASH_START && pWrite <= FLASH_END )
//	{
//									// in flash gestiamo solo puntatori allineati
//		if( ((dword)pWrite & 0x00000003) == 0x00000000 )
//		{
//			if( pWrite >= FLASH_AREA1 )
//
//			if( pWrite >= (byte *) 0x8000 )	// chiaramente non puo' scrivere nella flash in cui risiede il loader
//				result = 0; //selfprog_multiWrite( (long *) pWrite, (long *) pData, count);
//			else
//				result = 0;
//		}
//		else
//			result = 1;
//	}
//	else
//	{
//		if( pWrite >= XFLASH_START && pWrite <= XFLASH_END )
//		{
//			if( ((dword)pWrite & 0x00000002) == 0x00000000 )
//			{
//				while( count )			// copia come richiesto
//				{
//					sector = get_sector_number( (long) pWrite);				// verifica in quale settore cade..
//
//					if( sector < 0 )
//						break;
//
//					ramcpy(&fData, pData, 2);						// copia i dati da scrivere
//
//					if( READ_SECTOR_ERASED_FLAG(sector) )			// se il banco di flash è cancellato...
//					{
//								         									// puo' scrivere scioltamente
//						result = flash_write_word( (word *) pWrite, fData);
//
//						if( result != 1 )
//						{
//							result = 2;
//							break;
//						}
//					}
//					else
//					{
//						result = flash_erase_sector(sector);				// cancella il banco di flash
//
//						if( result == 1 )
//						{
//							SET_SECTOR_ERASED_FLAG(sector);					// questo banco è stato cancellato !!  via libera alla scrittura
//
//								         									// puo' scrivere scioltamente
//							result = flash_write_word((word *) pWrite, fData);
//
//							if( result != 1 )
//							{
//								result = 4;
//								break;
//								// Programmazione, errore scrittura pacch.		2 sec	---		ON		---		Fast Blink
//							}
//							else
//							{
//							}
//						}
//						else
//						{
//							result = 3;
//							break;
//										// Programmazione, errore canc. banco.			2 sec	ON		---		---		Fast Bk
//			//				err_ges=10;
//			//				statoLoader = LOADER_PROGERR_ERASE;
//						}
//					}
//
//					pData += 2;
//					pWrite += 2;
//					count -= 2;				// altri 2 bytes scritti
//				}
//			}
//			else
//				result = 1;
//		}
//		else
//		{
//										// allineamento non importante
//			while( count-- )			// copia come richiesto
//				*pWrite++ = *pData++;
//		}
//
//		result = 0;					// tutto ok !!
//	}

	ramcpy(dtx_ser, &result, 2);			// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfReadBlock(void)
{
byte *pWrite;
byte *pRead;
long count;
long dataSize;
								           	/* Codice del Comando "readBlock" : legge un blocco di dati dalla */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash/ram) di partenza del blocco di dati da leggere */
											/* 	-dword-  Size    	-  numero di bytes da leggere eg. dimensione del blocco da trasferire */
											/* Dati restituiti: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash/ram) di partenza del blocco di dati letti */
											/* 	-dword-  Size    	-  numero di bytes in transfer */
											/* 	-byte[]- DataBlock 	-  dati a seguire  */
	pRead = (byte *) xctLong(&drx_ser[0]);

											// dimensione blocco richiesta
	count = xctLong(&drx_ser[4]);

	if( count > DIM_BUF_TX - 24 )			// piccolo controllo limiti anti-autodistruzione
		count = DIM_BUF_TX - 24;

											// crea l'intestazione:
	ramcpy(&dtx_ser[0], &pRead, 4);			// 	indirizzo effettivo
	ramcpy(&dtx_ser[4], &count, 4);			// 	numero di bytes effettivo

	pWrite = &dtx_ser[8];					// non necessario

	dataSize = count;						// copia prima che sia troppo tardi

	while( count-- )						// copia come richiesto i dati
		*pWrite++ = *pRead++;

	return( protoEncodeAndTx(dataSize+8) );	// spedisce .... il paccone
}


bool ExecSelfVerifBlock(void)
{
int result;
byte *pRead;
byte *pData;
long count;

								           	/* Codice del Comando "verifBlock" : verifica un blocco di dati in flash con i dati passati */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address 	-  indirizzo (flash) di partenza del blocco di dati da scrivere */
											/* 	-dword-  Size    	-  numero di bytes da scrivere eg. dimensione del blocco trasferito */
											/* 	-byte-   DataBlock 	-  blocco di dati da scrivere a seguire  */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	pRead = (byte *) xctLong(&drx_ser[0]);
	count = xctLong(&drx_ser[4]);

	if( count > DIM_BUF_RX - 24 )			// piccolo controllo limiti anti-autodistruzione
		count = DIM_BUF_RX - 24;

	pData = &drx_ser[8];					// dati passati

	result = 0;								// tutto ok !!...... per il momento

	while( count )							// verifica come richiesto
	{
		if( *pData++ != *pRead++ )
		{
			result = 1;
			break;
		}

		count--;
	}

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfSignature(void)
{

							/* Codice del Comando "signatureRead" : legge il codice interno del dispositivo */
							/* Dati aggiuntivi: 	nessuno	*/
							/* Dati restituiti: 				*/
							/* 	-xxxx-  code 		-  codice del dispositivo e altri ciappini */

	romstrcpy( (char *) &dtx_ser[0], "uPD70F3107", 16);		// copia i dati in partenza

	return( protoEncodeAndTx(32) );			// spedisce .... il risultato
}

bool ExecSelfCtrlVpp(void)
{
int result;
		          							/* Codice del Comando "setVPP" : esegue il controllo dell'alimentazione per la prog. */
											/* Dati aggiuntivi: 				*/
											/* 	-byte-  Enable -  se =0 disattiva VPP, se <> 0 attiva VPP */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
//	result = selfprog_setVpp(drx_ser[0]);

//	if( result == 0 && drx_ser[0] )			// se il VPP è ok ed era stato richiesto...
//	{
//		chk_erased_bank();					// prepara la lista dei banchi cancellati
											// inizia una nuova programmazione
//	}

//	result = 0;
	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfAreaEr(void)
{
int result;
dword address;

								           	/* Codice del Comando "areaErase" : esegue l'erase di un'area con il prewrite implicito */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire l'erase con eventuale prewrite automatico */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);
//	selfprog_startproc();

//	if( address == 0x20000 )
//		result = selfprog_areaErase(1);
//	else
//		result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfFlashEr(void)
{
int result;
dword address;
								          	/* Codice del Comando "flashErase" : esegue il solo comando di erase (da usare con molta delicatezza) */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area su cui eseguire il solo erase brutale */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);
//	selfprog_startproc();

//	result = selfprog_flashErase(address);
	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfRewrLoader(void)
{
int result;
dword address;
								         	/* Codice del Comando "rewriteLoaqder": forza la copiatura del loader dalla ram alla flash */
											/* Dati aggiuntivi: 	*/
											/* 	-dword-  Address -  indirizzo dell'area di memoria ram in cui è stata copiata l'immagine del loader */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);

//	result = selfprog_rewriteLoader(address);
	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfInternVerif(void)
{
int result;
dword address;
								           	/* Codice del Comando "internalVerify": comando internal verify */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address -  indirizzo dell'area di memoria su cui eseguire il verify */
											/* Dati restituiti: 				*/
											/* 	-word-  result 	-  risultato dell'operazione:ok-no_ok */
	address = (dword) xctLong(&drx_ser[0]);

//	result = in_verify(address);
//	result = 0;

	ramcpy(dtx_ser, &result, 2);			// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfParam(void)
{
long Freq, WriteTime, EraseTime, WriteBackTime;
int result;
								          	/* Codice del Comando "setparam": imposta i parametri di programmazione */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Freq 	   		-  frequenza di clock del micretto (in Hz) */
											/* 	-dword-  WriteTime 		-  tempo di scrittura del micretto (in uSec)  */
											/* 	-dword-  EraseTime 		-  tempo di cancellazione del micretto (in uSec)  */
											/* 	-dword-  WriteBackTime 	-  tempo di write-back del micretto (in uSec)  */
											/* Dati restituiti: 				*/
											/* Dati restituiti: nessuno				*/
	Freq = xctLong(&drx_ser[0]);
	WriteTime = xctLong(&drx_ser[4]);
	EraseTime = xctLong(&drx_ser[8]);
	WriteBackTime = xctLong(&drx_ser[12]);

//	selfprog_setparam(Freq, WriteTime, EraseTime, WriteBackTime);

	result = 0;
	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}


bool ExecSelfJmpToApp(void)
{
dword address;
int result;

								           	/* Codice del Comando JMPTOAPP: forza il salto dal loader all'applicazione all'indirizzo richiesto */
											/* Dati aggiuntivi: 				*/
											/* 	-dword-  Address   		-  indirizzo salto nell'app utente: se = 0xFFFF.FFFF -> salta all'indirizzo di default dell'app.  */
											/* Dati restituiti: nessuno				*/
	address = (dword) xctLong(&drx_ser[0]);

	fpstart = (int(*)()) address;			// force jump
	fpjump = MAGIC_JUMP;					// setta il flag
	result = 0;

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(2) );			// spedisce .... il risultato
}



bool ExecSelfJmpToLoader(void)
{
int result;
							            	/* Codice del Comando JMPTOLOAD: forza il salto dall'applicazione al loader */
											/* Dati aggiuntivi: nessuno (usa indirizzo loader di default) */
											/* Dati restituiti: nessuno				*/
	fpstart = (int(*)()) FLASH_KEEP_LOAD; 	// soft-reset restarting from reset vector
	fpjump = MAGIC_JUMP;					// setta il flag

	result = 2;								// già nel loader....!!

	ramcpy(dtx_ser, &result, 2);		// copia i dati in partenza

	return( protoEncodeAndTx(0) );			// spedisce .... il risultato
}





/*	stato_ges (in esadecimale)

		Fase   0 = attesa     STX
		Fase   1 = ricezione  MA_NO_ID
		Fase   2 = decodifica SL_NO_ID
		Fase   3 = errore     SL_NO_ID
		Fase   4 = ricezione  COM_NO
		Fase  10 = ricezione  LUNG_TOT basso
		Fase  11 = ricezione  LUNG_TOT alto
		Fase  20 = ricezione  COMANDO
		Fase  30 = ricezione  PARAMETRI
		Fase  40 = decodifica CHK_SUM basso
		Fase  41 = errore     CHK_SUM basso
		Fase  43 = decodifica CHK_SUM alto
		Fase  44 = errore     CHK_SUM alto
		Fase  46 = decodifica ETX
		Fase  47 = errore     ETX
		Fase  50 = fine ricezione attesa nuovo comando

	err_ges
		err   fe = errore COMANDO sconosciuto
		err   ff = errore il programma non ha risposto al comando
*/



/*
STRUTTURA DELLA STRINGA DI TRASMISSIONE (ancora da codificare)
        E DELLA STRINGA DI RICEZIONE    (gi… decodificata)

A       STX         0x02
C       MA_NO_ID    128-144 numero stazione di chi chiama (master)
E       SL_NO_ID    0-16  numero della stazione chiamata (slave)
                          la stazione risponde solo al suo e allo '0'
F       COM_NO      0-255 numero progressivo della comunicazione
G       LUNG_TOT H  0-1   lunghezza byte L
G       LUNG_TOT L  0-255 lunghezza byte H  (lunghezza dei bytes: H + I)
H       COMANDO     0-255 comando da eseguire
I       PARAMETRI   0-255 coda dei parametri
I          "         "           "
I          "         "           "
I       PARAMETRI   0-255        "
J       CHK_SUM  H  0-255 somma di controllo byte L
J       CHK_SUM  L  0-255 somma di controllo byte H (0xffff+B+C+D+E+F+G+H+I)
K       ETX         0x03


COME CODIFICARE / DECODIFICARE LA STRINGA :

STX     0x02, 0x02  viene indicato dalla sequenza dei due bytes [UNIVOCA]

ETX     0x02, 0x03  viene indicato dalla sequenza dei due bytes [UNIVOCA]

DATO 2  0x02, 0x00  viene indicato dalla sequenza dei due bytes [UNIVOCA]
0x02

DATO                viene indicato dal singolo byte [UNIVOCO]
0 - 1   0 - 1
3 - 255 3 - 255


QUINDI :

La sequenza 0x02, 0x02 indica l' STX.
La sequenza 0x02, 0x03 indica l' ETX.
La sequenza 0x02, 0x00 indica il DATO "2".
I numeri 0, 1, 3 ... 255 non vengono alterati.


(comando per lettura stato)


Cicli


1
2
3
4
5
6
7

(buffer su ciclo 0?)

(comando per scrittura singolo ciclo)
(comando per lettura singolo ciclo)
(comando per conferma scrittura in flash ciclo completo)


Step (di programma)

1
2
3
4
5
6
7
8

(comando per scrittura singolo step->in pagina ram)
(comando per lettura singolo step->da flash)

(comando per scrittura blocchi di step->in pagina ram)
(comando per lettura blocchi di step->da flash)


*/


/* Esecuzione comandi speciali
*/
bool protoExecUserCmd(void)
{


    switch( cmd_rx )                /* Risposta a Comando */
    {
		case    SERCMD_TST_COM:		return( UserTestCom() );

								// comandi gruppo 1: trasferimento dati
								////////////////////////////////////////////////////////////
		case    SERCMD_WR_CICLO:	return( UserWrCicloCmd() );
		case    SERCMD_RD_CICLO:	return( UserRdCicloCmd() );
		case    SERCMD_FLASH_CICLO:	return( UserFlashCicloCmd() );

		case    SERCMD_WR_STEP:		return( UserWrStepCmd() );
		case    SERCMD_RD_STEP:		return( UserRdStepCmd() );

		case    SERCMD_WR_STEP_TAB:	return( UserWrStepTabCmd() );
		case    SERCMD_RD_STEP_TAB:	return( UserRdStepTabCmd() );

		case    SERCMD_RD_RESULT:	return( UserRdResultCmd() );

								// comandi gruppo 2: controllo esecuzione
								////////////////////////////////////////////////////////////
		case    SERCMD_SEL_CICLO:   return( UserSelCicloCmd() );        	/* selezione prova con start esterno */
		case    SERCMD_STT_CICLO:   return( UserStartCicloCmd() );        	/* start prova */
		case    SERCMD_STT_CICTAB:	return( UserStartCicTabCmd() );
		case    SERCMD_STP_CICLO:   return( UserStopCicloCmd() );        	/* stop prova */
		case    SERCMD_RES_CICLO:   return( UserResetCicloCmd() );        	/* reset prova */
		case    SERCMD_TST_CICLO:	return( UserTestCicloCmd() );			// lettura stato esecuzione

								// comandi gruppo 3: controllo (manuale/diretto) dispositivi
								////////////////////////////////////////////////////////////
		case    SERCMD_RD_INP:		return( UserRdInputCmd() );
		case    SERCMD_RD_OUT:		return( UserRdOutputCmd() );
		case    SERCMD_WR_OUT:		return( UserWrOutputCmd() );
		case    SERCMD_SET_OUT:		return( UserSetOutCmd() );
		case    SERCMD_RES_OUT:		return( UserResOutCmd() );
		case    SERCMD_RD_ADC:		return( UserRdAdcCmd() );
		case    SERCMD_RD_DAC:      return( UserRdDacCmd() );
		case    SERCMD_WR_DAC:      return( UserWrDacCmd() );

								// comandi gruppo 4: calibrazioni e par. di calibrazione
								////////////////////////////////////////////////////////////
		case    SERCMD_RD_PAR_ADC:	return( UserRdParAdcCmd() );
		case    SERCMD_WR_PAR_ADC:  return( UserWrParAdcCmd() );
		case    SERCMD_RD_PAR_CMP:  return( UserRdParCmpCmd() );
		case    SERCMD_WR_PAR_CMP:  return( UserWrParCmpCmd() );
		case    SERCMD_RD_PAR_DAC:  return( UserRdParDacCmd() );
		case    SERCMD_WR_PAR_DAC:  return( UserWrParDacCmd() );
		case    SERCMD_RD_PAR_STEP: return( UserRdParStepCmd() );
		case    SERCMD_WR_PAR_STEP: return( UserWrParStepCmd() );
		case    SERCMD_RD_PAR_DCMOT:return( UserRdParDcMotCmd() );
		case    SERCMD_WR_PAR_DCMOT:return( UserWrParDcMotCmd() );
		case    SERCMD_RD_PAR_STAB: return( UserRdParStabCmd() );
		case    SERCMD_WR_PAR_STAB: return( UserWrParStabCmd() );

		case    SERCMD_ADC_ZERO:    return( UserAdcZeroCmd() );
		case    SERCMD_ADC_SPAN:    return( UserAdcSpanCmd() );
        default:			    	return( False );
    }

	return( False );
}





bool UserTestCom(void)
{
TestComInfo *ps;
word dataSize;

static TestComInfo comInfo;

	if( comInfo.Count3 > 1000.0 )
		comInfo.Count3 = 0.0;

	if( comInfo.Count4 > 1000.0 )
		comInfo.Count4 = 0.0;

	comInfo.test = stato_rx;
	comInfo.Count1++;
	comInfo.Count2--;
	comInfo.Count3 += 0.25;
	comInfo.Count4 += 0.55;

	ps = (TestComInfo *) &dtx_ser[0];		// piglia il puntatore al blocco

	ramcpy(dtx_ser, &comInfo, sizeof(comInfo));	// copia i dati in partenza

//	cmd_tx = SERTST_COM;
//	com_n_ser = 0;

	dataSize = sizeof(comInfo);

	return( protoEncodeAndTx(sizeof(comInfo)) );	// rispedisce ....
}

bool UserWrCicloCmd(void)
{
	return( True );
}


bool UserRdCicloCmd(void)
{
	return( True );
}

bool UserFlashCicloCmd(void)
{
	return( True );
}


bool UserWrStepCmd(void)
{
	return( True );
}

bool UserRdStepCmd(void)
{
	return( True );
}


bool UserWrStepTabCmd(void)
{
	return( True );
}

bool UserRdStepTabCmd(void)
{
	return( True );
}


bool UserRdResultCmd(void)
{
	return( True );
}


////////////////////////////////////////////////////////////
// comandi gruppo 2: controllo esecuzione
////////////////////////////////////////////////////////////
bool UserSelCicloCmd(void)
{
	return( True );
}

bool UserStartCicloCmd(void)
{
	return( True );
}

bool UserStartCicTabCmd(void)
{
	return( True );
}

bool UserStopCicloCmd(void)
{
	return( True );
}

bool UserResetCicloCmd(void)
{
	return( True );
}

bool UserTestCicloCmd(void)
{
	return( True );
}


////////////////////////////////////////////////////////////
// comandi gruppo 3: controllo (manuale/diretto) dispositivi
////////////////////////////////////////////////////////////
bool UserRdInputCmd(void)
{
	return( True );
}

bool UserRdOutputCmd(void)
{
	return( True );
}

bool UserWrOutputCmd(void)
{
	return( True );
}

bool UserSetOutCmd(void)
{
	return( True );
}

bool UserResOutCmd(void)
{
	return( True );
}

bool UserRdAdcCmd(void)
{
	return( True );
}

bool UserRdDacCmd(void)
{
	return( True );
}

bool UserWrDacCmd(void)
{
	return( True );
}


////////////////////////////////////////////////////////////
// comandi gruppo 4: calibrazioni e par. di calibrazione
////////////////////////////////////////////////////////////
bool UserRdParAdcCmd(void)
{
	return( True );
}

bool UserWrParAdcCmd(void)
{
	return( True );
}

bool UserRdParCmpCmd(void)
{
	return( True );
}

bool UserWrParCmpCmd(void)
{
	return( True );
}

bool UserRdParDacCmd(void)
{
	return( True );
}

bool UserWrParDacCmd(void)
{
	return( True );
}

bool UserRdParStepCmd(void)
{
	return( True );
}

bool UserWrParStepCmd(void)
{
	return( True );
}

bool UserRdParDcMotCmd(void)
{
	return( True );
}

bool UserWrParDcMotCmd(void)
{
	return( True );
}

bool UserRdParStabCmd(void)
{
	return( True );
}

bool UserWrParStabCmd(void)
{
	return( True );
}


bool UserAdcZeroCmd(void)
{
	return( True );
}

bool UserAdcSpanCmd(void)
{
	return( True );
}







/////////////////////////////////////////////////////////////////////////////////////////



/* Estrae un long da una stringa
*/
dword xctLong(byte *pdata)
{
dword	d;
byte 	*pd = (byte *) &d;

	*pd++ = *pdata++;
	*pd++ = *pdata++;
	*pd++ = *pdata++;
	*pd++ = *pdata++;

	return( d );
}


bool protoEncodeAndTx(word dataSize)
{
word i;
word chkCalc = 0;
word packSize = dataSize + 1;
byte c=0;

//	if( !dataSize )
//		return(False);

	tx_mem = 0;								// reset indice inserimento

//    tx_buffer[tx_mem++] = STX;         		// Intestazione
    tx_buffer[tx_mem++] = ma_gr_id;
    tx_buffer[tx_mem++] = ma_no_id;
    tx_buffer[tx_mem++] = GROUP_ID;
    tx_buffer[tx_mem++] = NUMBER_ID;
    tx_buffer[tx_mem++] = com_n_ser;
    tx_buffer[tx_mem++] = packSize & 0xff;
    tx_buffer[tx_mem++] = packSize >> 8;
    tx_buffer[tx_mem++] = cmd_rx;			// comando da spedire...uguale a quello ricevuto ??

    chkCalc = 0xFFFF;

    for(i=0; i < tx_mem; i++)				// calcolo chk basale...saltando l'stx
        chkCalc += tx_buffer[i];

    for(i=0; i < dataSize; i++)				// calcolo chk dati
        chkCalc += tx_buffer[tx_mem++];

    tx_buffer[tx_mem++] = chkCalc & 0xFF;	// infila il chk calcolato
    tx_buffer[tx_mem++] = chkCalc >> 8;		// infila il chk calcolato
//    tx_buffer[tx_mem++] = ETX;				// l'etx chiude il pacco

	uart_putc(hUartDevA, 0x02);				// carattere STX
	uart_putc(hUartDevA, 0x02);

    for(i=0; i < tx_mem; i++)			// calcolo chk basale...saltando l'stx
	{
		c = tx_buffer[i];

		if( c == 0x02 )						// zero pdding per i dati
		{
			uart_putc(hUartDevA, 0x02);
			c = 0x00;
		}
		uart_putc(hUartDevA, c);
	}

	uart_putc(hUartDevA, 0x02);				// chiude il pacco con l'ETX
	uart_putc(hUartDevA, 0x03);

    return(True);
}


// Clone della ramcpy ad uso interno che gira in ram
void  ramcpy(void *pdst, void *psrc, int size)
{
register byte *src = (byte *) psrc;
register byte *dst = (byte *) pdst;

	while(size--)
		*dst++ = *src++;

}

