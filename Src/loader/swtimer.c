/**
@file		swtimer.c
@brief		Software timers routines
			Starting project SWTIMER.C (COMETA).
			Modified code syntax according to the new Medica standard

@author		Nicola/Enrico
@date		07/01/2013
@version	01.00.00
*/
#include <stdint.h>
#include "swtimer.h"

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

volatile uint32_t g_globalTick;

struct decTimerCent_t	g_timCent;						// Timers a decremento... t=1/100sec
struct decTimerSec_t	g_timSec;						// Timers a decremento... t=1sec


//--------------------------------------------------------//
// Definizione/istanze delle funzioni
//--------------------------------------------------------//


void swtimer_init(void)
{
register int i;
timer_centSec_t *pt;
timer_Sec_t 	*ps;

	g_globalTick = 0;

    g_timCent.timBase = 0;
    g_timSec.timBase = 0;

 #if EXCLUDE_SONAR_CUBE
	pt = &g_timCent.timBase;

	for(i=0; i<sizeof(struct decTimerCent_t); i+= sizeof(timer_centSec_t))
{
		*pt  = 0;		// reset timer
		pt++;			// next timer
	}

	ps = &g_timSec.timBase;

	for(i=0; i<sizeof(struct decTimerSec_t); i += sizeof(timer_Sec_t) )
	{
		//*ps = 0;		// reset timer
		ps++;			// next timer
	}
#endif
}



// Decrementa i contatori ad uso timers
// nota: da chiamare possibilmente
// ogni 10msec....
void swtimer_decCount(void)
{
register uint8_t i;
register timer_centSec_t *pt;
register timer_Sec_t 	  *ps;

static uint8_t cntCentSec;

#if EXCLUDE_SONAR_CUBE
	pt = &g_timCent.timBase;

	for(i=0; i<sizeof(struct decTimerCent_t); i+= sizeof(timer_centSec_t) )
	{
		if(*pt)							// se non è arrivato...
		{
			(*pt)--;						// lo decrementa
        }

        if(!pt)
		  pt++;
	}
#endif

	cntCentSec++;

	if( cntCentSec >= 100 )						// ogni 100 centesimi....è passato un secondo
	{
		cntCentSec = 0;							// reset...

        g_timSec.timBase = 0;
#if EXCLUDE_SONAR_CUBE

		ps = &g_timSec.timBase;

		for(i=0; i<sizeof(struct decTimerSec_t); i += sizeof(timer_Sec_t) )
		{
			if( *ps )							// se non è arrivato...
				(*ps)--;						// lo decrementa

			ps++;
		}
#endif
	}

}



// calcola il tempo trascorso tra start e stop
void measCalc(measMicroTime_t *__theMeas)
{
uint32_t report;

	if( __theMeas->usecStart >__theMeas->usecStop  )
	{
		__theMeas->measAct = __theMeas->usecStart - __theMeas->usecStop;

	}
	else
	{
		report = 0xFFFF - __theMeas->usecStop;
		report += __theMeas->usecStart;

		__theMeas->measAct = report;

	}

	if( __theMeas->measAct > __theMeas->measMax )
		__theMeas->measMax = __theMeas->measAct;

}


