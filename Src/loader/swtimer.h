/**
@file		swtimer.h
@brief		Software timers routines
			Starting project SWTIMER.C (COMETA).
			Modified code syntax according to the new Medica standard

@author		Nicola/Enrico
@date		07/01/2012
@version	01.00.00
*/

#ifndef __SWTIMER_H__
#define __SWTIMER_H__

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define getGlobalTick()	(g_globalTick)

#define preset(__theTimer, __msecTime)    ((__theTimer.espTime) = (getGlobalTick() + (__msecTime)))
#define start(theTimer) ((theTimer.isEnable = 1))

#define match( __theTimer)      ( (((getGlobalTick() >= (__theTimer.espTime))) && (__theTimer.isEnable)) )
#define stop(__theTimer) (__theTimer.isEnable = 0)

#define clear(__theTimer)    	((__theTimer.clearTime) = (getGlobalTick()))
#define elapsed( __theTimer)    (getGlobalTick() - (__theTimer.clearTime))

#define measStart(__theMeas)    (__theMeas.usecStart = TMR6)
#define measStop(__theMeas)    (__theMeas.usecStop = TMR6)

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

typedef uint32_t 	timer_centSec_t;						// definizione del timer al centesimo di secondo
typedef uint32_t	timer_Sec_t;							// definizione del timer al secondo


struct decTimerCent_t{									// blocco di timers a decremento con risoluzione = 1/100s
	timer_centSec_t	timBase;
	//... aggiungere in coda gli altri timers

	timer_centSec_t	spare;
};
#ifdef __cplusplus
#else
typedef struct decTimerCent_t decTimerCent_t;		// timer a decremento con base tempi == 1msec
#endif

struct decTimerSec_t{										// blocco di timers a decremento con risoluzione 1s
	timer_Sec_t	timBase;
	//... aggiungere in coda gli altri timers

	timer_Sec_t	spare;
};
#ifdef __cplusplus
#else
typedef struct decTimerSec_t decTimerSec_t;		// timer a decremento con base tempi == 1msec
#endif


struct decTimer_t{							// siccome il timer è a decremento viene memorizzato
	uint32_t espTime;						// il count di sistema in cui il timer stesso espira
	uint8_t isEnable;
};
#ifdef __cplusplus
#else
typedef struct decTimer_t decTimer_t;		// timer a decremento con base tempi == 1msec
#endif

struct incTimer_t{							// siccome il timer è a incremento viene memorizzato
	uint32_t clearTime;						// il count di sistema in cui il timer stesso viene azzerato
};
#ifdef __cplusplus
#else
typedef struct incTimer_t incTimer_t;		// timer a decremento con base tempi == 1msec
#endif

// strutturina per misurare tempi in microsecondi
struct measMicroTime_t{
	uint16_t usecStart;		    // timing inizio misura
	uint16_t usecStop;			// timing fine misura
	uint32_t measAct;			// misura attuale
	uint32_t measMax;			// misura picco max
};
#ifdef __cplusplus
#else
typedef struct measMicroTime_t measMicroTime_t;
#endif


//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

void swtimer_init(void)						__attribute__ (( section(".ramcode") ));
void swtimer_decCount(void)					__attribute__ (( section(".ramcode") ));
void measCalc(measMicroTime_t *__theMeas)	__attribute__ (( section(".ramcode") ));

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern struct decTimerCent_t		timCent;					// Timers a decremento... t=1/100sec
extern struct decTimerSec_t			timSec;						// Timers a decremento... t=1sec
extern volatile uint32_t 			g_globalTick;

#endif


