/*************************************************************
** Header file generato automaticamente da CRRTAlarmManager **
** Generato il: sab 5. gen 08:50:48 2013					**
****** NON MODIFICARE MANUALMENTE QUESTO FILE ****************
**************************************************************/

#ifndef _CRRT_ALARMS_HEADER
#define _CRRT_ALARMS_HEADER

#include "stdDataTypes.h"

#define PRO_NOT_RUN                         0x00 	//   0
#define CON_NOT_RUN                         0x01 	//   1
#define USE_NOT_RUN                         0x02 	//   2
#define WEI_PCB_NOT_RUN                     0x03 	//   3
#define PRS_PCB_NOT_RUN                     0x04 	//   4
#define UI_CRITICAL_ERROR                   0x05 	//   5
#define PRE_ART_DIF                         0x06 	//   6
#define PRE_FIL_DIF                         0x07 	//   7
#define PRE_VEN_DIF                         0x08 	//   8
#define PRE_UF_DIF                          0x09 	//   9
#define WEI_INF_DIFF                        0x0a 	//  10
#define WEI_UF_DIFF                         0x0b 	//  11
#define WEI_CIT_DIFF                        0x0c 	//  12
#define AIR_TST_ABS                         0x0d 	//  13
#define BLD_TST_ABS                         0x0e 	//  14
#define DATA_ERROR                          0x0f 	//  15
#define CAL_DATA_ERROR                      0x10 	//  16
#define TST_REL_CIT                         0x11 	//  17
#define TST_REL_EPA                         0x12 	//  18
#define ANALOGIC_VOLTAGE                    0x13 	//  19
#define BATTERY_VOLTAGE_TOO_LOW             0x14 	//  20
#define BATTERY_RECHARGE_FAILURE            0x15 	//  21
#define TMP_INF_DIFF                        0x16 	//  22
#define TMP_DIA_DIFF                        0x17 	//  23
#define FATAL_ERROR                         0x18 	//  24
#define PRESSURES_UNCALIBRATED              0x19 	//  25
#define PRESSURIZATION_FAILED               0x1a 	//  26
#define EXCESSIVE_VENOUS_LEAKAGE            0x1b 	//  27
#define PRE_ART_MIN                         0x1c 	//  28
#define PRE_ART_BLOCKED                     0x1d 	//  29
#define PRE_ART_MAX                         0x1e 	//  30
#define PRE_ART_NOT_ZERO                    0x1f 	//  31
#define PRE_ART_DISC                        0x20 	//  32
#define PRE_ART_UNCALIBRATED                0x21 	//  33
#define PRE_ART_DATA_CORRUPTED              0x22 	//  34
#define PRE_FIL_MIN                         0x23 	//  35
#define PRE_FIL_BLOCKED                     0x24 	//  36
#define PRE_FIL_MAX                         0x25 	//  37
#define PRE_FIL_NOT_ZERO                    0x26 	//  38
#define PRE_FIL_DISC                        0x27 	//  39
#define PRE_FIL_UNCALIBRATED                0x28 	//  40
#define PRE_FIL_DATA_CORRUPTED              0x29 	//  41
#define PRE_VEN_MIN                         0x2a 	//  42
#define PRE_VEN_BLOCKED                     0x2b 	//  43
#define PRE_VEN_MAX                         0x2c 	//  44
#define PRE_VEN_UNCALIBRATED                0x2d 	//  45
#define PRE_VEN_DATA_CORRUPTED              0x2e 	//  46
#define PRE_VEN_NOT_ZERO                    0x2f 	//  47
#define PRE_UF_MIN                          0x30 	//  48
#define PRE_UF_BLOCKED                      0x31 	//  49
#define PRE_UF_MAX                          0x32 	//  50
#define PRE_UF_NOT_ZERO                     0x33 	//  51
#define PRE_UF_DISC                         0x34 	//  52
#define PRE_UF_UNCALIBRATED                 0x35 	//  53
#define PRE_UF_DATA_CORRUPTED               0x36 	//  54
#define PRE_DROP_MAX                        0x37 	//  55
#define PRE_MAX_TMP                         0x38 	//  56
#define INLET_DOME_OPEN                     0x39 	//  57
#define PRE_FILTER_DOME_OPEN                0x3a 	//  58
#define UF_DOME_OPEN                        0x3b 	//  59
#define WEI_INF_NOT_ZERO                    0x3c 	//  60
#define WEI_INF_UNCALIBRATED                0x3d 	//  61
#define WEI_INF_DATA_CORRUPTED              0x3e 	//  62
#define WEI_INF_DISCONNECTED                0x3f 	//  63
#define WEI_INF_EXCESSIVE                   0x40 	//  64
#define WEI_INF_VARIATION                   0x41 	//  65
#define WEI_UF_NOT_ZERO                     0x42 	//  66
#define WEI_UF_UNCALIBRATED                 0x43 	//  67
#define WEI_UF_DATA_CORRUPTED               0x44 	//  68
#define WEI_UF_DISCONNECTED                 0x45 	//  69
#define WEI_UF_EXCESSIVE                    0x46 	//  70
#define WEI_UF_VARIATION                    0x47 	//  71
#define WEI_CIT_NOT_ZERO                    0x48 	//  72
#define WEI_CIT_UNCALIBRATED                0x49 	//  73
#define WEI_CIT_DATA_CORRUPTED              0x4a 	//  74
#define WEI_CIT_DISCONNECTED                0x4b 	//  75
#define WEI_CIT_EXCESSIVE                   0x4c 	//  76
#define WEI_CIT_VARIATION                   0x4d 	//  77
#define UF_BAG_EMPTY                        0x4e 	//  78
#define UF_BAG_FULL                         0x4f 	//  79
#define EXCESSIVE_WEIGHT_VARIANCE           0x50 	//  80
#define EXCESSIVE_DIALYSATE_VARIANCE        0x51 	//  81
#define NO_AIR_ALARM                        0x52 	//  82
#define AIR_VEN_LIN                         0x53 	//  83
#define AIR_TEST_ABSENT                     0x54 	//  84
#define AIR_TEST_FAILED                     0x55 	//  85
#define BLD_TUB_PRESENT                     0x56 	//  86
#define BLOOD_LEAKAGE                       0x57 	//  87
#define BLD_AMB_LIG                         0x58 	//  88
#define BLD_TUB_INS                         0x59 	//  89
#define BLD_TEST_FAILED                     0x5a 	//  90
#define BLD_TEST_ABSENT                     0x5b 	//  91
#define BLD_TX_OVE                          0x5c 	//  92
#define BLD_DATA_CORRUPTED                  0x5d 	//  93
#define LEVEL_TEST_FAILED                   0x5e 	//  94
#define LEVEL_TEST_ABSENT                   0x5f 	//  95
#define LOW_LEVEL_ADJUSTEMENT_FAILED        0x60 	//  96
#define HIGH_LEVEL_ADJUSTEMENT_FAILED       0x61 	//  97
#define LINE_SELECTOR_DRIVER_FAULT          0x62 	//  98
#define LINE_SELECTOR_NOT_CLOSED            0x63 	//  99
#define LINE_SELECTOR_NOT_OPENED            0x64 	// 100
#define TEMP_INF_DATA_CORRUPTED             0x65 	// 101
#define TEMP_DIAL_DATA_CORRUPTED            0x66 	// 102
#define HIGH_TEMPERATURE_INF                0x67 	// 103
#define HIGH_TEMPERATURE_DIA                0x68 	// 104
#define HEATER_INF_RELAY_FAULT              0x69 	// 105
#define HEATER_DIA_RELAY_FAULT              0x6a 	// 106
#define LOW_INFUSION_TEMPERATURE            0x6b 	// 107
#define HIGH_INFUSION_TEMPERATURE           0x6c 	// 108
#define LOW_DIALYSATE_TEMPERATURE           0x6d 	// 109
#define HIGH_DIALYSATE_TEMPERATURE          0x6e 	// 110
#define CLAMP_RELAY_FAULT                   0x6f 	// 111
#define CLAMP_DRIVER_FAULT                  0x70 	// 112
#define CLAMP_NOT_CLOSED                    0x71 	// 113
#define CLAMP_NOT_OPENED                    0x72 	// 114
#define BLOOD_PUMP_DOOR_OPENED              0x73 	// 115
#define BLOOD_PUMP_RELAY_FAULT              0x74 	// 116
#define BLOOD_PUMP_DRIVER_FAULT             0x75 	// 117
#define BLOOD_PUMP_SPEED_ERROR              0x76 	// 118
#define BLOOD_PUMP_NOT_STOPPED              0x77 	// 119
#define BLOOD_PUMP_NOT_RUNNING              0x78 	// 120
#define INFUSION_PUMP_DOOR_OPENED           0x79 	// 121
#define INFUSION_PUMP_RELAY_FAULT           0x7a 	// 122
#define INFUSION_PUMP_DRIVER_FAULT          0x7b 	// 123
#define INFUSION_PUMP_SPEED_ERROR           0x7c 	// 124
#define INFUSION_PUMP_NOT_STOPPED           0x7d 	// 125
#define INFUSION_PUMP_NOT_RUNNING           0x7e 	// 126
#define DIALYSATE_PUMP_DOOR_OPENED          0x7f 	// 127
#define DIALYSATE_PUMP_RELAY_FAULT          0x80 	// 128
#define DIALYSATE_PUMP_DRIVER_FAULT         0x81 	// 129
#define DIALYSATE_PUMP_SPEED_ERROR          0x82 	// 130
#define DIALYSATE_PUMP_NOT_STOPPED          0x83 	// 131
#define DIALYSATE_PUMP_NOT_RUNNING          0x84 	// 132
#define CITRATE_PUMP_DOOR_OPENED            0x85 	// 133
#define CITRATE_PUMP_RELAY_FAULT            0x86 	// 134
#define CITRATE_PUMP_DRIVER_FAULT           0x87 	// 135
#define CITRATE_PUMP_SPEED_ERROR            0x88 	// 136
#define CITRATE_PUMP_NOT_STOPPED            0x89 	// 137
#define CITRATE_PUMP_NOT_RUNNING            0x8a 	// 138
#define UF_PUMP_DOOR_OPENED                 0x8b 	// 139
#define UF_PUMP_RELAY_FAULT                 0x8c 	// 140
#define UF_PUMP_DRIVER_FAULT                0x8d 	// 141
#define UF_PUMP_SPEED_ERROR                 0x8e 	// 142
#define UF_PUMP_NOT_STOPPED                 0x8f 	// 143
#define UF_PUMP_NOT_RUNNING                 0x90 	// 144
#define HEPARIN_CALCIUM_PUMP_OVERLOAD       0x91 	// 145
#define HEPARIN_CALCIUM_MOTOR_BLOCKED       0x92 	// 146
#define SYRINGE_NOT_PRESENT                 0x93 	// 147
#define SYRINGE_EMPTY                       0x94 	// 148
#define BLOOD_PUMP_STOPPED                  0x95 	// 149
#define PWR_MON_DATA_CORRUPTED              0x96 	// 150
#define DIALYSATE_FLOW_MISMATCH             0x97 	// 151
#define INFUSION_FLOW_MISMATCH              0x98 	// 152
#define UF_FLOW_MISMATCH                    0x99 	// 153
#define FILLING_TIMEOUT                     0x9a 	// 154
#define TEST_COCU_FAILED                    0x9b 	// 155
#define TEST_WEIGHT_FAILED                  0x9c 	// 156
#define VAR_WEI_UF                          0x9d 	// 157
#define VAR_WEI_INF                         0x9e 	// 158
#define TREATMENT_PARA_OUT_OF_RANGE         0x9f 	// 159
#define WRN_UI                              0xa0 	// 160
#define WRN_OVE_BLD_2                       0xa1 	// 161  //AVOID REDEFINITION IN VETSMART
#define WARNING_BLOOD_PUMP_STOPPED          0xa2 	// 162
#define WRN_TREATMENT_COMPLETED             0xa3 	// 163
#define CRRT_ALA_UNUSED_04                  0xa4 	// 164
#define CRRT_ALA_UNUSED_05                  0xa5 	// 165
#define CRRT_ALA_UNUSED_06                  0xa6 	// 166
#define CRRT_ALA_UNUSED_07                  0xa7 	// 167
#define CRRT_ALA_UNUSED_08                  0xa8 	// 168
#define CRRT_ALA_UNUSED_09                  0xa9 	// 169
#define CRRT_ALA_UNUSED_10                  0xaa 	// 170
#define CRRT_ALA_UNUSED_11                  0xab 	// 171
#define CRRT_ALA_UNUSED_12                  0xac 	// 172
#define CRRT_ALA_UNUSED_13                  0xad 	// 173
#define CRRT_ALA_UNUSED_14                  0xae 	// 174
#define CRRT_ALA_UNUSED_15                  0xaf 	// 175
#define CRRT_ALA_UNUSED_16                  0xb0 	// 176
#define CRRT_ALA_UNUSED_17                  0xb1 	// 177
#define CRRT_ALA_UNUSED_18                  0xb2 	// 178
#define CRRT_ALA_UNUSED_19                  0xb3 	// 179
#define CRRT_ALA_UNUSED_20                  0xb4 	// 180
#define CRRT_ALA_UNUSED_21                  0xb5 	// 181
#define CRRT_ALA_UNUSED_22                  0xb6 	// 182
#define CRRT_ALA_UNUSED_23                  0xb7 	// 183
#define CRRT_ALA_UNUSED_24                  0xb8 	// 184
#define CRRT_ALA_UNUSED_25                  0xb9 	// 185
#define CRRT_ALA_UNUSED_26                  0xba 	// 186
#define CRRT_ALA_UNUSED_27                  0xbb 	// 187
#define CRRT_ALA_UNUSED_28                  0xbc 	// 188
#define CRRT_ALA_UNUSED_29                  0xbd 	// 189
#define CRRT_ALA_UNUSED_30                  0xbe 	// 190
#define CRRT_ALA_UNUSED_31                  0xbf 	// 191


/**
 *bit-field per la gestione allarmi
 */
struct alarms_bitf_t
{
    uint16_t pro_not_run                         : 1;
    uint16_t con_not_run                         : 1;
    uint16_t use_not_run                         : 1;
    uint16_t wei_pcb_not_run                     : 1;
    uint16_t prs_pcb_not_run                     : 1;
    uint16_t ui_critical_error                   : 1;
    uint16_t pre_art_dif                         : 1;
    uint16_t pre_fil_dif                         : 1;
     //8
    uint16_t pre_ven_dif                         : 1;
    uint16_t pre_uf_dif                          : 1;
    uint16_t wei_inf_diff                        : 1;
    uint16_t wei_uf_diff                         : 1;
    uint16_t wei_cit_diff                        : 1;
    uint16_t air_tst_abs                         : 1;
    uint16_t bld_tst_abs                         : 1;
    uint16_t data_error                          : 1;
     //16
    uint16_t cal_data_error                      : 1;
    uint16_t tst_rel_cit                         : 1;
    uint16_t tst_rel_epa                         : 1;
    uint16_t analogic_voltage                    : 1;
    uint16_t battery_voltage_too_low             : 1;
    uint16_t battery_recharge_failure            : 1;
    uint16_t tmp_inf_diff                        : 1;
    uint16_t tmp_dia_diff                        : 1;
     //24
    uint16_t fatal_error                         : 1;
    uint16_t pressures_uncalibrated              : 1;
    uint16_t pressurization_failed               : 1;
    uint16_t excessive_venous_leakage            : 1;
    uint16_t pre_art_min                         : 1;
    uint16_t pre_art_blocked                     : 1;
    uint16_t pre_art_max                         : 1;
    uint16_t pre_art_not_zero                    : 1;
     //32
    uint16_t pre_art_disc                        : 1;
    uint16_t pre_art_uncalibrated                : 1;
    uint16_t pre_art_data_corrupted              : 1;
    uint16_t pre_fil_min                         : 1;
    uint16_t pre_fil_blocked                     : 1;
    uint16_t pre_fil_max                         : 1;
    uint16_t pre_fil_not_zero                    : 1;
    uint16_t pre_fil_disc                        : 1;
     //40
    uint16_t pre_fil_uncalibrated                : 1;
    uint16_t pre_fil_data_corrupted              : 1;
    uint16_t pre_ven_min                         : 1;
    uint16_t pre_ven_blocked                     : 1;
    uint16_t pre_ven_max                         : 1;
    uint16_t pre_ven_uncalibrated                : 1;
    uint16_t pre_ven_data_corrupted              : 1;
    uint16_t pre_ven_not_zero                    : 1;
     //48
    uint16_t pre_uf_min                          : 1;
    uint16_t pre_uf_blocked                      : 1;
    uint16_t pre_uf_max                          : 1;
    uint16_t pre_uf_not_zero                     : 1;
    uint16_t pre_uf_disc                         : 1;
    uint16_t pre_uf_uncalibrated                 : 1;
    uint16_t pre_uf_data_corrupted               : 1;
    uint16_t pre_drop_max                        : 1;
     //56
    uint16_t pre_max_tmp                         : 1;
    uint16_t inlet_dome_open                     : 1;
    uint16_t pre_filter_dome_open                : 1;
    uint16_t uf_dome_open                        : 1;
    uint16_t wei_inf_not_zero                    : 1;
    uint16_t wei_inf_uncalibrated                : 1;
    uint16_t wei_inf_data_corrupted              : 1;
    uint16_t wei_inf_disconnected                : 1;
     //64
    uint16_t wei_inf_excessive                   : 1;
    uint16_t wei_inf_variation                   : 1;
    uint16_t wei_uf_not_zero                     : 1;
    uint16_t wei_uf_uncalibrated                 : 1;
    uint16_t wei_uf_data_corrupted               : 1;
    uint16_t wei_uf_disconnected                 : 1;
    uint16_t wei_uf_excessive                    : 1;
    uint16_t wei_uf_variation                    : 1;
     //72
    uint16_t wei_cit_not_zero                    : 1;
    uint16_t wei_cit_uncalibrated                : 1;
    uint16_t wei_cit_data_corrupted              : 1;
    uint16_t wei_cit_disconnected                : 1;
    uint16_t wei_cit_excessive                   : 1;
    uint16_t wei_cit_variation                   : 1;
    uint16_t uf_bag_empty                        : 1;
    uint16_t uf_bag_full                         : 1;
     //80
    uint16_t excessive_weight_variance           : 1;
    uint16_t excessive_dialysate_variance        : 1;
    uint16_t no_air_alarm                        : 1;
    uint16_t air_ven_lin                         : 1;
    uint16_t air_test_absent                     : 1;
    uint16_t air_test_failed                     : 1;
    uint16_t bld_tub_present                     : 1;
    uint16_t blood_leakage                       : 1;
     //88
    uint16_t bld_amb_lig                         : 1;
    uint16_t bld_tub_ins                         : 1;
    uint16_t bld_test_failed                     : 1;
    uint16_t bld_test_absent                     : 1;
    uint16_t bld_tx_ove                          : 1;
    uint16_t bld_data_corrupted                  : 1;
    uint16_t level_test_failed                   : 1;
    uint16_t level_test_absent                   : 1;
     //96
    uint16_t low_level_adjustement_failed        : 1;
    uint16_t high_level_adjustement_failed       : 1;
    uint16_t line_selector_driver_fault          : 1;
    uint16_t line_selector_not_closed            : 1;
    uint16_t line_selector_not_opened            : 1;
    uint16_t temp_inf_data_corrupted             : 1;
    uint16_t temp_dial_data_corrupted            : 1;
    uint16_t high_temperature_inf                : 1;
     //104
    uint16_t high_temperature_dia                : 1;
    uint16_t heater_inf_relay_fault              : 1;
    uint16_t heater_dia_relay_fault              : 1;
    uint16_t low_infusion_temperature            : 1;
    uint16_t high_infusion_temperature           : 1;
    uint16_t low_dialysate_temperature           : 1;
    uint16_t high_dialysate_temperature          : 1;
    uint16_t clamp_relay_fault                   : 1;
     //112
    uint16_t clamp_driver_fault                  : 1;
    uint16_t clamp_not_closed                    : 1;
    uint16_t clamp_not_opened                    : 1;
    uint16_t blood_pump_door_opened              : 1;
    uint16_t blood_pump_relay_fault              : 1;
    uint16_t blood_pump_driver_fault             : 1;
    uint16_t blood_pump_speed_error              : 1;
    uint16_t blood_pump_not_stopped              : 1;
     //120
    uint16_t blood_pump_not_running              : 1;
    uint16_t infusion_pump_door_opened           : 1;
    uint16_t infusion_pump_relay_fault           : 1;
    uint16_t infusion_pump_driver_fault          : 1;
    uint16_t infusion_pump_speed_error           : 1;
    uint16_t infusion_pump_not_stopped           : 1;
    uint16_t infusion_pump_not_running           : 1;
    uint16_t dialysate_pump_door_opened          : 1;
     //128
    uint16_t dialysate_pump_relay_fault          : 1;
    uint16_t dialysate_pump_driver_fault         : 1;
    uint16_t dialysate_pump_speed_error          : 1;
    uint16_t dialysate_pump_not_stopped          : 1;
    uint16_t dialysate_pump_not_running          : 1;
    uint16_t citrate_pump_door_opened            : 1;
    uint16_t citrate_pump_relay_fault            : 1;
    uint16_t citrate_pump_driver_fault           : 1;
     //136
    uint16_t citrate_pump_speed_error            : 1;
    uint16_t citrate_pump_not_stopped            : 1;
    uint16_t citrate_pump_not_running            : 1;
    uint16_t uf_pump_door_opened                 : 1;
    uint16_t uf_pump_relay_fault                 : 1;
    uint16_t uf_pump_driver_fault                : 1;
    uint16_t uf_pump_speed_error                 : 1;
    uint16_t uf_pump_not_stopped                 : 1;
     //144
    uint16_t uf_pump_not_running                 : 1;
    uint16_t heparin_calcium_pump_overload       : 1;
    uint16_t heparin_calcium_motor_blocked       : 1;
    uint16_t syringe_not_present                 : 1;
    uint16_t syringe_empty                       : 1;
    uint16_t blood_pump_stopped                  : 1;
    uint16_t pwr_mon_data_corrupted              : 1;
    uint16_t dialysate_flow_mismatch             : 1;
     //152
    uint16_t infusion_flow_mismatch              : 1;
    uint16_t uf_flow_mismatch                    : 1;
    uint16_t filling_timeout                     : 1;
    uint16_t test_cocu_failed                    : 1;
    uint16_t test_weight_failed                  : 1;
    uint16_t var_wei_uf                          : 1;
    uint16_t var_wei_inf                         : 1;
    uint16_t treatment_para_out_of_range         : 1;
     //160
    uint16_t wrn_ui                              : 1;
    uint16_t wrn_ove_bld                         : 1;
    uint16_t warning_blood_pump_stopped          : 1;
    uint16_t wrn_treatment_completed             : 1;
    uint16_t crrt_ala_unused_04                  : 1;
    uint16_t crrt_ala_unused_05                  : 1;
    uint16_t crrt_ala_unused_06                  : 1;
    uint16_t crrt_ala_unused_07                  : 1;
     //168
    uint16_t crrt_ala_unused_08                  : 1;
    uint16_t crrt_ala_unused_09                  : 1;
    uint16_t crrt_ala_unused_10                  : 1;
    uint16_t crrt_ala_unused_11                  : 1;
    uint16_t crrt_ala_unused_12                  : 1;
    uint16_t crrt_ala_unused_13                  : 1;
    uint16_t crrt_ala_unused_14                  : 1;
    uint16_t crrt_ala_unused_15                  : 1;
     //176
    uint16_t crrt_ala_unused_16                  : 1;
    uint16_t crrt_ala_unused_17                  : 1;
    uint16_t crrt_ala_unused_18                  : 1;
    uint16_t crrt_ala_unused_19                  : 1;
    uint16_t crrt_ala_unused_20                  : 1;
    uint16_t crrt_ala_unused_21                  : 1;
    uint16_t crrt_ala_unused_22                  : 1;
    uint16_t crrt_ala_unused_23                  : 1;
     //184
    uint16_t crrt_ala_unused_24                  : 1;
    uint16_t crrt_ala_unused_25                  : 1;
    uint16_t crrt_ala_unused_26                  : 1;
    uint16_t crrt_ala_unused_27                  : 1;
    uint16_t crrt_ala_unused_28                  : 1;
    uint16_t crrt_ala_unused_29                  : 1;
    uint16_t crrt_ala_unused_30                  : 1;
    uint16_t crrt_ala_unused_31                  : 1;
     //192
}__attribute__ ((packed));

#define NUM_ALARMS	192
#define NO_ALARM	NUM_ALARMS
#endif
