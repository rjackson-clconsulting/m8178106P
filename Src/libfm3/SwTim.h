// uguale tra uC e uP, preso da ASHAI tale e quale

/**
@file		swtim.h
@brief		Classes to manage software timers.
@author
@date		18/01/2011
@version	01.0
*/

#ifndef _SWTIM_H_
#define _SWTIM_H_

#include "stdDataTypes.h"
#include "board_define.h"
#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #include "mb9bd10t.h"
#else
    #include "mb9bf50x.h"
#endif


//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
#define GET_USEC_HW_TIMER()           (FM3_DTIM->TIMER2VALUE>>1)

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//


// Elapsed time
struct milliSecTime_t{
    uint32_t elapsed;			// elapsed time in ms
    uint16_t usec;			// micro sec. elapsed within the ms
};

//typedef struct milliSecTime_t milliSecTime_t;

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

/** Software Timer base class.
    It is the time base for all the others timers. */
class Timer{
public:

	Timer();

	/** Increment the elapsed time
	    @param msec elapsed time increment in msec
	    @param usec micro sec. elapsed within the current msec.*/
	void clockTick(uint32_t __msec, uint16_t __usec) { m_sysTimer.elapsed += __msec, m_sysTimer.usec = __usec; }

	/** @return the elapsed time in sec. */
    	float getTime() { return( (float) m_sysTimer.elapsed / 1000.0 ); }

	/** @return the elapsed time in msec. */
    	uint32_t getMsec() { return( m_sysTimer.elapsed ); }

    /** @return the elapsed time in usec. */
    	uint16_t getUsec() { return( m_sysTimer.usec ); }

private:
	milliSecTime_t m_sysTimer;    // orologio interno (system clock)
};

/** Global time. Time Base for all the timers.*/
extern Timer g_globalTimer;

/** Incremental Timer */
class IncTimer
{
public:
	/** Class constructor */
    	IncTimer() { m_startTime = m_latchTime = g_globalTimer.getMsec(), m_running = m_latched = False; }

	/** Starts the timer. */
	void start() { m_running = True, m_latched = False; }

	/** Stops the timer */
	void stop() { m_running = False; }

	/** latches the msec elapsed */
    	void latch() { m_latchTime = g_globalTimer.getMsec(), m_latched = True; }

	/** resets the timer*/
    	void reset() { m_startTime = g_globalTimer.getMsec(), m_running = True, m_latched = False; }

	/** @return true if the timer is running, false otherwise. */
    	bool getStatus() { return( m_running ); }

    	float    getTime();
    	uint32_t getMsec();

private:
    	bool       m_running;
   	bool       m_latched;
    	uint32_t   m_startTime;
    	uint32_t   m_latchTime;
};

/** Decrement Timer */
class DecTimer{
public:
	/** Class constructor */
	DecTimer(void) { m_matchTime = g_globalTimer.getMsec(); }

	/** Preset the counting time and start the timer.
	    @param msec counting time in msec. */
	void preset(uint32_t __msec)   {
		m_startTime = g_globalTimer.getMsec();
		m_matchTime = m_startTime + __msec;
		m_running = true;
	};

	void Preset(uint32_t __msec)   {
		m_startTime = g_globalTimer.getMsec();
		m_matchTime = m_startTime + __msec;
		m_running = true;
	};

	bool match();
    bool Match() { return match(); }

	/** Stops the timer. */
	void stop() { m_running = false; };
	void Stop() { m_running = false; };

	/** Starts the timer. */
	void start(){m_running = true;};
	void Start(){m_running = true;};

	/** @return true if the timer is running, false otherwise. */
    	bool getStatus() { return( m_running ); };

private:
    	volatile uint32_t   m_startTime;
    	volatile uint32_t   m_matchTime;
    	volatile bool    m_running;
};

/** Decrement Timer */
class DecTimerUsec{
public:
	/** Class constructor */
	DecTimerUsec(void) { m_matchTime = GET_USEC_HW_TIMER(); }


	void Preset(uint32_t __usec)   {
		m_startTime = GET_USEC_HW_TIMER();
		m_matchTime = m_startTime - __usec;
		m_running = true;
	};

	bool match();
    bool Match() { return match(); }

	/** Stops the timer. */
	void stop() { m_running = false; };
	void Stop() { m_running = false; };

	/** Starts the timer. */
	void start(){m_running = true;};
	void Start(){m_running = true;};

	/** @return true if the timer is running, false otherwise. */
    	bool getStatus() { return( m_running ); };

private:
    	volatile uint32_t   m_startTime;
    	volatile uint32_t   m_matchTime;
    	volatile bool    m_running;
};

/** Measuring Timer */
class uSecMeasTimer
{
public:
	/** Class constructor */
    //uSecMeasTimer() { m_startTime = m_latchTime = GET_USEC_HW_TIMER(), m_running = m_latched = False; }
    uSecMeasTimer() { m_startTime = GET_USEC_HW_TIMER(), m_running =  False; }
	/** Class distructor */
    //~uSecMeasTimer() { m_startTime = m_latchTime = 0, m_running = m_latched = False; }
    ~uSecMeasTimer() { m_startTime =  0, m_running = False; }
	/** Starts the timer. */
	//void start() { m_running = True, m_latched = False; }
    void start() { m_running = True; }
	/** Stops the timer */
	void stop() { m_running = False; }

	/** latches the usec elapsed */
    	//void latch() { m_latchTime = GET_USEC_HW_TIMER(), m_latched = True; }

	/** resets the timer*/
    	//void reset() { m_startTime = GET_USEC_HW_TIMER(), m_running = True, m_latched = False; }
        void reset() { m_startTime = GET_USEC_HW_TIMER(), m_running = True; }
	/** @return true if the timer is running, false otherwise. */
    	bool getStatus() { return( m_running ); }
	/** Clean timer*/
        void clean() { m_startTime = 0; }

    	float    getTime();
    	uint32_t getUsec();

private:
    	bool       m_running;
        //bool       m_latched;
    	uint32_t   m_startTime;
    	//uint32_t   m_latchTime;
};



//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

#ifndef __SWTIM__
	extern Timer g_globalTimer;
#endif

#endif
