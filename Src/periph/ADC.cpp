/**
@file		adc.cpp
@brief		ADC driver for FM3 family by Cometa (Enrico) - Review
@author		Fernando Morani
@date		04/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#include "adc.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------
ADChan adcSet[MAX_AD];              // canali ad converter preposti
long resolCount;

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
//static int	statoTC3400;
word adcFifoBuf[MAX_AD];


//-------------------------------------------------------
// Corpo funzioni di questo modulo
//-------------------------------------------------------

/**
** Init ADC peripheral for this board
** This init suppose you are initialising the ADC in sequential order
** That is the case of Flexiper Protective, from 0 to 13.
@params uint8_t maxAD: number of ADC input to be used
*/
uint8_t adc_init(uint8_t maxAD)
{
uint8_t u8RetVal = ADC_OK;
int i = 0;
ADChan *pAdc = NULL; //&adcSet[0];

    if (maxAD > MAX_AD) {
        //Not plausible - Return fail
        return ADC_ERR_INIT;
        }
        //else not needed - carry on with Init

	for(i=0; i<maxAD; i++)					// init tutti i dati in ram
	{
        RESET_ZERO_STRUCT(adcSet[i]);
        pAdc = &adcSet[i];

    	pAdc->ctrl.resol = 0x18;			// 10-Bit resolution, sampl. 128 cycles,  comp. 352 cycles

		mem_alloc(pAdc->filter.max * pAdc->filter.size, (void **) &pAdc->filter.buffer);   // Riserva solo la memoria richiesta....
		pAdc->filter.idxMem = 0;			// Indice ultimo Valore per Filtro
		pAdc->filter.idxGet = 0;            // Indice primo Valore per Filtro
		pAdc->filter.num = 0;               // numero valori acquisiti

    	pAdc->filter.sum = 0;               // somma dei valori memorizzati nel buffer
	    pAdc->filter.value = 0;				// Valore analogico con filtro

		pAdc->conv.Points = NULL;			// reset converitore per l'um
		pAdc->conv.decimals = 1;
		pAdc->conv.numValidPoints = 0;

	    pAdc->value = 0;					// Valore analogico istantaneo
    	pAdc->minPeak = +1;                 // valore minimo rilevato
    	pAdc->maxPeak = -1;                 // valore maassimo rilevato
		pAdc->used = 0;                    	// canale utilizzato
		pAdc->init = ADC_INITIALIZED;       //Initialized channel
	}


	// Questo setting è valido SOLO per il FLEXIPER PROTECTIVE MICRO
	// Questo codice non è quindi portabile se non lo si modifica
	// Enable scan channel
    FM3ADC0->SCIS3 = 0x00; // Scan channel select AN31-AN24
    FM3ADC0->SCIS2 = 0x00; // Scan channel select AN23-AN16
    FM3ADC0->SCIS1 = 0x3F; // Scan channel select AN15-AN08 -> AN08: AN13
    FM3ADC0->SCIS0 = 0xFF; // Scan channel select AN07-AN00 -> AN00: AN07

    FM3ADC0->ADST1 = 0x2F; // Sampling Time 1
    FM3ADC0->ADST0 = 0x2F; // Sampling Time 0

    FM3ADC0->ADSS3 = 0x00; // Sampling Time Select AN31-AN24
    FM3ADC0->ADSS2 = 0x00; // Sampling Time Select AN23-AN16
    FM3ADC0->ADSS1 = 0x00; // Sampling Time Select AN15-AN08
    FM3ADC0->ADSS0 = 0x00; // Sampling Time Select AN07-AN00 -> Use Samplin Time 0 for AN0

    FM3ADC0->ADCT  = 0x00; // Comparison Time = 14 x (ADCT + 2) / HCLK

    FM3ADC0->ADCEN = 0x01; // Enable ADC

    while (3 != FM3ADC0->ADCEN); // wait until ADC operation is enabled

    FM3ADC0->ADSR  = 0x00; // Stop ADC, Places conversion result on the MSB side.

    FM3ADC0->CMPCR = 0x00; // No comparsion
    FM3ADC0->CMPD  = 0x00; // No comparsion value

    FM3ADC0->SFNS  = 0x00; // Set Fifo Stage Count Interrupt
    FM3ADC0->ADCR  = 0x00; // Disable ADC interrupts

    return (u8RetVal);

}


uint8_t adc_setupChannel(ADChan *pAdc, int chan, int filtmax, int filtsize, int filtdiv,
									const CalPoint *Points, int numMaxPoints, int decimals)
{
uint8_t u8RetVal = ADC_OK;
int numValidPt;
long precPt = 0;

	if( pAdc->used == true ){
        u8RetVal = ADC_ERR_ALREADY_USED;
        return u8RetVal;
	}


    pAdc->ctrl.chan = chan ;						// numero canale
	pAdc->filter.max = filtmax;              	// numero max valori acquisibili
	pAdc->filter.size = filtsize;				// dimensione di ogni elemento
	pAdc->filter.byfilt = filtdiv;           	// divisore valori filtro

	pAdc->conv.Points = Points;           		// segmenti per conversione

	if( numMaxPoints > 16 )
		numMaxPoints = 16;
												// calcola un attimo il numero di punti validi
	for(numValidPt=0; numValidPt<numMaxPoints; numValidPt++, Points++)
	{
		if( Points->PtValue > 0x007FFFFF || Points->PtValue < -100000000 )
		{
			break;
		}
		else
		{
			if( numValidPt > 0 )					// ad esclusione del primo
			{
				if( Points->PtValue == precPt )		// due punti uguali ?
					break;
													// due punti uguali ?
				if( Points->PtValue == Points[1].PtValue )
					break;							// buca !!
			}
		}
		precPt = Points->PtValue;
	}

	pAdc->conv.decimals = decimals;           // numero di decimali della conversione
	pAdc->conv.numValidPoints = numValidPt;    	// numero di segmenti realmente validi
	pAdc->used = true;								// lo vogliono propio utilizzare

	return u8RetVal;
}


long adc_getBufValue(ADCFilt *pf, int index)
{
long val;

	if( index >= pf->max )
		return(0);

	switch( pf->size )						// adatta per ogni dimensione...
	{
		case	1:						// byte tradizionale...
        	val = pf->buffer[index];					// estrae ..
        	break;

		case	2:						// short tradizionale...
        	val = ((short *) pf->buffer)[index];		// estrae ...
        	break;

		case	3:						// fritto misto...
			val = 0;
		    memcpy(&val, pf->buffer + index * 3, 3);
			if( (val & 0x0080000) != 0 )					// se era negatives...
				val |= 0xFF000000;							// estende il segno
        	break;

		case	4:						// long tradizionale
        	val = ((long *) pf->buffer)[index];			// estrae ...come richiesto
		    break;

		default:
			val = 0;
			break;
	}
	return(val);
}



/* Esegue la conversione di scala nell'unità di misura dei canali AD campionati
*/
void adc_scale(void)
{
int nch;
ADChan *pAdc = &adcSet[0];
long valueToScale;
long valueScaled;

	for(nch=0; nch<MAX_AD; nch++, pAdc++)
	{
		if( pAdc->used != 0 )
		{
			valueToScale = pAdc->filter.value;			// copia .....

			adc_scaleBySegment(&valueScaled, &valueToScale, &pAdc->conv);	// scala ....

			pAdc->conv.valnoff = valueScaled;				// senza offset ....

			pAdc->conv.value = valueScaled - pAdc->conv.offset;				// toglie offset ....

		}
	}
}


/* Esegue la conversione nell'unità di misura interpolando i punti nella tabella di conversione/calibrazione in eeprom
*/
void adc_scaleBySegment(long *pdest, long *psrc, ADCSegCal *pconv)
{
byte v;
long deltaPoint;
float deltaValue;
float realValue;
float gain;

const CalPoint *calPoints = pconv->Points;
byte decimals = pconv->decimals;
byte numValidPoints = pconv->numValidPoints;
byte totalSegment = numValidPoints;

static const float decimalAdjTab[] = { 1.0, 10.0, 100.0, 1000.0 };


	*pdest = 0;								// iniziamo per benino

	if( numValidPoints == 0xFF )
	{
		numValidPoints = 0;
		return;
	}

	if( numValidPoints > 16 || calPoints == NULL)
		return;

	for(v=0; v<totalSegment; v++)			// scorre la tabella di punti di cal. richiesti
	{
		if( psrc[0] >= calPoints[v].PtValue )			// se è maggiore o uguale al punto attuale
		{
			if( psrc[0] < calPoints[v+1].PtValue || (v == totalSegment-1) ) 	// ed è minore del punto succesivo.. oppure è l'ultimo punto valido
			{
				if( (v == totalSegment-1) || (calPoints[v+1].PtValue > 0x001FFFFF) )		// se il punto succ. non esiste oppure non è valido...
				{
											// devo utilizzare il punto prec. per esegure la conversione
					if( v == 0 )			// se è il primo punto..
					{
						pdest[0] = 0;		// valori d'ufficio con un solo punto di calibrazione == 0
						break;				// canale OK: avanti il prossimo
					}

					deltaPoint = calPoints[v].PtValue - calPoints[v-1].PtValue;			// calcola i punti nel segmento prec...
					deltaValue = calPoints[v].UmValue - calPoints[v-1].UmValue;			// calcola il delta di valore nel segmento prec...
				}
				else
				{
													// posso utilizzare questo segmento per esegure la conversione
					deltaPoint = calPoints[v+1].PtValue - calPoints[v].PtValue;			// calcola i punti nel segmento..
					deltaValue = calPoints[v+1].UmValue - calPoints[v].UmValue;			// calcola il delta di valore nel segmento..
				}

				if( deltaPoint != 0 )	// salvo errori e/o omissioni...
				{
					gain = deltaValue / deltaPoint;										// calcola il gain del segmento

					deltaPoint = psrc[0] - calPoints[v].PtValue;						// calcola il delta rispetto all'inizio del segmento
					deltaValue = gain * deltaPoint;										// converte nel fattore numerico il segmento
					realValue = calPoints[v].UmValue + deltaValue;						// somma il valore di inizio segmento.. e ottiene il valore interpolato

					if( decimals < 3 )													// converte da float ad intero con i decimali richiesti.
						pdest[0] = (long) (decimalAdjTab[decimals] * realValue);
					else
						pdest[0] = (long) (10.0 * realValue);							// se esagerano.... il valore di default
				}
				else
				{
					pdest[0] = 0;			// valori d'ufficio con delta == 0
				}

				break;						// canale OK: avanti il prossimo
			}
			else
			{
				continue;					// prossimo punto di calibrazione..
			}
		}
		else								// numeri negativi fuori scala ???
		{									// puo' capitare solo con v == 0

			if( (v == totalSegment-1) || (calPoints[v+1].PtValue > 0x001FFFFF) )		// se il punto succ. non esiste oppure non è valido...
			{
				pdest[0] = 0;		// valori d'ufficio con un solo punto di calibrazione == 0
				break;				// canale OK: avanti il prossimo
			}
			else
			{
										// posso utilizzare questo segmento per esegure la conversione
				deltaPoint = calPoints[v+1].PtValue - calPoints[v].PtValue;			// calcola i punti nel segmento..
				deltaValue = calPoints[v+1].UmValue - calPoints[v].UmValue;			// calcola il delta di valore nel segmento..
			}

			if( deltaPoint != 0 )	// salvo errori e/o omissioni...
			{
				gain = deltaValue / deltaPoint;										// calcola il gain del segmento

				deltaPoint = psrc[0] - calPoints[v].PtValue;						// calcola il delta rispetto all'inizio del segmento
				deltaValue = gain * deltaPoint;										// converte nel fattore numerico il segmento
				realValue = calPoints[v].UmValue + deltaValue;						// somma il valore di inizio segmento.. e ottiene il valore interpolato

				if( decimals < 3 )													// converte da float ad intero con i decimali richiesti.
					pdest[0] = (long) (decimalAdjTab[decimals] * realValue);
				else
					pdest[0] = (long) (10.0 * realValue);
			}
			else
			{
				pdest[0] = 0;			// valori d'ufficio con delta == 0
			}
			break;						// canale OK: avanti il prossimo
		}
	}
}


/* Esegue la conversione in punti adc interpolando i valori nell'unità di misura nella tabella di conversione/calibrazione in eeprom
*/
void adc_valueBySegment(long *pdest, long *psrc, ADCSegCal *pconv)
{
byte v;
long deltaPoint;
long realPoint;
float deltaValue;
float reqValue;
float gain;

const CalPoint *calPoints = pconv->Points;
byte decimals = pconv->decimals;
byte numValidPoints = pconv->numValidPoints;
byte totalSegment = numValidPoints;

static const float decimalAdjTab[] = { 1.0, 10.0, 100.0, 1000.0 };

	reqValue = (float) psrc[0];				// piglia il valore di riferimento

	if( decimals < 3 )						// converte da intero a float ad intero con i decimali richiesti.
		reqValue /= decimalAdjTab[decimals];
	else
		reqValue /= 10.0;					// se esagerano.... il valore di default

	*pdest = 0;								// iniziamo per benino

	if( numValidPoints == 0xFF )
	{
		numValidPoints = 0;
		return;
	}

	if( numValidPoints > 16 || calPoints == NULL)
		return;

	for(v=0; v<totalSegment; v++)			// scorre la tabella di punti di cal. richiesti
	{
		if( reqValue >= calPoints[v].UmValue )			// se è maggiore o uguale al punto attuale
		{
			if( reqValue < calPoints[v+1].UmValue || (v == totalSegment-1) ) 				// ed è minore del punto succesivo.. oppure è l'ultimo punto valido
			{
				if( (v == totalSegment-1) || (calPoints[v+1].PtValue > 0x001FFFFF) )		// se il punto succ. non esiste oppure non è valido...
				{
											// devo utilizzare il punto prec. per esegure la conversione
					if( v == 0 )			// se è il primo punto..
					{
						pdest[0] = 0;		// valori d'ufficio con un solo punto di calibrazione == 0
						break;				// canale OK: avanti il prossimo
					}

					deltaPoint = calPoints[v].PtValue - calPoints[v-1].PtValue;			// calcola i punti nel segmento prec...
					deltaValue = calPoints[v].UmValue - calPoints[v-1].UmValue;			// calcola il delta di valore nel segmento prec...
				}
				else
				{
													// posso utilizzare questo segmento per eseguire la conversione
					deltaPoint = calPoints[v+1].PtValue - calPoints[v].PtValue;			// calcola i punti nel segmento..
					deltaValue = calPoints[v+1].UmValue - calPoints[v].UmValue;			// calcola il delta di valore nel segmento..
				}

				if( deltaPoint != 0 )				// salvo errori e/o omissioni...
				{
					gain = ((float) (deltaPoint)) / deltaValue;							// calcola il gain del segmento

					deltaValue = reqValue - calPoints[v].UmValue;						// calcola il delta rispetto all'inizio del segmento

					deltaPoint = (long) (gain * deltaValue);										// converte nel fattore numerico il segmento
					realPoint = (long) (calPoints[v].PtValue + deltaPoint);						// somma il valore di inizio segmento.. e ottiene il valore interpolato
					pdest[0] = realPoint;												// aggiorna i punti scala dell'ADC
				}
				else
				{
					pdest[0] = 0;			// valori d'ufficio con delta == 0
				}
				break;						// canale OK: avanti il prossimo
			}
			else
			{
				continue;					// prossimo punto di calibrazione..
			}
		}
		else								// numeri negativi fuori scala ???
		{									// puo' capitare solo con v == 0

			if( (v == totalSegment-1) || (calPoints[v+1].PtValue > 0x001FFFFF) )		// se il punto succ. non esiste oppure non è valido...
			{
				pdest[0] = 0;		// valori d'ufficio con un solo punto di calibrazione == 0
				break;				// canale OK: avanti il prossimo
			}
			else
			{
										// posso utilizzare questo segmento per esegure la conversione
				deltaPoint = calPoints[v+1].PtValue - calPoints[v].PtValue;			// calcola i punti nel segmento..
				deltaValue = calPoints[v+1].UmValue - calPoints[v].UmValue;			// calcola il delta di valore nel segmento..
			}

			if( deltaPoint != 0 )	// salvo errori e/o omissioni...
			{
				gain = ((float) (deltaPoint)) / deltaValue;							// calcola il gain del segmento

				deltaValue = reqValue - calPoints[v].UmValue;						// calcola il delta rispetto all'inizio del segmento

				deltaPoint = (long) (gain * deltaValue);										// converte nel fattore numerico il segmento
				realPoint = calPoints[v].PtValue + deltaPoint;						// somma il valore di inizio segmento.. e ottiene il valore interpolato
				pdest[0] = realPoint;												// aggiorna i punti scala dell'ADC
			}
			else
			{
				pdest[0] = 0;			// valori d'ufficio con delta == 0
			}
			break;						// canale OK: avanti il prossimo
		}
	}
}


/* Esegue una scansione nultiplay sui canali ADC per raccogliere i dati pronti
   Tempo di esecuzione misurato: 500uSec
*/


void adc_scan(void)
{
static word numChanCpu;						// numero canale in scansione
static word statoScan;

uint32_t adcFifoVal;
ADChan *pc;


    switch( statoScan )
    {
        case    0:      // IDLE
            statoScan++;
            break;

        case    1:      // start conv
            FM3ADC0->SCCR = 0x11;      // FIFO clear, start ADC Single Conversion
            statoScan++;
            break;

        case    2:      // wait endof conv
            if( !FM3ADC0->ADSR_f.SCS )
            {

                for(numChanCpu=0; numChanCpu<MAX_AD; numChanCpu++)
                {
                    adcFifoVal = FM3ADC0->SCFD;                // poppa tutto dalla fifo
                    adcFifoBuf[numChanCpu] = adcFifoVal >> 20;  // salta i bit di controllo e copia solo i dati
                }

                numChanCpu = 0;

                //pc = &adcSet[numChanCpu];					// pointer to actual channel
                //if (pc->init == ADC_INITIALIZED){
                    //pc->value = adcFifoBuf[numChanCpu];			// update
                    //adc_eval(pc);					            // evaluate
                //}
                //numChanCpu++;
                statoScan++;
            }
            break;

        case    3:      // wait endof conv

            if( numChanCpu < MAX_AD )
            {
                pc = &adcSet[numChanCpu]; //Pointer to the actual channel
                if (pc->init == ADC_INITIALIZED){
                    pc->value = adcFifoBuf[numChanCpu];			// update
                    adc_eval(pc);					            // evaluate
                }
                numChanCpu++;
            }
            if( numChanCpu >=  MAX_AD )     // se ha finito l'ultimo
            {
                FM3ADC0->SCCR = 0x11;      // FIFO clear, start ADC Single Conversion
                statoScan = 2;
            }
            break;
        default:
            break;
    }

}


word adc_scan_group(word numChan, int minChan, int maxChan)
{
word wVal=0;
short sVal;
long lVal;
ADChan *pc;								// puntatore al canale

static int numConv;							// numero canale in scansione

	if( numChan >= MAX_AD )					// se sfora....
		numChan = 0;						// aggiusta il tiro

	pc = &adcSet[numChan];					// puntatore al canale attuale

	while( numChan < MAX_AD )
	{
										// se il canale è valido...
		if( pc->used && pc->ctrl.chan >= minChan && pc->ctrl.chan <= maxChan )
		{
								// data collection
			//switch( pc->ctrl.chan )
			//{

					numConv++;							// un sample in + per il canale

					if( numConv	> 0 )
					{
						numConv = 0;					// reset conversione

//						wVal =  ADCR0;					// somma le 4 letture eseguite
//						wVal += ADCR1;
//						wVal += ADCR2;
//						wVal += ADCR3;

			            pc->value = wVal;				// aggiorna
			            adc_eval(pc);					// fa valutare un attimo...

			            numChan++;						// avanti il prossimo
			        }
			        else
			        {
//						wVal =  ADCR0;					// somma le 4 letture eseguite
//						wVal += ADCR1;
//						wVal += ADCR2;
//						wVal += ADCR3;

//						_ADCE = 1;					// start conversione

			        	return(numChan);				// non deve cambiare mux
			        }
					break;
				//break;
			//}
			//break;
		}
		else
		{
			numChan++;							// prossimo canale
			pc++;
		}
	}

	if( numChan >= MAX_AD )						// se sfora....
		numChan = 0;							// aggiusta il tiro

	pc = &adcSet[numChan];						// puntatore al nuovo canale

				// selezione mux prossimo canale
				//////////////////////////////////////////////////////
	while( numChan < MAX_AD )
	{
										// se il canale è valido...
		if( pc->used && pc->ctrl.chan >= minChan && pc->ctrl.chan <= maxChan )
		{
			//switch( pc->ctrl.chan )
			//{

//					ADM0 = 0x30 | pc->ctrl.chan;		// enable ADC + 4buffer + select mode + input channel X
//					_ADCE = 1;							// start conversione
					return(numChan);					// fine dell'opera
					//break;

			//}

			break;
		}

		numChan++;							// prossimo canale
		pc++;
	}

	if( numChan >= MAX_AD )						// se sfora.... non ha precaricato
	{
		numChan = 0;							// aggiusta il tiro

		pc = &adcSet[numChan];						// puntatore al nuovo canale

		while( numChan < MAX_AD )
		{
											// se il canale è valido...
			if( pc->used && pc->ctrl.chan >= minChan && pc->ctrl.chan <= maxChan )
			{
				//switch( pc->ctrl.chan )
				//{

//						ADM0 = 0x30 | pc->ctrl.chan;		// enable ADC + 4buffer + select mode + input channel X
//						_ADCE = 1;							// start conversione
						return(numChan);					// fine dell'opera
						//break;

			//}

				break;
			}

			numChan++;							// prossimo canale
			pc++;
		}
	}

	return(numChan);
}




void adc_read(void)
{
int nch;
word wVal;
word numConv = 0;
short sVal;
long lVal;
ADChan *pc = &adcSet[0];

	for(nch=0; nch<MAX_AD; nch++, pc++)
	{
		if( pc->used == 0 )
		{
//			nch++;
			continue;
		}

		//switch( pc->ctrl.chan )
		//{
			numConv++;						// un sample in + per il canale

//				ADM0 = 0x30 | pc->ctrl.chan;		// enable ADC + 4buffer + select mode + input channel X

				wVal = adc_getConvResult();			// fa convertire in locale

                pc->value = wVal;				// aggiorna
                adc_eval(pc);					// fa valutare un attimo...
				break;
		//}
	}
}

word adc_getConvResult(void)
{
word result = 1;
/*
	_ADCE = 1;			// start conversione

	while( !_ADIF  );

	_ADCE = 0;			// start conversione
	_ADIF = 0;

	_ADCE = 1;			// start conversione

	while( !_ADIF  );

	_ADCE = 0;			// start conversione
	_ADIF = 0;

	result =  ADCR0;					// somma le 4 letture eseguite
	result += ADCR1;
	result += ADCR2;
	result += ADCR3;
*/
	return result;
}

/* Controllo picchi min e max + tipo di filtraggio
*/
void adc_eval(ADChan *pc)
{

    if( pc->value < pc->minPeak )   // recording min peak value
        pc->minPeak = pc->value;

    if( pc->value > pc->maxPeak )   // recording max peak value
        pc->maxPeak = pc->value;


    if( pc->filter.byfilt )
    {
		adc_filter(&pc->filter, pc->value);      // fa filtrare
    }
    else
    {
//        dvalue = (pc->value - pc->conv.zero);
    }

//    pc->conv.dvalue = dvalue * pc->conv.dcoeff;   // fa convertire

}


/* Aggiorna e filtra sulla base della lettura attuale
*/
void adc_filter(ADCFilt *pf, long value)
{
long sub = 0;

    if( pf->idxMem >= pf->max )       		// se e` troppo avanti
        pf->idxMem = 0;                 	// reset indice

    if( pf->idxGet >= pf->max )       		// se e` troppo avanti
        pf->idxGet = 0;                 	// reset indice

	switch( pf->size )						// adatta per ogni dimensione...
	{
		case	1:						// byte tradizionale...
		    pf->buffer[pf->idxMem++] = value;   			// salva il valore campionato

        	sub = pf->buffer[pf->idxGet];					// estrae il primo inserito
        	break;

		case	2:						// short tradizionale...
		    ((short *) pf->buffer)[pf->idxMem++] = value;   	// salva il valore campionato

        	sub = ((short *) pf->buffer)[pf->idxGet];		// estrae il primo inserito
        	break;

		case	3:						// fritto misto...
//			sub = value >> 8;				// occhio al big-endian .... little-endian
		    memcpy(pf->buffer + pf->idxMem * 3, &value, 3);   	// salva il valore campionato

		    sub = 0;
		    memcpy(&sub, pf->buffer + pf->idxGet * 3, 3);   	// estrae il primo inserito
//			sub <<= 8;					// autoestende il segno

			if( (sub & 0x0080000) != 0 )					// se era negatives...
				sub |= 0xFF000000;							// estende il segno

		    pf->idxMem++;									// memorizzato !!
        	break;

		case	4:						// long tradizionale
		    ((long *) pf->buffer)[pf->idxMem++] = value;   	// salva il valore campionato

        	sub = ((long *) pf->buffer)[pf->idxGet];		// estrae il primo inserito
		    break;

		default:
			break;
	}

    pf->sum += value;               		// somma il nuovo valore
    pf->num++;								// avanti il numero dii valori sommati

    if( pf->num >= pf->byfilt )				// se il buffer è pieno...
    {
    	pf->num = pf->byfilt;
        pf->value = pf->sum / pf->byfilt; 	// divide il risultato
	    pf->sum -= sub;               		// sottrae il primo valore inserito
        pf->idxGet++;						// avanti il ricircolo ...
    }
    else
    {
        pf->value = pf->sum / pf->num; 		// divide il risultato
    }
}


