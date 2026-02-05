//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               COMBUF.CPP
//  Descrizione:             Routines gestione buffer di comunicazione generico
//							 realizzato con una coda circolare
//  Note:             		 L'inserimento e l'estrazione dei dati viene
//							 realizzato tramite indici anzichè puntatori per
//							 risparmiare un po' di ram
//  Supporto Fisico:         Hw non specifico(ad eccezione delle istruzioni
//							 per abilitare e disabilitare gli interrupt
//	Utilizzato su:			 EVB PicSel
//							 Multichannel PH Menfis
//  Versione:                $Revision: 1.5 $
//  Data Versione:           $Date: 2002/04/03 00:00:00 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include "combuf.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------
static ComRingBuf *combuf_checkHandle(CBufHandle hBuf);
static bool ringbuf_checkPointer(RingBuf *pbuf);


//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
static ComRingBuf comRegTab[COMBUF_MAX_CHAN];
static CBUFErrors combufError;
static short numChanReg;


/* Cerca di inizializzare la gestione csi
*/
void combuf_init(void)
{
	combufError = CBUF_ERR_NONE;
	numChanReg = 0;						// per ora niente di fatto..
}


/* Apre un canale di comunicazione bufferizzato generico
*/
CBufHandle combuf_create(word txBufSize, word rxBufSize, word sogliaTx, word sogliaRx)
{
CBufHandle hChan;
ComRingBuf *pChan;

	hChan.theHandle = 0;										// parte da un handle non valido
	hChan.mod_id = 0;

	//Input parameter Check
	if ((txBufSize <= 0) || (rxBufSize <= 0) ||
        (sogliaRx <= 0) || (sogliaTx <= 0))
        return (hChan);

	if( numChanReg >= COMBUF_MAX_CHAN )
		return( hChan );

	pChan = &comRegTab[numChanReg];

	pChan->userLock = 0;

	pChan->txBuf.count = 0;										// reset anche se non serve
	pChan->txBuf.size = 0;
	pChan->txBuf.idxInsert = 0;
	pChan->txBuf.idxExtract = 0;
	pChan->txBuf.pData = 0;
	pChan->txBuf.pfSogliaFun = 0;
	pChan->txBuf.sogliaCall = sogliaTx;

	pChan->rxBuf.count = 0;										// reset anche se non serve
	pChan->rxBuf.size = 0;
	pChan->rxBuf.idxInsert = 0;
	pChan->rxBuf.idxExtract = 0;
	pChan->rxBuf.pData = 0;
	pChan->rxBuf.pfSogliaFun = 0;
	pChan->rxBuf.sogliaCall = sogliaRx;

	if( txBufSize > 1 )											// se vogliono il buffer di tx consistente
		mem_alloc(txBufSize, (void **) &pChan->txBuf.pData);
	else
		pChan->txBuf.pData = (byte *) &pChan->txBuf.idxInsert;			// buffer di un solo carattere ricavato sui dati interni

	if( pChan->txBuf.pData != NULL )							// se ce n'è
	{
		pChan->txBuf.size = txBufSize;							// ne tiene conto

		if( rxBufSize > 1 )										// se vogliono il buffer di rx consistente
			mem_alloc(rxBufSize, (void **) &pChan->rxBuf.pData);
		else
			pChan->rxBuf.pData = (byte *) &pChan->rxBuf.idxInsert;		// buffer di un solo carattere ricavato sui dati interni

		if( pChan->rxBuf.pData != NULL )						// se ce n'è
		{
			pChan->rxBuf.size = rxBufSize;						// ne tiene conto
			hChan.theHandle = numChanReg + COMBUF_BASE_HANDLE;	// somma un offset comune a tutti

			numChanReg++;										// abbiamo registrato un canale in +
		}
		else
		{
			if( txBufSize )										// se l'aveva realmente allocato
				mem_free(pChan->txBuf.pData);					// deve lasciare pure questa (mica puo' andare zoppo)

			pChan->txBuf.pData = NULL;
			pChan->txBuf.size = 0;								// reset pure questo
		}
	}

	return( hChan );
}

/* Distrugge un buffer pre allocato
*/
bool combuf_destroy(CBufHandle hBuf)
{
ComRingBuf *pChan;

	pChan = combuf_checkHandle(hBuf);

#ifdef __DEBUG__

	if( pChan == NULL )
	{
		error_trace("COMBUF", "combuf_destroy", combufError);	// aspetta che lo vengano a prendere
		return(False);
	}

#else

	if( pChan == NULL )
		return(False);
#endif

										// se erano stati allocati ..
										// li libera e azzera
	if( pChan->rxBuf.pData != NULL && pChan->rxBuf.size > 1 )
	{
		mem_free(pChan->rxBuf.pData);
		pChan->rxBuf.pData = NULL;
	}

	if( pChan->txBuf.pData != NULL && pChan->txBuf.size > 1 )
	{
		mem_free(pChan->txBuf.pData);
		pChan->txBuf.pData = NULL;
	}

	return(True);
}


/* Ritorna il puntatore diretto al buffer per usi rapidi
*/
bool combuf_getPointer(CBufHandle hBuf, ComRingBuf **pcom)
{

	*pcom = combuf_checkHandle(hBuf);		// rapido controllo

#ifdef __DEBUG__

	if( *pcom == NULL )
	{
		error_trace("COMBUF", "combuf_getPointer", combufError);	// aspetta che lo vengano a prendere
		return(False);
	}

#else

	if( *pcom == NULL )
		return(False);
#endif

	return( True );
}


/* Piglia un carattere dal buffer di ricezione
*/
bool combuf_getc(CBufHandle hBuf, byte *c)
{
ComRingBuf *pbuf;

	pbuf = combuf_checkHandle(hBuf);

#ifdef __DEBUG__

	if( pbuf == NULL )
	{
		error_trace("COMBUF", "combuf_getc", combufError);	// aspetta che lo vengano a prendere
		return(False);
	}

#else

	if( pbuf == NULL )
		return(False);
#endif


	while( pbuf->rxBuf.count == 0 );			// aspetta che arrivi un carattere

	return( ringbuf_getc(&pbuf->rxBuf, c) );
}

/* Spara un carattere nel buffer di trasmissione
*/
bool combuf_putc(CBufHandle hBuf, byte c)
{
ComRingBuf *pbuf;

	pbuf = combuf_checkHandle(hBuf);

#ifdef __DEBUG__

	if( pbuf == NULL )
	{
		error_trace("COMBUF", "combuf_putc", combufError);	// aspetta che lo vengano a prendere
		return(False);
	}

#else

	if( pbuf == NULL )
		return(False);
#endif

    while( pbuf->txBuf.count >= pbuf->txBuf.size );			// aspetta che ci venga un po' di posto

	return( ringbuf_putc(&pbuf->txBuf, c) );
}


/* Controlla che l'handle al buffer sia valido e ne ritorna il puntatore
*/
static ComRingBuf *combuf_checkHandle(CBufHandle hBuf)
{
register int elem = 0xFFFF;   //Init with a not valid value

	if( hBuf.theHandle < COMBUF_BASE_HANDLE || hBuf.theHandle >= numChanReg + COMBUF_BASE_HANDLE )
	{
		combufError = CBUF_ERR_INV_HANDLE;
		return(NULL);
	}

	elem = hBuf.theHandle - COMBUF_BASE_HANDLE;

	return(&comRegTab[elem]);
}


////////////////////////////////////////////////////////////////////
//
// Funzioni per accesso diretto ai buffers
//
////////////////////////////////////////////////////////////////////

/* Ritorna il numero di caratteri nel buffer
*/
bool ringbuf_getCount(RingBuf *pbuf, word *count)
{
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_getCount", combufError);	// segnala l'errore
		return(False);
	}

#endif

	*count = pbuf->count;
	return( True );
}

/* Ritorna il numero di caratteri "liberi" nel buffer
*/

bool ringbuf_getFreeCount(RingBuf *pbuf, word *count)
{
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_getFreeCount", combufError);	// segnala l'errore
		return(False);
	}

#endif

	*count = pbuf->size - pbuf->count;
	return( True );
}

/* Prova a pigliare un carattere dal buffer richiesto
*/
bool ringbuf_getc(RingBuf *pbuf, byte *c)
{

#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_getc", combufError);	// segnala l'errore
		return(False);
	}

#endif

    if( pbuf->count == 0 )						// se sono finiti
    {
    	combufError = CBUF_ERR_UNDER_RUN;		// spiacente ma non posso farci niente

    	//pbuf->pfEnd();						// segnala che la festa è finita (nella prossima versione)
    	return(False);
    }

	//asm ("DI");	sergio								// mask int.

    pbuf->count--;

	//asm ("EI");	sergio								// restore mask int.

    *c = pbuf->pData[pbuf->idxExtract++];       // Get character from buffer

    if( pbuf->idxExtract >= pbuf->size )
        pbuf->idxExtract = 0;

    return( True );								// tutto sotto controllo
}

/* Prova a sparare un carattere nel buffer richiesto
*/
bool ringbuf_putc(RingBuf *pbuf, byte c)
{
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_putc", combufError);	// segnala l'errore
		return(False);
	}

#endif

	if( pbuf->size > 1 )						// se il buffer esiste
	{
		if( pbuf->count >= pbuf->size )				// se non c'è + posto
		{
			combufError = CBUF_ERR_OVER_RUN;		// spiacente
			return( False );
		}

		pbuf->pData[pbuf->idxInsert++] = c;

		//asm ("DI");		sergio							// mask int.

		pbuf->count++;

		//asm ("EI");		sergio							// enable int.

		if( pbuf->idxInsert >= pbuf->size )
		    pbuf->idxInsert = 0;

		if( pbuf->count >= pbuf->sogliaCall && pbuf->pfSogliaFun )	// se va chiamata
		    pbuf->pfSogliaFun(c);					// la chiama
	}
	else
	{
		pbuf->pData[0] = c;							// lo mette da parte

		pbuf->pfSogliaFun(c);						// chiama lo sparatore
	}

	return(True);
}


/* Prova a pigliare un pacco di dati dal buffer richiesto
*/
bool ringbuf_read(RingBuf *pbuf, void *pData, word lenData, bool burstMode)
{
word lenRead = 0;
register byte *pd = (byte *) pData;

#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_read", combufError);	// segnala l'errore
		return(False);
	}

#endif

	if( burstMode == True )									// se sono pazzi e vogliono il burst mode
	{

		if( pbuf->size > 1 )						// se ha un buffer non si può (solo polling o DMA burst)
		{

			combufError = CBUF_ERR_INV_BURST;						// non possiamo proprio
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
			error_trace("COMBUF", "ringbuf_read", combufError);	// segnala l'errore
#endif
			return(False);
		}

		pbuf->count = lenData;						// imposta contatori e puntatori passati
		pbuf->pData = pd;

		pbuf->pfSogliaFun(1);		// il parametro '1' serve per chiedere la direzione di transfer (input)
	}
	else
	{
		if( pbuf->size == 1 )										// se c'è un solo byte come buffer
		{
	   																// di sicuro usano la periferica in polling
			while( lenRead < lenData )
			{
		        pbuf->pfSogliaFun(0);								// fa spedire il prossimo byte (solo per comunicare)

				*pd++ = *pbuf->pData;							// piglia il nuovo carattere ricevuto

				lenRead++;											// un byte in + sparato
			}
		}
		else
		{
			while( lenRead < lenData )
			{
			    if( pbuf->count == 0 )									// se sono finiti
			    {
			    	combufError = CBUF_ERR_UNDER_RUN;						// spiacente ma non posso farci niente
			    	return(False);
			    }
				else
				{
					pbuf->count--;										// toglie un carattere

				    *pd++ = pbuf->pData[pbuf->idxExtract++];

				    lenRead++;											// un byte in + letto

				    if( pbuf->idxExtract >= pbuf->size )
				        pbuf->idxExtract = 0;
				}
			}
		}
	}

	return(True);
}


/* Prova a copiare un pacco di dati sul buffer richiesto
*/
bool ringbuf_write(RingBuf *pbuf, void *pData, word lenData, bool burstMode)
{
word lenWrite = 0;
byte c = 0xFF;
register byte *pd = (byte *) pData;

#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_write", combufError);	// segnala l'errore
		return(False);
	}

#endif


	if( burstMode == True )									// se sono pazzi e vogliono il burst mode
	{

		if( pbuf->size > 1 )						// se ha un buffer non si può (solo polling o DMA burst)
		{

			combufError = CBUF_ERR_INV_BURST;						// non possiamo proprio
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
			error_trace("COMBUF", "ringbuf_write", combufError);	// segnala l'errore
#endif
			return(False);
		}

		pbuf->count = lenData;						// imposta contatori e puntatori passati
		pbuf->pData = pd;
		pbuf->idxExtract = 1;						// flag estrazione = 1 => write

		pbuf->pfSogliaFun(0);		// il parametro '0' serve per chiedere la direzione di transfer (output)

		pbuf->pData = (byte *) &pbuf->idxInsert;				// ripristina l'originale
	}
	else
	{
		if( pbuf->size == 1 )										// se c'è un solo byte come buffer
		{
																	// di sicuro usano la periferica in polling
			while( lenWrite < lenData )
			{
				c = *pd++;										// piglia il nuovo carattere da spedire
				*pbuf->pData = c;

		        pbuf->pfSogliaFun(c);								// fa spedire il prossimo byte

				lenWrite++;											// un byte in + sparato
			}
		}
		else
		{
			while( lenWrite < lenData )
			{
			    if( pbuf->count >= pbuf->size )							// se non c'è + posto
			    {
		    		combufError = CBUF_ERR_OVER_RUN;					// spiacente
					return( False );
				}
				else
				{
					pbuf->count++;										// aggiunge un carattere

				    pbuf->pData[pbuf->idxInsert++] = *pd++;

				    lenWrite++;											// un byte in + sparato

				    if( pbuf->idxInsert >= pbuf->size )
				        pbuf->idxInsert = 0;
				 }
			}

		    if( pbuf->count >= pbuf->sogliaCall )						// se va chiamata
		        pbuf->pfSogliaFun(c);									// la chiama
		}
	}

	return(True);
}

/* Prova a riempire il buffer col valore richiesto
*/
bool ringbuf_fill(RingBuf *pbuf, byte filler, word lenData)
{
word lenWrite = 0;

#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_fill", combufError);	// segnala l'errore
		return(False);
	}

#endif


	if( pbuf->size == 1 )										// se c'è un solo byte come buffer
	{
																// di sicuro usano la periferica in polling
		while( lenWrite < lenData )
		{
			*pbuf->pData = filler;
	        pbuf->pfSogliaFun(filler);							// fa spedire il prossimo byte
			lenWrite++;											// un byte in + sparato
		}
	}
	else
	{
		while( lenWrite < lenData )
		{
		    if( pbuf->count >= pbuf->size )							// se non c'è + posto
		    {
	    		combufError = CBUF_ERR_OVER_RUN;							// spiacente
				return( False );
			}
			else
			{
				pbuf->count++;										// aggiunge un carattere

			    pbuf->pData[pbuf->idxInsert++] = filler;

			    lenWrite++;											// un byte in + sparato

			    if( pbuf->idxInsert >= pbuf->size )
			        pbuf->idxInsert = 0;
			 }
		}

	    if( pbuf->count >= pbuf->sogliaCall )						// se va chiamata
	        pbuf->pfSogliaFun(filler);								// la chiama
	}

	return(True);
}

/* Azzera il buffer richiesto
*/
bool ringbuf_clear(RingBuf *pbuf)
{
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_clear", combufError);	// segnala l'errore
		return(False);
	}

#endif

	pbuf->count = 0;											// reset conteggi e indici
    pbuf->idxInsert = 0;
    pbuf->idxExtract = 0;

	return(True);
}

bool ringbuf_flush(RingBuf *pbuf)
{
#ifdef __DEBUG__

				// controlla che il puntatore sia valido
	if( ringbuf_checkPointer(pbuf) == False )				// se non va bene
	{
		error_trace("COMBUF", "ringbuf_flush", combufError);	// segnala l'errore
		return(False);
	}

#endif

	while( pbuf->count )						// fino a che ci sono dati
        pbuf->pfSogliaFun(0);					// cerca di svuotarlo

	return(True);
}


/* Controlla che il puntatore al buffer sia valido e sensato
*/
static bool ringbuf_checkPointer(RingBuf *pbuf)
{
int 	i;
ComRingBuf *pComReg;

	if( pbuf != NULL )											// chiaramente solo se ha senso
	{
		pComReg = &comRegTab[0];

		for(i=0; i<numChanReg; i++, pComReg++)					// controlla che sia uno dei possibili puntatori
		{
			if( pbuf == &pComReg->txBuf || pbuf == &pComReg->rxBuf )
				return(True);
		}
	}

	combufError = CBUF_ERR_INV_POINTER;								// puntatore non valido
	return(False);
}



//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2002/03/22 00:00:00 $
//@@  			Il vettore "comRegTab" viene istanziato per primo perchè
//@@			il compilatore non allinea ai 4 bytes le variabili statiche
//@@			globali e quindi se prima del vettore dichiaro una varibile
//@@ 			di tipo "word" il vettore viene istanziato ad un indirizzo
//@@			allineato ai 2bytes anzichè ai 4bytes con conseguente
//@@			disallineamento degli elementi della struttura del vettore.
//@@
//@@  $Revision: 1.3 $
//@@  $Date: 2002/03/26 00:00:00 $
//@@  			Viene aggiunta la funzione "ringbuf_getFreeCount" per
//@@			comodità in modo da sapere quanti byte scrivere nel buffer
//@@			con la "ringbuf_write" senza incappare in errori
//@@
//@@  $Revision: 1.4 $
//@@  $Date: 2002/04/03 00:00:00 $
//@@  			Viene aggiunta la funzione "ringbuf_fill" per riempire
//@@			com dati omogenei il buffer e accelerare la scrittura
//@@			evitando dei loop con la "ringbuf_putc"
//@@
//@@  $Revision: 1.5 $
//@@  $Date: 2003/05/12 00:00:00 $
//@@  			Viene modificata la funzione "ringbuf_putc" perchè
//@@			mantenva disabilitate le interruzioni (istruz. DI).
//@@
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: COMBUF.CPP,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
