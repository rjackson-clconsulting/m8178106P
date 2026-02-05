/**
@file		globalApplication.c
@brief		Module to implement global routine to be used from application
@author		Fernando Morani
@date		09/01/2018
@version	01.00.00
@source     VetSmart Protective Application
*/

#include "globalApplication.h"
#include "app_info.h"

/*****************************************************
** GLOBAL VAR SECTION
*****************************************************/
//For Debug Purpose
const FM3_GPIO_TypeDef *g_pFM3_GPIO = FM3_GPIO;
const FM3_DMAC_TypeDef *g_pFM3_DMAC = (FM3_DMAC_TypeDef*)0x40060000;//FM3_DMAC;
const FM3_ADC_TypeDef *g_pFM3_ADC0 = FM3_ADC0;
const FM3_EXBUS_TypeDef *g_pFM3_EXBUS= FM3_EXBUS;
const FM3_CAN_TypeDef *g_pFM3_CAN0= FM3_CAN0;
const FM3_MFS03_UART_TypeDef *g_pFM3_UART3 = FM3_MFS3_UART;

DecTimer timLoad;	             // timer override vrain pressure
DecTimer timForceCsi;
DecTimer timOveVen;              // timer override pressione venosa
DecTimer timTestAdc;
DecTimer timTestVcc;
DecTimer timVel_Blo_Max;
DecTimer timVel_Blo_Min;
DecTimer timVel_Uf_Max;
DecTimer timVel_Uf_Min;
DecTimer timGesCal;
DecTimer timStartComm;

UBYTE oveVen;

dat Dat;

char sAppName[]     = APPLICATION_NAME;
char sAppRevision[] = APPLICATION_REVISION;
char sAppDate[]     = APPLICATION_DATE;

//extern volatile uint32_t 	g_time_msec;          //CPU Time expressed in ms
UWORD 	g_time_sec;           //CPU Time expressed in s

//Add here global var to be used in different cpp files
uint32_t g_PSMainCounter;   //Main Counter for PS motor
uint32_t g_UFMainCounter;   //Main Counter for UF motor
uint32_t g_UFFreqCounter;
uint32_t g_PSFreqCounter;
//uint32_t g_freqPS;
//uint32_t g_freqUF;

static CalPoint calRun1[2];
static CalPoint calRun2[2];

static long parz_calop;
static long totale_calop;
static long teoric_calop;
static long time_tre;          /* Time for Treatment Time Computation            */
static uint32_t g_alaGesMonitor;
uint32_t g_StartController;       //As soon as the controller send the first message this signal goes ON

byte rall_calop=0;
byte rdurata_tratt=0;


// TEST_FAILURE

uint8_t fail_dat=0x00;
extern bool eep_writeMem(void *__data_buf, int __count, int __start_addr);

// TEST_FAILURE



#ifdef LOADER_ENGINE
/**
 * Loader protocol
 */
comProto_t g_protoLoader;
/**
 * Loader protocol's data buffer
 */
uint8_t g_aLoaderProtoDataBuf[LOADER_PROTODATA_BUF_LEN];

#endif //LOADER_ENGINE

/*****************************************************
** EXTERN VAR SECTION
*****************************************************/
extern volatile uint32_t     g_globalTick;
extern void AlaCLA_Ges(byte cla_set);
extern CsiXfrBlockRx chkBlock;
extern void DrvAIR_Ini(void);
extern void AlaPRE_Ini(void);
extern ALA ala;
extern ADChan adcSet[MAX_AD];
extern word keypres(void);
extern void Ala_Ges(void);
extern void adc_scale(void);
extern uint8_t cal_init();
extern bool eep_writeParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk);
//extern word PsFlow;

extern CalPoint calRunCH0[NUM_POINTS_CH0];
extern CalPoint calRunCH1[NUM_POINTS_CH1];
extern CalPoint calRunCH2[NUM_POINTS_CH2];
extern CalPoint calRunCH3[NUM_POINTS_CH3];
extern CalPoint calRunCH4[NUM_POINTS_CH4];
extern CalPoint calRunCH5[NUM_POINTS_CH5];
extern CalPoint calRunCH6[NUM_POINTS_CH6];
extern CalPoint calRunCH7[NUM_POINTS_CH7];
extern CalPoint calRunCH8[NUM_POINTS_CH8];
extern CalPoint calRunCH9[NUM_POINTS_CH9];
extern CalPoint calRunCH10[NUM_POINTS_CH10];
extern CalPoint calRunCH11[NUM_POINTS_CH11];
extern CalPoint calRunCH12[NUM_POINTS_CH12];
extern CalPoint calRunCH13[NUM_POINTS_CH13];

/**
**
**/
void global_var_init()
{
	g_pFM3_GPIO  = FM3_GPIO;
	g_pFM3_DMAC  = FM3_DMAC;
	g_pFM3_ADC0  = FM3_ADC0;
	g_pFM3_EXBUS = FM3_EXBUS;
	g_pFM3_CAN0  = FM3_CAN0;
	g_pFM3_UART3 = FM3_MFS3_UART;
	g_StartController = OFF;

}
/**
 * hardware application initialization
 */
void hwAppInit(void)
{
uint8_t u8RetVal;

	uart_init();  //It is defined in UART.cpp
	eep_init(0); //Init
	global_var_init();

    SetupDati();
	cal_init();

    u8RetVal = adc_init_channel();
    if( u8RetVal )
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
        ala.cal_num_pro=u8RetVal;
    }
    encoderInit();

   	adcPressArt.conv.offset = Dat.Offset_Art;
	adcPressFil.conv.offset = Dat.Offset_Fil;
	adcPressVen.conv.offset = Dat.Offset_Ven;
	adcGauge2.conv.offset = Dat.Offset_P2;



}


/*********************************************
 * uint8_t mainAppInit()
 * \brief Init of the sw application resources
 * \return ERROR_NONE  or ERROR_INIT if fails
 *********************************************/
uint8_t mainAppInit()
{
uint8_t u8RetVal = ERROR_NONE;

    DrvAIR_Ini();
    AlaPRE_Ini();

    //Ala_Ini();

    timTestAdc.Preset(5000);
    timTestVcc.Preset(5000);
    timVel_Blo_Max.Preset(10000);
    timVel_Blo_Min.Preset(10000);
    timVel_Uf_Max.Preset(10000);
    timVel_Uf_Min.Preset(10000);
	timGesCal.Preset(1000);

    u8RetVal = IniXfr_P();    //Init the SPI communication device
    if  (u8RetVal == ERROR_NONE){
        if (u8RetVal == ERROR_NONE){
            //If no error occurs in the init enable all the irq's
            __enable_irq();
            } //else not needed
    }//else not needed
    PinLedLife_1 = 0;   //Start Value for LedLife1
    PinLedLife_2 = 1;   //Start Value for LedLife2



    return u8RetVal;
}

/*********************************************
 * uint8_t mainAppRun()
 * \brief Init of the sw application resources
 * \return ERROR_NONE  or ERROR_INIT if fails
 *********************************************/
uint8_t mainAppRun()
{
uint8_t u8RetVal = ERROR_NONE;
bool bContinueFlag = false;

    timStartComm.Preset(ALARM_DELAY_TIMER);
    timStartComm.Start();

    while (bContinueFlag != true){
        adc_scale();

        u8RetVal = GesXfr_P();              //Manage the SPI communication device

        if  (u8RetVal != ERROR_NONE)
        {
            if( u8RetVal > GESXFR_MAX_PKT_LOSS )
                if (timStartComm.Match() || (g_StartController == ON))
                {
                    Ala_Pre(ALP_T0T_CON_RUN,1);
                    PinBUZZER = 0;
                }
			//protoEntry();					// gestione decode seriale
        }
        else
        {
			Ges_app();                          // Gestione applicazione
			if (g_StartController == ON){
                Ges_ala_adc();
            }//else{}
		}

		if(g_alaGesMonitor > GES_ALA_WAIT_CYCLE){
            Ges_ala_pmp();					    // Gestione allarme velocita' pompe
            Ges_rele();   						// Gestione rele
            Ges_key();                          // Gestione tasti
            Ala_Ges();  // Gestione allarmi
            g_alaGesMonitor = 0;
		}else{
            g_alaGesMonitor++;
		}

        blinkLedLife();

    #ifdef LOADER_ENGINE
        loader_proto_query(&g_protoLoader);
    #endif //LOADER_ENGINE

    }

    return u8RetVal;
}



/********************************************************************************
 ** uint8_t blinkLife
 ** \brief  Alternative blink of the led life
 **
 ** \retval ERROR_NONE
 ******************************************************************************/
uint8_t blinkLedLife()
{
    if ((g_globalTick % BLINK_LED_SWAP_TIME) == 0){
        if (PinLedLife_1 == 0){
            PinLedLife_1 = 1;
            }else{
            PinLedLife_1 = 0;
            }
        if (PinLedLife_2 == 0){
            PinLedLife_2 = 1;
            }else{
            PinLedLife_2 = 0;
            }
    }
    return ERROR_NONE;
}


/********************************************************************************
 ** void Ges_rele
 ** \brief  Switch ON rele
 **
 ******************************************************************************/

void Ges_rele(void)   // Gestione relè
{

		if((chkBlock.Stato == STATE_PRI) && ((chkBlock.Phase == PHASE_TST_REL_STA) || (chkBlock.Phase == PHASE_TST_REL_STO)))		// Test rele
		{
			PinRelePS		=1;
			PinReleUF		=1;
			//PinRelePinza	=1;
			return;
		}


        if (ala.ala_sta[ALP_T0T_CON_RUN] || ala.ala_sta[ALC_PMP_BLO_SPE] || ala.ala_sta[ALC_PMP_DRA_SPE] )   /* se errore di comunicazione con controllo */
		{
			PinRelePS		=1;
			PinReleUF		=1;
		}
        else
		{
			PinRelePS		=0;
			PinReleUF		=0;
		}

/*
		if( ala.loc_blo )
			PinRelePS = 1;
		else
			PinRelePS = 0;

		if( ala.loc_flu )
			PinReleUF = 1;
		else
			PinReleUF = 0;
*/
}


void Ges_app(void)   // Gestione applicazione
{

		if( chkBlock.disable_pa == 0 )
		{
			ala.ala_ena[ALC_PRE_ART_MIN]=Enable;
			ala.ala_ena[ALC_PRE_ART_MAX]=Enable;
		}
		else
		{
			ala.ala_ena[ALC_PRE_ART_MIN]=Disable;
			ala.ala_ena[ALC_PRE_ART_MAX]=Disable;
		}

		switch( chkBlock.Stato )
		{
			case	STATE_NUL:					/* Logo                           */
					//protoEntry();				// gestione decode seriale
				break;

			case	STATE_IDL:					/* Idle                           */
					//protoEntry();				// gestione decode seriale
			case	STATE_MON_LIN:				/* Montaggio linee                */
					ala.ala_ena[ALC_PRE_ART_MAX]=Disable;
					ala.ala_ena[ALC_PRE_ART_MIN]=Disable;
					ala.ala_ena[ALC_PRE_VEN_MAX]=Disable;
					ala.ala_ena[ALC_PRE_VEN_MIN]=Disable;
				break;

			case	STATE_PRI:					/* Priming                        */
					ala.ala_ena[ALC_PRE_ART_MAX]=Enable;
					ala.ala_ena[ALC_PRE_ART_MIN]=Enable;
					ala.ala_ena[ALC_PRE_VEN_MAX]=Enable;
					ala.ala_ena[ALC_PRE_VEN_MIN]=Enable;
					Pre_Lim(PRE_ART,  100, -200);
					Pre_Lim(PRE_VEN,  350, -100);
					AlaPRE_Ges();
				break;

			case	STATE_TEST:				/* Attacco                        */
					ala.air=0;					// resetta eventuali allarmi aria avuti in priming
				break;

			case	STATE_TRE:					/* Trattamento                    */

			case	STATE_STACCO:				/* Stacco                         */

					if( chkBlock.disable_air == 0 )
					{
						if( ala.air )
							Ala_Pre(ALC_AIR_VEN_LIN,1);

						if( ala.air_test )
							Ala_Pre(ALC_T0T_AIR_TST,1);
					}

					Pre_Lim(PRE_ART,  100, chkBlock.P_Art_Min);

                    if( chkBlock.disable_pv == 0 )
                    {
                        Pre_Lim(PRE_VEN,  350,  -40);
                    }

					AlaPRE_Ges();

				break;

			default :
				break;

		}


}

void Ges_ala_adc(void)   // Gestione allarme ADC
{
UBYTE ub1 = 0;
UBYTE ub2 = 0;

	if ( chkBlock.ind_dat )
		return;

	ub1=0;
/*
	if( chkBlock.disable_pa == 0 )
	{
		if (abs(adcPressArt.conv.value-chkBlock.dat1) > 10)
			ub1=ALC_T0T_DIF_ART;
	}
*/
	if( chkBlock.disable_pf == 0 )
	{
		if (abs(adcPressFil.conv.value-chkBlock.dat2) > 10)
			ub1=ALC_T0T_DIF_FIL;
	}

	if( chkBlock.disable_pv == 0 )
	{
		if (abs(adcPressVen.conv.value-chkBlock.dat3) > 10)
			ub1=ALC_T0T_DIF_VEN;
	}

/*
	if (adcGauge1.conv.value > 4000)
		ub2 = ((adcGauge1.conv.value - 4000)/60)+50;
	else
		ub2 = 50;

	if (abs(adcGauge1.conv.value-chkBlock.dat4) > ub2)  		    // Confronta la prima cella fra due micro
		ub1=ALC_T0T_DIF_DRA;

	if (abs(adcGauge2.conv.value-chkBlock.dat5) > ub2)  		    // Confronta la seconda cella fra due micro
		ub1=ALC_T0T_DIF_DRA;

	if (abs(adcGauge1.conv.value-adcGauge2.conv.value) > ub2)		// Confronta le celle fra di loro
		ub1=ALC_T0T_DIF_DRA;
*/

	if ( ub1 == 0 )
		 timTestAdc.Preset(30000);

	if( timTestAdc.Match() )
	{
		Ala_Pre(ub1,1);
	}


	ub1=0;
	if (abs(vTest1.conv.value - 1500) > 150)   // 1,500 Volt
		ub1=1;

	if (abs(vTest2.conv.value - 2500) > 250)   // 2,500 Volt
		ub1=1;

	if (abs(monAvrefp.conv.value - 1500) > 150)   // 1,500 Volt
		ub1=1;

	if (abs(monAvrefc.conv.value - 1500) > 150)   // 1,500 Volt
		ub1=1;

	if (abs(mon5V.conv.value - 2500) > 250)   // 2,500 Volt
		ub1=1;

	if (abs(mon33Vp.conv.value - 1650) > 165)   // 1,650 Volt
		ub1=1;

	if (abs(mon33V.conv.value - 1650) > 165)   // 1,650 Volt
		ub1=1;

	if ( ub1 == 0 )
		 timTestVcc.Preset(5000);

	if( timTestVcc.Match() )
	{
		Ala_Pre(ALP_TENSIONI,1);
	}


}



void Ges_key(void)   // Gestione tasti
{
int c;

    key_scan(); // sarebbe da fare ogni 5 ms , come anche adc_scan

    if( keypres() )     // c'è un tasto ?
    {
        c = getk();

        switch( c )     // TASTI GENERALI
        {

            case    CTR_SHF_F1:
                break;

            case    CTR_SHF_F2:
					Ala_Kal();
                break;

            case    CTR_SHF_F3:
                break;

            default:
                break;
        }

    }

}


void Ges_ala_pmp(void)   // Gestione allarme velocita' pompe
{
word w,wmax,wmin,wperc;

		// POMPA SANGUE
        if( chkBlock.hiflow == True )
            w=(word)(chkBlock.PsFlow * 1.0);
		else
            w=(word)(chkBlock.PsFlow * 1.7);

		if( w > 10 )
			wperc=w/10;                         // 10% di errore
        else
            wperc=w;

		wmax=w+wperc;
		wmin=w-wperc;

		if( g_PSFreqCounter > wmax )
			timVel_Blo_Min.Preset(10000);
		else
			timVel_Blo_Max.Preset(10000);

		if( g_PSFreqCounter < wmin )
			timVel_Blo_Max.Preset(10000);
		else
			timVel_Blo_Min.Preset(10000);

		if( timVel_Blo_Min.Match() || timVel_Blo_Max.Match() )
		{
			Ala_Pre(ALC_PMP_BLO_SPE,1);
		}



		// POMPA ULTRAFILTRAZIONE

		w=(word)(chkBlock.UfFlow * 4.25);
		wperc=w/10;                         // 10% di errore
		wmax=w+wperc;
		wmin=w-wperc;

		if( g_UFFreqCounter > wmax )
			timVel_Uf_Min.Preset(15000);
		else
			timVel_Uf_Max.Preset(15000);

		if( g_UFFreqCounter < wmin )
			timVel_Uf_Max.Preset(15000);
		else
			timVel_Uf_Min.Preset(15000);

		if( timVel_Uf_Min.Match() || timVel_Uf_Max.Match() )
		{
			Ala_Pre(ALC_PMP_DRA_SPE,1);
		}

}


/*****************************************************
uint8_t Ges_cmd(void)
*****************************************************/

uint8_t Ges_cmd(void)   // Gestione comandi
{

bool calOk;

			switch( chkBlock.cmd )
			{
				case	CAL_ART_ZERO:
					calRun1[0].PtValue=adcPressArt.filter.value;
					calRun1[0].UmValue=0.0;
					//calRun2[0].PtValue=adcPressArt2.filter.value;
					//calRun2[0].UmValue=0.0;
					break;

				case	CAL_ART_GAIN:
					calRun1[1].PtValue=adcPressArt.filter.value;
					calRun1[1].UmValue= -250.0 ;
					//calRun2[1].PtValue=adcPressArt2.filter.value;
					//calRun2[1].UmValue= -250.0 ;
					calOk = WriteCalPressione(calRun1,calRun2,1);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=124;
	   				}

					break;

				case	CAL_UF_ZERO:
					calRun1[0].PtValue=adcPressArt2.filter.value;
					calRun1[0].UmValue=0.0;
					break;

				case	CAL_UF_GAIN:
					calRun1[1].PtValue=adcPressArt2.filter.value;
					calRun1[1].UmValue= -250.0 ;
					calOk = WriteCalPressione(calRun1,calRun2,4);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=124;
	   				}
					break;

				case	CAL_FIL_ZERO:
					calRun1[0].PtValue=adcPressFil.filter.value;
					calRun1[0].UmValue=0.0;
					break;

				case	CAL_FIL_GAIN:
					calRun1[1].PtValue=adcPressFil.filter.value;
					calRun1[1].UmValue= 250.0 ;
					calOk = WriteCalPressione(calRun1,calRun2,2);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=125;
	   				}
					break;

				case	CAL_VEN_ZERO:
					calRun1[0].PtValue=adcPressVen.filter.value;
					calRun1[0].UmValue=0.0;
					break;

				case	CAL_VEN_GAIN:
					calRun1[1].PtValue=adcPressVen.filter.value;
					calRun1[1].UmValue= 250.0 ;
					calOk = WriteCalPressione(calRun1,calRun2,3);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=126;
	   				}
					break;

				case	CAL_GAUGE_ZERO:
					calRun1[0].PtValue=adcGauge1.filter.value;
					calRun1[0].UmValue=0.0;

					calRun2[0].PtValue=adcGauge2.filter.value;
					calRun2[0].UmValue=0.0;

					calOk = WriteCalBilancia(calRun1,1,calRun2,1);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=127;
	   				}
					break;

				case	CAL_GAUGE_ZERO_GAIN:
					calRun1[0].PtValue=adcGauge1.filter.value;
					calRun1[0].UmValue=0.0;

					calRun2[0].PtValue=adcGauge2.filter.value;
					calRun2[0].UmValue=0.0;
					break;

				case	CAL_GAUGE_GAIN:
					calRun1[1].PtValue=adcGauge1.filter.value;
					calRun1[1].UmValue=2000.0;

					calRun2[1].PtValue=adcGauge2.filter.value;
					calRun2[1].UmValue=2000.0;

					calOk = WriteCalBilancia(calRun1,NUM_POINTS_CH4,calRun2,NUM_POINTS_CH5);
					if(calOk != True)
				    {
	        			Ala_Pre(ALP_T1S_ERR_CAL,1);
						ala.cal_num_pro=128;
	   				}
					break;

				case	START_TRATT:
						Dat.Gauge1_ini=adcGauge1.conv.value;
						totale_calop=0;
						parz_calop=0;
						Dat.cumu_calop=0;
						Dat.durata_tratt=0;
						WriteDati();
					break;

				case	TARA_TRATT:
						Dat.cumu_calop=totale_calop;
						parz_calop=0;
						totale_calop=0;
						Dat.Gauge1_ini=adcGauge1.conv.value;
						Dat.durata_tratt=0;
						WriteDati();
					break;

				case	RESET_CSI_SLAVE:
						//reset_csi=true;
					break;

				case	RESET_ALA_SLAVE:
						Ala_Res();
					break;

				case	OFFSET_PRESSIONI:
						Offset_Pressioni();
						WriteDati();
					break;

				case	RESET_OFFSET_PRESSIONI:
						Reset_Offset_Pressioni();
						WriteDati();
					break;

// TEST_FAILURE

				case	20: //PROTECTIVE FATAL ERROR
                        ala.sta=10;
					break;

				case	21: //PROT. CALIBRATION ERROR
                        eep_writeMem(&fail_dat, sizeof(fail_dat), 0x01C0);
                        fail_dat=55;
                        eep_writeMem(&fail_dat, sizeof(fail_dat), 0x7BC0);
					break;

// TEST_FAILURE


				default:
					break;
			}

    writeEIGHT();       //Debug on 7Seg Display
    return ERROR_NONE;


}


bool WriteCalPressione(CalPoint *CalRun1,CalPoint *CalRun2,UBYTE qpres)
{
bool chkOk;
bool writeOk=False;
long l;

		l=CalRun1[0].PtValue;
		if((l < 500) || (l > 800))
		{
			return(False);
		}

		l=CalRun1[1].PtValue-CalRun1[0].PtValue;
		if((l < 800) || (l > 1200))
		{
			return(False);
		}


		if(qpres == 1) // arteriosa
		{
	        memcpy(calRunCH2, CalRun1, (sizeof(CalPoint)*NUM_POINTS_CH2));
	        writeOk = eep_writeParam(calRunCH2, sizeof(calRunCH2), OFS_POINTS_CH2, &chkOk);
		}

		if(qpres == 4) // plasma
		{
	        memcpy(calRunCH5, CalRun1, (sizeof(CalPoint)*NUM_POINTS_CH5));
	        writeOk = eep_writeParam(calRunCH5, sizeof(calRunCH5), OFS_POINTS_CH5, &chkOk);
		}

		if(qpres == 2)  // prefiltro
		{
	        memcpy(calRunCH3, CalRun1, (sizeof(CalPoint)*NUM_POINTS_CH3));
	        writeOk = eep_writeParam(calRunCH3, sizeof(calRunCH3), OFS_POINTS_CH3, &chkOk);
		}

		if(qpres == 3)  // venosa
		{
	        memcpy(calRunCH4, CalRun1, (sizeof(CalPoint)*NUM_POINTS_CH4));
	        writeOk = eep_writeParam(calRunCH4, sizeof(calRunCH4), OFS_POINTS_CH4, &chkOk);
		}

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=122;
	    }

		return(writeOk);
}




bool WriteCalBilancia(CalPoint *CalRun1,UBYTE num_point1,CalPoint *CalRun2,UBYTE num_point2)
{
bool chkOk;
bool writeOk1,writeOk2;
long l;

		if(num_point1 == 1)
		{
			CalRun1[1].PtValue=calRunCH0[1].PtValue-calRunCH0[0].PtValue + CalRun1[0].PtValue;
			CalRun1[1].UmValue=2000.0;
		}

		l=CalRun1[0].PtValue;
		if((CalRun1[0].PtValue < 5) || (CalRun1[0].PtValue > 500))
		{
			return(False);
		}

		l=CalRun1[1].PtValue-CalRun1[0].PtValue;
		if((l < 800) || (l > 1200))
		{
			return(False);
		}

        memcpy(calRunCH0, CalRun1, (sizeof(CalPoint)*NUM_POINTS_CH0));

        writeOk1 = eep_writeParam(calRunCH0, sizeof(calRunCH0), OFS_POINTS_CH0, &chkOk);


		if(num_point2 == 1)
		{
			CalRun2[1].PtValue=calRunCH1[1].PtValue-calRunCH1[0].PtValue + CalRun2[0].PtValue;
			CalRun2[1].UmValue=2000.0;
		}

		l=CalRun2[0].PtValue;
		if((CalRun2[0].PtValue < 5) || (CalRun2[0].PtValue > 500))
		{
			return(False);
		}

		l=CalRun2[1].PtValue-CalRun2[0].PtValue;
		if((l < 800) || (l > 1200))
		{
			return(False);
		}

        memcpy(calRunCH1, CalRun2, (sizeof(CalPoint)*NUM_POINTS_CH1));

        writeOk2 = eep_writeParam(calRunCH1, sizeof(calRunCH1), OFS_POINTS_CH1, &chkOk);

        if( (writeOk1 || writeOk2) == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=123;
	    }

		return(writeOk1 || writeOk2);

}


void WriteDati(void)
{
bool chkOk;
bool writeOk;

        writeOk = eep_writeParam(&Dat, sizeof(Dat), OFS_POINTS_DAT, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
   			Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_122;
	    }
}


// inizializzazione dati programma
void SetupDati(void)
{
bool chkOk;
bool readOk;
bool writeOk;


static const dat calDefaultDat[1] = {
							0,   //    Gauge1_ini
							0,   //    Gauge2_ini
							0,	 //    Durata_Tratt
							0,	 //    cumu_calop
							0,	 //    Offset_Art
							0,	 //    Offset_Fil
							0,	 //    Offset_Ven
  							};


    readOk = eep_readParam(&Dat, sizeof(Dat), OFS_POINTS_DAT, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
		Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_153;
                                        // parte dai valori di default
        memcpy(&Dat, calDefaultDat, sizeof(Dat));
                                        // li aggiorna in fram
        writeOk = eep_writeParam(&Dat, sizeof(Dat), OFS_POINTS_DAT, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
   			Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_154;
	    }

    }
}



void Treat_Time_Ges(void)
{
long l;
   /*
      Treatment Time
   */
   if((chkBlock.Wct <= totale_calop) && (chkBlock.Wct))  // Se fine trattamento con trattamento programmato
		return;

   if (g_time_sec != time_tre)
      {
        time_tre=g_time_sec;
        Dat.durata_tratt++;
	    rdurata_tratt++;
	    if(rdurata_tratt > 10)									// ogni 10"
        {
			l=abs(Dat.durata_tratt - chkBlock.Durata_Tratt);	// se la differenza fra le durate trattamento
			if((l>20) && (l<60))								// si scosta di 20" ma rimane entro i 60"
				Dat.durata_tratt=chkBlock.Durata_Tratt;			// risincronizza le durate

			WriteDati();										// memorizza Dat.durata_tratt in fram
			rdurata_tratt=0;									// azzera contatore 10"
        }
      }
}

// offset trasduttori di pressione
void Offset_Pressioni(void)
{
	Dat.Offset_Art = adcPressArt.conv.offset = adcPressArt.conv.valnoff;
	Dat.Offset_Fil = adcPressFil.conv.offset = adcPressFil.conv.valnoff;
	Dat.Offset_Ven = adcPressVen.conv.offset = adcPressVen.conv.valnoff;
	Dat.Offset_Art2 = adcPressArt2.conv.offset = adcPressArt2.conv.valnoff;

}



void Reset_Offset_Pressioni(void)
{
	Dat.Offset_Art = adcPressArt.conv.offset = 0;
	Dat.Offset_Fil = adcPressFil.conv.offset = 0;
	Dat.Offset_Ven = adcPressVen.conv.offset = 0;
	Dat.Offset_Art2 = adcPressArt2.conv.offset = 0;

}


#ifdef LOADER_ENGINE
/***
* loaderInit:
* Init the UART resources uses from the loader state machine
* Introduced in File Ver. 01.01
***/
uint8_t loaderInit()
{
    uint8_t u8RetVal = ERROR_NONE;

    /**Init the uart used to receive command from external PC (via Controller-BootLoader)
    */
    uart3_init();
    /**Init the protocol*/
    protocol_init();
    /**Global Var Init*/
	g_pFM3_UART3 = FM3_MFS3_UART;
    /**Loader Status Init*/
	loader_status_init();

    return u8RetVal;
}
/***
* protocol_init:
* Init the protocol resources used from the loader state machine
* Introduced in File Ver. 01.01
***/
void protocol_init()
{
	loader_proto_static_init(	&g_protoLoader,
								&g_uart3,
								g_aLoaderProtoDataBuf,
								LOADER_PROTODATA_BUF_LEN,
								1, SLAVE_THIS_DEVICE);
}
#endif //LOADER_ENGINE

#ifdef LOADER_ENGINE_INT
/**
To manage the IRQ from Controller by means of the UART port connected
**/
void uartRxFromControllerIrq()
{
    loader_proto_irqFromController();
}
#endif
