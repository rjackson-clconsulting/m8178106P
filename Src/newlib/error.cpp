// uguali tra uC e uP???
// pari pari quello del kibou

//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                LIBGEN
//  Nome File:               ERROR.CPP
//  Descrizione:             Routines gestione traccia errori nei vari moduli
//  Supporto Fisico:         Hw Generico
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2011/03/31 15:50:52 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "error.h"
#include "print.h"
#include "semihosting.h"
//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
static int errorCount;

/* Cerca di inizializzare il modulo di gestione errori */
void error_init(void)
{
	errorCount = 0;
}

/* Traccia un errore sparazzandolo sulla finestra di monitor */
void error_trace(const char *sModule, const char *sFunction, int modErr)
{
	errorCount++;										// uno in +
	SH_SendString("error_trace:");
	SH_SendString(sModule);
	SH_SendString(", ");
	SH_SendString(sFunction);
	SH_SendString("\n");

}


