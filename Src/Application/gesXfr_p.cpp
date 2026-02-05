/**
@file		gesXfr_p.cpp
@brief		Module to implement SPI communication with controller micro
@author		Fernando Morani
@date		10/04/2017
@version	01.00
@source     M75-CPU Protective
*/

#include "gesXfr_p.h"

/*****************************************************
** GLOBAL AND STATIC VAR SECTION
*****************************************************/

CBufHandle hBufCsiA;			//Buffer to be used with CSI Channel
CSIHandle hCsiDevA;
int  pak_nul = PAK_NULL_DEFAULT;
CsiXfrBlockRx chkBlock; //Block to be used for interprocessors commuication - Receive
CsiXfrBlockTx bigBlock; //Block to be used for interprocessors commuication - Transmit
//struct CsiXfrBlockRx chkBlock; //Block to be used for interprocessors commuication - Receive
static long totale_calop;      //When this variable is set with a value different from 0?
static long teoric_calop;      //When this variable is set with a value different from 0?
static int  pak_timeout;
static int  zero_csi_read;
static int  char_rcv;
static int  pak_rcvd;
static int g_csi_error;

static int loop_counter;
static int loop_data_chk;


static int statoTest;
bool setDataTest = false;   //Solo per test poi cancellare
//word PsFlow;
bool reset_csi;


/*****************************************************
** EXTERNAL VAR SECTION
*****************************************************/
extern ADChan adcSet[MAX_AD];
//extern uint32_t    g_freqPS;
//extern uint32_t    g_freqUF; //Not in VetSmart
extern uint32_t    g_UFFreqCounter;
extern uint32_t    g_PSFreqCounter;
extern SystemStatus SysStatus;		// stato di funzionamento
extern DecTimer timForceCsi;
extern ALA   ala;
extern dat Dat;
extern DecTimer timStartComm;
extern volatile uint32_t     g_globalTick;
extern uint32_t g_StartController;
/****************************************************************************
uint8_t GesXfr_P(void)
** \brief Interproccessor communication by Means of the dedicated SPI Channel
** \return
**  ERROR_NONE if OK, FATAL_ERROR if impossible to go aehaed, number of
**  packets null
****************************************************************************/
uint8_t GesXfr_P()
{
uint8_t u8RetVal = ERROR_NONE;
word csiCount;
UBYTE 	bitIndex  = 0;
UBYTE   byteIndex = 0;
UBYTE   bitMask   = 0;
UBYTE   idxBit    = 0;

FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;


    pFM3_MFSx = FM3_MFS4_CSIO;      //This is a board setting. In Flexiper Protective Micro
                                    //the SIO_CHAN_2 is associated to the MFS4.
                                    //This code is not very strong for riusability. Should be changed
                                    //in the future. FM 10/4/2017
    loop_counter++;


	if( statoTest == INTERCOM_STATE_SEND_BLOCK  )
	{
		bigBlock.codeSttBlock = BLOCK_START_VAL;    // Start Value of the block
		bigBlock.cntBlockTop++;						// block counter

        bigBlock.ala_act= 0;  //Should be set to ala.act when introducing CD_ALA in the project;
        bigBlock.cla=PinRelePinza;

/*		idxAlarm=0;
		for(idxBit=0; idxBit<sizof(AlarmBit); idxBit)
		{
			bitMask = 0;
			if( idxAlarm < ALA_MAX )
			{
		        for(nAlarm=0; shiftBit=0x01; nAlarm < 8; shiftBit<<=0x01, nAlarm++)
        		{
					if( idxAlarm < ALA_MAX )
					{
			          if ( ala.ala_sta[idxAlarm] )
						bitMask |= shiftBit;
						idxAlarm++;
					}
				}
			}
			ala.ala[idxBit] = bitMask;
*/

        //Setting zero the alarm array - This is a reset i guess
		for(idxBit=0; idxBit < sizeof(bigBlock.ala_bit); idxBit++)
			bigBlock.ala_bit[idxBit] = 0 ;

        for (bitIndex=0; bitIndex<ALA_MAX; bitIndex++)
        {
			if ( ala.ala_sta[bitIndex] )  //To reintroduce when using CD_ALA in the project
            { //To reintroduce when using CD_ALA in the project
				byteIndex = bitIndex >> 3;
				bitMask = 1 << (bitIndex & 0x07);
			  	bigBlock.ala_bit[byteIndex] |= bitMask;
            }//To reintroduce when using CD_ALA in the project
        }

		//memcpy(bigBlock.ala_bit, ala.ala_bit, sizeof(bigBlock.ala_bit));
        bigBlock.ind_dat= chkBlock.ind_dat;
        bigBlock.fatal_num= ala.fatal_num;
		bigBlock.cal_num_pro= ala.cal_num_pro;
		bigBlock.durata_tratt= 0; //Dat.durata_tratt; //Should be set to Dat.durata_tratt when introducing CD_APP in the project;
		bigBlock.totale_calop=totale_calop;
		bigBlock.teoric_calop=teoric_calop;

		bigBlock.dat1=0;
		bigBlock.dat2=0;
		bigBlock.dat3=0;
		bigBlock.dat4=0;
		bigBlock.dat5=0;
		bigBlock.dat6=0;

		switch( chkBlock.ind_dat )
		{
			case	0:
				bigBlock.dat1= adcPressArt.conv.value;
				bigBlock.dat2= adcPressFil.conv.value;
				bigBlock.dat3= adcPressVen.conv.value;
				bigBlock.dat4= adcGauge1.conv.value;
				bigBlock.dat5= adcGauge2.conv.value;
				bigBlock.dat6= adcPressArt2.conv.value;
				break;

			case	1:
				bigBlock.dat1= adcPressArt.value;
				bigBlock.dat2= adcPressArt.filter.value;
				bigBlock.dat3= adcPressArt.conv.Points[0].PtValue;
				bigBlock.dat4= adcPressArt.conv.Points[1].PtValue - adcPressArt.conv.Points[0].PtValue;
				bigBlock.dat5= adcPressArt.conv.value;
				break;

			case	2:
				bigBlock.dat1=adcPressFil.value;
				bigBlock.dat2=adcPressFil.filter.value;
				bigBlock.dat3=adcPressFil.conv.Points[0].PtValue;
				bigBlock.dat4=adcPressFil.conv.Points[1].PtValue - adcPressFil.conv.Points[0].PtValue;
				bigBlock.dat5=adcPressFil.conv.value;
				break;

			case	3:
				bigBlock.dat1=adcPressVen.value;
				bigBlock.dat2=adcPressVen.filter.value;
				bigBlock.dat3=adcPressVen.conv.Points[0].PtValue;
				bigBlock.dat4=adcPressVen.conv.Points[1].PtValue - adcPressVen.conv.Points[0].PtValue;
				bigBlock.dat5=adcPressVen.conv.value;
				break;

			case	4:
				bigBlock.dat1=adcGauge1.value;
				bigBlock.dat2=adcGauge1.filter.value;
				bigBlock.dat3=adcGauge1.conv.Points[0].PtValue;
				bigBlock.dat4=adcGauge1.conv.Points[1].PtValue - adcGauge1.conv.Points[0].PtValue;
				bigBlock.dat5=adcGauge1.conv.value;
				break;

			case	5:
				bigBlock.dat1= adcGauge2.value;
				bigBlock.dat2= adcGauge2.filter.value;
				bigBlock.dat3= adcGauge2.conv.Points[0].PtValue;
				bigBlock.dat4= adcGauge2.conv.Points[1].PtValue - adcGauge2.conv.Points[0].PtValue;
				bigBlock.dat5= adcGauge2.conv.value;
				break;

			case	6:
				bigBlock.dat1=g_PSFreqCounter;  //Pulse counter
				bigBlock.dat2=g_UFFreqCounter;   //Pulse counter In flexiper is g_freqUF;  //Period in ms
				bigBlock.dat3=0;// bld.rx_fil;
				break;

			case	7:
				bigBlock.dat1=adcPressArt2.value;
				bigBlock.dat2=adcPressArt2.filter.value;
				bigBlock.dat3=adcPressArt2.conv.Points[0].PtValue;
				bigBlock.dat4=adcPressArt2.conv.Points[1].PtValue - adcPressArt.conv.Points[0].PtValue;
				bigBlock.dat5=adcPressArt2.conv.value;
				break;

			case	8:
				bigBlock.dat1=SysStatus.ChkCalc;
				bigBlock.dat2=SysStatus.ChkLoader;
				break;

			default:
                bigBlock.dat1=0;
                bigBlock.dat2=0;
                bigBlock.dat3=0;
                bigBlock.dat4=0;
                bigBlock.dat5=0;
                bigBlock.dat6=0;
                writeC(); //Nothing good
				break;
		}

        uint8_t *pBlock = &bigBlock.l1;

		bigBlock.cntBlockBot++;						// Block counter
		bigBlock.codeEndBlock = BLOCK_END_VAL;		// End block marker
		timForceCsi.Preset(WAIT_TIME_BEFORE_START);
	}


	switch( statoTest )
	{
		case INTERCOM_STATE_SEND_BLOCK:
            // Just Started
			PinSAFETY_BUSY = SET_BIT_LOW;                                   //Master now could not send
			csi_write(hCsiDevA, (byte *) &bigBlock, sizeof(bigBlock) );		//Prepare data to TX
			csi_clear(hCsiDevA);	// Clear channel

			timForceCsi.Preset(WAIT_TIME_BEFORE_ENABLE);
			PinSAFETY_BUSY = SET_BIT_HIGH;  //Master now could  send
			statoTest = INTERCOM_STATE_READY_TO_RX;
			u8RetVal = ERROR_NONE;
			break;

		case INTERCOM_STATE_READY_TO_RX:
            // Waiting for datas from Master
			if( csi_getCount(hCsiDevA, csiCount) )
			{
				if( csiCount > 1 )
					PinSAFETY_BUSY = SET_BIT_LOW;     //As soon as i received I stop the master

				if( csiCount >= sizeof(chkBlock) )			// If receieved entire packet.
				{
					csi_read(hCsiDevA, (byte *) &chkBlock, sizeof(chkBlock));	// copy last block
					csi_clear(hCsiDevA);	// Clear channel
                    csi_freeTx(hCsiDevA);								// sega eventuali intrusi...

					statoTest = INTERCOM_STATE_DATA_CHK;
					timForceCsi.Preset(WAIT_TIME_BEFORE_ENABLE);
					/*if (PinSAFETY_BUSY == SET_BIT_HIGH){
                        PinSAFETY_BUSY = SET_BIT_LOW;
					}*/
				}
				else
				{
                    //Nothingh
                    //pak_nul++;				// packet null counter increased
                    //u8RetVal = pak_nul;
                    u8RetVal = ERROR_NONE;
                }

			}else {
                //pak_nul++;				// packet null counter increased
				//u8RetVal = pak_nul;
				u8RetVal = ERROR_NONE;
			}

            if( timForceCsi.Match() )
            {
                pak_timeout++;
                PinSAFETY_BUSY = SET_BIT_LOW;
                csi_clear(hCsiDevA);								// Clear channel
                if (timStartComm.Match())
                {
                    Ala_Pre(ALP_T0T_CON_RUN,1);
                    PinBUZZER = 0;
                }
                statoTest = INTERCOM_STATE_TMT_EXPIRED;
            }

                    //}
			break;

		case    INTERCOM_STATE_DATA_CHK:
            loop_data_chk++;
		// Data verification and control
			if((chkBlock.codeSttBlock != BLOCK_START_VAL) || (chkBlock.codeEndBlock != BLOCK_END_VAL))		// dato di avvio e fine del blocco
			{
				pak_nul++;				// packet null counter increased
				u8RetVal = pak_nul;
				writeI();

			}
			else
			{
                if (g_StartController == OFF){
                    //1st packet received
                    g_StartController = ON;
                }//else{}
				pak_nul=0;
				u8RetVal = ERROR_NONE;
				Ges_cmd();				// Manage command master
				//PsFlow=chkBlock.PsFlow;
			}
			timForceCsi.Preset(WAIT_TIME_BASE);
			statoTest = INTERCOM_STATE_WAITING;
			break;

		case    INTERCOM_STATE_WAITING:
            // Waiting for sync
            if( timForceCsi.Match() )
            {
                pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW; //Disable Tx Interrupt
                pFM3_MFSx->SCR_f.RIE = SET_BIT_LOW; //Disable Rx Interrupt
                csi_clear(hCsiDevA);                //Clear buffer
                //csi_freeTx(hCsiDevA);
                pFM3_MFSx->ESCR_f.RESERVED1 = OFF;  //Reset h/w tx flag
                pFM3_MFSx->SCR_f.UPCL = SET_BIT_HIGH;   // reset seriale
                pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH; //Enable Rx Interrupt
                timForceCsi.Preset(WAIT_TIME_BEFORE_ENABLE);
                statoTest = INTERCOM_STATE_SEND_BLOCK;
            }
			break;
            case INTERCOM_STATE_TMT_EXPIRED:
            // preparing new block
            if( chkBlock.codeSttBlock != BLOCK_START_VAL || chkBlock.codeEndBlock != BLOCK_END_VAL || reset_csi == true)
            {
                reset_csi = false;
                csi_clear(hCsiDevA);        // sega eventuali intrusi...
                csi_freeTx(hCsiDevA);
            }
            writeE(); //Expired
            //csi_freeTx(hCsiDevA);
            csi_write(hCsiDevA, (byte *) &bigBlock, sizeof(bigBlock) );  // spara per il prossimo giro
            csi_clear(hCsiDevA);
                      // sega eventuali intrusi...
            timForceCsi.Preset(WAIT_TIME_BEFORE_ENABLE);
            //pak_nul=0;
            PinSAFETY_BUSY = SET_BIT_HIGH;  //Master now could  send

            //Se sono qua il time out è espirato senza che ritornasse a zero il pin
            statoTest = INTERCOM_STATE_READY_TO_RX;
   break;
        default:
            u8RetVal = FATAL_ERROR_GES_XFR;
            break;
	}
	return u8RetVal;
}
/*****************************************************
uint8_t IniXfr_P(void)
** \brief Init the CSI SPI Peripheral to communicate with controller Micro
** \return ERROR_NONE  or ERROR_INIT
*****************************************************/
uint8_t IniXfr_P(void)
{
uint8_t u8RetVal = ERROR_NONE;
bool bOkA = false;

    // Configure sio CHAN2 (CSI2) for interprocessor SPI communication
	hBufCsiA = combuf_create(CSI_TX_BUF_SIZE, CSI_RX_BUF_SIZE, CSI_TX_TRESHOLD, CSI_RX_TRESHOLD);

    if (hBufCsiA.theHandle == 0){
        //Error
        u8RetVal = ERROR_INIT;
    } else {
        // Configure hw
        bOkA = sio_cfgChan(SIO_CHAN2, hBufCsiA, SIO_SPI_SLAVE_FULL, SIO_RATE_EXT, SIO_8BIT,
				                  SIO_NOSTOP, SIO_NOPARITY, SIO_MSB_FIRST,
				                  SIO_CLKHIGH_REV, SIO_XFR_INT_ALL, IRQ_PRIO0);

        if( bOkA == True ){
            hCsiDevA = csi_create(hBufCsiA, NULL);
            bOkA = csi_open(hCsiDevA);
            if( bOkA == True ){
                u8RetVal = ERROR_NONE;
                }else {
                    u8RetVal = ERROR_INIT;
                }
            }
        else{
            hCsiDevA.theHandle = 0;
            u8RetVal = ERROR_INIT;
        }
	}
	char_rcv = 0;   //Init character received
	g_csi_error = 0;
	zero_csi_read = 0;
	pak_rcvd = 0;
	statoTest = INTERCOM_STATE_WAITING;
	PinSAFETY_BUSY = SET_BIT_LOW;
	timForceCsi.Preset(WAIT_TIME_BEFORE_START);

	return u8RetVal;
}

