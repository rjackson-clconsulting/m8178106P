/**
@file		Cd_Air.cpp
@brief		Module to implement Air Drivers - Former "CDSC_DR1.C"
@author		Fernando Morani
@date		19/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#include "cd_air.h"


/*****************************************************
** GLOBAL AND STATIC VAR SECTION
*****************************************************/
AIR   air;

static int  Trg_PinAIR_IN_P;
static int   cnt_trg_air_hi;
static int   cnt_trg_air_low;

static int  Trg_PinTEST_ARIA_P;
static int   cnt_trg_test_hi;
static int   cnt_trg_test_low;

/*****************************************************
** EXTERN VAR SECTION
*****************************************************/
extern uint32_t 	g_time_msec;
extern ALA      ala;

/****************************************************************************
void DrvAIR_Ini(void)
** \brief Init driver AIR
** \return void
****************************************************************************/
void DrvAIR_Ini(void)
{
   air.ini=0;
   air.state=0;
   air.test=False;
   ala.air_test=False;
   ala.air=False;
   cnt_trg_air_hi=0;
   cnt_trg_air_low=0;
   Trg_PinAIR_IN_P = 0;
   Trg_PinTEST_ARIA_P = 0;
}
/****************************************************************************
void DrvAIR_Ges(void)
** \brief Handling driver AIR
** \return void
****************************************************************************/
void DrvAIR_Ges(void)
{


    if( PinAIR_IN_P == 0 )
    {
        cnt_trg_air_hi=0;

        cnt_trg_air_low++;

        if( cnt_trg_air_low > 1 )
        {
            cnt_trg_air_low = 2;
            Trg_PinAIR_IN_P = 0;
        }
    }
    else
    {
        cnt_trg_air_low=0;

        cnt_trg_air_hi++;

        if( cnt_trg_air_hi > 1 )
        {
            cnt_trg_air_hi = 2;
            Trg_PinAIR_IN_P = 1;
        }

    }


    if( PinTEST_ARIA_P == 0 )
    {
        cnt_trg_test_hi=0;

        cnt_trg_test_low++;

        if( cnt_trg_test_low > 1 )
        {
            cnt_trg_test_low = 2;
            Trg_PinTEST_ARIA_P = 0;
        }
    }
    else
    {
        cnt_trg_test_low=0;

        cnt_trg_test_hi++;

        if( cnt_trg_test_hi > 1 )
        {
            cnt_trg_test_hi = 2;
            Trg_PinTEST_ARIA_P = 1;
        }

    }

	if( air.ini == 0)
	{
		air.ini = 1;
		air.time=g_time_msec;
		return;

	}


   switch( air.state )
      {
       case 0 :
            if( (Trg_PinTEST_ARIA_P) == 0 )
               {
					air.test=False;
    	            air.state=1;
        	        air.time=g_time_msec;
					break;
               }
            if (Trg_PinAIR_IN_P == 0)
               {
					ala.air=1;
               }

            if ((g_time_msec-air.time) > AIR_TIME_LARGE_TSHLD)
               {
					ala.air_test=1;
     				air.ini=0;
               }
            break;

       case 1 :
            if ((Trg_PinAIR_IN_P ) == 0)
                air.test = True;
            if( (Trg_PinTEST_ARIA_P) == 1 )
               {
				    if( air.test == False )
	                {
						ala.air_test=1;
	                }
	                air.state=2;
    	            air.time=g_time_msec;
               }
            if ((g_time_msec-air.time) > AIR_TIME_NARROW_TSHLD)
               {
					ala.air_test=1;
     				air.ini=0;
               }
            break;

       case 2 :
                air.state=3;
            break;

       case 3 :
                air.state=4;
            break;

       case 4 :
                air.state=0;
            break;

       default :
           Fatal_Error(FATAL_ERROR_AIR);
           break;
      }

}






