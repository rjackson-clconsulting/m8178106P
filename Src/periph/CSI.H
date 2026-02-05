//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               CSI.H
//  Descrizione:             Definizioni per gestione seriale sincrona
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2001/09/07 06:49:06 $
//
//---------------------------------------------------------------------------//

#ifndef _CSI_H
#define _CSI_H

#include "COMBUF.H"
//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define CSI_BASE_HANDLE		BASEMOD_CSI		// numero di partenza per l'handle al buffer
#define CSI_BASE_ERR		BASEMOD_CSI		// numero di partenza per gli errori
#define CSI_MAX_CHAN		8				// numero massimo di canali linkabili


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

enum CSIErrors{ CSI_ERR_NONE,
				CSI_ERR_INV_HANDLE=CSI_BASE_ERR,	// handle non valido
				CSI_ERR_END_CHAN					// fine canali disponibili
				};

typedef enum CSIErrors CSIErrors;


enum CSIState{ CSI_CHAN_IDLE=0,
			   CSI_CHAN_OPEN
			};
typedef enum CSIState CSIState;

union CSIHandle{
	int theHandle;
	word mod_id;						// parte bassa usata come module id per varie cose
};

typedef union CSIHandle CSIHandle;

												// dati di registrazione link canali CSI
struct CsiChan{
	void (*pfPort)(int activate);				// funzione che attiva e disattiva il chipsel a richiesta

	ComRingBuf		*pComBuf;								// puntatore al buffer per lo scambio dei dati
	CSIState		stato;									// stato di esecuzione del canale CSI
};

typedef struct CsiChan CsiChan;

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

void csi_init(void);
bool csi_status(CSIHandle hChan, CSIState *pstat);
CSIHandle csi_create(CBufHandle hBuf, void (*pfPort)(int) );
bool csi_open(CSIHandle hChan);
bool csi_close(CSIHandle hChan);
bool csi_flush(CSIHandle hChan);
bool csi_flush_clear(CSIHandle hChan);
bool csi_freeTx(CSIHandle hChan);
bool csi_clear(CSIHandle hChan);
bool csi_putc(CSIHandle hChan, byte c);
bool csi_putw(CSIHandle hChan, word w);
bool csi_getc(CSIHandle hChan, byte *c);
bool csi_getw(CSIHandle hChan, word &w);
bool csi_read(CSIHandle hChan, byte *pData, word lenData);
bool csi_write(CSIHandle hChan, byte *pData, word lenData);
bool csi_fill(CSIHandle hChan, byte filler, word lenDataFill);
bool csi_readBurst(CSIHandle hChan, byte *pData, word lenData);
bool csi_writeBurst(CSIHandle hChan, byte *pData, word lenData);
word csi_getFreeRxCount(CSIHandle hChan);
word csi_getFreeTxCount(CSIHandle hChan);
bool csi_getCount(CSIHandle hChan, word &n);
bool csi_isEmpty(CSIHandle hChan);
bool csi_chipSel(CSIHandle hChan, int activate);

#endif
