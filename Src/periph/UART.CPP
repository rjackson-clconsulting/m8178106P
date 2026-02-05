//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               UART.CPP
//  Descrizione:             Routines gestione com. su seriale asincrona
//  Note:             		 I pin di scambio rts-cts sono ricavati direttamente
//							 con pin di un port del micro
//  Supporto Fisico:         Hw Generico
//  Versione:                $Revision: 1.3 $
//  Data Versione:           $Date: 2002/03/26 00:00:00 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include "global.h"
#include "error.h"
#include "combuf.h"
#include "uart.h"


//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------
static UartChan *checkHandle(UARTHandle hChan);

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
static UartChan uartRegTab[UART_MAX_CHAN];
static short numChanReg;
static UARTErrors uartError;

/* Cerca di inizializzare la gestione uart (universal asinc rx tx)
*/
void uart_init(void)
{
int i;

	for(i=0; i<UART_MAX_CHAN; i++)					// init statoli dei canali
		uartRegTab[i].stato = UART_CHAN_IDLE;

	uartError = UART_ERR_NONE;
}



/* Ritorna lo stato di un canale
*/
bool uart_status(UARTHandle hChan, UARTState *pstat)
{

#ifdef __DEBUG__
	if( hChan.theHandle < UART_BASE_HANDLE || hChan.theHandle >= numChanReg + UART_BASE_HANDLE )
	{
		uartError = UART_ERR_INV_HANDLE;
		error_trace("UART", "uart_status", uartError);	// aspetta che lo vengano a prendere
		return(False);
	}

#endif

	hChan.theHandle -= UART_BASE_HANDLE;
	*pstat = uartRegTab[hChan.theHandle].stato;

	return(True);
}


/* Crea un nuovo canale di comunicazione per UART (universal asyn. rx+tx)
	ovvero... seriale sincrona
*/
UARTHandle uart_create(CBufHandle hBuf, void (*pfPort)(int) )
{
UARTHandle hChan;
ComRingBuf *pComBuf;

	hChan.theHandle = 0;					// null handle

	if( numChanReg >= UART_MAX_CHAN )
	{
		uartError = UART_ERR_END_CHAN;

#ifdef __DEBUG__
		error_trace("UART", "uart_create", uartError);		// aspetta che lo vengano a prendere
#endif
		return( hChan );
	}

	uartRegTab[numChanReg].pfPort = pfPort;				// funzione che modifica il port tramite un latch o altro

	if( pfPort )
		pfPort(OFF);									// disattiva subito il ChipSel

	combuf_getPointer(hBuf, &pComBuf); 					// si fa dare il puntatore al buffer per cortesia

	uartRegTab[numChanReg].pComBuf = pComBuf;				// lo registra

	hChan.theHandle = numChanReg + UART_BASE_HANDLE;

	numChanReg++;										// abbiamo registrato un canale in +

	return( hChan );
}


/* Apre un canale di comunicazione precedentemente creato
*/
bool uart_open(UARTHandle hChan)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_open", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_IDLE )					// se è già aperto non può riaprirlo
		return(False);

	if( pChan->pComBuf->userLock != 0 )						// se qualcuno lo sta usando non può usarlo
		return(False);

	pChan->stato = UART_CHAN_OPEN;							// il canale è aperto !!

	pChan->pComBuf->userLock = hChan.mod_id;				// lo blocca un attimino (col proprio handle)

	if( pChan->pfPort )
		pChan->pfPort(ON);									// attiva il chip select

	return(True);
}


/* Chiude un canale di comunicazione precedentemente creato ed aperto
	liberando cosi' il buffer di comunicazione per un'altro canale
*/
bool uart_close(UARTHandle hChan)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_close", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_flush(&pChan->pComBuf->txBuf);					// quantomeno svuota il buffer

	pChan->pComBuf->userLock = 0;							// sblocca il buffer per gli altri

	if( pChan->pfPort )
		pChan->pfPort(OFF);									// disattiva subito il ChipSel

	pChan->stato = UART_CHAN_IDLE;							// è di nuovo libero e bello

	return(True);
}

/* Flusha il canale di comunicazione precedentemente creato ed aperto
	terminando cosi' la scrittura
*/
bool uart_flush(UARTHandle hChan)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_flush", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	ringbuf_flush(&pChan->pComBuf->txBuf);					// quantomeno svuota il buffer

	return(True);
}


/* Spara un carattere in un canale di comunicazione precedentemente aperto
*/
bool uart_putc(UARTHandle hChan, byte c)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_putc", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_putc(&pChan->pComBuf->txBuf, c) );	// schiaffa nel buffer
}


/* Piglia un carattere in un canale di comunicazione precedentemente aperto
*/
bool uart_getc(UARTHandle hChan, byte *c)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_getc", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_getc(&pChan->pComBuf->rxBuf, c) );	// piglia dal buffer direttamente
}


/* Spara un pacco in un canale di comunicazione precedentemente aperto
*/
bool uart_read(UARTHandle hChan, void *pDataRx, word lenDataRx)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_read", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_read(&pChan->pComBuf->rxBuf, pDataRx, lenDataRx, False) );
}


/* Scrive un pacco di dati dal canale richiesto
*/
bool uart_write(UARTHandle hChan, void *pDataTx, word lenDataTx)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_write", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_write(&pChan->pComBuf->txBuf, pDataTx, lenDataTx, False) );
}


/* Spara una stringa sul canale richiesto
*/
bool uart_puts(UARTHandle hChan, char *pText)
{
UartChan *pChan;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_write", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	return( ringbuf_write(&pChan->pComBuf->txBuf, pText, strlen(pText), False) );
}




/* Ritorna il numero di caratteri presenti nel buffer di ricezione
*/
word uart_getRxCount(UARTHandle hChan)
{
UartChan *pChan = NULL;
word n = 0;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_getRxCount", uartError);		// aspetta che lo vengano a prendere
#endif
		return(0);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(0);

	if( ringbuf_getCount(&pChan->pComBuf->rxBuf, &n) == True )
		return( n );			// piglia dal buffer

	return( 0 );				// niente di fatto
}


/* Ritorna il numero di caratteri presenti nel buffer di trasmissione
*/
word uart_getTxCount(UARTHandle hChan)
{
UartChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_getTxCount", uartError);		// aspetta che lo vengano a prendere
#endif
		return(0);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(0);

	if( ringbuf_getCount(&pChan->pComBuf->txBuf, &n) == True )
		return( n );			// piglia dal buffer

	return( 0 );				// niente di fatto
}


/* Ritorna lo spazio disponibile nel buffer di trasmissione
*/
word uart_getFreeTxCount(UARTHandle hChan)
{
UartChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_getFreeTxCount", uartError);		// aspetta che lo vengano a prendere
#endif
		return(0);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(0);

	if( ringbuf_getFreeCount(&pChan->pComBuf->txBuf, &n) == True )
		return( n );			// piglia dal buffer

	return( 0 );				// niente di fatto
}

/* Verifica se il buffer di trasmissione è vuoto
*/
bool uart_isTxEmpty(UARTHandle hChan)
{
UartChan *pChan;
word n;

	pChan = checkHandle(hChan);

	if( pChan == NULL )
	{
#ifdef __DEBUG__
		error_trace("UART", "uart_isTxEmpty", uartError);		// aspetta che lo vengano a prendere
#endif
		return(False);
	}

	if( pChan->stato != UART_CHAN_OPEN )					// se non già aperto non può chiuderlo
		return(False);

	if( ringbuf_getCount(&pChan->pComBuf->txBuf, &n) == False )
		return(False);

	if( n == 0 )
		return(True);
	else
		return(False);
}



/* Controlla che l'handle al buffer sia valido e ne ritorna il puntatore
*/
static UartChan *checkHandle(UARTHandle hChan)
{
int elem;

	if( hChan.theHandle < UART_BASE_HANDLE || hChan.theHandle >= numChanReg + UART_BASE_HANDLE )
	{
		uartError = UART_ERR_INV_HANDLE;
		return(NULL);
	}

	elem = hChan.theHandle - UART_BASE_HANDLE;

	return(&uartRegTab[elem]);
}




//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2002/03/22 00:00:00 $
//@@  			Il vettore "uartRegTab" viene istanziato per primo perchè
//@@			il compilatore non allinea ai 4 bytes le variabili statiche
//@@			globali e quindi se prima del vettore dichiaro una varibile
//@@ 			di tipo "word" il vettore viene istanziato ad un indirizzo
//@@			allineato ai 2bytes anzichè ai 4bytes con conseguente
//@@			disallineamento degli elementi della struttura del vettore.
//@@
//@@  $Revision: 1.3 $
//@@  $Date: 2002/03/26 00:00:00 $
//@@  			Viene aggiunta la funzione "uart_getFreeTxCount" per
//@@			comodità in modo da sapere quanti byte scrivere nel buffer
//@@			con la "uart_write" senza incappare in errori
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: UART.CPP,v $
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


