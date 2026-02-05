//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                MULTICH
//  Nome File:               UART.H
//  Descrizione:             Definizioni per gestione seriale asincrona
//  Versione:                $Revision: 1.3 $
//  Data Versione:           $Date: 2002/03/26 00:00:00 $
//
//---------------------------------------------------------------------------//


#ifndef _UART_H_
#define _UART_H_

#include "combuf.h"

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define UART_BASE_HANDLE	BASEMOD_UART	// numero di partenza per l'handle al buffer
#define UART_BASE_ERR		BASEMOD_UART	// numero di partenza per gli errori
#define UART_MAX_CHAN		4				// numero massimo di canali linkabili


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

enum UARTErrors{ UART_ERR_NONE,
				 UART_ERR_INV_HANDLE=UART_BASE_ERR,	// handle non valido
				 UART_ERR_END_CHAN					// fine canali disponibili
			};

typedef enum UARTErrors UARTErrors;

enum UARTState{ UART_CHAN_IDLE=0,
			   UART_CHAN_OPEN
			};

typedef enum UARTState UARTState;


union UARTHandle{
	int theHandle;
	word mod_id;			// parte bassa usata come module id per varie cose
};

typedef union UARTHandle UARTHandle;

//UARTHandle hUartDevB;
												// dati di registrazione link canali UART
struct UartChan{
	void (*pfPort)(int activate);				// funzione che attiva e disattiva il chipsel a richiesta

	ComRingBuf		*pComBuf;								// puntatore al buffer per lo scambio dei dati
	UARTState		stato;									// stato di esecuzione del canale uart
};

typedef struct UartChan UartChan;

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

void uart_init(void);
bool uart_status(UARTHandle hChan, UARTState *pstat);
UARTHandle uart_create(CBufHandle hBuf, void (*pfPort)(int) );
bool uart_open(UARTHandle hChan);
bool uart_close(UARTHandle hChan);
bool uart_flush(UARTHandle hChan);
bool uart_putc(UARTHandle hChan, byte c);
bool uart_getc(UARTHandle hChan, byte *c);
bool uart_read(UARTHandle hChan, void *pData, word lenData);
bool uart_write(UARTHandle hChan, void *pData, word lenData);
bool uart_puts(UARTHandle hChan, char *pText);
word uart_getRxCount(UARTHandle hChan);
word uart_getTxCount(UARTHandle hChan);
word uart_getFreeTxCount(UARTHandle hChan);
bool uart_isTxEmpty(UARTHandle hChan);

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//




#endif





//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
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
//@@  $RCSfile: UART.H,v $
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
