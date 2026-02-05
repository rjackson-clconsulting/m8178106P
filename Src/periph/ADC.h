/**
@file		adc.h
@brief		ADC driver for FM3 family by Cometa (Enrico) - Review
@author		Fernando Morani
@date		04/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#ifndef _ADCDRV_H_
#define _ADCDRV_H_

/*******************
** INCLUDE SECTION **
********************/
#include "stdDataTypes.h"
#include "alloc.h"
#include "mb9bf50x.h"
#include "util.h"

/*******************
** DEFINE SECTION **
********************/
#define FM3ADC0        ((ADC_TypeDef *)FM3_ADC0_BASE)

#define FILT_SIZE	10
#define ACQ_SIZE	100
#define CAL_POINTS	4					// numero max. di punti per la calibrazione

#define MAX_AD		32					// Max ADC channel available in FM3


#define Adc1         adcSet[0]
#define Adc2         adcSet[1]
#define Adc3         adcSet[2]
#define Adc4         adcSet[3]
#define Adc5         adcSet[4]
#define Adc6         adcSet[5]
#define Adc7         adcSet[6]
#define Adc8         adcSet[7]
#define Adc9         adcSet[8]
#define Adc10        adcSet[9]
#define Adc11        adcSet[10]
#define Adc12        adcSet[11]
#define Adc13        adcSet[12]
#define Adc14        adcSet[13]
#define Adc15        adcSet[14]
#define Adc16        adcSet[15]
#define Adc17        adcSet[16]
#define Adc18        adcSet[17]
#define Adc19        adcSet[18]
#define Adc20        adcSet[19]
#define Adc21        adcSet[20]
#define Adc22        adcSet[21]
#define Adc23        adcSet[22]
#define Adc24        adcSet[23]
#define Adc25        adcSet[24]
#define Adc26        adcSet[25]
#define Adc27        adcSet[26]
#define Adc28        adcSet[27]
#define Adc29        adcSet[28]
#define Adc30        adcSet[29]
#define Adc31        adcSet[30]
#define Adc32        adcSet[31]

#define ADC_OK                  0
#define ADC_ERR_INIT            1
#define ADC_ERR_ALREADY_USED    2
#define ADC_ERR_NO_ZERO_CHAN    3

#define ADC_CPU_CH0			0x00
#define ADC_CPU_CH1			0x01
#define ADC_CPU_CH2			0x02
#define ADC_CPU_CH3			0x03
#define ADC_CPU_CH4			0x04
#define ADC_CPU_CH5			0x05
#define ADC_CPU_CH6			0x06
#define ADC_CPU_CH7			0x07
#define ADC_CPU_CH8			0x08
#define ADC_CPU_CH9			0x09
#define ADC_CPU_CH10		0x0A
#define ADC_CPU_CH11		0x0B
#define ADC_CPU_CH12		0x0C
#define ADC_CPU_CH13		0x0D

#define ADC_INITIALIZED     1
#define ADC_NOT_INITIALIZED 0

/*******************
** TYPE DEFINITION **
********************/

struct CalPoint{                      	/* struttura per singolo punto di calibrazione */
        long	PtValue; 				// valore in punti adc o altro
        float	UmValue; 				// valore corrispondente nell'unità di misura richiesta
    };

typedef struct CalPoint CalPoint;


struct ADCtrl{                      // definizione control register ad
    byte chan;              		// numero canale
    byte resol;            			// risoluzione e velocità di conversione
};

typedef struct ADCtrl ADCtrl;


/* Dati per il filtro dei valori letti da adc
*/
struct ADCFilt{
    byte   	buffer[200];            		// buffer dinamico di x valori per filtro
    byte    idxMem;                         // Indice ultimo Valore per Filtro
    byte    idxGet;                         // Indice primo Valore per Filtro
    byte    num;                            // numero valori acquisiti
    byte    max;                            // numero max valori acquisibili
    byte	size;							// dimensione di ogni elemento
    byte    byfilt;                         // divisore valori filtro

    long    sum;                            // somma dei valori memorizzati nel buffer
    long    value;                          // Valore analogico con filtro
};

typedef struct ADCFilt ADCFilt;


/* Dati per conversione analogica su piu' punti
*/
struct ADCSegCal{
    const CalPoint *Points; 						// punti di calibrazione
    long   	value;                          // valore convertito
    byte	decimals;						// decimali per conversione ad intero
    byte	numValidPoints;					// numero di punti di conversione validi
    long   	valnoff;                        // valore convertito
	long    offset;
};

typedef struct ADCSegCal ADCSegCal;


struct ADChan{
    ADCtrl  	ctrl;                       // registro di controllo per questo canale
    ADCFilt 	filter;                     // filtro sul canale
    ADCSegCal	conv; 	                	// convertitore unita` di misura

    long    	value;                      // valore in uscita dal convertitore
    long    	minPeak;                    // valore minimo rilevato
    long    	maxPeak;                    // valore maassimo rilevato
	bool    	used;						// canale utilizzato
	short       init;                       // initialized channel
};

typedef struct ADChan ADChan;


/* Dati per la stabilizzazione della pressione
*/
struct ADCStab{
    long   PressDir;        /* valore di press. per stabiliz. diretta */
    long   Range;           /* valore di range per la finestra di stabilizzazione (in pascal) */
    word   MinTime;         /* tempo minimo di stabilizzazione del segnale */
};

typedef struct ADCStab ADCStab;


/* Dati per l'acquisizione della pressione a campionamento fisso
*/
struct ADCAcq{
    long Buff[ACQ_SIZE];                    /* buffer acquisizione */
    word IdxMem;                            /* indice acquisizione -> inserimento */
    word IdxGet;                            /* indice acquisizione <- estrazione */
    word Num;                               /* conteggio numero valori campionati */
    word CampReq;                           /* tempo campionamento richiesto */
    word CampAct;                           /* tempo campionamento attuale */
    byte Mode;                              /* modalita` di acquisizione */
};

typedef struct ADCAcq ADCAcq;

#ifndef _MB9BD10T_H_
/******************************************************************************
 * ADC12_MODULE
 ******************************************************************************/
// ADC12_MODULE register bit fields
typedef struct stc_fmadc_adsr_field
{
  __IO  uint8_t SCS        : 1;
  __IO  uint8_t PCS        : 1;
  __IO  uint8_t PCNS       : 1;
        uint8_t RESERVED1  : 3;
  __IO  uint8_t FDAS       : 1;
  __IO  uint8_t ADSTP      : 1;
} stc_fmadc_adsr_field_t;

typedef struct stc_fmadc_adcr_field
{
  __IO  uint8_t OVRIE      : 1;
  __IO  uint8_t CMPIE      : 1;
  __IO  uint8_t PCIE       : 1;
  __IO  uint8_t SCIE       : 1;
        uint8_t RESERVED1  : 1;
  __IO  uint8_t CMPIF      : 1;
  __IO  uint8_t PCIF       : 1;
  __IO  uint8_t SCIF       : 1;
} stc_fmadc_adcr_field_t;

typedef struct stc_fmadc_sfns_field
{
  __IO  uint8_t SFS0       : 1;
  __IO  uint8_t SFS1       : 1;
  __IO  uint8_t SFS2       : 1;
  __IO  uint8_t SFS3       : 1;
} stc_fmadc_sfns_field_t;

typedef struct stc_fmadc_sccr_field
{
  __IO  uint8_t SSTR       : 1;
  __IO  uint8_t SHEN       : 1;
  __IO  uint8_t RPT        : 1;
        uint8_t RESERVED1  : 1;
  __IO  uint8_t SFCLR      : 1;
  __IO  uint8_t SOVR       : 1;
  __IO  uint8_t SFUL       : 1;
  __IO  uint8_t SEMP       : 1;
} stc_fmadc_sccr_field_t;

typedef struct stc_fmadc_scfd_field
{
  __IO uint32_t SC0        : 1;
  __IO uint32_t SC1        : 1;
  __IO uint32_t SC2        : 1;
  __IO uint32_t SC3        : 1;
  __IO uint32_t SC4        : 1;
       uint32_t RESERVED1  : 3;
  __IO uint32_t RS0        : 1;
  __IO uint32_t RS1        : 1;
       uint32_t RESERVED2  : 2;
  __IO uint32_t INVL       : 1;
       uint32_t RESERVED3  : 7;
  __IO uint32_t SD0        : 1;
  __IO uint32_t SD1        : 1;
  __IO uint32_t SD2        : 1;
  __IO uint32_t SD3        : 1;
  __IO uint32_t SD4        : 1;
  __IO uint32_t SD5        : 1;
  __IO uint32_t SD6        : 1;
  __IO uint32_t SD7        : 1;
  __IO uint32_t SD8        : 1;
  __IO uint32_t SD9        : 1;
  __IO uint32_t SD10       : 1;
  __IO uint32_t SD11       : 1;
} stc_fmadc_scfd_field_t;

typedef struct stc_fmadc_scfdl_field
{
  __IO uint16_t SC0        : 1;
  __IO uint16_t SC1        : 1;
  __IO uint16_t SC2        : 1;
  __IO uint16_t SC3        : 1;
  __IO uint16_t SC4        : 1;
       uint16_t RESERVED1  : 3;
  __IO uint16_t RS0        : 1;
  __IO uint16_t RS1        : 1;
       uint16_t RESERVED2  : 2;
  __IO uint16_t INVL       : 1;
} stc_fmadc_scfdl_field_t;

typedef struct stc_fmadc_scfdh_field
{
       uint16_t RESERVED1  : 4;
  __IO uint16_t SD0        : 1;
  __IO uint16_t SD1        : 1;
  __IO uint16_t SD2        : 1;
  __IO uint16_t SD3        : 1;
  __IO uint16_t SD4        : 1;
  __IO uint16_t SD5        : 1;
  __IO uint16_t SD6        : 1;
  __IO uint16_t SD7        : 1;
  __IO uint16_t SD8        : 1;
  __IO uint16_t SD9        : 1;
  __IO uint16_t SD10       : 1;
  __IO uint16_t SD11       : 1;
} stc_fmadc_scfdh_field_t;

typedef struct stc_fmadc_scis23_field
{
  __IO uint16_t AN16       : 1;
  __IO uint16_t AN17       : 1;
  __IO uint16_t AN18       : 1;
  __IO uint16_t AN19       : 1;
  __IO uint16_t AN20       : 1;
  __IO uint16_t AN21       : 1;
  __IO uint16_t AN22       : 1;
  __IO uint16_t AN23       : 1;
  __IO uint16_t AN24       : 1;
  __IO uint16_t AN25       : 1;
  __IO uint16_t AN26       : 1;
  __IO uint16_t AN27       : 1;
  __IO uint16_t AN28       : 1;
  __IO uint16_t AN29       : 1;
  __IO uint16_t AN30       : 1;
  __IO uint16_t AN31       : 1;
} stc_fmadc_scis23_field_t;

typedef struct stc_fmadc_scis2_field
{
  __IO  uint8_t AN16       : 1;
  __IO  uint8_t AN17       : 1;
  __IO  uint8_t AN18       : 1;
  __IO  uint8_t AN19       : 1;
  __IO  uint8_t AN20       : 1;
  __IO  uint8_t AN21       : 1;
  __IO  uint8_t AN22       : 1;
  __IO  uint8_t AN23       : 1;
} stc_fmadc_scis2_field_t;

typedef struct stc_fmadc_scis3_field
{
  __IO  uint8_t AN24       : 1;
  __IO  uint8_t AN25       : 1;
  __IO  uint8_t AN26       : 1;
  __IO  uint8_t AN27       : 1;
  __IO  uint8_t AN28       : 1;
  __IO  uint8_t AN29       : 1;
  __IO  uint8_t AN30       : 1;
  __IO  uint8_t AN31       : 1;
} stc_fmadc_scis3_field_t;

typedef struct stc_fmadc_scis01_field
{
  __IO uint16_t AN0        : 1;
  __IO uint16_t AN1        : 1;
  __IO uint16_t AN2        : 1;
  __IO uint16_t AN3        : 1;
  __IO uint16_t AN4        : 1;
  __IO uint16_t AN5        : 1;
  __IO uint16_t AN6        : 1;
  __IO uint16_t AN7        : 1;
  __IO uint16_t AN8        : 1;
  __IO uint16_t AN9        : 1;
  __IO uint16_t AN10       : 1;
  __IO uint16_t AN11       : 1;
  __IO uint16_t AN12       : 1;
  __IO uint16_t AN13       : 1;
  __IO uint16_t AN14       : 1;
  __IO uint16_t AN15       : 1;
} stc_fmadc_scis01_field_t;

typedef struct stc_fmadc_scis0_field
{
  __IO  uint8_t AN0        : 1;
  __IO  uint8_t AN1        : 1;
  __IO  uint8_t AN2        : 1;
  __IO  uint8_t AN3        : 1;
  __IO  uint8_t AN4        : 1;
  __IO  uint8_t AN5        : 1;
  __IO  uint8_t AN6        : 1;
  __IO  uint8_t AN7        : 1;
} stc_fmadc_scis0_field_t;

typedef struct stc_fmadc_scis1_field
{
  __IO  uint8_t AN8        : 1;
  __IO  uint8_t AN9        : 1;
  __IO  uint8_t AN10       : 1;
  __IO  uint8_t AN11       : 1;
  __IO  uint8_t AN12       : 1;
  __IO  uint8_t AN13       : 1;
  __IO  uint8_t AN14       : 1;
  __IO  uint8_t AN15       : 1;
} stc_fmadc_scis1_field_t;

typedef struct stc_fmadc_pfns_field
{
  __IO  uint8_t PFS0       : 1;
  __IO  uint8_t PFS1       : 1;
        uint8_t RESERVED1  : 2;
  __IO  uint8_t TEST0      : 1;
  __IO  uint8_t TEST1      : 1;
} stc_fmadc_pfns_field_t;

typedef struct stc_fmadc_pccr_field
{
  __IO  uint8_t PSTR       : 1;
  __IO  uint8_t PHEN       : 1;
  __IO  uint8_t PEEN       : 1;
  __IO  uint8_t ESCE       : 1;
  __IO  uint8_t PFCLR      : 1;
  __IO  uint8_t POVR       : 1;
  __IO  uint8_t PFUL       : 1;
  __IO  uint8_t PEMP       : 1;
} stc_fmadc_pccr_field_t;

typedef struct stc_fmadc_pcfd_field
{
  __IO uint32_t PC0        : 1;
  __IO uint32_t PC1        : 1;
  __IO uint32_t PC2        : 1;
  __IO uint32_t PC3        : 1;
  __IO uint32_t PC4        : 1;
       uint32_t RESERVED1  : 3;
  __IO uint32_t RS0        : 1;
  __IO uint32_t RS1        : 1;
  __IO uint32_t RS2        : 1;
       uint32_t RESERVED2  : 1;
  __IO uint32_t INVL       : 1;
       uint32_t RESERVED3  : 7;
  __IO uint32_t PD0        : 1;
  __IO uint32_t PD1        : 1;
  __IO uint32_t PD2        : 1;
  __IO uint32_t PD3        : 1;
  __IO uint32_t PD4        : 1;
  __IO uint32_t PD5        : 1;
  __IO uint32_t PD6        : 1;
  __IO uint32_t PD7        : 1;
  __IO uint32_t PD8        : 1;
  __IO uint32_t PD9        : 1;
  __IO uint32_t PD10       : 1;
  __IO uint32_t PD11       : 1;
} stc_fmadc_pcfd_field_t;

typedef struct stc_fmadc_pcfdl_field
{
  __IO uint16_t PC0        : 1;
  __IO uint16_t PC1        : 1;
  __IO uint16_t PC2        : 1;
  __IO uint16_t PC3        : 1;
  __IO uint16_t PC4        : 1;
       uint16_t RESERVED1  : 3;
  __IO uint16_t RS0        : 1;
  __IO uint16_t RS1        : 1;
  __IO uint16_t RS2        : 1;
       uint16_t RESERVED2  : 1;
  __IO uint16_t INVL       : 1;
} stc_fmadc_pcfdl_field_t;

typedef struct stc_fmadc_pcfdh_field
{
       uint16_t RESERVED1  : 4;
  __IO uint16_t PD0        : 1;
  __IO uint16_t PD1        : 1;
  __IO uint16_t PD2        : 1;
  __IO uint16_t PD3        : 1;
  __IO uint16_t PD4        : 1;
  __IO uint16_t PD5        : 1;
  __IO uint16_t PD6        : 1;
  __IO uint16_t PD7        : 1;
  __IO uint16_t PD8        : 1;
  __IO uint16_t PD9        : 1;
  __IO uint16_t PD10       : 1;
  __IO uint16_t PD11       : 1;
} stc_fmadc_pcfdh_field_t;

typedef struct stc_fmadc_pcis_field
{
  __IO  uint8_t P1A0       : 1;
  __IO  uint8_t P1A1       : 1;
  __IO  uint8_t P1A2       : 1;
  __IO  uint8_t P2A0       : 1;
  __IO  uint8_t P2A1       : 1;
  __IO  uint8_t P2A2       : 1;
  __IO  uint8_t P2A3       : 1;
  __IO  uint8_t P2A4       : 1;
} stc_fmadc_pcis_field_t;

typedef struct stc_fmadc_cmpcr_field
{
  __IO  uint8_t CCH0       : 1;
  __IO  uint8_t CCH1       : 1;
  __IO  uint8_t CCH2       : 1;
  __IO  uint8_t CCH3       : 1;
  __IO  uint8_t CCH4       : 1;
  __IO  uint8_t CMD0       : 1;
  __IO  uint8_t CMD1       : 1;
  __IO  uint8_t CMPEN      : 1;
} stc_fmadc_cmpcr_field_t;

typedef struct stc_fmadc_cmpd_field
{
       uint16_t RESERVED1  : 6;
  __IO uint16_t CMAD2      : 1;
  __IO uint16_t CMAD3      : 1;
  __IO uint16_t CMAD4      : 1;
  __IO uint16_t CMAD5      : 1;
  __IO uint16_t CMAD6      : 1;
  __IO uint16_t CMAD7      : 1;
  __IO uint16_t CMAD8      : 1;
  __IO uint16_t CMAD9      : 1;
  __IO uint16_t CMAD10     : 1;
  __IO uint16_t CMAD11     : 1;
} stc_fmadc_cmpd_field_t;

typedef struct stc_fmadc_adss23_field
{
  __IO uint16_t TS16       : 1;
  __IO uint16_t TS17       : 1;
  __IO uint16_t TS18       : 1;
  __IO uint16_t TS19       : 1;
  __IO uint16_t TS20       : 1;
  __IO uint16_t TS21       : 1;
  __IO uint16_t TS22       : 1;
  __IO uint16_t TS23       : 1;
  __IO uint16_t TS24       : 1;
  __IO uint16_t TS25       : 1;
  __IO uint16_t TS26       : 1;
  __IO uint16_t TS27       : 1;
  __IO uint16_t TS28       : 1;
  __IO uint16_t TS29       : 1;
  __IO uint16_t TS30       : 1;
  __IO uint16_t TS31       : 1;
} stc_fmadc_adss23_field_t;

typedef struct stc_fmadc_adss2_field
{
  __IO  uint8_t TS16       : 1;
  __IO  uint8_t TS17       : 1;
  __IO  uint8_t TS18       : 1;
  __IO  uint8_t TS19       : 1;
  __IO  uint8_t TS20       : 1;
  __IO  uint8_t TS21       : 1;
  __IO  uint8_t TS22       : 1;
  __IO  uint8_t TS23       : 1;
} stc_fmadc_adss2_field_t;

typedef struct stc_fmadc_adss3_field
{
  __IO  uint8_t TS24       : 1;
  __IO  uint8_t TS25       : 1;
  __IO  uint8_t TS26       : 1;
  __IO  uint8_t TS27       : 1;
  __IO  uint8_t TS28       : 1;
  __IO  uint8_t TS29       : 1;
  __IO  uint8_t TS30       : 1;
  __IO  uint8_t TS31       : 1;
} stc_fmadc_adss3_field_t;

typedef struct stc_fmadc_adss01_field
{
  __IO uint16_t TS0        : 1;
  __IO uint16_t TS1        : 1;
  __IO uint16_t TS2        : 1;
  __IO uint16_t TS3        : 1;
  __IO uint16_t TS4        : 1;
  __IO uint16_t TS5        : 1;
  __IO uint16_t TS6        : 1;
  __IO uint16_t TS7        : 1;
  __IO uint16_t TS8        : 1;
  __IO uint16_t TS9        : 1;
  __IO uint16_t TS10       : 1;
  __IO uint16_t TS11       : 1;
  __IO uint16_t TS12       : 1;
  __IO uint16_t TS13       : 1;
  __IO uint16_t TS14       : 1;
  __IO uint16_t TS15       : 1;
} stc_fmadc_adss01_field_t;

typedef struct stc_fmadc_adss0_field
{
  __IO  uint8_t TS0        : 1;
  __IO  uint8_t TS1        : 1;
  __IO  uint8_t TS2        : 1;
  __IO  uint8_t TS3        : 1;
  __IO  uint8_t TS4        : 1;
  __IO  uint8_t TS5        : 1;
  __IO  uint8_t TS6        : 1;
  __IO  uint8_t TS7        : 1;
} stc_fmadc_adss0_field_t;

typedef struct stc_fmadc_adss1_field
{
  __IO  uint8_t TS8        : 1;
  __IO  uint8_t TS9        : 1;
  __IO  uint8_t TS10       : 1;
  __IO  uint8_t TS11       : 1;
  __IO  uint8_t TS12       : 1;
  __IO  uint8_t TS13       : 1;
  __IO  uint8_t TS14       : 1;
  __IO  uint8_t TS15       : 1;
} stc_fmadc_adss1_field_t;

typedef struct stc_fmadc_adst01_field
{
  __IO uint16_t ST10       : 1;
  __IO uint16_t ST11       : 1;
  __IO uint16_t ST12       : 1;
  __IO uint16_t ST13       : 1;
  __IO uint16_t ST14       : 1;
  __IO uint16_t STX10      : 1;
  __IO uint16_t STX11      : 1;
  __IO uint16_t STX12      : 1;
  __IO uint16_t ST00       : 1;
  __IO uint16_t ST01       : 1;
  __IO uint16_t ST02       : 1;
  __IO uint16_t ST03       : 1;
  __IO uint16_t ST04       : 1;
  __IO uint16_t STX00      : 1;
  __IO uint16_t STX01      : 1;
  __IO uint16_t STX02      : 1;
} stc_fmadc_adst01_field_t;

typedef struct stc_fmadc_adst1_field
{
  __IO  uint8_t ST10       : 1;
  __IO  uint8_t ST11       : 1;
  __IO  uint8_t ST12       : 1;
  __IO  uint8_t ST13       : 1;
  __IO  uint8_t ST14       : 1;
  __IO  uint8_t STX10      : 1;
  __IO  uint8_t STX11      : 1;
  __IO  uint8_t STX12      : 1;
} stc_fmadc_adst1_field_t;

typedef struct stc_fmadc_adst0_field
{
  __IO  uint8_t ST00       : 1;
  __IO  uint8_t ST01       : 1;
  __IO  uint8_t ST02       : 1;
  __IO  uint8_t ST03       : 1;
  __IO  uint8_t ST04       : 1;
  __IO  uint8_t STX00      : 1;
  __IO  uint8_t STX01      : 1;
  __IO  uint8_t STX02      : 1;
} stc_fmadc_adst0_field_t;

typedef struct stc_fmadc_adct_field
{
  __IO  uint8_t CT0        : 1;
  __IO  uint8_t CT1        : 1;
  __IO  uint8_t CT2        : 1;
  __IO  uint8_t CT3        : 1;
  __IO  uint8_t CT4        : 1;
  __IO  uint8_t CT5        : 1;
  __IO  uint8_t CT6        : 1;
  __IO  uint8_t CT7        : 1;
} stc_fmadc_adct_field_t;

typedef struct stc_fmadc_prtsl_field
{
  __IO  uint8_t PRTSL0     : 1;
  __IO  uint8_t PRTSL1     : 1;
  __IO  uint8_t PRTSL2     : 1;
  __IO  uint8_t PRTSL3     : 1;
} stc_fmadc_prtsl_field_t;

typedef struct stc_fmadc_sctsl_field
{
  __IO  uint8_t SCTSL0     : 1;
  __IO  uint8_t SCTSL1     : 1;
  __IO  uint8_t SCTSL2     : 1;
  __IO  uint8_t SCTSL3     : 1;
} stc_fmadc_sctsl_field_t;

typedef struct stc_fmadc_adcen_field
{
  __IO  uint8_t ENBL       : 1;
  __IO  uint8_t READY      : 1;
        uint8_t RESERVED1  : 2;
  __IO  uint8_t CYCLSL0    : 1;
  __IO  uint8_t CYCLSL1    : 1;
} stc_fmadc_adcen_field_t;

typedef struct
{
  union {
    __IO  uint8_t ADSR;
    stc_fmadc_adsr_field_t ADSR_f;
  };
  union {
    __IO  uint8_t ADCR;
    stc_fmadc_adcr_field_t ADCR_f;
  };
        uint8_t RESERVED0[6];
  union {
    __IO  uint8_t SFNS;
    stc_fmadc_sfns_field_t SFNS_f;
  };
  union {
    __IO  uint8_t SCCR;
    stc_fmadc_sccr_field_t SCCR_f;
  };
        uint8_t RESERVED1[2];
  union {
    union {
      __IO uint32_t SCFD;
      stc_fmadc_scfd_field_t SCFD_f;
    };
    struct {
      union {
        __IO uint16_t SCFDL;
        stc_fmadc_scfdl_field_t SCFDL_f;
      };
      union {
        __IO uint16_t SCFDH;
        stc_fmadc_scfdh_field_t SCFDH_f;
      };
    };
  };
  union {
    union {
      __IO uint16_t SCIS23;
      stc_fmadc_scis23_field_t SCIS23_f;
    };
    struct {
      union {
        __IO  uint8_t SCIS2;
        stc_fmadc_scis2_field_t SCIS2_f;
      };
      union {
        __IO  uint8_t SCIS3;
        stc_fmadc_scis3_field_t SCIS3_f;
      };
    };
  };
        uint8_t RESERVED2[2];
  union {
    union {
      __IO uint16_t SCIS01;
      stc_fmadc_scis01_field_t SCIS01_f;
    };
    struct {
      union {
        __IO  uint8_t SCIS0;
        stc_fmadc_scis0_field_t SCIS0_f;
      };
      union {
        __IO  uint8_t SCIS1;
        stc_fmadc_scis1_field_t SCIS1_f;
      };
    };
  };
        uint8_t RESERVED3[2];
  union {
    __IO  uint8_t PFNS;
    stc_fmadc_pfns_field_t PFNS_f;
  };
  union {
    __IO  uint8_t PCCR;
    stc_fmadc_pccr_field_t PCCR_f;
  };
        uint8_t RESERVED4[2];
  union {
    union {
      __IO uint32_t PCFD;
      stc_fmadc_pcfd_field_t PCFD_f;
    };
    struct {
      union {
        __IO uint16_t PCFDL;
        stc_fmadc_pcfdl_field_t PCFDL_f;
      };
      union {
        __IO uint16_t PCFDH;
        stc_fmadc_pcfdh_field_t PCFDH_f;
      };
    };
  };
  union {
    __IO  uint8_t PCIS;
    stc_fmadc_pcis_field_t PCIS_f;
  };
        uint8_t RESERVED5[3];
  union {
    __IO  uint8_t CMPCR;
    stc_fmadc_cmpcr_field_t CMPCR_f;
  };
        uint8_t RESERVED6;
  union {
    __IO uint16_t CMPD;
    stc_fmadc_cmpd_field_t CMPD_f;
  };
  union {
    union {
      __IO uint16_t ADSS23;
      stc_fmadc_adss23_field_t ADSS23_f;
    };
    struct {
      union {
        __IO  uint8_t ADSS2;
        stc_fmadc_adss2_field_t ADSS2_f;
      };
      union {
        __IO  uint8_t ADSS3;
        stc_fmadc_adss3_field_t ADSS3_f;
      };
    };
  };
        uint8_t RESERVED7[2];
  union {
    union {
      __IO uint16_t ADSS01;
      stc_fmadc_adss01_field_t ADSS01_f;
    };
    struct {
      union {
        __IO  uint8_t ADSS0;
        stc_fmadc_adss0_field_t ADSS0_f;
      };
      union {
        __IO  uint8_t ADSS1;
        stc_fmadc_adss1_field_t ADSS1_f;
      };
    };
  };
        uint8_t RESERVED8[2];
  union {
    union {
      __IO uint16_t ADST01;
      stc_fmadc_adst01_field_t ADST01_f;
    };
    struct {
      union {
        __IO  uint8_t ADST1;
        stc_fmadc_adst1_field_t ADST1_f;
      };
      union {
        __IO  uint8_t ADST0;
        stc_fmadc_adst0_field_t ADST0_f;
      };
    };
  };
        uint8_t RESERVED9[2];
  union {
    __IO  uint8_t ADCT;
    stc_fmadc_adct_field_t ADCT_f;
  };
        uint8_t RESERVED10[3];
  union {
    __IO  uint8_t PRTSL;
    stc_fmadc_prtsl_field_t PRTSL_f;
  };
  union {
    __IO  uint8_t SCTSL;
    stc_fmadc_sctsl_field_t SCTSL_f;
  };
        uint8_t RESERVED11[2];
  union {
    __IO  uint8_t ADCEN;
    stc_fmadc_adcen_field_t ADCEN_f;
  };
}ADC_TypeDef;

#endif //ADC definition for FM3 Project different from Flexiper Controller TYPE2 Micro

/************************
** FUNCTION DEFINITION **
*************************/

#ifdef __cplusplus
extern "C" {
#endif


uint8_t adc_init(uint8_t maxAD);
uint8_t adc_setupChannel(ADChan *pAdc, int chan, int filtmax, int filtsize, int filtdiv,
										const CalPoint *Points, int numMaxPoints, int decimals);
void adc_scale(void);
void adc_scaleBySegment(long *pdest, long *psrc, ADCSegCal *pconv);
void adc_valueBySegment(long *pdest, long *psrc, ADCSegCal *pconv);
void adc_read(void);
void adc_scan(void);
word adc_scan_group(word numChan, int minChan, int maxChan);
word adc_getConvResult(void);
long adc_getBufValue(ADCFilt *pf, int index);
void adc_eval(ADChan *pc);
void adc_filter(ADCFilt *pf, long value);

#ifdef __cplusplus
}
#endif


#endif  /* _ADCDRV_H */


