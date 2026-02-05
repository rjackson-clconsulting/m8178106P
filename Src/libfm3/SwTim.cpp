/**
@file		swtim.cpp
@brief		Classes to manage software timers.
@author
@date		18/01/2011
@version	01.0
*/

#ifdef __SWTIM__
#error __SWTIM__ already defined !!
#else
#define __SWTIM__
#endif

#include "swtim.h"

#ifdef __SDATA__							// se utilizza il segmento sdata ....
Timer g_globalTimer		__attribute__ ((section (".sdata")));
#else
Timer g_globalTimer;
#endif

/** Class constructor. */
Timer::Timer()
{
    m_sysTimer.elapsed = 0;
    m_sysTimer.usec = 0;
}

/** @return the elapsed time since the timer has been latched in sec.
    If the timer hasn't been latched return the time elapsed since the timer has been started. */
float IncTimer::getTime()
{
uint32_t ltime;
float dtime;

	if( m_latched )
		ltime = m_latchTime;
	else
		if( m_running )
			ltime = g_globalTimer.getMsec();
		else
			ltime = m_startTime;

	if( ltime >= m_startTime )     			// se il tempo e` andato avanti
		ltime -= m_startTime;
	else
        ltime += 0x00000000 - m_startTime;    		// considera l'overflow

	dtime = ltime;
    	dtime /= 1000.0;

	return( dtime );
}

/** @return the elapsed time since the timer has been latched in msec.
    If the timer hasn't been latched return the time elapsed since the timer has been started. */
uint32_t IncTimer::getMsec(void)
{
uint32_t ltime;

	if( m_latched )
		ltime = m_latchTime;
	else
		if( m_running )
			ltime = g_globalTimer.getMsec();
		else
			ltime = m_startTime;

	if( ltime >= m_startTime )     			// se il tempo e` andato avanti
		ltime -= m_startTime;
	else
        ltime += 0x00000000 - m_startTime;    		// considera l'overflow

	return( ltime );
}

/** @return true if the presetted counting time is elapsed and the timer is running, false otherwise. */
bool DecTimer::match()
{
uint32_t ltime = g_globalTimer.getMsec();

    if( !m_running )
        return( false );

    if( m_matchTime >= m_startTime )        		// se non c'e` overflow di sorta
    {
		if(ltime < m_startTime || ltime >= m_matchTime)     // basta il confronto diretto
			return( true );
		else
			return( false );
	}
	else
	{
		if(ltime < m_startTime && ltime >= m_matchTime)     // doppio confronto
            return( true );
        else
            return( false );
    }

}


/** @return true if the presetted counting time is elapsed and the timer is running, false otherwise. */
bool DecTimerUsec::match()
{
uint32_t ltime = GET_USEC_HW_TIMER();

    if( !m_running )
        return( false );

    if( m_matchTime <= m_startTime )
    {
		if(ltime < m_startTime && ltime <= m_matchTime)     // basta il confronto diretto
			return( true );
		else
			return( false );
	}
	else
	{
		if(ltime > m_startTime && ltime <= m_matchTime)     // doppio confronto
            return( true );
        else
            return( false );
    }

}

float uSecMeasTimer::getTime()
{
uint32_t ltime;
float dtime;

//	if( m_latched )
	//	ltime = m_latchTime;
	//else
		if( m_running )
			ltime = GET_USEC_HW_TIMER();
		else
			ltime = m_startTime;

	if( ltime >= m_startTime )     			// se il tempo e` andato avanti
		ltime -= m_startTime;
	else
        ltime += 0x00000000 - m_startTime;    		// considera l'overflow

	dtime = ltime;
    	dtime /= 1000.0;

	return( dtime );
}

/** @return the elapsed time since the timer has been latched in msec.
    If the timer hasn't been latched return the time elapsed since the timer has been started. */
uint32_t uSecMeasTimer::getUsec()
{
uint32_t ltime;

//	if( m_latched )
	//	ltime = m_latchTime;
	//else
		if( m_running )
			ltime = GET_USEC_HW_TIMER();
		else
			ltime = m_startTime;

	if( ltime >= m_startTime )     			// se il tempo e` andato avanti
		ltime += 0xFFFFFFFF - m_startTime;    		// considera l'overflow
	else {
        ltime = 0xFFFFFFFF - ltime;
        ltime = ltime - (0xFFFFFFFF - m_startTime);
	}

	return( ltime );
}
