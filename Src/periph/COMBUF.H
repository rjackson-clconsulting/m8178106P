//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               COMBUF.H
//  Descrizione:             Definizioni gestione buffer di comunic. generico
//  Versione:                $Revision: 1.3 $
//  Data Versione:           $Date: 2002/03/26 00:00:00 $
//
//---------------------------------------------------------------------------//


#ifndef _COMBUF_H_
#define _COMBUF_H_

#include "error.h"
#include "alloc.h"
#include "fm3_define.h"
#include "stdDataTypes.h"

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define COMBUF_BASE_HANDLE	BASEMOD_COMBUF		// numero di partenza per l'handle al buffer
#define COMBUF_BASE_ERR		BASEMOD_COMBUF		// numero di partenza per gli errori
#define COMBUF_MAX_CHAN		8					// numero massimo di buffer allocabili
                                                // 8 for MB9BFD18T Fujitsu-Cypress


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//


enum CBUFErrors{ CBUF_ERR_NONE,
				 CBUF_ERR_INV_HANDLE=COMBUF_BASE_ERR,	// handle non valido
				 CBUF_ERR_INV_POINTER,		// puntatore non valido
				 CBUF_ERR_UNDER_RUN,		// sotto-scorta
				 CBUF_ERR_OVER_RUN,			// sopra scorta
				 CBUF_ERR_INV_BURST		// modalità burst non corretta
				};

typedef enum CBUFErrors CBUFErrors;


union CBufHandle{						// handle al buffer di comunicazione
	int theHandle;
	word mod_id;						// parte bassa usata come module id per varie cose
};

typedef union CBufHandle CBufHandle;


struct RingBuf{							// definizione di un buffer circolare
    word  	count;                    	// number of characters in the buffer
    word	size;						// buffer maximum size
    word 	idxInsert;            		// index to where next character will be inserted
    word 	idxExtract;                 // index from where next character will be extracted
    byte  	*pData;     				// Ring buffer character storage
 	void	(*pfSogliaFun)(byte c);
    word	sogliaCall;					// soglia (numero di car nel buffer) a cui chiamare la funzione utente
};										// n.d.r.: usando i puntatori è molto + veloce l'esecuzione
										// (circa il doppio) ma si spreca un sacco di memoria ram
										// (circa il triplo) quindi parsimonia...

typedef struct RingBuf RingBuf;


struct ComRingBuf{					// doppio buffer per comunicazione
	RingBuf	txBuf;						// ring buffer tx
	RingBuf rxBuf;						// ring buffer rx
	word	userLock;					// handle utente in blocco buffer
};

typedef struct ComRingBuf ComRingBuf;

void combuf_init(void);
CBufHandle combuf_create(word txBufSize, word rxBufSize, word sogliaTx, word sogliaRx);
bool combuf_destroy(CBufHandle hBuf);
bool combuf_getPointer(CBufHandle hBuf, ComRingBuf **pcom);
bool combuf_getc(CBufHandle hBuf, byte *c);
bool combuf_putc(CBufHandle hBuf, byte c);

bool ringbuf_getCount(RingBuf *pbuf, word *count);
bool ringbuf_getFreeCount(RingBuf *pbuf, word *count);

bool ringbuf_getc(RingBuf *pbuf, byte *c);
bool ringbuf_putc(RingBuf *pbuf, byte c);
bool ringbuf_read(RingBuf *pbuf, void *pData, word lenData, bool burstMode);
bool ringbuf_write(RingBuf *pbuf, void *pData, word lenData, bool burstMode);
bool ringbuf_fill(RingBuf *pbuf, byte filler, word lenData);
bool ringbuf_clear(RingBuf *pbuf);
bool ringbuf_flush(RingBuf *pbuf);

#endif
