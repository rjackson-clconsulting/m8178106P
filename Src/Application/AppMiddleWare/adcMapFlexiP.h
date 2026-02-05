/**
@file		adcMapFlexiP.h
@brief		Use ADC driver in Flexiper Protective application
@author		Fernando Morani
@date		05/05/2017
@version	01.00
@source     Flexiper protective Micro
*/

#ifndef _ADCMAPFLEXIP_H
#define _ADCMAPFLEXIP_H

#include "globalAppDefine.h"
#include "ADC.h"
#include "EEPROM.h"
#include "cd_ala.h"

/**
Define to be used in this file
*/
#define NUM_POINTS_CH0  2
#define NUM_POINTS_CH1  2
#define NUM_POINTS_CH2  2
#define NUM_POINTS_CH3  2
#define NUM_POINTS_CH4  2
#define NUM_POINTS_CH5  2
#define NUM_POINTS_CH6  2
#define NUM_POINTS_CH7  2
#define NUM_POINTS_CH8  2
#define NUM_POINTS_CH9  2
#define NUM_POINTS_CH10 2
#define NUM_POINTS_CH11 2
#define NUM_POINTS_CH12 2
#define NUM_POINTS_CH13 2

#define ADC_USED_HERE       14   //Number of ADC used in this project


#define ERR_CAL_101     101
#define ERR_CAL_102     102
#define ERR_CAL_103     103
#define ERR_CAL_104     104
#define ERR_CAL_105     105
#define ERR_CAL_106     106
#define ERR_CAL_107     107
#define ERR_CAL_108     108
#define ERR_CAL_109     109
#define ERR_CAL_110     110
#define ERR_CAL_111     111
#define ERR_CAL_112     112
#define ERR_CAL_113     113
#define ERR_CAL_114     114
#define ERR_CAL_115     115
#define ERR_CAL_116     116
#define ERR_CAL_117     117
#define ERR_CAL_118     118
#define ERR_CAL_119     119
#define ERR_CAL_120     120
#define ERR_CAL_121     121
#define ERR_CAL_122     122
#define ERR_CAL_123     123
#define ERR_CAL_124     124
#define ERR_CAL_125     125
#define ERR_CAL_126     126
#define ERR_CAL_127     127
#define ERR_CAL_128     128
#define ERR_CAL_129     129
#define ERR_CAL_130     130
#define ERR_CAL_131     131
#define ERR_CAL_132     132
#define ERR_CAL_133     133
#define ERR_CAL_134     134
#define ERR_CAL_135     135
#define ERR_CAL_136     136
#define ERR_CAL_137     137
#define ERR_CAL_138     138
#define ERR_CAL_139     139
#define ERR_CAL_140     140
#define ERR_CAL_141     141
#define ERR_CAL_142     142
#define ERR_CAL_143     143
#define ERR_CAL_144     144
#define ERR_CAL_145     145
#define ERR_CAL_146     146
#define ERR_CAL_147     147
#define ERR_CAL_148     148
#define ERR_CAL_149     149
#define ERR_CAL_150     150
#define ERR_CAL_151     151
#define ERR_CAL_152     152
#define ERR_CAL_153     153
#define ERR_CAL_154     154

#define ERR_CAL_155     155
#define ERR_CAL_156     156
#define ERR_CAL_157     157
#define ERR_CAL_158     158
#define ERR_CAL_159     159
#define ERR_CAL_160     160
#define ERR_CAL_161     161


/**
*  Function declaration
**/
uint8_t adc_init_channel (void);
uint8_t cal_init();

#endif
