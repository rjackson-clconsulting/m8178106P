/**
@file		SIOFM3.C
@brief		Serial channel Manager for FM3 Microprocessor
@author		Enrico Cavazza (former)/Fernando Morani
@date		18/01/2011 (1st release)/ 10/04/2017
@version	01.2
*/
/**
History: Ver 01.2 comes from 01.1.
Author of modification: Fernando Morani
Modification:
a) Changed this header to comply to doxygen format
b) Moved the h file include list in the siofm3.h
c) Declared here the global variable to be used for all the serial channel:
i.e. FM3_MFS03_UART_TypeDef    *pFM3_MFS0=FM3_MFS0_UART;
Version 01.2 has been used in TYPE0 FM3 processor
In the Flexiper TYPE2 s/w we use the follwing settings:
    MFS0 -> UART DEBUG
    MFS1 -> UART RS485 (su pin posizione _0, pin clock cambia de-re)
    MFS1 -> UART DRV  (su pin posizione _2)
    MFS1 -> UART BLUE (su pin posizione _1)
    MFS2 -> non disponibile (pin address)
    MFS3 -> UART UPFWP (scambio dati verso micro prot)
    MFS4 -> SAFETY_SPI verso micro prot.
    MFS5 -> non disponibile (pin dato o addr o ?)
    MFS6 -> SPI per scheda temperature+rtc+pld+fram
    MFS7 -> UART EXP
In the Flexiper TYPE0 s/w we use the follwing settings:
    MFS0 -> UART DEBUG
    MFS1 -> Channel not used
    MFS2 -> Channel not used
    MFS3 -> UART UPFWP - Used for firmware update
    MFS4 -> SAFETY_SPI SPI configuration to comunicate with FM3 TYPE2 processor named as controller.
    MFS5 -> Channel not used
    MFS6 -> SPI to manage fram
    MFS7 -> UART EXP
*/

#include "siofm3.h"
#include <cstring>

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------
FM3_MFS03_UART_TypeDef    *pFM3_MFS0=FM3_MFS0_UART;   //DEBUG UART
FM3_MFS03_UART_TypeDef    *pFM3_MFS1=FM3_MFS1_UART;
FM3_MFS03_UART_TypeDef    *pFM3_MFS3=FM3_MFS3_UART;
FM3_MFS47_UART_TypeDef    *pFM3_MFS7=FM3_MFS7_UART;

FM3_MFS47_CSIO_TypeDef    *pFM3_MFS4=FM3_MFS4_CSIO;
FM3_MFS47_CSIO_TypeDef    *pFM3_MFS6=FM3_MFS6_CSIO;

int provaPollCounter_Get;
int provaPollCounter_Put;
int provaTestFlagNotRcv;
int provaTestTxNotEnabled;
//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

static bool	calcSettingsCSI(SIOChan reqChan, SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
												SIODirBits reqDirBits, SIOClkMode reqClkMode,
												SIOXfrMode reqXfrMode, word *calcDivisor,
												word *calcPrescal, byte *calcBitset, byte *calcBitCount);

static bool	calcSettingsI2C(SIOChan reqChan, SIOMode reqMode, SIORate reqRate, byte *calcPrescal);

static bool	calcSettingsUART(SIOChan reqChan, SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
												SIOStopBits reqStopBits, SIOParityBits reqParityBits,
												word *calcDivisor, byte *calcPrescal, byte *calcBitset);

static bool fillHandleXfrCSI(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pRxFun)(byte),
									void (**pPollFun)(void),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) );

static bool fillHandleXfrI2C(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pPollFun)(void),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) );

static bool fillHandleXfrUART(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pPollFun)(void),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) );

static void setupCh0(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh1(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh2(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh3(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh4(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh5(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );

static void setupCh6(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) );


static void sioCsiPOLLCh1(void);
static void sioCsiPOLLCh2(void);
static void sioSttCsiPOLLCh1(byte c);
static void sioSttCsiPOLLCh2(byte c);
static void sioSttCsiINTCh0(byte c);
static void sioSttCsiINTCh1(byte c);
static void sioSttCsiINTCh2(byte c);
static void sioSttCsiINTCh3(byte c);
static void sioSttCsiINTCh4(byte c);
static void sioSttCsiDMACh0(byte c);
static void sioIrqHandCsiDMACh0(int vn);
static void sioXfrCsiPOLLCh1(byte c);
static void sioXfrCsiREADCh1(byte c);
static void sioXfrCsiREADCh2(byte c);
static void sioXfrCsiSOFTCh0(byte c);
static void sioXfrCsiSOFTCh1(byte c);
static void sioXfrCsiSOFTCh2(byte c);
static void sioXfrCsiSOFTCh3(byte c);
static void sioXfrCsiSOFTCh4(byte c);
static void sioIrqHandCsiINTCh0(int vn);
static void sioIrqHandCsiINTCh1(int vn);
static void sioIrqHandCsiINTCh2(int vn);
static void sioI2cPOLLCh0(void);
static void sioI2cPOLLCh2(void);
static void sioSttI2cPOLLCh0(byte c);
static void sioSttI2cPOLLCh2(byte c);
static void sioSttI2cINTCh0(byte c);
static void sioSttI2cINTCh2(byte c);
static void sioSttI2cDMACh0(byte c);
static void sioSttI2cDMACh2(byte c);
static void sioXfrI2cPOLLCh0(byte c);
static void sioXfrI2cPOLLCh2(byte c);
static void sioIrqHandI2cINTCh0(int vn);
static void sioIrqHandI2cINTCh2(int vn);
static void sioUartPOLLCh0(void);
static void sioUartPOLLCh1(void);
static void sioUartPOLLCh3(void);
static void sioSttUartPOLLCh0(byte c);
static void sioSttUartPOLLCh1(byte c);
static void sioSttUartPOLLCh3(byte c);
static void sioSttUartINTCh0(byte c);
static void sioSttUartINTCh3(byte c);
static void sioSttUartINTCh5(byte c);
static void sioSttUartINTCh6(byte c);
static void sioSttUartDMACh0(byte c);
static void sioSttUartDMACh1(byte c);
static void sioSttUartDMACh3(byte c);
static void sioIrqHandUartINTCh0(int vn);
static void sioIrqHandUartINTCh3(int vn);
static void sioIrqHandUartINTCh5(int vn);
static void sioIrqHandUartINTCh6(int vn);

static void setSioSPIModeMasterCh1(uint8_t clockMode);
static void setSioSPIModeSlaveCh1(uint8_t clockMode);
static void setSioSPIModeSlaveCh2(uint8_t clockMode);

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

#define UART0_RX_IRQ Vector5C
#define UART0_TX_IRQ Vector60

#define UART1_RX_IRQ Vector64
#define UART1_TX_IRQ Vector68

#define UART3_RX_IRQ Vector74
#define UART3_TX_IRQ Vector78

#define CSIO4_RX_IRQ Vector7C
#define CSIO4_TX_IRQ Vector80

#define UART6_RX_IRQ Vector8C
#define UART6_TX_IRQ Vector90

#define UART7_RX_IRQ Vector94
#define UART7_TX_IRQ Vector98


													// valori numerici di SIORate
static const long baudRateTab[] = { 4800, 9600, 19200, 38400, 57600, 76800, 115200, 230400,// UART
									200000, 400000, 								// I2C
									500000, 1000000, 1500000, 2000000  				// CSI
								};

													// maschere msb per trasmissione soft
static const word bitMsbMaskTab[] = { 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800,
									  0x1000, 0x2000, 0x4000, 0x8000 };


                                        // Riepilogo funzioni per trasferimento dati dirette (a polling)
                                        // Channel 1 SPI-C to SPI-P Transfer Polling
static const funpb TabXfrCsiPOLL[] =	{ NULL, sioXfrCsiPOLLCh1, NULL, NULL, NULL };

static const funpb TabXfrI2cPOLL[] =	{ sioXfrI2cPOLLCh0,	NULL, sioXfrI2cPOLLCh2, NULL, NULL };

											// Riepilogo funzioni per trasferimento in modalità software (emulazione periferica)
static const funpb TabXfrCsiSOFT[] =	{ sioXfrCsiSOFTCh0,	sioXfrCsiSOFTCh1, sioXfrCsiSOFTCh2, NULL, sioXfrCsiSOFTCh4 };

											// Riepilogo funzioni per avvio trasferimento dati
static const funpb TabSttCsiPOLL[] =	{ NULL, sioSttCsiPOLLCh1, sioSttCsiPOLLCh2, NULL, NULL }; // Channel 1 SPI-C to SPI-P Transfer Polling
static const funpb TabSttCsiINT[] =		{ sioSttCsiINTCh0,	sioSttCsiINTCh1, sioSttCsiINTCh2, NULL, NULL };
static const funpb TabSttCsiDMA[] =		{ NULL, NULL, NULL, NULL, NULL };
static const funpb TabSttUartPOLL[] =	{ sioSttUartPOLLCh0, sioSttUartPOLLCh1, NULL, sioSttUartPOLLCh3, NULL };
static const funpb TabSttUartINT[] =	{ sioSttUartINTCh0, NULL, NULL, sioSttUartINTCh3, NULL,sioSttUartINTCh5, sioSttUartINTCh6, NULL };
static const funpb TabSttUartDMA[] =	{ sioSttUartDMACh0, sioSttUartDMACh1, NULL, sioSttUartDMACh3, NULL };
static const funpb TabSttI2cPOLL[] =	{ sioSttI2cPOLLCh0, NULL, sioSttI2cPOLLCh2, NULL, NULL };
static const funpb TabSttI2cINT[] =		{ sioSttI2cINTCh0, NULL, sioSttI2cINTCh2,   NULL, NULL };
static const funpb TabSttI2cDMA[] = 	{ sioSttI2cDMACh0, NULL, sioSttI2cDMACh2,	NULL, NULL	};

static const funpb TabXfrCsiREAD[] =    { NULL,sioXfrCsiREADCh1, sioXfrCsiREADCh2, NULL, NULL };
											// Riepilogo funzioni per gestione trasferimento ad interrupt
static const funpi TabIrqHandCsiINT[] = 	{ sioIrqHandCsiINTCh0, sioIrqHandCsiINTCh1, sioIrqHandCsiINTCh2, NULL, NULL };
static const funpi TabIrqHandCsiDMA[] = 	{ NULL, NULL, NULL, NULL, NULL };
static const funpi TabIrqHandUartINT[] = 	{ sioIrqHandUartINTCh0, NULL, NULL, sioIrqHandUartINTCh3, NULL,sioIrqHandUartINTCh5, sioIrqHandUartINTCh6, NULL };
static const funpi TabIrqHandUartDMA[] = 	{ NULL, NULL, NULL, NULL, NULL };
static const funpi TabIrqHandI2cINT[] = 	{ sioIrqHandI2cINTCh0, NULL, sioIrqHandI2cINTCh2, NULL, NULL };
static const funpi TabIrqHandI2cDMA[] = 	{ NULL, NULL, NULL, NULL, NULL };

											// Riepilogo funzioni per gestione trasferimento a polling (richiamo periodico)
static const funpv TabCsiPOLL[] 	= { NULL, sioCsiPOLLCh1, sioCsiPOLLCh2, NULL, NULL }; // Channel 1 SPI-C to SPI-P Transfer Polling
static const funpv TabUartPOLL[] 	= { sioUartPOLLCh0, sioUartPOLLCh1, NULL, sioUartPOLLCh3, NULL };
static const funpv TabI2cPOLL[]		= { sioI2cPOLLCh0, NULL, sioI2cPOLLCh2, NULL, NULL };

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
									// configurazioni eseguite per ogni canale
static SioCfgParam sioCfgTab[SIO_CHAN_TOT];
static SIOErrors sioError;

//-------------------------------------------------------
// Corpo funzioni di questo modulo
//-------------------------------------------------------



/* Cerca di inizializzare il modulo sio
*/
void sio_init(void)
{

	sioError = SIO_ERR_NONE;
	memset(&sioCfgTab[0], 0, sizeof(sioCfgTab));
}


/* Polla tutti i canali predisposti per questo tipo di trasferimento
*/
void sio_poll(void)
{
byte	nChan;

	for(nChan=0; nChan<SIO_CHAN_TOT; nChan++)
	{
		if( sioCfgTab[nChan].configured )		// prima di tutto se è configurato
			if( sioCfgTab[nChan].pfPoller != NULL )
				sioCfgTab[nChan].pfPoller();	// chiama la funzione predestinata
	}
}


/* Cerca di cambiare marcia */
bool sio_changeUartSettings(SIOChan reqChan, SIORate reqRate, SIODataBits reqDataBits,
											 SIOStopBits reqStopBits, SIOParityBits reqParityBits)
{
word divisor;
byte flagSMR;
byte flagASIM;
byte copia_sric;
byte copia_stic;

	switch( reqChan )
	{
		case	SIO_CHAN0:				// primo bel canale

        /*
			copia_sric = SRIC0;			// fa una copia
			copia_stic = STIC0;			// fa una copia

            SRIC0 = 0x47;                // disabilita interrupt
            STIC0 = 0x47;                // disabilita interrupt

			if( calcSettingsUART(reqChan, SIO_UART, reqRate, reqDataBits, reqStopBits, reqParityBits,
							 &divisor, &prescal, &flagASIM) == False )
				return(False);

			ASIM0 = 0x80;				// iniziamo clocckando la periferica

			CKSR0 = prescal;			// imposta il prescaler gentilmente ricevuto
			BRGC0 = divisor;			// schiaffa il divisore per il rate calcolato prima

			SRIC0 = copia_sric & 0x7F;		// ripristina copia
			STIC0 = copia_stic & 0x7F;		// ripristina copia

			ASIM0 |= flagASIM;			// imposta parità e numero bits come desiderato
			ASIM0 |= 0x60;				// abilitiamo sia ricezione che trasmissione
			*/
			break;

		case	SIO_CHAN1:
        /*
			copia_sric = SRIC1;			// fa una copia
			copia_stic = STIC1;			// fa una copia

            SRIC1 = 0x47;                // disabilita interrupt
            STIC1 = 0x47;                // disabilita interrupt

			if( calcSettingsUART(reqChan, SIO_UART, reqRate, reqDataBits, reqStopBits, reqParityBits,
							 &divisor, &prescal, &flagASIM) == False )
				return(False);

			ASIM1 = 0x80;				// iniziamo clocckando la periferica

			CKSR1 = prescal;			// imposta il prescaler gentilmente ricevuto
			BRGC1 = divisor;			// schiaffa il divisore per il rate calcolato prima

			SRIC1 = copia_sric & 0x7F;		// ripristina copia
			STIC1 = copia_stic & 0x7F;		// ripristina copia

			ASIM1 |= flagASIM;			// imposta parità e numero bits come desiderato
			ASIM1 |= 0x60;				// abilitiamo sia ricezione che trasmissione
			*/
			break;

		case	SIO_CHAN2:
			sioError = SIO_ERR_BAUD_CHANGE;
			break;

		case	SIO_CHAN3:
            byte flagESCR;

			if( calcSettingsUART(reqChan, SIO_UART, reqRate, reqDataBits, reqStopBits, reqParityBits,
							 &divisor, &flagSMR, &flagESCR) == False )
				return(False);

            pFM3_MFS1->SCR |=   BIT7;   						// UART reset
            pFM3_MFS1->SMR &= ~(BIT5 + BIT6 + BIT7); 			// UART0 mode selected
            pFM3_MFS1->SMR &= ~(BIT4); 							// Disables the Wake-up function.
            pFM3_MFS1->SMR &= ~(BIT2); 							// LSB first (The least significant bit is first transferred.)

            pFM3_MFS1->SSR |=  (BIT7);   							// clear receive error flags

            pFM3_MFS1->ESCR = flagESCR;
            pFM3_MFS1->ESCR &= ~(BIT5);
            pFM3_MFS1->ESCR &= ~(BIT7);                        // set flow ctrl = none

			pFM3_MFS1->BGR  = divisor;			                // schiaffa il divisore per il rate calcolato prima
            pFM3_MFS1->SMR |= flagSMR;

    		break;


		case	SIO_CHAN4:
        /*
			sioError = SIO_ERR_BAUD_CHANGE;
			*/
			break;

		case	SIO_CHAN5:
        /*
			sioError = SIO_ERR_BAUD_CHANGE;
			*/
			break;
		case	SIO_CHAN6:
        /*
			sioError = SIO_ERR_BAUD_CHANGE;
			*/
			break;

		case	SIO_CHAN7:
        /*
			sioError = SIO_ERR_BAUD_CHANGE;
			*/
			break;

		default:
			sioError = SIO_ERR_INV_CHAN;			// ma sei proprio sicuro ???
			break;
	}
    return(True);
}



/* Configura un canale come vuoi tu
*/
bool sio_cfgChan(SIOChan reqChan, CBufHandle hBuf, SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
				                  SIOStopBits reqStopBits, SIOParityBits reqParityBits,
				                  SIODirBits reqDirBits, SIOClkMode reqClkMode, SIOXfrMode reqXfrMode,
				                  IRQPriority reqPrio)
{
word divisor = 0;  //Init done to 0 for now
byte flagSMR  = 0;
byte flagESCR  = 0;
byte flagCSIM = 0;
word flagCSIS = 0;
byte flagCSIB = 0;
void (*pTxFun)(byte c) = NULL;									// in principio era nullo (il puntatore)
void (*pPollFun)(void) = NULL;									// in principio era nullo (il puntatore)
void (*pIrqHandTx)(int v) = NULL;									// in principio era nullo (il puntatore)
void (*pIrqHandRx)(int v) = NULL;									// in principio era nullo (il puntatore)
SIOChan absChan = SIO_CHAN_CLOSE;                                 //Init Value
void (*pRxFun)(byte c) = NULL;                                     //To use for RX

	sioError = SIO_ERR_NONE;										// facciamo finta che non ci siano errori

        // trappa gli alias del sio chan 3
    if( (reqChan == SIO_CHAN3_0) || (reqChan == SIO_CHAN3_1) || (reqChan == SIO_CHAN3_2))
    {
        absChan = SIO_CHAN3;
    }
    else
        absChan = reqChan;

	combuf_getPointer(hBuf, &sioCfgTab[absChan].pComBuf);		// si fa dare il puntatore al buffer per cortesia


	if( sioCfgTab[absChan].pComBuf == NULL )					// se non e' stato gentile
		sioError = SIO_ERR_INV_BUFFER;								// brutta storia questa


#ifdef __DEBUG__

	if( sioError )										// se lo vogliono
	{
		error_trace("SIOFM3", "sio_CfgChan", sioError);	// aspetta che lo vengano a prendere
		return(False);
	}
#else
	if( sioError )
		return(False);
#endif


	switch( reqMode )				// partiamo dalla prima richiesta
	{
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
		if( calcSettingsCSI(absChan, reqMode, reqRate, reqDataBits, reqDirBits, reqClkMode,
								 reqXfrMode, &divisor, &flagCSIS, &flagCSIM, &flagCSIB) == False)
				return(False);
				break;
        case    SIO_SPI_MASTER_FULL:
        case    SIO_SPI_MASTER_ONLY_TX:
        case    SIO_SPI_MASTER_ONLY_RX:
        case    SIO_SPI_SLAVE_FULL:
        case    SIO_SPI_SLAVE_ONLY_TX:
        case    SIO_SPI_SLAVE_ONLY_RX:
			if( calcSettingsCSI(absChan, reqMode, reqRate, reqDataBits, reqDirBits, reqClkMode,
								 reqXfrMode, &divisor, &flagCSIS, &flagSMR, &flagESCR) == False )
				return(False);

			if( fillHandleXfrCSI(absChan, reqXfrMode, &pTxFun, &pRxFun,&pPollFun, &pIrqHandTx, &pIrqHandRx) == False )
				return(False);
			break;

		case	SIO_I2C:
			if( calcSettingsI2C(absChan, reqMode, reqRate, &flagSMR) == False )
				return(False);

			if( fillHandleXfrI2C(absChan, reqXfrMode, &pTxFun, &pPollFun, &pIrqHandTx, &pIrqHandRx) == False )
				return(False);
			break;

		case	SIO_UART:
			if( calcSettingsUART(absChan, reqMode, reqRate, reqDataBits, reqStopBits, reqParityBits,
								 &divisor, &flagSMR, &flagESCR) == False )
				return(False);

			if( fillHandleXfrUART(absChan, reqXfrMode, &pTxFun, &pPollFun, &pIrqHandTx, &pIrqHandRx) == False )
				return(False);
			break;
		default:
			sioError = SIO_ERR_INV_MODE;							// segnala la cappella sistina
			break;
	}

	if( reqPrio > IRQ_PRIO7 )					// piccola verifica sulla priorità
		sioError = SIO_ERR_INV_PRIO;

#ifdef __DEBUG__

	if( sioError )						// se lo vogliono
	{
		error_trace("SIOFM3", "sio_CfgChan", sioError);	// aspetta che lo vengano a prendere
		return(False);
	}
#endif

	switch( reqChan )
	{
		case	SIO_CHAN0:
			setupCh0(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN1:
			setupCh1(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN2:
			setupCh2(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN3:
		case	SIO_CHAN3_0:

		        // commuta la seriale 1 su mux _0
            FM3_GPIO->EPFR07_f.SIN1S0 = 1;
            FM3_GPIO->EPFR07_f.SOT1B0 = 1;

			setupCh3(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN3_1:

            // commuta la seriale 1 su mux _1
            FM3_GPIO->EPFR07_f.SIN1S1 = 1;
            FM3_GPIO->EPFR07_f.SOT1B1 = 1;

			setupCh3(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN3_2:

                // commuta la seriale 1 su mux _2
            FM3_GPIO->EPFR07_f.SIN1S0 = 1;
            FM3_GPIO->EPFR07_f.SIN1S1 = 1;

            FM3_GPIO->EPFR07_f.SOT1B0 = 1;
            FM3_GPIO->EPFR07_f.SOT1B1 = 1;

			setupCh3(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN4:
			setupCh4(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN5:
			setupCh5(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		case	SIO_CHAN6:
			setupCh6(reqMode, flagCSIM, flagCSIS, flagCSIB, divisor, flagSMR, flagESCR, reqXfrMode, reqPrio, pIrqHandTx, pIrqHandRx);	// setup canale + pins
			break;

		default:
			sioError = SIO_ERR_INV_CHAN;			// ma sei proprio sicuro ???
			break;
	}

#ifdef __DEBUG__

	if( sioError )						// se lo vogliono
	{
		error_trace("SIOFM3", "sio_CfgChan", sioError);	// aspetta che lo vengano a prendere
		return(False);
	}
#else
	if( sioError )
		return(False);
#endif


	sioCfgTab[absChan].configured = True;			// possiamo dire che sia configurato

	sioCfgTab[absChan].errCount = 0;
	sioCfgTab[absChan].cfgMode = reqMode;
	sioCfgTab[absChan].cfgRate = reqRate;
	sioCfgTab[absChan].cfgDataBits = reqDataBits;
	sioCfgTab[absChan].cfgStopBits = reqStopBits;
	sioCfgTab[absChan].cfgParityBits = reqParityBits;
	sioCfgTab[absChan].cfgDirBits = reqDirBits;
	sioCfgTab[absChan].cfgClkMode = reqClkMode;
	sioCfgTab[absChan].cfgXfrMode = reqXfrMode;
	sioCfgTab[absChan].pfPoller = pPollFun;

	sioCfgTab[absChan].csiSoftBitCnt = reqDataBits + 7;	// bit count per csi in modalità softw
	sioCfgTab[absChan].csiSoftMsbMask = bitMsbMaskTab[reqDataBits];			// maschera bit msb per csi in modalità softw

	sioCfgTab[absChan].pComBuf->txBuf.pfSogliaFun = pTxFun;					// assegna la funzione giusta
	sioCfgTab[absChan].pComBuf->rxBuf.pfSogliaFun = pRxFun;					// assegna la funzione giusta

	return(True);											// tutto ok !!
}


/* Esegue, su richiesta della sio_cfgChan, il setup del canale 0
*/
static void setupCh0(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

    uint8_t mask = (1 << 0) | (1 << 1) | (1 << 3)  | (1 << 7); //SONAR_QUBE
	switch( reqMode )
	{
		case	SIO_UART:
            // Setting the Serial Control Register
            pFM3_MFS0->SCR |=   mask;   // UART reset
            //pFM3_MFS0->SCR |=   BIT7;   // UART reset
            // Setting the Serial Mode Register
            pFM3_MFS0->SMR &= ~(BIT5 + BIT6 + BIT7); 			// UART0 mode selected
            pFM3_MFS0->SMR &= ~(BIT4); 							// RESERVED
            pFM3_MFS0->SMR &= ~(BIT2); 							// LSB first (The least significant bit is first transferred.)

            // Setting the Serial Status Register
            pFM3_MFS0->SSR |=  (BIT7);   							// clear receive error flags

            //Setting the Extended Communication Control Register
            pFM3_MFS0->ESCR = flagESCR &~(BIT5);                // Setting the communication using the function parameter and anyway forcing the NRZ
            pFM3_MFS0->ESCR  &= ~(BIT7);                        // set flow ctrl = none


			pFM3_MFS0->BGR  = divisor;			                // schiaffa il divisore per il rate calcolato prima
            pFM3_MFS0->SMR |= flagSMR;

            pFM3_MFS0->SMR |=  (BIT0); 							// Enables a serial data output.


			break;
        // We use this channel only in UART Mode
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			return;
	}


	if( reqXfr == SIO_XFR_INT_RX || reqXfr == SIO_XFR_INT_ALL )	// tutto ad irq (errori compresi)
	{

	//---- UART 0 TX------------------------------------
        if( pfIrqHandTx )
        {
            NVIC_ClearPendingIRQ(MFS0TX_IRQn);
            NVIC_EnableIRQ(MFS0TX_IRQn);
            NVIC_SetPriority(MFS0TX_IRQn, 14);
        }

		if( pfIrqHandRx )					// se puo' aggancia i vettori richiesti
		{
            NVIC_ClearPendingIRQ(MFS0RX_IRQn);
            NVIC_EnableIRQ(MFS0RX_IRQn);
            NVIC_SetPriority(MFS0RX_IRQn, 13);
        }

	}
}

/* Esegue, su richiesta della sio_cfgChan, il setup del canale 1
*/
static void setupCh1(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

    /*stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
    stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;
    stc_mfs47_csio_smr_field_t *pSMRReg = NULL;*/
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;

    pFM3_MFSx = FM3_MFS4_CSIO;


    /*pSCRReg = &FM3_MFS4_CSIO->SCR_f;
    pSSRReg = &FM3_MFS4_CSIO->SSR_f;
    pSMRReg = &FM3_MFS4_CSIO->SMR_f;*/

	switch( reqMode )
	{
		case	SIO_SPI_MASTER_ONLY_TX:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for TX
            /*pSMRReg->SOE = SET_BIT_HIGH;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_LOW;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_LOW;
            break;
		case	SIO_SPI_MASTER_ONLY_RX:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for RX
            /*pSMRReg->SOE = SET_BIT_LOW;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_HIGH;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_SPI_MASTER_FULL:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for TX/RX
            /*pSMRReg->SOE = SET_BIT_HIGH;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_HIGH;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_SPI_SLAVE_ONLY_TX:
            setSioSPIModeSlaveCh1(flagSMR);
            //Add specific setting for TX
            /*pSMRReg->SOE = SET_BIT_HIGH;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_LOW;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_LOW;
            break;
		case	SIO_SPI_SLAVE_ONLY_RX:
            setSioSPIModeSlaveCh1(flagSMR);
            /*pSMRReg->SOE = SET_BIT_LOW;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_HIGH;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            //Add specific setting for RX
            break;
		case	SIO_SPI_SLAVE_FULL:
            setSioSPIModeSlaveCh1(flagSMR);
            /*pSMRReg->SOE = SET_BIT_HIGH;
            pSCRReg->TXE = SET_BIT_HIGH;
            pSCRReg->RXE = SET_BIT_HIGH;*/
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            //Add specific setting for TX/RX
            break;
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
			break;

		case	SIO_UART:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			break;
	}

    //Setting the Bit Lenght
    pFM3_MFS4->ESCR |= flagESCR;
    pFM3_MFSx->ESCR |= flagESCR;
    //Setting the baudRate
    pFM3_MFS4->BGR |= divisor;
    pFM3_MFSx->BGR |= divisor;
	if( sioError )						// mica so scemo !!
		return;

    // Enabling/Disabling Interrupt
    switch (reqXfr)
    {
    case SIO_XFR_INT_RX:
       /* pSCRReg->TIE = SET_BIT_LOW;
        pSCRReg->TBIE = SET_BIT_LOW;
        pSCRReg->RIE = SET_BIT_HIGH;*/
        pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.TBIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH;
        break;
    case SIO_XFR_INT_ALL:
        /*pSCRReg->TIE = SET_BIT_HIGH;
        pSCRReg->TBIE = SET_BIT_HIGH;
        pSCRReg->RIE = SET_BIT_HIGH;*/
        pFM3_MFSx->SCR_f.TIE = SET_BIT_HIGH;
        pFM3_MFSx->SCR_f.TBIE = SET_BIT_HIGH;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH;
        break;
    case SIO_XFR_POLL:
    case SIO_XFR_DMA_RX:
    case SIO_XFR_DMA_ALL:
    case SIO_XFR_SOFT:
        /*pSCRReg->TIE = SET_BIT_LOW;
        pSCRReg->TBIE = SET_BIT_LOW;
        pSCRReg->RIE = SET_BIT_LOW;*/
        pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.TBIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH;
        break;
    default:
        sioError = SIO_ERR_XFR_MODE;
        break;
    }
    return;
}


/* Esegue, su richiesta della sio_cfgChan, il setup del canale 2
*/
static void setupCh2(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;

    pFM3_MFSx = FM3_MFS4_CSIO;      //This is a board setting. In Flexiper Protective Micro
                                    //the SIO_CHAN_2 is associated to the MFS4.
                                    //This code is not very strong for riusability. Should be changed
                                    //in the future. FM 10/4/2017

	switch( reqMode )
	{
		case	SIO_SPI_MASTER_ONLY_TX:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for TX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_LOW;
            break;
		case	SIO_SPI_MASTER_ONLY_RX:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for RX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_SPI_MASTER_FULL:
            setSioSPIModeMasterCh1(flagSMR);
            //Add specific setting for TX/RX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_SPI_SLAVE_ONLY_TX:
            setSioSPIModeSlaveCh1(flagSMR);
            //Add specific setting for TX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_LOW;
            break;
		case	SIO_SPI_SLAVE_ONLY_RX:
            setSioSPIModeSlaveCh1(flagSMR);
            //Add specific setting for RX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_LOW;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_SPI_SLAVE_FULL:
            setSioSPIModeSlaveCh2(flagSMR);
            //Add specific setting for TX/RX
            pFM3_MFSx->SMR_f.SOE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.TXE = SET_BIT_HIGH;
            pFM3_MFSx->SCR_f.RXE = SET_BIT_HIGH;
            break;
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
			break;

		case	SIO_UART:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			break;
	}

    //Setting the Bit Lenght
    //pFM3_MFS4->ESCR |= flagESCR;
    pFM3_MFSx->ESCR |= flagESCR;
    //Setting the baudRate
    //pFM3_MFS4->BGR |= divisor;
    pFM3_MFSx->BGR |= divisor;
	if( sioError )						// mica so scemo !!
		return;

    // Enabling/Disabling Interrupt
    switch (reqXfr)
    {
    case SIO_XFR_INT_RX:
        pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW;
        //pFM3_MFSx->SCR_f.TBIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH;
        break;
    case SIO_XFR_INT_ALL:
        //pFM3_MFSx->SCR_f.TIE = SET_BIT_HIGH;
        //pFM3_MFSx->SCR_f.TBIE = SET_BIT_HIGH;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_HIGH;
        break;
    case SIO_XFR_POLL:
    case SIO_XFR_DMA_RX:
    case SIO_XFR_DMA_ALL:
    case SIO_XFR_SOFT:
        pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.TBIE = SET_BIT_LOW;
        pFM3_MFSx->SCR_f.RIE = SET_BIT_LOW;
        break;
    default:
        sioError = SIO_ERR_XFR_MODE;
        break;
    }
    return;


}

/**
* Esegue, su richiesta della sio_cfgChan, il setup del canale 3
* Parameters:
* reqMode: set the operative mode fo the MFS
* flagESCR: Per impostare la comunicazine seriale ... ma è usato davvero?
*/
static void setupCh3(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

    uint8_t mask = (1 << 0) | (1 << 1) | (1 << 3)  | (1 << 7); //SONAR_QUBE

	switch( reqMode )
	{
		case	SIO_UART:
            // Setting the Serial Control Register
            pFM3_MFS1->SCR |=   mask;   						// UART reset

            // Setting the Serial Mode Register
            pFM3_MFS1->SMR &= ~(BIT5 + BIT6 + BIT7); 			// UART0 mode selected
            pFM3_MFS1->SMR &= ~(BIT4); 							// RESERVED
            pFM3_MFS1->SMR &= ~(BIT2); 							// LSB first (The least significant bit is first transferred.)

            // Setting the Serial Status Register
            pFM3_MFS1->SSR |=  (BIT7);   							// clear receive error flags

            //Setting the Extended Communication Control Register
            pFM3_MFS1->ESCR = flagESCR &~(BIT5);                // Setting the communication using the function parameter and anyway forcing the NRZ
            pFM3_MFS1->ESCR  &= ~(BIT7);                        // set flow ctrl = none


			pFM3_MFS1->BGR  = divisor;			                // schiaffa il divisore per il rate calcolato prima
            pFM3_MFS1->SMR |= flagSMR;

            pFM3_MFS1->SMR |=  (BIT0); 							// Enables a serial data output.



 			break;
        // We use the channel 3 only in UART Mode
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			return;
	}


	if( reqXfr == SIO_XFR_INT_RX || reqXfr == SIO_XFR_INT_ALL )	// tutto ad irq (errori compresi)
	{

	//---- UART 1 TX------------------------------------
        if( pfIrqHandTx )
        {
            NVIC_ClearPendingIRQ(MFS1TX_IRQn);
            NVIC_EnableIRQ(MFS1TX_IRQn);
            NVIC_SetPriority(MFS1TX_IRQn, 14);
        }

		if( pfIrqHandRx )					// se puo' aggancia i vettori richiesti
		{
            NVIC_ClearPendingIRQ(MFS1RX_IRQn);
            NVIC_EnableIRQ(MFS1RX_IRQn);
            NVIC_SetPriority(MFS1RX_IRQn, 13);
        }

	}

}

/**
* Performing setup register of the channel 4
*/
static void setupCh4(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte prescal, byte flagASIM, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

	switch( reqMode )
	{
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
											// solo sw emulata su pin PLD
			break;

		case	SIO_UART:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			break;
	}


	if( reqXfr == SIO_XFR_INT_RX || reqXfr == SIO_XFR_INT_ALL )	// tutto ad irq (errori compresi)
	{
		sioError = SIO_ERR_INV_MODE;		// segnala la cappella sistina
	}

	if( reqXfr == SIO_XFR_DMA_RX || reqXfr == SIO_XFR_DMA_ALL )	// tutto a dma (errori compresi)
	{
		sioError = SIO_ERR_INV_MODE;		// segnala la cappella sistina
	}

}


/**
* Esegue, su richiesta della sio_cfgChan, il setup del canale 5
* Parameters:
* reqMode: set the operative mode fo the MFS
* flagESCR: Per impostare la comunicazine seriale ... ma è usato davvero?
*/
static void setupCh5(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

    uint8_t mask = (1 << 0) | (1 << 1) | (1 << 3)  | (1 << 7); //SONAR_QUBE
	switch( reqMode )
	{
		case	SIO_UART:
            // Setting the Serial Control Register
            pFM3_MFS3->SCR |=   mask;   						// UART reset
            //pFM3_MFS3->SCR |=   BIT7;   						// UART reset
            // Setting the Serial Mode Register
            pFM3_MFS3->SMR &= ~(BIT5 + BIT6 + BIT7); 			// UART0 mode selected
            pFM3_MFS3->SMR &= ~(BIT4); 							// RESERVED
            pFM3_MFS3->SMR &= ~(BIT2); 							// LSB first (The least significant bit is first transferred.)

            // Setting the Serial Status Register
            pFM3_MFS3->SSR |=  (BIT7);   							// clear receive error flags

            //Setting the Extended Communication Control Register
            pFM3_MFS3->ESCR = flagESCR &~(BIT5);                // Setting the communication using the function parameter and anyway forcing the NRZ
            pFM3_MFS3->ESCR  &= ~(BIT7);                        // set flow ctrl = none


			pFM3_MFS3->BGR  = divisor;			                // schiaffa il divisore per il rate calcolato prima
            pFM3_MFS3->SMR |= flagSMR;

            pFM3_MFS3->SMR |=  (BIT0); 							// Enables a serial data output.


			break;
        // We use the channel 5 only in UART Mode
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			return;
	}


	if( reqXfr == SIO_XFR_INT_RX || reqXfr == SIO_XFR_INT_ALL )	// tutto ad irq (errori compresi)
	{

	//---- UART 3 TX------------------------------------
        if( pfIrqHandTx )
        {
            NVIC_ClearPendingIRQ(MFS3TX_IRQn);
            NVIC_EnableIRQ(MFS3TX_IRQn);
            NVIC_SetPriority(MFS3TX_IRQn, 14);
        }

		if( pfIrqHandRx )					// se puo' aggancia i vettori richiesti
		{
            NVIC_ClearPendingIRQ(MFS3RX_IRQn);
            NVIC_EnableIRQ(MFS3RX_IRQn);
            NVIC_SetPriority(MFS3RX_IRQn, 13);
        }

	}

}


/**
* Esegue, su richiesta della sio_cfgChan, il setup del canale 6
* Parameters:
* reqMode: set the operative mode fo the MFS
* flagESCR: Per impostare la comunicazine seriale ... ma è usato davvero?
*/
static void setupCh6(SIOMode reqMode, byte flagCSIM, byte flagCSIS, byte flagCSIB,
							word divisor, byte flagSMR, byte flagESCR, SIOXfrMode reqXfr,
							IRQPriority reqPrio, void (*pfIrqHandTx)(int v), void (*pfIrqHandRx)(int v) )
{

    uint8_t mask = (1 << 0) | (1 << 1) | (1 << 3)  | (1 << 7); //sonar cube

	switch( reqMode )
	{
		case	SIO_UART:
            // Setting the Serial Control Register

            pFM3_MFS7->SCR |=   mask;   						// UART reset
            //pFM3_MFS7->SCR |=   BIT7;   						// UART reset
            // Setting the Serial Mode Register
            pFM3_MFS7->SMR &= ~(BIT5 + BIT6 + BIT7); 			// UART0 mode selected
            pFM3_MFS7->SMR &= ~(BIT4); 							// RESERVED
            pFM3_MFS7->SMR &= ~(BIT2); 							// LSB first (The least significant bit is first transferred.)

            // Setting the Serial Status Register
            pFM3_MFS7->SSR |=  (BIT7);   							// clear receive error flags

            //Setting the Extended Communication Control Register
            pFM3_MFS7->ESCR = flagESCR &~(BIT5);                // Setting the communication using the function parameter and anyway forcing the NRZ
            pFM3_MFS7->ESCR  &= ~(BIT7);                        // set flow ctrl = none


			pFM3_MFS7->BGR  = divisor;			                // schiaffa il divisore per il rate calcolato prima
            pFM3_MFS7->SMR |= flagSMR;

            pFM3_MFS7->SMR |=  (BIT0); 							// Enables a serial data output.


			break;
        // We use the channel 3 only in UART Mode
		case	SIO_CSI_MASTER_FULL:
		case	SIO_CSI_MASTER_RXONLY:
		case	SIO_CSI_SLAVE_FULL:
		case	SIO_CSI_SLAVE_RXONLY:
		case	SIO_I2C:
		default:
			sioError = SIO_ERR_INV_MODE;	// segnala la cappella sistina
			return;
	}


	if( reqXfr == SIO_XFR_INT_RX || reqXfr == SIO_XFR_INT_ALL )	// tutto ad irq (errori compresi)
	{

	//---- UART 7 TX------------------------------------
        if( pfIrqHandTx )
        {
            NVIC_ClearPendingIRQ(MFS7TX_IRQn);
            NVIC_EnableIRQ(MFS7TX_IRQn);
            NVIC_SetPriority(MFS7TX_IRQn, 14);
        }

		if( pfIrqHandRx )					// se puo' aggancia i vettori richiesti
		{
            NVIC_ClearPendingIRQ(MFS7RX_IRQn);
            NVIC_EnableIRQ(MFS7RX_IRQn);
            NVIC_SetPriority(MFS7RX_IRQn, 13);
        }

	}

}








/** Da una mano alla "sio_CfgChan" eseguendo un po' calcoli per la modalità "CSI"
* Params:
* reqChan: Channel to be configured - Not Used
* reqMode: How to use the channel
* reqRate: baudRate to calculate
* reqDataBits: Number of bit compsing the frame
* reqDirBits: Direction of the bits - who is first? Most or Less?
* reqClkMode: Clock Mode in Syncro communication
* reqXfrMode: Way to transmission - Not used
* calcDivisor: Register value for setting clock rate
* calcPrescal: Not used
* calcBitSet: Not used for SPI and syncro comunication
* calcBitCount: Register value for bit number setting. To pass to the caller
*/
static bool	calcSettingsCSI(SIOChan reqChan, SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
												SIODirBits reqDirBits, SIOClkMode reqClkMode,
												SIOXfrMode reqXfrMode, word *calcDivisor,
												word *calcPrescal, byte *calcBitset, byte *calcBitCount)
{
uint32_t ui32TabRate = 0;
uint16_t ui16Reload = 0;
uint8_t ui8FlagSMR = 0x0;			// Used for SMR register
uint8_t ui8FlagESCR = 0x00;     // Used for ESCR register
bool bIsSPIMode = false;

    //Verify if we want to use the SPI communication
    switch (reqMode)
    {
        case SIO_SPI_MASTER_FULL:
        case SIO_SPI_MASTER_ONLY_TX:
        case SIO_SPI_MASTER_ONLY_RX:
        case SIO_SPI_SLAVE_FULL:
        case SIO_SPI_SLAVE_ONLY_TX:
        case SIO_SPI_SLAVE_ONLY_RX:
            bIsSPIMode = true;
            break;
        default:
            bIsSPIMode = false;
            break;
    }

    //The reqRate value are fixed in the baudRateTab[] vector.

	switch( reqRate )				// This value comes from sio_CfgChan()
	{
		case	SIO_RATE4800:
		case	SIO_RATE9600:
		case	SIO_RATE19200:
		case	SIO_RATE38400:
		case	SIO_RATE76800:
		case	SIO_RATE115200:
			sioError = SIO_ERR_INV_RATE;	// This setting should be used only for UART communication
            break;                          // so we set it as a mistake if it happens
		case	SIO_RATE200K:
		case	SIO_RATE400K:
		case	SIO_RATE500K:
		case	SIO_RATE1MEG:
		case	SIO_RATE1_5MEG:
		case	SIO_RATE2MEG: 				// Cases to use with I2C and SPI communication
                // baudrate setting works with the following formula:
                // V = F / b - 1
                // V : Reload value;  b : Baud rate;  F: Bus clock frequency
                ui32TabRate = baudRateTab[reqRate];		// fetch the tabelled rate
                // Reload value calculation
                ui16Reload = (FM3_SPICLK / ui32TabRate) - RELOAD_VALUE_CONSTANT;
                if(( ui16Reload < CSI_MIN_RELOAD_VAL ) || (ui16Reload > MAX_RELOAD_VAL)	)
                // Set the reload value to 3 or more.
                    {
                        sioError = SIO_ERR_BASEFREQ;		// Calculated value is not suitable
                        break;
                    }
                    else
                    {
                        //Nothing to be done here
                    }
				break;
		case	SIO_RATE_EXT:				// External Clock
                //Should I Add some specific setting more?
                break;
		default:
			sioError = SIO_ERR_INV_RATE;		// Not available rate
			break;
	}
            // Analyzing the reqDataBits
            switch (reqDataBits)
            {
                case SIO_9BIT:
                    ui8FlagESCR |= BIT2; //Set Bit0=Bit1= 0, Bit2 = 1
                    break;
                case SIO_8BIT:
                    ui8FlagESCR = 0x0; //Set Bit0=Bit1=Bit2 = 0
                    break;
                case SIO_7BIT:
                    ui8FlagESCR |= (BIT0 + BIT1); //Set Bit0=Bit1= 1, Bit2 = 0
                    break;
                default:
                    //Possibilities to use 5 or 6 bit lenght are not considered
                    // althogh the H/W allowes them
                    sioError = SIO_ERR_DATABIT;
                    break;
            }

            //Analyzing the request Clock Mode
			switch( reqClkMode )
			{
				case	SIO_CLKHIGH:				// Normal Mode 1: clock high: fall edge = shift out, rise edge = shift in
					if (bIsSPIMode == true)
                        {
                            sioError = SIO_ERR_CLKMODE; //this is not congruent
                        }
                        else
                        {
                            //code to set proprerly
                        }
					break;
		  	 	case	SIO_CLKLOW:					// Normal Mode 2:clock low: rise edge = shift out, fall edge = shift in
                        if (bIsSPIMode == true)
                        {
                            sioError = SIO_ERR_CLKMODE; //this is not congruent
                        }
                        else
                        {
                            //code to set proprerly
                        }
					break;
		  	 	case	SIO_CLKHIGH_REV:			// SPI mode clock high: rise edge = shift out, fall edge = shift in
                        if (bIsSPIMode == true)
                        {
                            ui8FlagSMR |= 0x00;    //Set SMR: SCINV = 0
                        }
                        else
                        {
                            sioError = SIO_ERR_CLKMODE; //this is not congruent
                        }
					break;
		  	 	case	SIO_CLKLOW_REV:				// SPI mode clock low: fall edge = shift out, rise edge = shift in
                        if (bIsSPIMode == true)
                        {
                            ui8FlagSMR |= BIT3;    //Set SMR: SCINV = 1
                        }
                        else
                        {
                            sioError = SIO_ERR_CLKMODE; //this is not congruent
                        }
					break;
				default:
					sioError = SIO_ERR_CLKMODE;			// Not a valid choice
  	  				break;
		    }

		    switch (reqDirBits)
		    {
                case SIO_MSB_FIRST:
                    ui8FlagSMR |= BIT2;
                    break;
                case SIO_LSB_FIRST:
                    ui8FlagSMR &= ~(BIT2);
                    break;
                default:
                    sioError = SIO_ERR_DIRBIT;
                    break;

		    }

            //Returned value to the caller
            *calcBitset = ui8FlagSMR;
            *calcBitCount = ui8FlagESCR;
            *calcDivisor = ui16Reload;
            *calcPrescal = 0XFF; //Return a not usable value

#ifdef __DEBUG__

	if( sioError )							// se c'è un errore
	{
		error_trace("SIOFM3", "calcSettingsCSI", sioError);	// lo segnala senza indugi
		return(False);
	}
#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);							// vai tranquillo mario!!
}


/* Da una mano alla "sio_CfgChan" selezionando le funzioni di transfer per la modalità "CSI"
*/
static bool fillHandleXfrCSI(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pRxFun)(byte),
									void (**pPollFun)(void),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) )
{

	*pTxFun = NULL;
	*pRxFun = NULL;

        // trappa gli alias del sio chan 3
    if( (reqChan == SIO_CHAN3_0) || (reqChan == SIO_CHAN3_1) || (reqChan == SIO_CHAN3_2))
    {
        reqChan = SIO_CHAN3;
    }
	switch( reqXfrMode )
	{
		case	SIO_XFR_POLL:
			if( sioCfgTab[reqChan].pComBuf->txBuf.size == 1 )	// se c'ha un buffer piccolo piccolo
			{
				*pTxFun = TabXfrCsiPOLL[reqChan];	// si fa dare il puntatore
			}
			else
			{
				*pTxFun = TabSttCsiPOLL[reqChan];			// rimanda al poller del buffer
				*pPollFun = TabCsiPOLL[reqChan];			// si tira dietro anche il trasferitore a polling
			}
			*pRxFun = TabXfrCsiREAD[reqChan];
			break;

		case	SIO_XFR_INT_RX:
		case	SIO_XFR_INT_ALL:
			*pTxFun = TabSttCsiINT[reqChan];
			*pIrqHandTx = TabIrqHandCsiINT[reqChan];
			pIrqHandRx = pIrqHandTx;
			break;

		case	SIO_XFR_DMA_RX:
		case	SIO_XFR_DMA_ALL:
			*pTxFun = TabSttCsiDMA[reqChan];
			*pIrqHandTx = TabIrqHandCsiDMA[reqChan];	// modalità DMA ho bisogno di due canali
			pIrqHandRx = pIrqHandTx;					// uno per tx + uno per rx oppure posso
														// swicchare la direzione
			break;

		case	SIO_XFR_SOFT:							// emulazione software
			*pTxFun = TabXfrCsiSOFT[reqChan];
			//*flagCSIM &= 0x7F;							// disabilita la periferica
			break;
	}

	if( *pTxFun == NULL )								// se non esiste
		sioError = SIO_ERR_XFR_NOTIMP;						// segnala la cappella sistina

#ifdef __DEBUG__

	if( sioError )							// se c'è un errore
	{
		error_trace("SIOFM3", "fillHandleXfrCSI", sioError);	// lo segnala senza indugi
		return(False);
	}
#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);
}


/* Da una mano alla "sio_CfgChan" eseguendo un po' calcoli per la modalità "I2C"
*/
static bool	calcSettingsI2C(SIOChan reqChan, SIOMode reqMode, SIORate reqRate, byte *calcPrescal)
{
long rate;
byte m = 0x02;		// variabile 'm' copiata pari pari da manuale NEC dove fBaud= fsys / 2(m+1) * k *


	switch( reqRate )				// vediamo come va forte..
	{
		case	SIO_RATE4800:
		case	SIO_RATE9600:
		case	SIO_RATE19200:
		case	SIO_RATE38400:
		case	SIO_RATE57600:
		case	SIO_RATE76800:
		case	SIO_RATE115200:
		case	SIO_RATE230400:				// solo per uart mode

		case	SIO_RATE500K:
		case	SIO_RATE1MEG:
		case	SIO_RATE1_5MEG:
		case	SIO_RATE2MEG: 				// per CSI e  I2C mode
			sioError = SIO_ERR_INV_RATE;		// non va bene
			break;

		case	SIO_RATE200K:				// PER I2C
		case	SIO_RATE400K:
		case	SIO_RATE_EXT:				// clock esterno
			if( reqMode != SIO_I2C )		// vorrei che fosse chiaro
			{
				sioError = SIO_ERR_INV_RATE;	// non va bene
				break;
			}
/*
			rate = SIO_BASEFREQ / baudRateTab[reqRate];		// calcola un divisore approssimativo


			if( rate >= 120 )				// se deve dividere tanto
			{
				if( SIO_BASEFREQ > 4000000 )	// se il quarzo è oltre ai 4M
					m = 0x06;				// usa il filtro digitale e fxx/176
				else
					m = 0x02;				// niente filtro digitale e fxx/172
			}

			if( rate >= 60 && rate < 120 )	// se deve dividere una cosa giusta
				m = 0x02;					// niente filtro digitale e fxx/86

			if( rate >= 30 && rate < 60 )	// se deve dividere un po' meno
				m = 0x0A;					// niente filtro digitale e fxx/48

			if( rate < 30 )					// se deve dividere poco poco
				m = 0x08;					// niente filtro digitale e fxx/24

			*calcPrescal = m;				// prescaler scelto
			break;

			sioError = SIO_ERR_INV_RATE;		// non va bene
*/
			break;
	}

#ifdef __DEBUG__

	if( sioError )						// se lo vogliono
	{
		error_trace("SIOFM3", "sio_calcSettingsI2C", sioError);	// aspetta che lo vengano a prendere
		return(False);
	}

#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);							// vai tranquillo mario!!
}

/* Da una mano alla "sio_CfgChan" selezionando le funzioni di transfer per la modalità "I2C"
*/
static bool fillHandleXfrI2C(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pPollFun)(),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) )
{

	*pTxFun = NULL;

        // trappa gli alias del sio chan 3
    if( (reqChan == SIO_CHAN3_0) || (reqChan == SIO_CHAN3_1) || (reqChan == SIO_CHAN3_2))
    {
        reqChan = SIO_CHAN3;
    }
	switch( reqXfrMode )
	{
		case	SIO_XFR_POLL:
			if( sioCfgTab[reqChan].pComBuf->txBuf.size == 1 )	// se c'ha un buffer piccolo piccolo
			{
				*pTxFun = TabXfrI2cPOLL[reqChan];		// trasferisce direttamente
			}
			else
			{
				*pTxFun = TabSttI2cPOLL[reqChan];		// la funzione giusta al posto giusto
				*pPollFun = TabI2cPOLL[reqChan];
			}
			break;

		case	SIO_XFR_INT_RX:
		case	SIO_XFR_INT_ALL:
			*pTxFun = TabSttI2cINT[reqChan];				// la funzione giusta al posto giusto

			*pIrqHandTx = TabIrqHandI2cINT[reqChan];
			*pIrqHandRx = *pIrqHandTx;
			break;

		case	SIO_XFR_DMA_RX:
		case	SIO_XFR_DMA_ALL:
			*pTxFun = TabSttI2cDMA[reqChan];				// la funzione giusta al posto giusto

			*pIrqHandTx = TabIrqHandI2cDMA[reqChan];
			*pIrqHandRx = *pIrqHandTx;
			break;

		case	SIO_XFR_SOFT:							// emulazione software
			break;
	}

	if( *pTxFun == NULL )							// se non esiste
		sioError = SIO_ERR_XFR_NOTIMP;					// segnala la cappella sistina

#ifdef __DEBUG__

	if( sioError )							// se c'è un errore
	{
		error_trace("SIOFM3", "fillHandleXfrI2C", sioError);	// lo segnala senza indugi
		return(False);
	}
#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);
}



/* Da una mano alla "sio_CfgChan" eseguendo un po' calcoli per la modalità "UART"
*  Params:
*  reqChan: Parameter not used. Kept for code armonization.
*  reqMode: Operative mode of the MFS. In this case should be always UART
*  reqRate: Used to perform the right calculation of the Baud reate using the baud generator
*  reqDataBits: Used to set the lenght of communication frame
*  reqStopBits: Used to calculate the number of STOP BIT requested
*  reqParityBits: Used to calculate the parity BIT policy
*  calcDivisor: Used to pass the calculate divisor to the upcalling function
*  calcSerMode: Used to pass the SMR settings to the upcalling function
*  calcBitSet: Used to pass the ESCR settings to the upcalling function
*
*/
static bool	calcSettingsUART(SIOChan reqChan,SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
												SIOStopBits reqStopBits, SIOParityBits reqParityBits,
												word *calcDivisor, byte *calcSerMode, byte *calcBitset)
{
long tabrate = 0;
long freq = 0;
byte m = 0;		// variabile 'm' copiata pari pari da manuale NEC dove fBaud= fsys / 2(m+1) * k *
byte flagESCR = 0;
byte flagSMR = 0;


	switch( reqRate )				// vediamo come va forte..
	{
		case	SIO_RATE4800:			// meno di 4800baud fa scappare da ridere...
		case	SIO_RATE9600:
		case	SIO_RATE19200:
		case	SIO_RATE38400:
		case	SIO_RATE57600:
		case	SIO_RATE76800:
		case	SIO_RATE115200:
		case	SIO_RATE230400:				// solo per uart mode
			if( reqMode != SIO_UART )
			{
				sioError = SIO_ERR_INV_RATE;	// non va bene
				break;
			}


												// baudrate = fxclk / (k-1)
			tabrate = baudRateTab[reqRate];		// piglia il rate tabellato

												// calcola il divisore intero
			freq = (FM3_UARTCLK / tabrate) -1;	//Il valore FM3_APB2CLK        30000000 come è stato deciso?


			if( freq > 8000 )					    // se dobbiamo gestire un prescaler troppo alto. Come è stata ricavata questa soglia?
			{
				sioError = SIO_ERR_BASEFREQ;		// non va bene
				break;
			}


									// NOTA!!! volendo esagerare qui potremmo fare una stima dell'errore di
									// baudrate cosi' ottenuto ed eventualmente verificare con un punto
									// in + o in - sul divisore con quale configurazione si ottiene l'errore
									// + basso.........

			*calcDivisor = freq;				// imposta il divisore calcolato
									// configurazione bitset
			flagESCR = 0;						// partiamo senza bits
			flagSMR = 0;						// partiamo senza bits

//Preparing the request for setting ESCR Register
			if( reqDataBits == SIO_7BIT )		// if we want to use 7 Bits
				flagESCR |= (BIT0 + BIT1);      // mask to request 7 bits xxxxx011
			else
				if( reqDataBits != SIO_8BIT )	// se non si accontentano
					sioError = SIO_ERR_DATABIT;		// non va bene

			if( reqStopBits == SIO_2STOP )		// se dobbiamo sparare 2 stop bits
				{
				flagESCR = ~(BIT6);
				flagSMR |= (BIT3);              // Preparing the SMR Register
				}
			else
				if( reqStopBits != SIO_1STOP )	// se non si accontentano
					sioError = SIO_ERR_STOPBIT;		// non va bene


			switch( reqParityBits )
			{
				case	SIO_NOPARITY:			// niente controllo parità
					break;						// già fatto !!

			  	case	SIO_ODDPARITY:			// parità dispari
					flagESCR |= (BIT3);
					flagESCR |= (BIT4);         //Enable the Parity Bit
					break;

			  	case	SIO_EVENPARITY:			// controllo su parità pari
					flagESCR &= ~(BIT3);
					flagESCR |= (BIT4);         //Enable the Parity Bit
					break;

			  	case	SIO_SPACEPARITY:		// niente controllo su parità ma space aggiunto ad ogni carattere
					flagESCR |= (BIT3);
					flagESCR |= (BIT4);         //Enable the Parity Bit
					break;

			  	default:
			  		sioError = SIO_ERR_INV_PARITY;	// non è molto bello..
			  		break;
			}

			*calcBitset = flagESCR;				// imposta bitset corrispondente
			*calcSerMode = flagSMR;
			break;

		case	SIO_RATE200K:
		case	SIO_RATE400K:
		case	SIO_RATE500K:
		case	SIO_RATE1MEG:
		case	SIO_RATE1_5MEG:
		case	SIO_RATE2MEG: 				// per CSI e  I2C mode
		case	SIO_RATE_EXT:				// clock esterno
			sioError = SIO_ERR_INV_RATE;		// non va bene
			break;
	}

#ifdef __DEBUG__

	if( sioError )						// se lo vogliono
	{
		error_trace("SIOFM3", "sio_calcSettingsUART", sioError);	// segnala dov'è la magagna
		return(False);
	}
#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);							// vai tranquillo mario!!
}

/* Da una mano alla "sio_CfgChan" selezionando le funzioni di transfer per la modalità "UART"
* Params:
* reqChan: Logical channel to set
* reqXfrMode: Transfr Mode
* pTxFun:
* pPollFun:
* pIrqHandTx:
* pIrqHandRx:
*/
static bool fillHandleXfrUART(SIOChan reqChan, SIOXfrMode reqXfrMode,
									void (**pTxFun)(byte), void (**pPollFun)(void),
									void (**pIrqHandTx)(int), void (**pIrqHandRx)(int) )
{


	*pTxFun = NULL;

        // trappa gli alias del sio chan 3
    if( (reqChan == SIO_CHAN3_0) || (reqChan == SIO_CHAN3_1) || (reqChan == SIO_CHAN3_2))
    {
        reqChan = SIO_CHAN3;
    }
	switch( reqXfrMode )
	{
		case	SIO_XFR_POLL:
			*pTxFun = TabSttUartPOLL[reqChan];			// la funzione giusta al posto giusto
			*pPollFun = TabUartPOLL[reqChan];			// come si fa senza ???
			break;

		case	SIO_XFR_INT_RX:
		case	SIO_XFR_INT_ALL:
			*pTxFun = TabSttUartINT[reqChan];			// la funzione giusta al posto giusto

			*pIrqHandTx = TabIrqHandUartINT[reqChan];
			*pIrqHandRx = *pIrqHandTx;
			break;

		case	SIO_XFR_DMA_RX:
		case	SIO_XFR_DMA_ALL:
			*pTxFun = TabSttUartDMA[reqChan];			// la funzione giusta al posto giusto

			*pIrqHandTx = TabIrqHandUartDMA[reqChan];
			*pIrqHandRx = *pIrqHandTx;
			break;

		case	SIO_XFR_SOFT:							// emulazione software
			break;
	}

	if( *pTxFun == NULL )								// se non esiste
		sioError = SIO_ERR_XFR_NOTIMP;						// segnala la cappella sistina

#ifdef __DEBUG__

	if( sioError )							// se c'è un errore
	{
		error_trace("SIOFM3", "fillHandleXfrUART", sioError);	// lo segnala senza indugi
		return(False);
	}
#else

	if( sioError )							// se lo vogliono
		return(False);
#endif

	return(True);
}


/////////////////////////////////////////////////////////////////////////
//
// Blocco funzioni in-out per modalità CSI
//
/////////////////////////////////////////////////////////////////////////

/* Polla la CSI per vedere se ci sono dati da trasferire
*/
static void sioCsiPOLLCh1(void)
{
byte c;
/*
	if( _CSIF1 && _CSPR10 )			// se ha terminato un transfer
	{
		ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, SIOE1);	// salva quello ricevuto
		_CSIF1 = 0;						// reset flag trasferimento ultimato

										// se deve inviare un altro carattere
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
			SOTB1 = c;					// lo spara di brutto
		else
			_CSPR10 = 0;				// reset flag abilitazione al transfer
	}
	*/
	uint16_t ui16ByteToRead = 0;
	uint16_t ui16ByteToTransfer = 0;
	byte ui8ByteToPass = 0;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;
    bool bTempTxFail = false; //debug variable

    pFM3_MFSx = FM3_MFS4_CSIO;

    if (pFM3_MFSx->SCR_f.TXE  == ON)   //If the TX enabled
    {
            if (pFM3_MFSx->SSR_f.RDRF == ON)   //If the RX Data Register is full
                {
                    ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, pFM3_MFSx->RDR);

                    if(ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
                        pFM3_MFSx->TDR = c;    //Write in the register
                    else
                        pFM3_MFSx->SCR_f.TXE = SET_BIT_LOW;
                }
            else
                {
                    //There are no data in the RX Data Register
                }
        }
    else
        {
            //Tranfer not running
        }
}



/* Avvia il trasferimento in modalità polling
*/
static void sioSttCsiPOLLCh1(byte c)
{
/*
	if( _CSPR10 == 0 )					// se il transfer non è attivo
	{
		_CSIF1 = 0;						// reset flag trasferimento ultimato

		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
		{
			SOTB1 = c;						// spara il nuovo dato
			_CSPR10 = 1;					// abilita la comunicazione
		}
	}
	*/
	uint16_t ui16ByteToTransfer = 0;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;
    bool bTempTxFail = false; //debug variable

    pFM3_MFSx = FM3_MFS4_CSIO;

    if( pFM3_MFSx->SCR_f.TXE == OFF )   // se il transfer non è attivo
    {
	//Start the transfer

         if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
            {
                pFM3_MFSx->SCR_f.TXE = 1;   // abilita il trasmettitore
              //Fetching the data
              ui16ByteToTransfer = c;                 //Put in a 16 bit var
              pFM3_MFSx->TDR = ui16ByteToTransfer;    //Write in the register
            }
        else
            {
              bTempTxFail = true;//Fetchin Data Not possible
            }
    }
    else
    {
          bTempTxFail = true;//Tx not available
    }

return;
}

/* Avvia il trasferimento in modalità polling
*/
static void sioSttCsiPOLLCh2(byte c)
{
    //PAY ATTENTION
    //This setting here is valid if we use MFS4 and SIO_CHAN2
	uint16_t ui16ByteToTransfer = 0;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;
    bool bTempTxFail = false; //debug variable

    pFM3_MFSx = FM3_MFS4_CSIO;    //PAY ATTENTION. Flexiper protect uses MFS4

    if( pFM3_MFSx->ESCR_f.RESERVED1 == OFF )   // se il transfer non è attivo
    {
	//Start the transfer
    //PAY ATTENTION. Flexiper protect uses SIO_CHAN2

            if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is empty
            {
                if( ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == True )
                {
                    //Fetching the data
                    ui16ByteToTransfer = c;                 //Put in a 16 bit var
                    pFM3_MFSx->TDR = ui16ByteToTransfer;    //Write in the register
                }
                else
                {
                    bTempTxFail = true;//Fetchin Data Not possible
                }
            }
            pFM3_MFSx->ESCR_f.RESERVED1 = ON;     //  TX running
    }
    else
    {
          bTempTxFail = true;//Tx not available
    }

return;
}


/* Polla la CSI per vedere se ci sono dati da trasferire
*/
static void sioCsiPOLLCh2(void)
{
byte c;
/*
	if( _CSIF1 && _CSPR10 )			// se ha terminato un transfer
	{
		ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, SIOE1);	// salva quello ricevuto
		_CSIF1 = 0;						// reset flag trasferimento ultimato

										// se deve inviare un altro carattere
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
			SOTB1 = c;					// lo spara di brutto
		else
			_CSPR10 = 0;				// reset flag abilitazione al transfer
	}
	*/
	uint16_t ui16ByteToRead = 0;
	uint16_t ui16ByteToTransfer = 0;
	byte ui8ByteToPass = 0;
	stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
    stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;
    pSCRReg = &FM3_MFS4_CSIO->SCR_f;
    pSSRReg = &FM3_MFS4_CSIO->SSR_f;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = FM3_MFS4_CSIO;

	//Verify if interrupt has been corretcly routed
    bool bTempTxFail = false; //debug variable

    pFM3_MFSx = FM3_MFS4_CSIO;

    if (pSCRReg->RIE == INT_DISABLE){

        if (pFM3_MFSx->SSR_f.RDRF == ON)   //If the RX Data Register is full
        {
            ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, pFM3_MFSx->RDR);

            provaPollCounter_Put++;
        }
        }

    if (pFM3_MFSx->ESCR_f.RESERVED1  == ON)   //If the TX enabled
    {


            if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is empty
            {
                if(ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == True )
                    {
                        pFM3_MFSx->TDR = c;    //Write in the register
                        provaPollCounter_Get++;
                    }
                else
                    {
                        pFM3_MFSx->ESCR_f.RESERVED1 = OFF;
                    }
            }
                    //There are no data in the RX Data Register
//                    provaTestFlagNotRcv++;

        }
    else
        {
            //Tranfer not running
            provaTestTxNotEnabled++;
        }
}




/* Avvia il trasferimento in modalità interrupt
*/
static void sioSttCsiINTCh0(byte c)
{

//	if( _CSMK0 == 1 )			// se l'irq non è abilitato
//	{
//		if( _CSIF0 )			// se aveva già terminato una comunicazione
//			_CSIF0 = 0;			// lo azzera per evitare di avere irq subito
//
//								// piglia il primo carattere del buffer da spedire
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
//		{
//			SOTB0 = c;			// lo spara di brutto
//			_CSMK0 = 0;			// abilita irq
//		}
//	}
}


/* Avvia il trasferimento in modalità interrupt
*/
static void sioSttCsiINTCh1(byte c)
{
//	if( _CSMK1 == 1 )			// se l'irq non è abilitato
//	{
//		if( _CSIF1 )			// se aveva già terminato una comunicazione
//			_CSIF1 = 0;			// lo azzera per evitare di avere irq subito
//
//								// piglia il primo carattere del buffer da spedire
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
//		{
//			_CSMK1 = 0;			// abilita irq
//			SOTB1 = c;			// lo spara di brutto
//		}
//	}
}

static void sioSttCsiINTCh2(byte c)
{
   //PAY ATTENTION
    //This setting here is valid if we use MFS4 and SIO_CHAN2
	uint16_t ui16ByteToTransfer = 0;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;
    bool bTempTxFail = false; //debug variable

    pFM3_MFSx = FM3_MFS4_CSIO;    //PAY ATTENTION. Flexiper protect uses MFS4

    if( pFM3_MFSx->ESCR_f.RESERVED1 == OFF )   // se il transfer non è attivo
    {
	//Start the transfer
    //PAY ATTENTION. Flexiper protect uses SIO_CHAN2

            if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is empty
            {
                if( ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == True )
                {
                    //Fetching the data
                    ui16ByteToTransfer = c;                 //Put in a 16 bit var
                    pFM3_MFSx->TDR = ui16ByteToTransfer;    //Write in the register
                }
                else
                {
                    bTempTxFail = true;//Fetchin Data Not possible
                }
            }
            pFM3_MFSx->ESCR_f.RESERVED1 = ON;     //  TX running
            pFM3_MFSx->SCR_f.TIE = SET_BIT_HIGH;
    }
    else
    {
          bTempTxFail = true;//Tx not available
    }

return;
}

static void sioSttCsiINTCh3(byte c)
{
			//.....
			// non ancora implementata
}

static void sioSttCsiINTCh4(byte c)
{
			//.....
			// non ancora implementata
}


/* Avvia il trasferimento in modalità DMA
*/
static void sioSttCsiDMACh0(byte c)
{
//ComRingBuf *pComBuf;
//byte *pd, *pr;
//
//	pComBuf = sioCfgTab[SIO_CHAN0].pComBuf;
//
//	if( pComBuf->txBuf.count == 1 )			// se devo spedire un solo char
//	{
//		sioXfrCsiPOLLCh0(c);				// lo fa sparare in polling (che fa prima)
//	}
//	else
//	{
//		pd = pComBuf->txBuf.pData;
//		pr = (byte *) 0xFFFFB000;
//
//		if( pd < pr )		// se non è in ram (brutto affare...)
//		{
//			do{
//
//				sioXfrCsiPOLLCh0(*pComBuf->txBuf.pData++);	// lo fa sparare in polling (che fa prima)
//				pComBuf->txBuf.count--;
//
//			}while( pComBuf->txBuf.count );				// aspetta e spera
//		}
//		else
//		{
//			if( c == 0 )						// output mode
//			{
//				// DIOA1 = &SIO0;						// punta al registro CSI0
//
//				do{
//
//					//DRA1 = &pComBuf->txBuf.pData;	// punta al buffer passato (DMA Ram address
//					DCHC1 &= 0x80;					// reset transfer prec.
//
//					if( pComBuf->txBuf.count > 0xFF )
//					{
//						DBC1 = 0xFF;
//						pComBuf->txBuf.count -= 0xFF;
//						pComBuf->txBuf.pData += 0xFF;
//					}
//					else
//					{
//						DBC1 = 0xFF;
//						pComBuf->txBuf.count = 0;
//					}
//
//					DCHC1 |= 0x01;						// abilita il transfer
//
//					while( (DCHC1 & 0x80) == 0 );		// aspetta che termini
//
//				}while( pComBuf->txBuf.count );			// aspetta e spera
//
//				DCHC1 &= 0xFE;							// disabilita il transfer
//			}
//		}
//	}
}


// Gestore servizio Irq per sio in modalità CSI con trasferimento a DMA
static void sioIrqHandCsiDMACh0(int vn)
{
}

/**
* Reading the RDR Register of the SioChannel 1
* Used for Sync communication in polling mode for the moment.
*/
static void sioXfrCsiREADCh1(byte c)
{
uint16_t ui16ByteToTransfer = 0;
byte ui8ByteToPass = 0;
FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;

    pFM3_MFSx = FM3_MFS4_CSIO;
    if (pFM3_MFSx->SSR_f.RDRF == ON)        //Rx Register is full
        {
        ui16ByteToTransfer = pFM3_MFSx->RDR; // Reading From RDR Register
        ui16ByteToTransfer << 8;    //Verifiy this statement in debug
        ui8ByteToPass = ui16ByteToTransfer;
        ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, ui8ByteToPass);
        }
    else
        {
         //Register empty - Nothing to do
        }
return;
}

/**
* Reading the RDR Register of the SioChannel 2
* Used for Sync communication in polling mode for the moment.
*/
static void sioXfrCsiREADCh2(byte c)
{
uint16_t ui16ByteToTransfer = 0;
byte ui8ByteToPass = 0;
FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;

    pFM3_MFSx = FM3_MFS4_CSIO;   //Channel 2 is associated to MFS4 in the protective micro
    if (pFM3_MFSx->SSR_f.RDRF == ON)        //Rx Register is full
        {
        ui16ByteToTransfer = pFM3_MFSx->RDR; // Reading From RDR Register
        ui16ByteToTransfer << 8; //Verifiy this statement in debug
        ui8ByteToPass = ui16ByteToTransfer;
        ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, ui8ByteToPass);
        }
    else
        {
         //Register empty - Nothing to do
        }
return;
}


/* Esegue il trasferimento in modalità polling quando il buffer è ad un solo byte
*/
static void sioXfrCsiPOLLCh1(byte c)
{
	uint16_t ui16ByteToTransfer = 0;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = NULL;

    pFM3_MFSx = FM3_MFS4_CSIO;
	ui16ByteToTransfer = c;                 //Put in a 16 bit var

    if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is free
        {
            pFM3_MFSx->TDR = ui16ByteToTransfer;    //Write in the register
            while(pFM3_MFSx->SSR_f.TDRE == OFF);    //Wait the tx data is shifted out from the register
            //we put here the reading of the register
            sioXfrCsiREADCh1(c);
        }
    else
        {
            //Register not free??? Why??
        }


	//SOTB1 = c;							// spara il nuovo dato
//
//	while( !_CSIF1 );					// aspetta che esca tutto
//
//	c = SIOE1;							// piglia il dato ricevuto
//
//	_CSIF1 = 0;							// must reset !!
//										// fa memorizzare il dato ricevuto
//	ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, c);
return;
}


/* Esegue il trasferimento in modalità software
*/
static void sioXfrCsiSOFTCh0(byte c)
{
//int n;
//int value = c;
//int inval;
//int bitcnt;
//int bitmsb;
//
//
//	_SCLK0 = 0;							// clock basso
//
//	inval = 0;							// reset input value
//										// piglia il bit count richiesto
//	bitcnt = sioCfgTab[SIO_CHAN0].csiSoftBitCnt;
//										// piglia la maskera bit msb
//	bitmsb = sioCfgTab[SIO_CHAN0].csiSoftMsbMask;
//
//	if( bitcnt > 8 )					// se deve shiftare + di 8 bits
//	{
//		ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c);		// piglia la parte alta
//		value = c;
//		value <<= 8;
//
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == False )
//			c = 0;						// azzera la parte bassa
//
//		value |= c;						// somma la parte bassa
//	}
//
//    if( value & bitmsb )
//        _SDOUT0 = 1;           			// spara il dato
//    else
//        _SDOUT0 = 0;
//
//	for(n=0; n<bitcnt; n++)
//	{
//        _SCLK0 = 1;     					// clock alto= spara il dato
//
//        value <<= 1;					// fa quello che deve fare per
//        inval <<= 1;					// perdere tempo
//
//        _SCLK0 = 0;     					// clock basso = legge il dato
//
//		if( _SDIN0 )						// se il pin è alto
//			if( _SDIN0 )					// solo per essere sicuri
//            	inval |= 0x01;      	// campiona il dato in ingresso
//
//	    if( value & bitmsb )			// test sull'msb da sparare
//	        _SDOUT0 = 1;           		// fuori il nuovo dato
//	    else
//	        _SDOUT0 = 0;
//    }
//										// fa memorizzare il dato ricevuto
//	if( bitcnt > 8 )					// se ha shiftato + di 8 bits
//		ringbuf_putc(&sioCfgTab[SIO_CHAN0].pComBuf->rxBuf, inval >> 8);	// incamera la parte alta
//
//										// registra il valore letto
//	ringbuf_putc(&sioCfgTab[SIO_CHAN0].pComBuf->rxBuf, inval);
}


/* Esegue il trasferimento in modalità software
*/
static void sioXfrCsiSOFTCh1(byte c)
{
//int n;
//int value = c;
//int inval;
//int bitcnt;
//int bitmsb;
//
//
//	_SCLK1 = 0;							// clock basso
//
//	inval = 0;							// reset input value
//										// piglia il bit count richiesto
//	bitcnt = sioCfgTab[SIO_CHAN1].csiSoftBitCnt;
//										// piglia la maskera bit msb
//	bitmsb = sioCfgTab[SIO_CHAN1].csiSoftMsbMask;
//
//	if( bitcnt > 8 )					// se deve shiftare + di 8 bits
//	{
//		ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c);		// piglia la parte alta
//		value = c;
//		value <<= 8;
//
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == False )
//			c = 0;						// azzera la parte bassa
//
//		value |= c;						// somma la parte bassa
//	}
//
//    if( value & bitmsb )
//        _SDOUT1 = 1;           			// spara il dato
//    else
//        _SDOUT1 = 0;
//
//	for(n=0; n<bitcnt; n++)
//	{
//
//        _SCLK1 = 1;     				// clock alto= spara il dato
//
//        value <<= 1;					// fa quello che deve fare per
//        inval <<= 1;					// perdere tempo
//
//        _SCLK1 = 0;     				// clock basso = legge il dato
//
//		if( _SDIN1 )						// se il pin è alto
//			if( _SDIN1 )					// solo per essere sicuri
//            	inval |= 0x01;      	// campiona il dato in ingresso
//
//	    if( value & bitmsb )			// test sull'msb da sparare
//	        _SDOUT1 = 1;           		// fuori il nuovo dato
//	    else
//	        _SDOUT1 = 0;
//    }
//										// fa memorizzare il dato ricevuto
//	if( bitcnt > 8 )					// se ha shiftato + di 8 bits
//		ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, inval >> 8);	// incamera la parte alta
//
//										// registra il valore letto
//	ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, inval);
}

/* Esegue il trasferimento in modalità software
*/
static void sioXfrCsiSOFTCh2(byte c)
{
//int n;
//int value = c;
//int inval;
//int bitcnt;
//int bitmsb;
//
//
//	_SCLK2 = 0;							// clock basso
//
//	inval = 0;							// reset input value
//										// piglia il bit count richiesto
//	bitcnt = sioCfgTab[SIO_CHAN2].csiSoftBitCnt;
//										// piglia la maskera bit msb
//	bitmsb = sioCfgTab[SIO_CHAN2].csiSoftMsbMask;
//
//	if( bitcnt > 8 )					// se deve shiftare + di 8 bits
//	{
//		ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c);		// piglia la parte alta
//		value = c;
//		value <<= 8;
//
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == False )
//			c = 0;						// azzera la parte bassa
//
//		value |= c;						// somma la parte bassa
//	}
//
//    if( value & bitmsb )
//        _SDOUT2 = 1;           			// spara il dato
//    else
//        _SDOUT2 = 0;
//
//	for(n=0; n<bitcnt; n++)
//	{
//
//        _SCLK2 = 1;     					// clock alto= spara il dato
//
//        value <<= 1;					// fa quello che deve fare per
//        inval <<= 1;					// perdere tempo
//
//        _SCLK2 = 0;     					// clock basso = legge il dato
//
//		if( _SDIN2 )						// se il pin è alto
//			if( _SDIN2 )					// solo per essere sicuri
//            	inval |= 0x01;      	// campiona il dato in ingresso
//
//	    if( value & bitmsb )			// test sull'msb da sparare
//	        _SDOUT2 = 1;           		// fuori il nuovo dato
//	    else
//	        _SDOUT2 = 0;
//    }
//										// fa memorizzare il dato ricevuto
//	if( bitcnt > 8 )					// se ha shiftato + di 8 bits
//		ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, inval >> 8);	// incamera la parte alta
//
//										// registra il valore letto
//	ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, inval);
}

/* Esegue il trasferimento in modalità software
*/
static void sioXfrCsiSOFTCh3(byte c)
{
			//.....
			// non ancora pervenuta...
}

/* Esegue il trasferimento in modalità software
*/
static void sioXfrCsiSOFTCh4(byte c)
{
//int n;
//int value = c;
//int inval;
//int bitcnt;
//int bitmsb;
//
//	_SCLK4 = 0;							// clock basso
//
//	inval = 0;							// reset input value
//
//										// piglia il bit count richiesto
//	bitcnt = sioCfgTab[SIO_CHAN4].csiSoftBitCnt;
//										// piglia la maskera bit msb
//	bitmsb = sioCfgTab[SIO_CHAN4].csiSoftMsbMask;
//
//	if( bitcnt > 8 )					// se deve shiftare + di 8 bits
//	{
//		ringbuf_getc(&sioCfgTab[SIO_CHAN4].pComBuf->txBuf, &c);		// piglia la parte alta
//		value = c;
//		value <<= 8;
//
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN4].pComBuf->txBuf, &c) == False )
//			c = 0;						// azzera la parte bassa
//
//		value |= c;						// somma la parte bassa
//	}
//
//    if( value & bitmsb )			// test sull'msb da sparare
//        _SDOUT4 = 1;           		// fuori il nuovo dato
//    else
//        _SDOUT4 = 0;
//
//	n = 0;
//
//	do
//	{
//		_SCLK4 = 1;     					// clock alto= spara il dato
//
//	    if( value & bitmsb )				// test sull'msb da sparare
//	        _SDOUT4 = 1;           			// fuori il nuovo dato
//	    else
//	        _SDOUT4 = 0;
//
//        value <<= 1;						// fa quello che deve fare per
//        inval <<= 1;						// perdere tempo
//
//		if( _SDIN4 )						// se il pin è alto
//			if( _SDIN4 )					// solo per essere sicuri
//            	inval |= 0x01;      		// campiona il dato in ingresso
//
//	    _SCLK4 = 0;     					// clock basso = legge il dato
//
//	    if( value & bitmsb )				// test sull'msb da sparare
//	        _SDOUT4 = 1;           			// fuori il nuovo dato
//	    else
//	        _SDOUT4 = 0;
//
//		if( _SDIN4 && (inval & 0x01) )		// se il pin è alto
//        	inval |= 0x01;      			// campiona il dato in ingresso
//
//		n++;
//
//    }while( n < bitcnt );
//											// fa memorizzare il dato ricevuto
//	if( bitcnt > 8 )						// se ha shiftato + di 8 bits
//		ringbuf_putc(&sioCfgTab[SIO_CHAN4].pComBuf->rxBuf, inval >> 8);	// incamera la parte alta
//
//											// registra il valore letto
//	ringbuf_putc(&sioCfgTab[SIO_CHAN4].pComBuf->rxBuf, inval);
}




// Gestore servizio Irq per sio in modalità CSI con trasferimento ad interrupt
static void sioIrqHandCsiINTCh0(int vn)
{
byte c;

//	ringbuf_putc(&sioCfgTab[SIO_CHAN0].pComBuf->rxBuf, SIO0);		// salva il carattere ricevuto
//																	// se deve spedirne un'altro
//	if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
//		SIO0 = c;													// lo spedisce
//	else
//		_CSMK0 = 1;												// reset irq enable
}

// Gestore servizio Irq per sio in modalità CSI con trasferimento ad interrupt
static void sioIrqHandCsiINTCh1(int vn)
{
byte c;

//	ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, SIOE1);		// salva il carattere ricevuto
//																	// se deve spedirne un'altro
//	if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
//		SOTB1 = c;													// lo spedisce
//	else
//		_CSMK1 = 1;												// reset irq enable
}

// Gestore servizio Irq per sio in modalità CSI con trasferimento ad interrupt
static void sioIrqHandCsiINTCh2(int vn)
{
byte c;

//	ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, SIOE2);		// salva il carattere ricevuto
//																	// se deve spedirne un'altro
//	if( ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == True )
//		SOTB2 = c;													// lo spedisce
//	else
//		_CSMK2 = 1;													// reset irq enable
}



/////////////////////////////////////////////////////////////////////////
//
// Blocco funzioni in-out per modalità I2C
//
/////////////////////////////////////////////////////////////////////////

static void sioI2cPOLLCh0()
{

	//.....
	// non ancora implementata

}

static void sioI2cPOLLCh2()
{

	//.....
	// non ancora implementata

}




static void sioSttI2cPOLLCh0(byte c)
{

	//.....
	// non ancora implementata

}

static void sioSttI2cPOLLCh2(byte c)
{

	//.....
	// non ancora implementata

}



static void sioXfrI2cPOLLCh0(byte c)
{

	//.....
	// non ancora implementata

}

static void sioXfrI2cPOLLCh2(byte c)
{

	//.....
	// non ancora implementata

}




static void sioSttI2cINTCh0(byte c)
{

	//.....
	// non ancora implementata

}


static void sioSttI2cINTCh2(byte c)
{

			//.....
			// non ancora implementata

}



static void sioSttI2cDMACh0(byte c)
{

	//.....
	// non ancora implementata

}

static void sioSttI2cDMACh2(byte c)
{

			//.....
			// non ancora implementata

}

static void sioIrqHandI2cINTCh0(int vn)
{
	//.....
	// non ancora implementata
}

static void sioIrqHandI2cINTCh2(int vn)
{
	//.....
	// non ancora implementata
}


/////////////////////////////////////////////////////////////////////////
//
// Blocco funzioni in-out per modalità UART
//
/////////////////////////////////////////////////////////////////////////


/* Polla la UART0 per vedere se ci sono dati da trasferire
*/
static void sioUartPOLLCh0(void)
{
//byte c;
//
//	if( _SRIF0 )			// se è arrivato un carattere
//	{
//		_SRIF0 = 0;
//		ringbuf_putc(&sioCfgTab[SIO_CHAN0].pComBuf->rxBuf, RXB0);
//	}
//
//
//	if( _STPR00 == 1 )					// se il transfer è attivo
//	{
//		if( _STIF0 )					// se il buffer è vuoto
//		{
//			_STIF0 = 0;
//
//			if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
//				TXB0 = c;
//			else
//				_STPR10 = 0;			// fine dei giochi
//		}
//	}
}

/* Polla la UART1 per vedere se ci sono dati da trasferire
*/
static void sioUartPOLLCh1(void)
{
//byte c;
//
//	if( _SRIF1 )			// se è arrivato un carattere
//	{
//		_SRIF1 = 0;
//		ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, RXB1);
//	}
//
//
//	if( _STPR10 == 1 )					// se il transfer è attivo
//	{
//		if( _STIF1 )					// se il buffer è vuoto
//		{
//			_STIF1 = 0;
//
//			if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
//				TXB1 = c;
//			else
//				_STPR10 = 0;			// fine dei giochi
//		}
//	}
}

/* Polla la UART2 per vedere se ci sono dati da trasferire
*/
static void sioUartPOLLCh3(void)
{
//byte c;
//
//	if( _SRIF2 )			// se è arrivato un carattere
//	{
//		_SRIF2 = 0;
//		ringbuf_putc(&sioCfgTab[SIO_CHAN3].pComBuf->rxBuf, RXB2);
//	}
//
//
//	if( _STPR20 )						// se il transfer è attivo
//	{
//		if( _STIF2 )					// se il buffer è vuoto
//		{
//			_STIF2 = 0;
//
//			if( ringbuf_getc(&sioCfgTab[SIO_CHAN3].pComBuf->txBuf, &c) == True )
//				TXB2 = c;
//			else
//				_STPR20 = 0;			// fine dei giochi
//		}
//	}

}


static void sioSttUartPOLLCh0(byte c)
{
//	if( _STPR00 == 0 )					// se il transfer non è attivo
//	{
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
//		{
//			TXB0 = c;					// spara il nuovo dato
//			_STPR00 = 1;				// abilita la comunicazione
//		}
//	}
}

static void sioSttUartPOLLCh1(byte c)
{
//	if( _STPR10 == 0 )					// se il transfer non è attivo
//	{
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
//		{
//			TXB1 = c;					// spara il nuovo dato
//			_STPR10 = 1;				// abilita la comunicazione
//		}
//	}
}

static void sioSttUartPOLLCh3(byte c)
{
//	if( _STPR20 == 0 )						// se il transfer non è attivo
//	{
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN3].pComBuf->txBuf, &c) == True )
//		{
//			TXB2 = c;						// spara il nuovo dato
//			_STPR20 = 1;					// abilita la comunicazione
//		}
//	}
}

/**
* This function prepare the registers and fetch the data to be transmitted
* In order to serve the IRQ for channel 0
*/
static void sioSttUartINTCh0(byte c)
{
	if( pFM3_MFS0->SCR_f.TIE == INT_DISABLE )			// Verify if TX INT reqeust is disable/enable
	{
		if( pFM3_MFS0->SSR_f.TBI == SET_BIT_HIGH )
			pFM3_MFS0->SSR_f.TBI = SET_BIT_LOW;			// lo azzera per evitare di avere irq subito

        // piglia il primo carattere del buffer da spedire
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
		{
			if (pFM3_MFS0->SSR_f.TDRE == SET_BIT_HIGH)
                {
                // The TDR should be empty
                    pFM3_MFS0->TDR = c;			// Load data in TDR
                    pFM3_MFS0->SCR_f.TIE = INT_ENABLE;			// abilita irq
                }
                else
                {
                    // ERROR CODE TO IMPLEMENT
                }
		}
	}
}

/**
* This function prepare the registers and fetch the data to be transmitted
* In order to serve the IRQ for channel 3
*/
static void sioSttUartINTCh3(byte c)
{
	if( pFM3_MFS1->SCR_f.TIE == INT_DISABLE )			// Verify if TX INT reqeust is disable/enable
	{
		if( pFM3_MFS1->SSR_f.TBI == SET_BIT_HIGH )
			pFM3_MFS1->SSR_f.TBI = SET_BIT_LOW;			// lo azzera per evitare di avere irq subito

        // piglia il primo carattere del buffer da spedire
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN3].pComBuf->txBuf, &c) == True )
		{
			if (pFM3_MFS1->SSR_f.TDRE == SET_BIT_HIGH)
                {
                // The TDR should be empty
                    pFM3_MFS1->TDR = c;			// Load data in TDR
                    pFM3_MFS1->SCR_f.TIE = INT_ENABLE;			// abilita irq
                }
                else
                {
                    // ERROR CODE TO IMPLEMENT
                }
		}
	}
}


/**
* This function prepare the registers and fetch the data to be transmitted
* In order to serve the IRQ for channel 5
*/
static void sioSttUartINTCh5(byte c)
{
	if( pFM3_MFS3->SCR_f.TIE == INT_DISABLE )			// Verify if TX INT reqeust is disable/enable
	{
		if( pFM3_MFS3->SSR_f.TBI == SET_BIT_HIGH )
			pFM3_MFS3->SSR_f.TBI = SET_BIT_LOW;			// lo azzera per evitare di avere irq subito

        // piglia il primo carattere del buffer da spedire
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN5].pComBuf->txBuf, &c) == True )
		{
			if (pFM3_MFS3->SSR_f.TDRE == SET_BIT_HIGH)
                {
                // The TDR should be empty
                    pFM3_MFS3->TDR = c;			// Load data in TDR
                    pFM3_MFS3->SCR_f.TIE = INT_ENABLE;			// abilita irq
                }
                else
                {
                    // ERROR CODE TO IMPLEMENT
                }
		}
	}
}


/**
* This function prepare the registers and fetch the data to be transmitted
* In order to serve the IRQ for channel 6
*/
static void sioSttUartINTCh6(byte c)
{
	if( pFM3_MFS7->SCR_f.TIE == INT_DISABLE )			// Verify if TX INT reqeust is disable/enable
	{
		if( pFM3_MFS7->SSR_f.TBI == SET_BIT_HIGH )
			pFM3_MFS7->SSR_f.TBI = SET_BIT_LOW;			// lo azzera per evitare di avere irq subito

        // piglia il primo carattere del buffer da spedire
		if( ringbuf_getc(&sioCfgTab[SIO_CHAN6].pComBuf->txBuf, &c) == True )
		{
			if (pFM3_MFS7->SSR_f.TDRE == SET_BIT_HIGH)
                {
                // The TDR should be empty
                    pFM3_MFS7->TDR = c;			// Load data in TDR
                    pFM3_MFS7->SCR_f.TIE = INT_ENABLE;			// abilita irq
                }
                else
                {
                    // ERROR CODE TO IMPLEMENT
                }
		}
	}
}


static void sioSttUartDMACh0(byte c)
{

}

static void sioSttUartDMACh1(byte c)
{

}

static void sioSttUartDMACh3(byte c)
{

}


// Gestore servizio Irq per sio in modalità UART con per trasferimento ad interrupt
// NOTA: essendo la uart utilizzata principalmente in modalità half-duplex posso
// gestirla senza troppo appesantirla con un solo handler
static void sioIrqHandUartINTCh0(int vn)
{
byte c;

// solo per controllo errori event. in futuro
//	if( _CSMK1 == 0 )			// se la ricezione è abilitata
//	if( _STMK0 == 0 )			// se l'irq è abilitato


}

// Gestore servizio Irq per sio in modalità UART con per trasferimento ad interrupt
// NOTA: essendo la uart utilizzata principalmente in modalità half-duplex posso
// gestirla senza troppo appesantirla con un solo handler
static void sioIrqHandUartINTCh3(int vn)
{
byte c;

// solo per controllo errori event. in futuro
//	if( _CSMK1 == 0 )			// se la ricezione è abilitata
//	if( _STMK0 == 0 )			// se l'irq è abilitato

//	if( vn == IRQ_VEC_INTSR1 )				// se è arrivato un carattere (e l'irq è abilitato)
//	{
//		c = RXB1;				// lo piglia
//
//		ringbuf_putc(&sioCfgTab[SIO_CHAN1].pComBuf->rxBuf, c);
//	}
//
//	if( vn == IRQ_VEC_INTST1 )						// se il buffer è vuoto
//	{
//		if( ringbuf_getc(&sioCfgTab[SIO_CHAN1].pComBuf->txBuf, &c) == True )
//			TXB1 = c;		// prova a riempirlo
//		else
//			_STMK1 = 1;		// maskera l'irq (non mi serve +)
//	}
}


// Gestore servizio Irq per sio in modalità UART con per trasferimento ad interrupt
static void sioIrqHandUartINTCh5(int vn)
{
byte c;

// solo per controllo errori eventuali in futuro
//	if( _CSMK3 == 0 )					// se la ricezione è abilitata
//	if( _STMK1 == 0 )					// se l'irq è abilitato

////	if( vn == IRQ_VEC_INTSR2 )			// se è arrivato un carattere
////	{
////		c = RXB2;						// lo piglia
////
////		ringbuf_putc(&sioCfgTab[SIO_CHAN3].pComBuf->rxBuf, c);
////		return;
////	}
////
////
////	if( vn == IRQ_VEC_INTST2 )			// se il buffer è vuoto
////	{
////		if( ringbuf_getc(&sioCfgTab[SIO_CHAN3].pComBuf->txBuf, &c) == True )
////			TXB2 = c;					// prova a riempirlo
////		else
////			_STMK2 = 1;					// maskera l'irq (non mi serve +)
////
////		return;
////	}
}

static void sioIrqHandUartINTCh6(int vn)
{
byte c;
}

/**
* Common setting for SIO Channel 1 when used in SPI
* Master mode
*/
static void setSioSPIModeMasterCh1(uint8_t clockMode)
{
    stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
    stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;
    stc_mfs47_csio_escr_field_t *pESCRReg = NULL;
    stc_mfs47_csio_smr_field_t *pSMRReg = NULL;

    pSCRReg = &FM3_MFS4_CSIO->SCR_f;
    pSSRReg = &FM3_MFS4_CSIO->SSR_f;
    pESCRReg = &FM3_MFS4_CSIO->ESCR_f;
    pSMRReg = &FM3_MFS4_CSIO->SMR_f;

    // Setting the Serial Mode Register
    pSMRReg->MD2 = SET_BIT_LOW; // Set MD2 = 0
    pSMRReg->MD1 = SET_BIT_HIGH; // Set MD1 = 1
    pSMRReg->MD0 = SET_BIT_LOW; // Set MD0 = 0

    if (clockMode & BIT3)
        pSMRReg->SCINV = SET_BIT_HIGH; // Set the SCINV Bit
    else
        pSMRReg->SCINV = SET_BIT_LOW; // Set the SCINV Bit

    if (clockMode & BIT2)
        pSMRReg->BDS = SET_BIT_HIGH; // Set the MSB/LSB Bit
    else
        pSMRReg->BDS = SET_BIT_LOW; // Set the  MSB/LSB Bit

    pSMRReg->SCKE = SET_BIT_HIGH; // Set SCKE = 1
    //pSMRReg->SOE = SET_BIT_HIGH; //Set SOE = 1 - Enabled Serial Output Port

    // Setting the Serial Control Register
    pSCRReg->UPCL = SET_BIT_LOW;  // Set UPCL = 0
    pSCRReg->MS = SET_BIT_LOW;    // Set Master Mode
    pSCRReg->SPI = SET_BIT_HIGH;   // Set SPI Mode transfer

    // Setting the Serial Status Register
    pSSRReg->REC = SET_BIT_LOW;  //Set REC = 0 - Just to clean the register

    //Setting the Extended Communication Control Register
    pESCRReg->SOP = SET_BIT_LOW; //Set SOP = 0
    //No wait inserted in communication.
    pESCRReg->WT0 = SET_BIT_LOW;
    pESCRReg->WT1 = SET_BIT_LOW;

    return;
}

/**
* Common setting for SIO Channel 2 when used in SPI  in
* Slave mode
* SIO CHAN 2 in Flexiper Type 0 micro protetion is used as the
* SIO CHAN 1 in Flexuper Type 2 micro controller
*/
static void setSioSPIModeSlaveCh2(uint8_t clockMode)
{
     setSioSPIModeSlaveCh1(clockMode);
}
/**
* Common setting for SIO Channel 1 when used in SPI  in
* Slave mode
*/
static void setSioSPIModeSlaveCh1(uint8_t clockMode)
{
    stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
    stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;
    stc_mfs47_csio_escr_field_t *pESCRReg = NULL;
    stc_mfs47_csio_smr_field_t *pSMRReg = NULL;
    uint8_t temp = 0;

    pSCRReg = &FM3_MFS4_CSIO->SCR_f;
    pSSRReg = &FM3_MFS4_CSIO->SSR_f;
    pESCRReg = &FM3_MFS4_CSIO->ESCR_f;
    pSMRReg = &FM3_MFS4_CSIO->SMR_f;

    // Setting the Serial Mode Register
    pSMRReg->MD2 = SET_BIT_LOW; // Set MD2 = 0
    pSMRReg->MD1 = SET_BIT_HIGH; // Set MD1 = 1
    pSMRReg->MD0 = SET_BIT_LOW; // Set MD0 = 0
    temp = clockMode & BIT3;
    if (temp)
        pSMRReg->SCINV = SET_BIT_HIGH;   // Set the clock direction
    else
        pSMRReg->SCINV = SET_BIT_LOW;   // Set the clock direction
    temp = clockMode & BIT2;
    if (temp)
        pSMRReg->BDS = SET_BIT_HIGH;   // Set the Bit direction (MSB or LSB)
    else
        pSMRReg->BDS = SET_BIT_LOW;   // Set the Bit direction (MSB or LSB)

    pSMRReg->SCKE = SET_BIT_LOW; // Set SCKE = 0, for slave
    pSMRReg->SOE = SET_BIT_HIGH; //Set SOE = 1 - Enabled Serial Output Port

    // Setting the Serial Control Register
    pSCRReg->UPCL = SET_BIT_LOW;  // Set UPCL = 0
    pSCRReg->MS = SET_BIT_HIGH;    // Set Slave Mode
    pSCRReg->SPI = SET_BIT_HIGH;   // Set SPI Mode transfer

    // Setting the Serial Status Register
    pSSRReg->REC = SET_BIT_LOW;  //Set REC = 0 - Just to clean the register

    //Setting the Extended Communication Control Register
    pESCRReg->SOP = SET_BIT_LOW; //Set SOP = 0

    return;
}

//-------------------------------------------------------------------------
//------------------- UART ISR --------------------------------------------
//-------------------------------------------------------------------------

//----------------------------- UART 0 ----------------------------
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART0_TX_IRQ)
{
	byte c;

	if( ringbuf_getc(&sioCfgTab[SIO_CHAN0].pComBuf->txBuf, &c) == True )
		FM3_MFS0_UART->TDR = c;
    else
		FM3_MFS0_UART->SCR &= ~(BIT3);  // stop irq

}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART0_RX_IRQ)
{
uint16_t c;
uint8_t e;

	c = FM3_MFS0_UART->RDR;
	e = FM3_MFS0_UART->SSR;

    ringbuf_putc(&sioCfgTab[SIO_CHAN0].pComBuf->rxBuf, c);

	if( (e & (UART_SSR_ORE | UART_SSR_FRE)) != 0 )		// framing error o overrun error ?
	{													// il data register non contiene dati nuovi
		if( (e & UART_SSR_ORE) != 0 )
		{
			sioCfgTab[SIO_CHAN0].cntOverrunError++;
		}
		if( (e & UART_SSR_FRE) != 0 )
		{
			sioCfgTab[SIO_CHAN0].cntFrameError++;
		}
		FM3_MFS0_UART->SSR |= UART_SSR_REC;
	}
	else
	{
		if( (e & UART_SSR_PE) != 0 )
		{
			sioCfgTab[SIO_CHAN0].cntParityError++;
			FM3_MFS0_UART->SSR |= UART_SSR_REC;
		}
	}
}
#ifdef __cplusplus
}
#endif

//----------------------------- UART 1 ----------------------------
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART1_TX_IRQ)
{
	byte c;

	if( ringbuf_getc(&sioCfgTab[SIO_CHAN3].pComBuf->txBuf, &c) == True )
		{
		FM3_MFS1_UART->TDR = c;
		}
    else
		FM3_MFS1_UART->SCR_f.TIE = 0;  // stop irq
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART1_RX_IRQ)
{
	uint16_t c = 0;
	uint8_t e = 0;

	c = FM3_MFS1_UART->RDR;
	e = FM3_MFS1_UART->SSR;

    //ringbuf_putc(&sioCfgTab[SIO_CHAN3].pComBuf->rxBuf, c);

	if( (e & (UART_SSR_ORE | UART_SSR_FRE)) != 0 )		// framing error o overrun error ?
	{													// il data register non contiene dati nuovi
		if( (e & UART_SSR_ORE) != 0 )
		{
			sioCfgTab[SIO_CHAN3].cntOverrunError++;
		}
		if( (e & UART_SSR_FRE) != 0 )
		{
			sioCfgTab[SIO_CHAN3].cntFrameError++;
		}
		FM3_MFS1_UART->SSR |= UART_SSR_REC;
	}
	else
	{
		if( (e & UART_SSR_PE) != 0 )
		{
			sioCfgTab[SIO_CHAN3].cntParityError++;
			FM3_MFS1_UART->SSR |= UART_SSR_REC;
		}
		else
		    ringbuf_putc(&sioCfgTab[SIO_CHAN3].pComBuf->rxBuf, c);

	}
}
#ifdef __cplusplus
}
#endif


//----------------------------- UART 3 ----------------------------
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART3_TX_IRQ)
{
	byte c;

	if( ringbuf_getc(&sioCfgTab[SIO_CHAN5].pComBuf->txBuf, &c) == True )
		FM3_MFS3_UART->TDR = c;
    else
		FM3_MFS3_UART->SCR &= ~(BIT3);  // stop irq
}
#ifdef __cplusplus
}
#endif

#ifndef LOADER_ENGINE
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART3_RX_IRQ)
{
	uint16_t c;
	uint8_t e;

	c = FM3_MFS3_UART->RDR;
	e = FM3_MFS3_UART->SSR;

    ringbuf_putc(&sioCfgTab[SIO_CHAN5].pComBuf->rxBuf, c);

	if( (e & (UART_SSR_ORE | UART_SSR_FRE)) != 0 )		// framing error o overrun error ?
	{													// il data register non contiene dati nuovi
		if( (e & UART_SSR_ORE) != 0 )
		{
			sioCfgTab[SIO_CHAN5].cntOverrunError++;
		}
		if( (e & UART_SSR_FRE) != 0 )
		{
			sioCfgTab[SIO_CHAN5].cntFrameError++;
		}
		FM3_MFS3_UART->SSR |= UART_SSR_REC;
	}
	else
	{
		if( (e & UART_SSR_PE) != 0 )
		{
			sioCfgTab[SIO_CHAN5].cntParityError++;
			FM3_MFS3_UART->SSR |= UART_SSR_REC;
		}
	}
}
#ifdef __cplusplus
}
#endif
#endif
//-------------------------------------- UART 3 ---------------------------

/*********************** CSIO 4 ********************************************
*Interrupt routines for MFS04 used in syncronous mode for SPI communication

*/
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(CSIO4_RX_IRQ)
{
    stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
    stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;

    pSCRReg = &FM3_MFS4_CSIO->SCR_f;
    pSSRReg = &FM3_MFS4_CSIO->SSR_f;
    FM3_MFS47_CSIO_TypeDef * pFM3_MFSx = FM3_MFS4_CSIO;

	//Verify if interrupt has been corretcly routed
	if (pSCRReg->RIE == INT_ENABLE)
       {
         //Discover the cause of interrupt on RX
         //if ((pSSRReg->RDRF == SET_BIT_HIGH ) && (pSSRReg->ORE == SET_BIT_LOW))
         if ((pSSRReg->RDRF == SET_BIT_HIGH ))
            {
                ringbuf_putc(&sioCfgTab[SIO_CHAN2].pComBuf->rxBuf, pFM3_MFSx->RDR);

                //Received data without any doubt - Prepare to copy the data
                //Reading from the Received Data Register (RDR) until received FIFO is emptied
            }
        if ((pSSRReg->RDRF == SET_BIT_LOW ) && (pSSRReg->ORE == SET_BIT_HIGH))
            {
                //Overrrun error
                //Setting the Received Error Flag Clear bit (SSR:REC) to "1"
                pSSRReg->REC = SET_BIT_HIGH;
            }

       }



}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(CSIO4_TX_IRQ)
{
//A transmit interrupt occurs if transmit data is transferred from the Transmit Data Register (TDR)
//to the transmit shift register (SSR:TDRE=1) and the data transmission is started,
//or if no data is transmitted (SSR:TBI=1).

stc_mfs47_csio_scr_field_t *pSCRReg = NULL;
stc_mfs47_csio_ssr_field_t *pSSRReg = NULL;
stc_mfs47_csio_fcr1_field_t *pFCR1Reg = NULL;

byte c = 0;

CSIOIntCause eIntCause = CSIOINT_NONE; //Init val

pSCRReg = &FM3_MFS4_CSIO->SCR_f;
pSSRReg = &FM3_MFS4_CSIO->SSR_f;
pFCR1Reg = &FM3_MFS4_CSIO->FCR1_f;

FM3_MFS47_CSIO_TypeDef * pFM3_MFSx =  FM3_MFS4_CSIO;


//Detecting the interrupt cause
if (pSCRReg->TIE == INT_ENABLE)
    {
        //This kind of INT is enabled
        if((pSSRReg->TDRE == SET_BIT_HIGH)) //&&
                //(pSSRReg->TBI == SET_BIT_LOW) )//&&
                    //(pFCR1Reg->FDRQ == SET_BIT_LOW))
                    {
                        //Caused by a Transfer from the TDR
                        eIntCause = CSIOINT_TXDRE;
                    }
                    else
                    {
                        eIntCause = CSIOINT_NONE;
                    }
    }
if (pSCRReg->TBIE == INT_ENABLE)
    {
        //This kind of int is enabled
        if((pSSRReg->TDRE == SET_BIT_LOW) &&
                (pSSRReg->TBI == SET_BIT_HIGH) &&
                    (pFCR1Reg->FDRQ == SET_BIT_LOW))
                    {
                        if (eIntCause == CSIOINT_NONE)
                        {
                        //Caused by a not trasmitted data
                        eIntCause = CSIOINT_TXNODATA;
                        }
                        else
                        {
                         //Multiple cause for interrupt - What happen?
                        eIntCause = CSIOINT_TXINTNDCAUSE;
                        }
                    }
                    else
                    {
                        eIntCause = CSIOINT_NONE;
                    }
    }
if (pFCR1Reg->FTIE == INT_ENABLE)
    {
        //This kind of int is enabled
        if((pSSRReg->TDRE == SET_BIT_LOW) &&
                (pSSRReg->TBI == SET_BIT_LOW) &&
                    (pFCR1Reg->FDRQ == SET_BIT_HIGH))
                    {
                        if (eIntCause == CSIOINT_NONE)
                        {
                        //Caused by a TX FIFO EMPTY
                        eIntCause = CSIOINT_TXFIFOE;
                        }
                        else
                        {
                         //Multiple cause for interrupt - What happen?
                        eIntCause = CSIOINT_TXINTNDCAUSE;
                        }
                    }
                    else
                    {
                        eIntCause = CSIOINT_NONE;
                    }
    }

switch (eIntCause)
{
    case CSIOINT_NONE:
    //No cause detected
    //What TO DO??
        break;
    case CSIOINT_TXDRE:
    //Transfer from the TDR
    //Clear the interrupt to do
        if (pFM3_MFSx->SSR_f.TDRE == ON)   //If the TX Data Register is empty
            {
                if(ringbuf_getc(&sioCfgTab[SIO_CHAN2].pComBuf->txBuf, &c) == True )
                    {
                        pFM3_MFSx->TDR = c;    //Write in the register
                    }
                else
                    {
                        pFM3_MFSx->ESCR_f.RESERVED1 = OFF;
                        pFM3_MFSx->SCR_f.TIE = SET_BIT_LOW;
                    }
            }
        break;
    case CSIOINT_TXNODATA:
    // not trasmitted data
    // Clear the interrupt to do
        break;
    case CSIOINT_TXFIFOE:
    // Fifo Empty ACTION TO DO??
    // TO Clear the interrupt The FIFO transmit data request bit (FCR1:FDRQ) is set to "0"
    // or transmit FIFO is full
        pFCR1Reg->FDRQ = SET_BIT_LOW;
        break;
    case CSIOINT_TXINTNDCAUSE:
    default:
    // Problem in detecting the cause
    // Clearing all
        pFCR1Reg->FDRQ = SET_BIT_LOW;

    //Further action to do???
        break;
}

}
#ifdef __cplusplus
}
#endif



//---------------------------CSIO 4 END ----------------------------------------

//----------------------------- UART 7 ----------------------------
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART7_TX_IRQ)
{
	byte c;

	if( ringbuf_getc(&sioCfgTab[SIO_CHAN6].pComBuf->txBuf, &c) == True )
		FM3_MFS7_UART->TDR = c;
    else
		FM3_MFS7_UART->SCR &= ~(BIT3);  // stop irq
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UART7_RX_IRQ)
{
	uint16_t c;
	uint8_t e;

	c = FM3_MFS7_UART->RDR;
	e = FM3_MFS7_UART->SSR;

    ringbuf_putc(&sioCfgTab[SIO_CHAN6].pComBuf->rxBuf, c);

	if( (e & (UART_SSR_ORE | UART_SSR_FRE)) != 0 )		// framing error o overrun error ?
	{													// il data register non contiene dati nuovi
		if( (e & UART_SSR_ORE) != 0 )
		{
			sioCfgTab[SIO_CHAN6].cntOverrunError++;
		}
		if( (e & UART_SSR_FRE) != 0 )
		{
			sioCfgTab[SIO_CHAN6].cntFrameError++;
		}
		FM3_MFS3_UART->SSR |= UART_SSR_REC;
	}
	else
	{
		if( (e & UART_SSR_PE) != 0 )
		{
			sioCfgTab[SIO_CHAN6].cntParityError++;
			FM3_MFS3_UART->SSR |= UART_SSR_REC;
		}
	}
}
#ifdef __cplusplus
}
#endif
