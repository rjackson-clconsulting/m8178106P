//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               CSI.CPP
//  Descrizione:             Routines gestione com. su seriale sincrona
//  Note:             		 I chip select sono ricavati direttamente con
//							 pin di un port del micro
//  Supporto Fisico:         Hw non specifico
//	Utilizzato su:			 EVB PicSel
//							 Multichannel PH Menfis
//  Versione:                $Revision: 1.4 $
//  Data Versione:           $Date: 2002/03/08 00:00:00 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include "global.h"
#include "error.h"
#include "csi.h"


//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------
static CsiChan *checkHandle(CSIHandle hChan);


//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
													// dati di configurazione dei canali csi
static CsiChan csiRegTab[CSI_MAX_CHAN];
static CSIErrors csiError;
static short numChanReg;

/* Cerca di inizializzare la gestione csi
*/
void csi_init()
{
int i;

	for(i=0; i<CSI_MAX_CHAN; i++)					// init statoli dei canali
		csiRegTab[i].stato = CSI_CHAN_IDLE;

	csiError = CSI_ERR_NONE;
}

/* Ritorna lo stato di un canale
*/
bool csi_status(CSIHandle hChan, CSIState *pstat)
{

#ifdef __DEBUG__
	if( hChan.theHandle < CSI_BASE_HANDLE || hChan.theHandle >= numChanReg + CSI_BASE_HANDLE )
	{
		csiError = CSI_ERR_INV_HANDLE;
		error_trace("CSI", "csi_status", csiError);	// aspetta che lo vengano a prendere
		return(False);
	}

#endif

	hChan.theHandle -= CSI_BASE_HANDLE;

	*pstat = csiRegTab[hChan.theHandle].stato;

	return(True);
}


/* Crea un nuovo canale di comunicazione per CSI (clocked serial interface)
	ovvero... seriale sincrona
*/
CSIHandle csi_create(CBufHandle hBuf, void (*pfPort)(int) )
{
CSIHandle hChan;
ComRingBuf *pComBuf;

	hChan.theHandle = 0;					// null handle

	if( numChanReg >= CSI_MAX_CHAN )
	{
		csiError = CSI_ERR_END_CHAN;

#ifdef __DEBUG__
		error_trace("CSI", "csi_create", csiError);		// aspetta che lo vengano a prendere
#endif
		return( hChan );
	}

	csiRegTab[numChanReg].pfPort = pfPort;				// funzione che modifica il port tramite un latch o altro

	if( pfPort )
		pfPort(OFF);									// disattiva subito il ChipSel

	combuf_getPointer(hBuf, &pComBuf); 					// si fa dare il puntatore al buffer per cortesia

	csiRegTab[numChanReg].pComBuf = pComBuf;				// lo registra

	hChan.theHandle = numChanReg + CSI_BASE_HANDLE;

	numChanReg++;										// abbiamo registrato un canale in +

	return( hChan );
}

/* Apre un canale di comunicazione precedentemente creato
*/
bool csi_open(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_open", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_IDLE )					// se è già aperto non può riaprirlo
		return(False);

	if( pChan->pComBuf->userLock != 0 )					// se qualcuno lo sta usando non può usarlo
		return(False);

	pChan->stato = CSI_CHAN_OPEN;						// il canale è aperto !!

	pChan->pComBuf->userLock = hChan.mod_id;			// lo blocca un attimino (col proprio handle)

	if( pChan->pfPort )
		pChan->pfPort(ON);										// attiva il chip select

	return(True);
}


/* Chiude un canale di comunicazione precedentemente creato ed aperto
	liberando cosi' il buffer di comunicazione per un'altro canale
*/
bool csi_close(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_close", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_flush(&pChan->pComBuf->txBuf);				// quantomeno svuota il buffer

	pChan->pComBuf->userLock = 0;						// sblocca il buffer per gli altri

	if( pChan->pfPort )
		pChan->pfPort(OFF);								// disattiva subito il ChipSel

	pChan->stato = CSI_CHAN_IDLE;						// è di nuovo libero e bello

	return(True);
}

/* Flusha il canale di comunicazione precedentemente creato ed aperto
	terminando cosi' la scrittura
*/
bool csi_flush_clear(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_flush", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_clear(&pChan->pComBuf->txBuf);				// quantomeno svuota il buffer

	return(True);
}

bool csi_flush(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_flush", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_flush(&pChan->pComBuf->txBuf);				// quantomeno svuota il buffer

	return(True);
}

/* Azzera il buffer di ricezione del canale di comunicazione
	precedentemente creato ed aperto
*/
bool csi_freeTx(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_freeTx", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_clear(&pChan->pComBuf->txBuf);				// azzera il buffer

	return(True);
}


/* Azzera il buffer di ricezione del canale di comunicazione
	precedentemente creato ed aperto
*/
bool csi_clear(CSIHandle hChan)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_clear", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_clear(&pChan->pComBuf->rxBuf);				// azzera il buffer

	return(True);
}

/* Spara un carattere in un canale di comunicazione precedentemente aperto
*/
bool csi_putc(CSIHandle hChan, byte c)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_putc", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_putc(&pChan->pComBuf->txBuf, c) );	// schiaffa nel buffer
}

/* Spara una word (= 2bytes) in un canale di comunicazione precedentemente aperto
*/
bool csi_putw(CSIHandle hChan, word w)
{
CsiChan *pChan;
bool result;
byte *pd = (byte *) &w;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_putw", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	result = ringbuf_putc(&pChan->pComBuf->txBuf, pd[0]);	// schiaffa nel buffer
	result &= ringbuf_putc(&pChan->pComBuf->txBuf, pd[1]);	// schiaffa nel buffer

	return( result );
}


/* Piglia un carattere in un canale di comunicazione precedentemente aperto
*/
bool csi_getc(CSIHandle hChan, byte *c)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_getc", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_getc(&pChan->pComBuf->rxBuf, c) );	// piglia dal buffer direttamente
}

/* Piglia una word (=2 bytes)da un canale di comunicazione precedentemente aperto
*/
bool csi_getw(CSIHandle hChan, word &w)
{
CsiChan *pChan;
bool result;
byte *pd = (byte *) &w;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_getw", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	result = ringbuf_getc(&pChan->pComBuf->rxBuf, pd);	// piglia dal buffer direttamente
	result &= ringbuf_getc(&pChan->pComBuf->rxBuf, pd+1);
	return( result );
}

/* Spara un pacco in un canale di comunicazione precedentemente aperto
*/
bool csi_read(CSIHandle hChan, byte *pDataRx, word lenDataRx)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_read", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_read(&pChan->pComBuf->rxBuf, pDataRx, lenDataRx, False) );
}


// Scrive un pacco di dati dal canale richiesto
bool csi_write(CSIHandle hChan, byte *pDataTx, word lenDataTx)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_write", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_write(&pChan->pComBuf->txBuf, pDataTx, lenDataTx, False) );
}


// Riempie con lo stesso dato il canale richiesto
bool csi_fill(CSIHandle hChan, byte filler, word lenDataFill)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_fill", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_fill(&pChan->pComBuf->txBuf, filler, lenDataFill) );
}


// Legge un pacco di dati direttamente sul canale richiesto
bool csi_readBurst(CSIHandle hChan, byte *pDataRx, word lenDataRx)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_readBurst", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_read(&pChan->pComBuf->rxBuf, pDataRx, lenDataRx, True) );
}

// Scrive un pacco di dati direttamente sul buffer passato
bool csi_writeBurst(CSIHandle hChan, byte *pDataTx, word lenDataTx)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_writeBurst", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_write(&pChan->pComBuf->txBuf, pDataTx, lenDataTx, True) );
}



/* Ritorna lo spazio disponibile nel buffer di trasmissione
*/
word csi_getFreeTxCount(CSIHandle hChan)
{
CsiChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_getFreeTxCount", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(0);

	if( ringbuf_getFreeCount(&pChan->pComBuf->txBuf, &n) == True )
		return( n );			// piglia dal buffer

	return( 0 );				// niente di fatto
}


/* Ritorna lo spazio disponibile nel buffer di ricezione
*/
word csi_getFreeRxCount(CSIHandle hChan)
{
CsiChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_getFreeRxCount", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(0);

	if( ringbuf_getFreeCount(&pChan->pComBuf->rxBuf, &n) == True )
		return( n );			// piglia dal buffer

	return( 0 );				// niente di fatto
}



bool csi_getCount(CSIHandle hChan, word &n)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_getCount", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_getCount(&pChan->pComBuf->rxBuf, &n) );			// piglia dal buffer
}


bool csi_isEmpty(CSIHandle hChan)
{
CsiChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_isEmpty", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	if( ringbuf_getCount(&pChan->pComBuf->txBuf, &n) == False )
		return(False);

	if( n == 0 )
		return(True);
	else
		return(False);
}


/* Attiva o disattiva il chip-select (da usare con moderazione)
*/
bool csi_chipSel(CSIHandle hChan, int activate)
{
CsiChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("CSI", "csi_chipSel", csiError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != CSI_CHAN_OPEN )						// se non già aperto non può manomettere
		return(False);


	if( pChan->pfPort )
		pChan->pfPort(activate);							// comanda subito il ChipSel

	return(True);
}




/* Controlla che l'handle al buffer sia valido e ne ritorna il puntatore
*/
static CsiChan *checkHandle(CSIHandle hChan)
{
int elem;

	if( hChan.theHandle < CSI_BASE_HANDLE || hChan.theHandle >= numChanReg + CSI_BASE_HANDLE )
	{
		csiError = CSI_ERR_INV_HANDLE;
		return(NULL);
	}

	elem = hChan.theHandle - CSI_BASE_HANDLE;
	return(&csiRegTab[elem]);
}



//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2002/03/22 00:00:00 $
//@@  			Il vettore "csiRegTab" viene istanziato per primo perchè
//@@			il compilatore non allinea ai 4 bytes le variabili statiche
//@@			globali e quindi se prima del vettore dichiaro una variabile
//@@ 			di tipo "word" il vettore viene istanziato ad un indirizzo
//@@			allineato ai 2bytes anzichè ai 4bytes con conseguente
//@@			disallineamento degli elementi della struttura del vettore.
//@@
//@@  $Revision: 1.3 $
//@@  $Date: 2002/04/03 00:00:00 $
//@@  			Viene aggiunta la funzione "csi_clear" che azzera il buffer
//@@			di ricezione. Non ha senso, al momento, una clear per buffer
//@@			di trasmissione perchè già esiste la "csi_flush" che svuota
//@@ 			il buffer di trasmissione.
//@@  			Viene inoltre aggiunta la funzione "csi_fill" per sparare
//@@			blocchi di dati omogenei.
//@@  			Viene inoltre aggiunta la funzione "csi_putw" per sparare
//@@			due bytes alla volta (word) perchè fa' sempre comodo.
//@@
//@@  $Revision: 1.4 $
//@@  $Date: 2002/04/08 00:00:00 $
//@@  			Viene aggiunta la funzione "csi_chipSel" per accedere
//@@			dalle funzioni esterne al chip-select (senza dover passare
//@@			la funzione che attiva il chip-sel a tutto il mondo; in piu'
//@@ 			in questo modo rimane garantito il controllo sull'apertura
//@@  			del canale CSI.
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: CSI.CPP,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            P I C S E L                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//


