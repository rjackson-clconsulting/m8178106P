//---------------------------------------------------------------------------//
//                                                                           //
//                             C O M E T A                                   //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                CARDIOSMART
//  Nome File:               KEY.CPP
//  Descrizione:             Funzioni per gestione buffer di tastiera
//
//  Note:
//
//  Supporto Fisico:         CpuCardio
//  Versione:                $Revision: 1.2 $
//  Data Versione:           $Date: 2004/03/05 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include "key.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

long cntEncSelector;		// conteggio tipo encoder del selettore	manuale
long precEncSelector;		// conteggio tipo encoder del selettore	manuale

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

#define	KEYFLG_HELP		0x0001				// bittuzzi di stato dei tasti
#define	KEYFLG_SILENT	0x0002
#define	KEYFLG_START	0x0004
#define	KEYFLG_ENTER	0x0008

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
static word    keybuf[MAX_KEYBUF];
static word    keyind;

static word		keyStatus;
static word		selKeyTest;

static byte		cntKeyEnter;				// contatori per i tasti
static byte		cntKeySilent;
static byte		cntKeyStart;
static byte		cntKeyHelp;

static byte		cntCHA;
static byte		cntCHB;
static byte		precCHA;
static byte		precCHB;

//-------------------------------------------------------
// Corpo funzioni di questo modulo
//-------------------------------------------------------
void key_init(void)
{
	cntEncSelector = 0;
	precEncSelector = 0;

	keyStatus = 0x0000;
	selKeyTest = 0;

/*
	EGP0 = 0;
	EGN0 = 0;

	_PM100 = 1;			// PinTASTO_1 => input
	_PM101 = 1;         // PinTASTO_2 => input
	_PM102 = 1;         // PinTASTO_3 => input
    _PM103 = 1;         // PinTASTO_4 => input
	_PM104 = 1;         // PinTASTO_5 => input
	_PM04 = 1;          // PinSEL_CHA => input
	_PM03 = 1;          // PinSEL_CHB => input
	_PM00 = 1;          // PinSEL_SW  => input
*/


}


/* Guarda se c'e` un tasto premuto
*/
word keypres()
{
    if( keyind )
        return(1);

    return( 0 );
}

/* Rimette il tasto premuto nel buffer di tastiera
*/
word ungetk(word c)
{
    if( keyind < MAX_KEYBUF )
        return( (keybuf[keyind++] = c) );
    else
        return(0);
}

/* Rimette la stringa di tasti premuti nel buffer di tastiera
*/
word ungetks(char *s)
{
word i=0;

    while( *s )
        i = ungetk(*s++);

    return( i );
}


/* Ritorna il carattere letto da tastiera
*/
word getk()
{
    if( keyind )    /* se ce n'e` nel serbatoio */
        return(keybuf[--keyind]);

	return(0);
}


void key_scan_selector(void)
{
							//
							// controllo selettore a polling
							//
	if(	PinSEL_CHA )		// rising edge ?
	{
		if(	cntCHB )
			cntCHB--;
		else
			precCHB	= 0;		// fronte di discesa

		if(	cntCHA < 2 )
			cntCHA++;
		else
		{
			if(	!precCHA )
			{
				if(	PinSEL_CHB )
					cntEncSelector++;
				else
					cntEncSelector--;

				precCHA	= 1;		// fronte di salita
			}
		}
	}
	else							// falling edge...
	{

		if(	cntCHA )
			cntCHA--;
		else
			precCHA	= 0;		// fronte di salita

		if(	cntCHB < 2 )
			cntCHB++;
		else
		{
			if(	!precCHB )
			{
				if(	PinSEL_CHB )
					cntEncSelector--;
				else
					cntEncSelector++;

				precCHB	= 1;		// fronte di discesa
			}
		}
	}


}


void key_scan(void)
{

	switch( selKeyTest )
	{
		case	0:
			if( !PinTASTO_1 )
			{
				if( cntKeyHelp < 5 )
					cntKeyHelp++;
				else
				{
					if( !(keyStatus & KEYFLG_HELP) )
					{
						ungetk(CTR_SHF_F1);
						keyStatus |= KEYFLG_HELP;
					}
				}
			}
			else
			{
				if( cntKeyHelp )
					cntKeyHelp--;
				else
					keyStatus &= ~KEYFLG_HELP;
			}
			selKeyTest++;
			break;

		case	1:
			if( !PinTASTO_2 )
			{
				if( cntKeySilent < 5 )
					cntKeySilent++;
				else
				{
					if( !(keyStatus & KEYFLG_SILENT) )
					{
						ungetk(CTR_SHF_F2);
						keyStatus |= KEYFLG_SILENT;
					}
				}
			}
			else
			{
				if( cntKeySilent )
					cntKeySilent--;
				else
					keyStatus &= ~KEYFLG_SILENT;
			}

			selKeyTest++;
			break;

		case	2:
			if( !PinTASTO_3 )
			{
				if( cntKeyStart < 5 )
					cntKeyStart++;
				else
				{
					if( !(keyStatus & KEYFLG_START) )
					{
						ungetk(CTR_SHF_F3);
						keyStatus |= KEYFLG_START;
					}
				}

			}
			else
			{
				if( cntKeyStart )
					cntKeyStart--;
				else
					keyStatus &= ~KEYFLG_START;
			}

			selKeyTest++;
			break;

		case	3:
			if( !PinSEL_SW )
			{
				if( cntKeyEnter < 5 )
					cntKeyEnter++;
				else
				{
					if( !(keyStatus & KEYFLG_ENTER) )
					{
						ungetk(ENTER);
						keyStatus |= KEYFLG_ENTER;
					}
				}
			}
			else
			{
				if( cntKeyEnter )
					cntKeyEnter--;
				else
					keyStatus &= ~KEYFLG_ENTER;
			}

			selKeyTest++;
			break;

		default:
			if( cntEncSelector < precEncSelector )			// se è andato avanti
			{
				ungetk('-');
				ungetk(AR_UP);
			}
			else
			{
				if( cntEncSelector > precEncSelector )		// se è andato indietro
				{
					ungetk('+');
					ungetk(AR_DN);
				}
			}

			cntEncSelector = precEncSelector;

			selKeyTest = 0;
			break;
	}
}


//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.1 $
//@@  $Date: 2004/01/08 00:00:00 $
//@@  	- Aggiunta primo porting dalla versione per PC-DOS
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2004/04/04 00:00:00 $
//@@  	- Aggiunta funzione key_scan per decodifica tasti e selettore
//@@
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: KEY.CPP,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2002/12/16 00:00:00 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
