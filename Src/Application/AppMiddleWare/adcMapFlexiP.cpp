/**
@file		adcMapFlexiP.cpp
@brief		Use ADC driver in Flexiper Protective application
@author		Fernando Morani
@date		05/05/2017
@version	01.00
@source     Flexiper protective Micro
*/

#include "adcMapFlexiP.h"

/**
Variables external to this module
*/
extern ADChan adcSet[MAX_AD];
extern bool eep_readParam(void *__param_buf, int __count, int __start_ddr, bool *__chkOk);
extern bool eep_writeParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk);   // scrive i parametri nella memoria dell'eep
extern void AlaPRE_Ini(void);
extern ALA ala;


/**
Global Variable Set in this module
*/
CalPoint calRunCH0[NUM_POINTS_CH0];
CalPoint calRunCH1[NUM_POINTS_CH1];
CalPoint calRunCH2[NUM_POINTS_CH2];
CalPoint calRunCH3[NUM_POINTS_CH3];
CalPoint calRunCH4[NUM_POINTS_CH4];
CalPoint calRunCH5[NUM_POINTS_CH5];
CalPoint calRunCH6[NUM_POINTS_CH6];
CalPoint calRunCH7[NUM_POINTS_CH7];
CalPoint calRunCH8[NUM_POINTS_CH8];
CalPoint calRunCH9[NUM_POINTS_CH9];
CalPoint calRunCH10[NUM_POINTS_CH10];
CalPoint calRunCH11[NUM_POINTS_CH11];
CalPoint calRunCH12[NUM_POINTS_CH12];
CalPoint calRunCH13[NUM_POINTS_CH13];

// GAUGE1
static const CalPoint calDefaultCh0[NUM_POINTS_CH0] = {
										200,	0.0,
										1300,2000.0
										};
// GAUGE2
static const CalPoint calDefaultCh1[NUM_POINTS_CH1] = {
										200,	0.0,
										1300,2000.0
										};
// ARTERIOSA
static const CalPoint calDefaultCh2[NUM_POINTS_CH2] = {
										674,	0.0,
										1788,-250.0
										};
// PREFILTRO
static const CalPoint calDefaultCh3[NUM_POINTS_CH3] = {
										674,	0.0,
										1788, 250.0
										};
// VENOSA
static const CalPoint calDefaultCh4[NUM_POINTS_CH4]	= {
										674,    0.0,
										1788, 250.0
                                        };
// INFUSIONE
static const CalPoint calDefaultCh5[NUM_POINTS_CH5] = {
										674,	0.0,
										1788,-250.0
                                        };
// BLD
static const CalPoint calDefaultCh6[NUM_POINTS_CH6] = {
										   0,	0.0,
										4096,4096.0
										};
// VTEST1
static const CalPoint calDefaultCh7[NUM_POINTS_CH7] = {
										  0,	0.0,
										4096,3000.0
										};
// VTEST2
static const CalPoint calDefaultCh8[NUM_POINTS_CH8] = {
										  0,	0.0,
										4096,3000.0
										};
// monAvrefp
static const CalPoint calDefaultCh9[NUM_POINTS_CH9] = {
										  0,	0.0,
										4096,3000.0
										};
// monAvrefc
static const CalPoint calDefaultCh10[NUM_POINTS_CH10] = {
										  0,	0.0,
										4096,3000.0
										};
// mon5V
static const CalPoint calDefaultCh11[NUM_POINTS_CH11] = {
										  0,	0.0,
										4096,3000.0
										};
// mon33Vp
static const CalPoint calDefaultCh12[NUM_POINTS_CH12]	= {
										  0,	0.0,
										4096,3000.0
                                        };
// mon33Vc
static const CalPoint calDefaultCh13[NUM_POINTS_CH13] = {
										  0,	0.0,
										4096,3000.0
                                        };



/********************************************************************************
 ** uint8_t cal_init()
 ** \brief  read the calibration value from the fram
 **
 ** \retval ERROR_NONE
 ******************************************************************************/
 uint8_t cal_init()
 {
    uint8_t u8RetVal = ERROR_NONE;
    bool readOk = false;
    bool chkOk = false;
    bool writeOk = false;
    bool cmpOk = false;

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 0     gauge1
    ********************************************************************/
    readOk = eep_readParam(calRunCH0, sizeof(calRunCH0), OFS_POINTS_CH0, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_125;
                                        // parte dai valori di default
        memcpy(calRunCH0, calDefaultCh0, sizeof(calRunCH0));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH0, sizeof(calRunCH0), OFS_POINTS_CH0, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_126;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 1     gauge2
    ********************************************************************/
    readOk = eep_readParam(calRunCH1, sizeof(calRunCH1), OFS_POINTS_CH1, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_127;
                                        // parte dai valori di default
        memcpy(calRunCH1, calDefaultCh1, sizeof(calRunCH1));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH1, sizeof(calRunCH1), OFS_POINTS_CH1, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_128;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 2     arteriosa
    ********************************************************************/
    readOk = eep_readParam(calRunCH2, sizeof(calRunCH2), OFS_POINTS_CH2, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_129;
                                        // parte dai valori di default
        memcpy(calRunCH2, calDefaultCh2, sizeof(calRunCH2));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH2, sizeof(calRunCH2), OFS_POINTS_CH2, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_130;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 3     prefiltro
    ********************************************************************/
    readOk = eep_readParam(calRunCH3, sizeof(calRunCH3), OFS_POINTS_CH3, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_131;
                                        // parte dai valori di default
        memcpy(calRunCH3, calDefaultCh3, sizeof(calRunCH3));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH3, sizeof(calRunCH3), OFS_POINTS_CH3, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_132;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 4     venosa
    ********************************************************************/
    readOk = eep_readParam(calRunCH4, sizeof(calRunCH4), OFS_POINTS_CH4, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_133;
                                        // parte dai valori di default
        memcpy(calRunCH4, calDefaultCh4, sizeof(calRunCH4));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH4, sizeof(calRunCH4), OFS_POINTS_CH4, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_134;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 5     infusione
    ********************************************************************/
    readOk = eep_readParam(calRunCH5, sizeof(calRunCH5), OFS_POINTS_CH5, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_135;
                                        // parte dai valori di default
        memcpy(calRunCH5, calDefaultCh5, sizeof(calRunCH5));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH5, sizeof(calRunCH5), OFS_POINTS_CH5, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_136;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 6     bld
    ********************************************************************/
    readOk = eep_readParam(calRunCH6, sizeof(calRunCH6), OFS_POINTS_CH6, &chkOk);

    if( readOk == False )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);
		ala.cal_num_pro=ERR_CAL_137;
                                        // parte dai valori di default
        memcpy(calRunCH6, calDefaultCh6, sizeof(calRunCH6));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH6, sizeof(calRunCH6), OFS_POINTS_CH6, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_138;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 7     vtest1
    ********************************************************************/
    readOk = eep_readParam(calRunCH7, sizeof(calRunCH7), OFS_POINTS_CH7, &chkOk);

    cmpOk = memcmp(calRunCH7, calDefaultCh7, sizeof(calRunCH7));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_139;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_155;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH7, calDefaultCh7, sizeof(calRunCH7));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH7, sizeof(calRunCH7), OFS_POINTS_CH7, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_140;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 8     vtest2
    ********************************************************************/
    readOk = eep_readParam(calRunCH8, sizeof(calRunCH8), OFS_POINTS_CH8, &chkOk);

    cmpOk = memcmp(calRunCH8, calDefaultCh8, sizeof(calRunCH8));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_141;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_156;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH8, calDefaultCh8, sizeof(calRunCH8));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH8, sizeof(calRunCH8), OFS_POINTS_CH8, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_142;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 9     monAvrefp
    ********************************************************************/
    readOk = eep_readParam(calRunCH9, sizeof(calRunCH9), OFS_POINTS_CH9, &chkOk);

    cmpOk = memcmp(calRunCH9, calDefaultCh9, sizeof(calRunCH9));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_143;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_157;
	    }
		                                // parte dai valori di default
        memcpy(calRunCH9, calDefaultCh9, sizeof(calRunCH9));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH9, sizeof(calRunCH9), OFS_POINTS_CH9, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_144;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 10        monAvrefc
    ********************************************************************/
    readOk = eep_readParam(calRunCH10, sizeof(calRunCH10), OFS_POINTS_CH10, &chkOk);

    cmpOk = memcmp(calRunCH10, calDefaultCh10, sizeof(calRunCH10));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_145;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_158;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH10, calDefaultCh10, sizeof(calRunCH10));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH10, sizeof(calRunCH10), OFS_POINTS_CH10, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_146;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 11        mon5V
    ********************************************************************/
    readOk = eep_readParam(calRunCH11, sizeof(calRunCH11), OFS_POINTS_CH11, &chkOk);

    cmpOk = memcmp(calRunCH11, calDefaultCh11, sizeof(calRunCH11));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_147;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_159;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH11, calDefaultCh11, sizeof(calRunCH11));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH11, sizeof(calRunCH11), OFS_POINTS_CH11, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_148;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 12        mon33Vp
    ********************************************************************/
    readOk = eep_readParam(calRunCH12, sizeof(calRunCH12), OFS_POINTS_CH12, &chkOk);

    cmpOk = memcmp(calRunCH12, calDefaultCh12, sizeof(calRunCH12));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_149;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_160;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH12, calDefaultCh12, sizeof(calRunCH12));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH12, sizeof(calRunCH12), OFS_POINTS_CH12, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_150;
	    }
    }

    /*******************************************************************
    read calibration Parameter from FRAM - CAL N° 13        mon33Vc
    ********************************************************************/
    readOk = eep_readParam(calRunCH13, sizeof(calRunCH13), OFS_POINTS_CH13, &chkOk);

    cmpOk = memcmp(calRunCH13, calDefaultCh13, sizeof(calRunCH13));

    if( readOk == False || cmpOk == True )               // se non ci sono calibrazioni valide per questo canale
    {
        Ala_Pre(ALP_T1S_ERR_CAL,1);

        if( cmpOk == False )
        {
            ala.cal_num_pro=ERR_CAL_151;
	    }
	    else
        {
            ala.cal_num_pro=ERR_CAL_161;
	    }
                                        // parte dai valori di default
        memcpy(calRunCH13, calDefaultCh13, sizeof(calRunCH13));
                                        // li aggiorna in eeprom
        writeOk = eep_writeParam(calRunCH13, sizeof(calRunCH13), OFS_POINTS_CH13, &chkOk);

        if( writeOk == False )          // se non ce la fa
	    {
	        Ala_Pre(ALP_T1S_ERR_CAL,1);
			ala.cal_num_pro=ERR_CAL_152;
	    }
    }
return u8RetVal;

 }



/**************************************************************
*** Set up alle the channel in this board - Flexiper Protective
***************************************************************/
uint8_t adc_init_channel (void)
{
uint8_t u8RetVal = ADC_OK;

   	u8RetVal = adc_init(ADC_USED_HERE);
   	if (u8RetVal != ADC_OK)
        return ERR_CAL_101;


    u8RetVal = adc_setupChannel(&adcGauge1,ADC_CPU_CH0,	25,	2, 25,calRunCH0,NUM_POINTS_CH0, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_102;

    u8RetVal = adc_setupChannel(&adcGauge2,ADC_CPU_CH1,	25,	2, 25,calRunCH1,NUM_POINTS_CH1, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_103;

    u8RetVal = adc_setupChannel(&adcPressArt, ADC_CPU_CH2,	25,	2, 25,calRunCH2,NUM_POINTS_CH2, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_104;

    u8RetVal = adc_setupChannel(&adcPressFil,ADC_CPU_CH3,	25,	2, 25,calRunCH3,NUM_POINTS_CH3, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_105;

    u8RetVal = adc_setupChannel(&adcPressVen,ADC_CPU_CH4,	25,	2, 25,calRunCH4,NUM_POINTS_CH4, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_106;

    u8RetVal = adc_setupChannel(&adcPressArt2,ADC_CPU_CH5,	25,	2, 25,calRunCH5,NUM_POINTS_CH5, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_107;

    u8RetVal = adc_setupChannel(&adcBldRx,ADC_CPU_CH6,1,2,1,calRunCH6,NUM_POINTS_CH6, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_108;

    u8RetVal = adc_setupChannel(&vTest1,ADC_CPU_CH7,8,2,8,calRunCH7,NUM_POINTS_CH7, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_109;

    u8RetVal = adc_setupChannel(&vTest2,ADC_CPU_CH8,8,2,8,calRunCH8,NUM_POINTS_CH8, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_110;

    u8RetVal = adc_setupChannel(&monAvrefp,ADC_CPU_CH9,8,2,8,calRunCH9,NUM_POINTS_CH9, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_111;

    u8RetVal = adc_setupChannel(&monAvrefc,ADC_CPU_CH10,8,2,8,calRunCH10,NUM_POINTS_CH10, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_112;

    u8RetVal = adc_setupChannel(&mon5V,ADC_CPU_CH11,8,2,8,calRunCH11,NUM_POINTS_CH11, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_113;

    u8RetVal = adc_setupChannel(&mon33Vp,ADC_CPU_CH12,8,2,8,calRunCH12,NUM_POINTS_CH12, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_114;

    u8RetVal = adc_setupChannel(&mon33V,ADC_CPU_CH13,8,2,8,calRunCH13,NUM_POINTS_CH13, 0);
    if (u8RetVal != ADC_OK)
        return ERR_CAL_115;


return u8RetVal;

}
