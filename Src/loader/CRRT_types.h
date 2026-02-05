/**
@file		CRRT_types.h

@brief		CRRT types definition

@author		Nicola Molinazzi
@date		16/07/2012
@version	01.00
*/

#ifndef _CRRT_TYPES_H_
#define _CRRT_TYPES_H_

#define _CRRT_TYPES_
//#define _SSP_TYPES_

#include "Global.h"
#include "crrt_tests.h"

#ifdef _CRRT_TYPES_
#include "crrt_alarms.h"
//#include "crrt_treatment_steps.h"
#endif

#ifdef _SSP_TYPES_
#include "sps_alarms.h"
#include "sps_production_steps.h"
#endif

/*************************************************************************/
/************************* COMMON CRRT-SPS *******************************/
/*************************************************************************/
/**
 * List of calibration cmds
 */
enum calibrationCmds_t
{
	ECAL_CMD_gainAndOffsetAndStatus = 0,
	ECAL_CMD_calibrateFirstPoint,
	ECAL_CMD_calibrateSecondPoint,
	ECAL_CMD_exitAbort,
	ECAL_CMD_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum calibrationCmds_t 	calibrationCmds_t;
#endif

/**
Calibration States
*/
enum calibrationStates_t
{
    ECAL_ST_waitToCalibrateFirstPoint = 0,
    ECAL_ST_calibratingFirstPoint,
    ECAL_ST_waitToCalibrateSecondPoint,
    ECAL_ST_calibratingSecondPoint,
    ECAL_ST_calibrationEndedCorrectly,
    ECAL_ST_calibrationSavedCorrectly,
    ECAL_ST_calibrationCanNotBeSaved,
    ECAL_ST_calibrationFirstPointStabilityError,
    ECAL_ST_calibrationSecondPointStabilityError,
    ECAL_ST_calibrationDistanceError,
    ECAL_ST_NUM

}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum calibrationStates_t 	calibrationStates_t;
#endif
/**
 * CRRT CONTROLLER POWER SUPPLY VALUES TO CHECK
 */
enum powerSupply_C_t
{
	EPWR_SUP_C_m12V = 0,
	EPWR_SUP_C_12V ,
	EPWR_SUP_C_24V,
	EPWR_SUP_C_MON_24V_BATT,
	EPWR_SUP_C_3V,
	EPWR_SUP_C_24V_PC,
	EPWR_SUP_C_5V_PROTECTIVE,
	EPWR_SUP_C_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum powerSupply_C_t 	powerSupply_C_t;
#endif

/**
 * CRRT PROTECTIVE POWER SUPPLY VALUES TO CHECK
 */
enum powerSupply_P_t
{
	EPWR_SUP_P_24V_MOT = 0,
	EPWR_SUP_P_3V,
	EPWR_SUP_P_5V_CONTROLLER,
	EPWR_SUP_P_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum powerSupply_P_t 	powerSupply_P_t;
#endif

/**
 * Scales' board Monitored Voltages
 */
enum scalesPCB_monitoredVolages_t
{
	ESCALE_PCB_monV_Vref_ADC_A = 0,
	ESCALE_PCB_monV_Vref_ADC_P,
	ESCALE_PCB_monV_pVCC1, //+VCC1
	ESCALE_PCB_monV_mVCC1, //-VCC1
	ESCALE_PCB_monV_pVCC2, //+VCC2
	ESCALE_PCB_monV_mVCC2, //-VCC2
	ESCALE_PCB_monV_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum scalesPCB_monitoredVolages_t 	scalesPCB_monitoredVolages_t;
#endif

/**
 * Prs' board Monitored Voltages
 */
enum prsPCB_monitoredVolages_t
{
	EPRS_PCB_monV_Vref_ADC_A = 0,
	EPRS_PCB_monV_3_3V, //3.3V
	EPRS_PCB_monV_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum prsPCB_monitoredVolages_t 	prsPCB_monitoredVolages_t;
#endif

/**
 * Conductivities' board Monitored Voltages
 */
enum condPCB_monitoredVolages_t
{
    ECOND_PCB_monV_Vref_ADC_P = 0,
    ECOND_PCB_monV_Vref_ADC_A,
    ECOND_PCB_monV_p5V,
    ECOND_PCB_monV_m5V,
    ECOND_PCB_monV_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum condPCB_monitoredVolages_t 	condPCB_monitoredVolages_t;
#endif

/**
 * Motor status
 */
enum motorStatus_t
{
	EMOT_ST_STOP = 0,
	EMOT_ST_START_CLOCKWISE,
	EMOT_ST_START_ANTI_CLOCKWISE,
	EMOT_ST_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum motorStatus_t 	motorStatus_t;
#endif

/**
  * Motor's kind of controls
  */
enum motor_controls_t
{
    EMOT_CTRL_POS = 0,
    EMOT_CTRL_VEL,
    EMOT_CTRL_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum motor_controls_t 	motor_controls_t;
#endif

/**
 * Syringe (heparine) movement types
 */
enum syringe_movements_t
{
	ESYRINGE_MOVE_STOP = 0,
	ESYRINGE_MOVE_FORWARD,
	ESYRINGE_MOVE_BACKWARD,
	ESYRINGE_MOVE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum syringe_movements_t 	syringe_movements_t;
#endif

/**
  * Motor control position or velocity
  */
union motor_ctrl_posVel
{
    int16_t speed;    // ml/min ors ml/h or 0.1*ml/h, according to the motor
    int16_t position; //deci grad
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef union motor_ctrl_posVel 	motor_ctrl_posVel;
#endif

/**
 * Motor type
 */
struct motor_t
{
	motorStatus_t status;
    motor_controls_t ctrl_type;
    motor_ctrl_posVel ctrlPosVel;

#ifdef __cplusplus
    bool operator ==( const motorStatus_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(motorStatus_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const motorStatus_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(motorStatus_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    motor_t()
    {
        for( uint16_t i = 0; i < sizeof(motor_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    motor_t(const motor_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(motor_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

	motor_t& operator= (const motor_t &__mot)
	{
        for( uint16_t i = 0; i < sizeof(motor_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__mot)+i));
		return *this;
	}

	void reset()
	{
		for( uint16_t i = 0; i < sizeof(motor_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
};
#ifdef __cplusplus
#else
typedef struct stepperMotor_t 	stepperMotor_t;
#endif

/**
 * Error codes for the Stop command
 */
enum stop_error_codes_t
{
    ESTOP_NO_ERR,
    ESTOP_WARNING,
    ESTOP_CRITICAL_ERROR
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum stop_error_codes_t 	stop_error_codes_t;
#endif

/**
  * Stepper motor debug parameters
  */
enum stepper_motor_info_para_t
{
    ESTEPPER_INFO_MOTOR_ID = 0,
    ESTEPPER_INFO_MOT_FSM_STATUS,
    ESTEPPER_INFO_iMOT,
    ESTEPPER_INFO_vMOT,
    ESTEPPER_INFO_VEL_ABS,
    ESTEPPER_INFO_POS_ABS,
    ESTEPPER_INFO_POS_REAL,
    ESTEPPER_INFO_POS_MAGNET,
    ESTEPPER_INFO_VEL_TARGET,
    ESTEPPER_INFO_POS_TARGET,
    ESTEPPER_INFO_CNT_PACK,
    ESTEPPER_INFO_CNT_PACK_100,
    ESTEPPER_INFO_CNT_PACK_101,
    ESTEPPER_INFO_CNT_PACK_102,
    ESTEPPER_INFO_CNT_PACK_103,
    ESTEPPER_INFO_STATUS,
    ESTEPPER_INFO_ROTOR_ZERO_POS,
    ESTEPPER_INFO_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum stepper_motor_info_para_t 	stepper_motor_info_para_t;
#endif

struct stepper_motor_info_t
{
    uint16_t info[ESTEPPER_INFO_NUM];

#ifdef __cplusplus
    stepper_motor_info_t()
    {
        for( uint16_t i = 0; i < sizeof(stepper_motor_info_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    stepper_motor_info_t(const stepper_motor_info_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(stepper_motor_info_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    stepper_motor_info_t& operator= (const stepper_motor_info_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(stepper_motor_info_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(stepper_motor_info_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct stepper_motor_info_t 	stepper_motor_info_t;
#endif

/*************************************************************************/
/******************************* CRRT ************************************/
/*************************************************************************/
#ifdef _CRRT_TYPES_

/**
 * Buzzer maximum volume (duty cycle ON)
 */
#define CRRT_BUZZER_VOLUME_MAX	10.5

/**
 * Buzzer minimum volume (duty cycle OFF)
 */
#define CRRT_BUZZER_VOLUME_MIN	0.5

/**
 * Buzzer valume step
 */
#define CRRT_BUZZER_VOLUME_STEP	0.5

#ifdef __cplusplus
#else
typedef struct alarms_bitf_t 		alarms_bitf_t;
#endif
struct alarms_t
{
    alarms_bitf_t m_alarms;

#ifdef __cplusplus
    bool operator ==( const alarms_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(alarms_bitf_t); i++)
            if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i)))
                return false;
        return true;
    }

    bool operator !=(const alarms_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(alarms_bitf_t); i++)
            if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i)))
                return true;
        return false;
    }

    alarms_t()
    {
        for( uint16_t i = 0; i < sizeof(alarms_bitf_t); i++)
            *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = 0;
    }

    alarms_t(const alarms_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(alarms_bitf_t); i++)
            *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i));
    }

    alarms_t& operator= (const alarms_t &__a)
    {
        for( uint16_t i = 0; i < sizeof(alarms_t); i++)
           *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = *((uint8_t*)(((uint8_t*)&__a.m_alarms)+i));
        return *this;
    }

    bool at(uint32_t _id)
    {
        uint32_t byteNum = _id / 8;
        uint32_t bitNum = _id % 8;
        return ((*(((uint8_t*)(&m_alarms))+byteNum) & (0x01 << bitNum)) > 0);
    }

    void setON(uint32_t _id)
    {
        uint32_t byteNum = _id / 8;
        uint32_t bitNum = _id % 8;
        *(((uint8_t*)(&m_alarms))+byteNum) |= (0x01 << bitNum);
    }

    void setOFF(uint32_t _id)
    {
        uint32_t byteNum = _id / 8;
        uint32_t bitNum = _id % 8;
        *(((uint8_t*)(&m_alarms))+byteNum) &= ~(0x01 << bitNum);
    }

    void reset()
   	{
   		for( uint16_t i = 0; i < sizeof(alarms_t); i++)
   			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
   	}

    void OR(const alarms_t &__al1, const alarms_t &__al2)
    {
         for( uint16_t i = 0; i < sizeof(alarms_bitf_t); i++)
          *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = (*((uint8_t*)(((uint8_t*)(&__al1.m_alarms))+i))) | ((*((uint8_t*)(((uint8_t*)(&__al2.m_alarms))+i))));
    }

    uint16_t getFirstBitSet()
    {
        uint16_t i, k;
        uint16_t firstBit = NUM_ALARMS;
        uint8_t bit;
		for( i = 0; i < sizeof(alarms_bitf_t); i++)
		{
			if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != 0)
			{
				for(k = 0; k < 8; k++)
				{
					bit = (uint8_t) (0x01 << k);
					if( ( (*((uint8_t*)(((uint8_t*)(&m_alarms))+i))) & bit) == bit)
					{
						firstBit = i*8 +k;
						return firstBit;
					}
				}
			}
		}
        return firstBit;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct alarms_t 		alarms_t;
#endif

/**
  * Switches bit fields struct
  * 16 bits (2 bytes)
  * uint16_t alignment
  */
struct switches_bitF_t
{
	uint16_t dome1 : 1;
	uint16_t dome2 : 1;
	uint16_t dome3 : 1;
	uint16_t syringe_in : 1;
	uint16_t syringe_type : 3; //0=non id; 1=30ml; 2=50ml; 3 to 7=unused;
	uint16_t air_on : 1; //0=off; 1=on

	uint16_t selector : 2; //0: both open; 1:dialysate; 2: pre-dilution
    uint16_t electroclamp : 1;
    uint16_t dropperLevelLow : 1;
    uint16_t dropperLevelHigh : 1;
    uint16_t prs_dome1 : 1;			// if 1 the pressure on dome 1 is higher than 10 mmHg
    uint16_t prs_dome2 : 1;			// if 1 the pressure on dome 2 is higher than 10 mmHg
    uint16_t prs_dome3 : 1;			// if 1 the pressure on dome 3 is higher than 10 mmHg

    uint16_t lit_motor_blood : 1;
    uint16_t lit_motor_infusion : 1;
    uint16_t lit_motor_dial : 1;
    uint16_t lit_motor_uf : 1;
    uint16_t lit_motor_citrate : 1;
    uint16_t unused_01 : 1;
    uint16_t unused_02 : 1;
    uint16_t unused_03 : 1;

    uint16_t unused_04 : 1;
    uint16_t unused_05 : 1;
    uint16_t unused_06 : 1;
    uint16_t unused_07 : 1;
    uint16_t unused_08 : 1;
    uint16_t unused_09 : 1;
    uint16_t unused_10 : 1;
    uint16_t unused_11 : 1;

#ifdef __cplusplus
    switches_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    switches_bitF_t(const switches_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    switches_bitF_t& operator= (const switches_bitF_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
		return *this;
	}

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(switches_bitF_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
}__attribute__ ((packed));

#ifdef __cplusplus
#else
typedef struct switches_bitF_t 		switches_bitF_t;
#endif

/**
 * Treatment Status...
 */
enum treatmentStatus_t
{
	ETR_STATUS_idle = 0,
	ETR_STATUS_init,
	ETR_STATUS_active,
	ETR_STATUS_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum treatmentStatus_t 		treatmentStatus_t;
#endif

/**
 * Phase CRRT
 */
enum phases_t
{
    EPHASE_init = 0,
	EPHASE_idle,
	EPHASE_preparation,
	EPHASE_priming,
	EPHASE_add_priming,
	EPHASE_recirculation,
    EPHASE_connect,
	EPHASE_treatment,
	EPHASE_disconnect,
	EPHASE_service,
    EPHASE_loader,
	EPHASE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum phases_t 		phases_t;
#endif

/**
 * Treatments
 */
enum treatments_t
{
	ETREATMENT_none = 0,
	ETREATMENT_CVVH,
	ETREATMENT_CVVHD,
	ETREATMENT_CVVHDF,
    ETREATMENT_SCUF,
	ETREATMENT_PE,
	ETREATMENT_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum treatments_t 		treatments_t;
#endif

/**
 * Patient's Age
 */
enum patientAge_t
{
	EPATIENT_AGE_NONE = 0,
	EPATIENT_AGE_ADULT,
	EPATIENT_AGE_PEDIATRIC,
	EPATIENT_AGE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum patientAge_t 		patientAge_t;
#endif

/**
 * Anticoagulants type
 */
enum anticoagulants_t
{
	EANTICOAGULANT_NONE = 0,
	EANTICOAGULANT_HEPARYNE,
	EANTICOAGULANT_HEPA_BOLI,
	EANTICOAGULANT_CITRATE,
	EANTICOAGULANT_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum anticoagulants_t 		anticoagulants_t;
#endif

/**
 * Working Modes
 */
enum working_modes_t
{
    EWORK_MODE_NONE,
    EWORK_MODE_FLOW,
    EWORK_MODE_VOLUME,
    EWORK_MODE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum working_modes_t 		working_modes_t;
#endif

/**
 * Filter types
 */
enum filters_t
{
    EFILTER_NONE = 0,
	EFILTER_DRY,
	EFILTER_WET,
    EFILTER_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum filters_t 	filters_t;
#endif

/**
 * Dialysate flow direction
 */
enum bld_dial_flow_direction_t
{
	EBLD_DIAL_FLOW_NONE = 0,
	EBLD_DIAL_FLOW_COO_CUR,
	EBLD_DIAL_FLOW_COUNTER_CUR,
	EBLD_DIAL_FLOW_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum bld_dial_flow_direction_t 	bld_dial_flow_direction_t;
#endif

/**
 * CRRT's scales
 */
enum scales_t
{
	ESCALE_INF = 0,
	ESCALE_UF,
	ESCALE_CITRATE,
	ESCALE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum scales_t 	scales_t;
#endif

/**
 * CRRT's pressures
 */
enum pressures_t
{
	EPRS_ART = 0,
	EPRS_PRE,
    EPRS_VEN,
    EPRS_UF,
    EPRS_TMP,
	EPRS_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum pressures_t 	pressures_t;
#endif

#define REAL_PRS_NUM 4 /*< Real Pressures (NOT calculated)*/
#define CLACULATED_PRS_NUM 1 /*< Calculated Pressures*/

/**
 * CRRT's temperature
 */
enum temperature_t
{
	ETEMP_INF = 0,
	ETEMP_DIAL,
	ETEMP_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum temperature_t 	temperature_t;
#endif

/**
 * Fatal errors
 */
enum fatal_errors_t
{
	EFATAL_ERROR_NONE = 0,		// nessun allarme fatal error
	EFATAL_ERROR_01,			// error in DrvAIR_Ges
	EFATAL_ERROR_02,			// error in DrvBLD_Ges
	EFATAL_ERROR_03,			// error in DrvBLD_Ges
	EFATAL_ERROR_04,			// error in Ala_Pre
	EFATAL_ERROR_05,			// error in Ala_Ges
	EFATAL_ERROR_06,			// error in Ala_Kal
	EFATAL_ERROR_07,			// error in Ala_Loc
	EFATAL_ERROR_08,			// error in DrvCLA_Ges
	EFATAL_ERROR_09,			// error in DrvCLA_Ges
	EFATAL_ERROR_10,			// error in GES_CLAMP_SWITC
	EFATAL_ERROR_11,			// error in  DrvLEVEL_Ges
	EFATAL_ERROR_12,			// error in  DrvHeater_Ges
	EFATAL_ERROR_13,			// error in  DrvHeater_Ges
	EFATAL_ERROR_14,			// error in  DrvHeater_Ges
	EFATAL_ERROR_15,			// error in  DrvHeater_Ges
    EFATAL_ERROR_16,			// error in  DrvPump
    EFATAL_ERROR_17,			// error in  DrvPump
    EFATAL_ERROR_18,			// error in  Pmp_Ges
    EFATAL_ERROR_19,			// error in  Pmp_Ges
    EFATAL_ERROR_20,			// error in  relays manager

	EFATAL_ERROR_30 = 30,	// error in CRRT_readWriteCalValues

	EFATAL_ERROR_50 = 50,	// error in CRRT_manager
	EFATAL_ERROR_51,		// error in CRRT_manager
	EFATAL_ERROR_52,		// error in CRRT_manager
	EFATAL_ERROR_53,		// error in CRRT_manager
	EFATAL_ERROR_54,		// error in CRRT_manager
	EFATAL_ERROR_55,		// error in CRRT_manager
	EFATAL_ERROR_56,		// error in CRRT_manager

	EFATAL_ERROR_70 = 70, 	// fatal error fs_preparation
	EFATAL_ERROR_71, 		// fatal error fs_priming
	EFATAL_ERROR_72, 		// fatal error fs_add_priming
	EFATAL_ERROR_73, 		// fatal error fs_connect
	EFATAL_ERROR_74, 		// fatal error fs_reculation
	EFATAL_ERROR_75, 		// fatal error fs_treatment
	EFATAL_ERROR_76, 		// fatal error fs_disconnect
	EFATAL_ERROR_77, 		// fatal error fs_init
	EFATAL_ERROR_78, 		// fatal error fs_service

	EFATAL_ERROR_80 = 80,	// fatal error regulator
	EFATAL_ERROR_81,  		// fatal error regulator
	EFATAL_ERROR_82,			// fatal error regulator
	EFATAL_ERROR_83, 		// fatal error regulator
	EFATAL_ERROR_84, 		// fatal error regulator

	EFATAL_ERROR_90 = 90,	// fatal error pressioni
	EFATAL_ERROR_91,  		// fatal error pressioni

	EFATAL_ERROR_100 = 100,	// fatal error eparina
	EFATAL_ERROR_101  		// fatal error eparina



}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum fatal_errors_t 	fatal_errors_t;
#endif

/**
  * Treatment status parameters
  */
struct treatmentStatusPara_t
{
    treatments_t treatment;
    patientAge_t patientAge;
    anticoagulants_t anticoagulant;
    filters_t filter;
    bld_dial_flow_direction_t bldDialFlowDirection;
    working_modes_t working_mode;
    phases_t phase;

#ifdef __cplusplus
    treatmentStatusPara_t()
    {
        for( uint16_t i = 0; i < sizeof(treatmentStatusPara_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    treatmentStatusPara_t(const treatmentStatusPara_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(treatmentStatusPara_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    treatmentStatusPara_t& operator= (const treatmentStatusPara_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(treatmentStatusPara_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(treatmentStatusPara_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct treatmentStatusPara_t 	treatmentStatusPara_t;
#endif

/**
 * Treatment parameters
 */
struct treatmentParameters_t
{
    uint16_t totalReplacement __attribute__ ((packed)); //ID=1
    uint16_t totalDialysateFluid __attribute__ ((packed)); //ID=2
    uint16_t totalPlasmaInfused __attribute__ ((packed)); //ID=3
    uint16_t replacementFLow __attribute__ ((packed)); //ID=15
    uint16_t dialysateFlow __attribute__ ((packed)); //ID=16
    uint16_t prePostPerc __attribute__ ((packed)); //ID=18
    uint16_t plasmaUfBldPerc __attribute__ ((packed)); //ID=19
    uint16_t plasmaReplUfPerc __attribute__ ((packed)); //ID=20
    uint16_t weightLossRate __attribute__ ((packed)); //ID=17
    uint16_t treatmentTime_hh __attribute__ ((packed)); //ID=4
    uint16_t treatmentTime_mm __attribute__ ((packed)); //ID=4 bis
    uint16_t totalWeightLoss __attribute__ ((packed)); //ID=5
    uint16_t TMP_maximum __attribute__ ((packed)); //ID=6
    uint16_t heparineFlow __attribute__ ((packed)); //ID=7
    uint16_t heparineBolus __attribute__ ((packed)); //ID=8
    uint16_t heparinePeriod __attribute__ ((packed)); //ID=9
    uint16_t calciumFlow __attribute__ ((packed)); //ID=10
    uint16_t citrateFlow __attribute__ ((packed)); //ID=11
    uint16_t replacementTemp __attribute__ ((packed)); //ID=12
    uint16_t dialysateTemp __attribute__ ((packed)); //ID=13
    uint16_t plasmaTemp __attribute__ ((packed)); //ID=14

#ifdef __cplusplus
    treatmentParameters_t()
    {
        for( uint16_t i = 0; i < sizeof(treatmentParameters_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    treatmentParameters_t(const treatmentParameters_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(treatmentParameters_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    treatmentParameters_t& operator= (const treatmentParameters_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(treatmentParameters_t); i++)
                *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
		return *this;
	}

    void reset()
   	{
   		for( uint16_t i = 0; i < sizeof(treatmentParameters_t); i++)
   			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
   	}
#endif
};
#ifdef __cplusplus
#else
typedef struct treatmentParameters_t 	treatmentParameters_t;
#endif

/**
  * CRRT working status
  */
struct CRRT_working_status_t
{
    treatmentStatus_t treatmentStatus;
    treatments_t treatment;
    patientAge_t patientAge;
    filters_t filter;
    bld_dial_flow_direction_t bldDialFlowDirection;
    anticoagulants_t anticoagulant;
    working_modes_t working_mode;
    phases_t phase;
    fatal_errors_t fatalError;
    uint8_t alarmsAreActive;
    uint16_t step __attribute__ ((packed));

#ifdef __cplusplus
    bool operator ==( const CRRT_working_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const CRRT_working_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    CRRT_working_status_t()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    CRRT_working_status_t(const CRRT_working_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    CRRT_working_status_t& operator= (const CRRT_working_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_working_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct CRRT_working_status_t 	CRRT_working_status_t;
#endif

/**
  * CRRT curretn values
  */
struct CRRT_current_values_t
{
    uint8_t infPumpSpeed; // 0 - 200 ml/min
    uint8_t dialPumpSpeed; // 0 - 200 ml/min
    uint8_t ufPumpSpeed; // 0 - 200 ml/min
    uint8_t heparynePumpSpeed; // 0 - 150 ml * 0,1min
    uint16_t bloodPumpSpeed __attribute__ ((packed)); //0 - 400 ml/min; 0 = off
    uint16_t ciratePumpSpeed __attribute__ ((packed)); // 0 - 600 ml/min
    int16_t prs[EPRS_NUM] __attribute__ ((packed)); //-350 - 400
    int16_t scale[ESCALE_NUM] __attribute__ ((packed)); //0 - 20000
    int16_t temp[ETEMP_NUM]	 __attribute__ ((packed));
    int16_t bld	__attribute__ ((packed));
    uint16_t newBloodPumpTargetSpeed __attribute__ ((packed)); // 0 - 400
    uint16_t bloodPumpTargetSpeed __attribute__ ((packed)); // 0 - 400
    switches_bitF_t switchesStatus;
    float buzzerVolume __attribute__ ((packed));

#ifdef __cplusplus
    bool operator ==( const CRRT_current_values_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const CRRT_current_values_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    CRRT_current_values_t()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    CRRT_current_values_t(const CRRT_current_values_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    CRRT_current_values_t& operator= (const CRRT_current_values_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_current_values_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct CRRT_current_values_t 	CRRT_current_values_t;
#endif

/**
  * CRRT totalizers
  */
struct CRRT_totalizers_t
{
    uint16_t elapsedTime __attribute__ ((packed)); //minutes: 0 - 4320
    uint16_t removedVolume __attribute__ ((packed)); //0 - 10000
    uint16_t removingSpeed __attribute__ ((packed));//0 - 1000
    uint16_t treatmentDuration __attribute__ ((packed)); // minutes: 0 - 4320
    uint16_t totTreatedBlood __attribute__ ((packed)); //dl 0 - 2000
    uint16_t totPreInfusion __attribute__ ((packed)); //g  0 - 20000
    uint16_t totPostInfusion __attribute__ ((packed)); //g  0 - 20000
    uint16_t totDialysate __attribute__ ((packed)); //g 0-20000
    uint16_t totUF __attribute__ ((packed)); //g 0-20000
    uint16_t totSyringe __attribute__ ((packed)); //0,1ml 0 - xx
    uint16_t totCitrate __attribute__ ((packed)); //0,1ml 0 - xx

#ifdef __cplusplus
    bool operator ==( const CRRT_totalizers_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const CRRT_totalizers_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    CRRT_totalizers_t()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    CRRT_totalizers_t(const CRRT_totalizers_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    CRRT_totalizers_t& operator= (const CRRT_totalizers_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_totalizers_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct CRRT_totalizers_t 	CRRT_totalizers_t;
#endif

/**
 * CRRT status
 */
struct CRRT_status_t
{
    CRRT_working_status_t workingStatus;
    CRRT_current_values_t currValues;
    CRRT_totalizers_t totalizers;
    alarms_t alarms;

#ifdef __cplusplus
    bool operator ==( const CRRT_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const CRRT_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    CRRT_status_t()
    {
        for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    CRRT_status_t(const CRRT_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    CRRT_status_t& operator= (const CRRT_status_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
		return *this;
	}

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(CRRT_status_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
};
#ifdef __cplusplus
#else
typedef struct CRRT_status_t 	CRRT_status_t;
#endif


/**
 * Pressure Limits
 */
struct pressure_limits_t
{
    int16_t infLim[EPRS_NUM] __attribute__ ((packed));
    int16_t supLim[EPRS_NUM] __attribute__ ((packed));
    int16_t range[EPRS_NUM] __attribute__ ((packed));

#ifdef __cplusplus
    pressure_limits_t()
    {
        for( uint16_t i = 0; i < sizeof(pressure_limits_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    pressure_limits_t(const pressure_limits_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pressure_limits_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    pressure_limits_t& operator= (const pressure_limits_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pressure_limits_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(pressure_limits_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct pressure_limits_t 	pressure_limits_t;
#endif

/**
 * Motors' IDs
 */
enum motors_IDs_t
{
    EMOTOR_ID_NONE = 0,
    EMOTOR_ID_BLOOD,
    EMOTOR_ID_INFUSION,
    EMOTOR_ID_DIAL,
    EMOTOR_ID_UF,
    EMOTOR_ID_CITRATE,
    EMOTOR_ID_SYRINGE,
    EMOTOR_ID_NUM
}__attribute__ ((packed));
#define MOTOR_ID_NUM 	7
#ifdef __cplusplus
#else
typedef enum motor_ID_t 	motor_ID_t;
#endif

/**
  * Motors' speed limits
  */
#define CRRT_MOT_BLOOD_MAX_SPEED        500 /*< Blood pump's max speed: 500 ml/min*/
#define CRRT_MOT_BLOOD_MIN_SPEED        0   /*< Blood pump's min speed: 0 ml/min*/

#define CRRT_MOT_INF_MAX_SPEED          200 /*< Infusion pump's max speed: 200 ml/min*/
#define CRRT_MOT_INF_MIN_SPEED          0   /*< Infusion pump's min speed: 0 ml/min*/

#define CRRT_MOT_DIAL_MAX_SPEED         200 /*< Dialysate pump's max speed: 200 ml/min*/
#define CRRT_MOT_DIAL_MIN_SPEED         0   /*< Dialysate pump's min speed: 0 ml/min*/

#define CRRT_MOT_UF_MAX_SPEED           200 /*< Ultrafiltrate pump's max speed: 200 ml/min*/
#define CRRT_MOT_UF_MIN_SPEED           0   /*< Ultrafiltrate pump's min speed: 0 ml/min*/

#define CRRT_MOT_CITRATE_MAX_SPEED       200 /*< Citrate pump's max speed: 200 ml/h*/
#define CRRT_MOT_CITRATE_MIN_SPEED       0   /*< Citrate pump's min speed: 0 ml/h*/

#define CRRT_MOT_SYRINGE_MAX_SPEED       60  /*< Syringe pump's max speed: 60 (0.1*ml)/min*/
#define CRRT_MOT_SYRINGE_MIN_SPEED       0   /*< Syringe pump's min speed: 0 ml/min*/

/**
 * List of channels that can be calibrated
 */
enum channelsToCalibrate_t
{
	ECH_TO_CAL_none = 0,
	ECH_TO_CAL_scale_INF,
	ECH_TO_CAL_scale_UF,
	ECH_TO_CAL_scale_CITRATE,
	ECH_TO_CAL_prs_ART,
	ECH_TO_CAL_prs_PRE,
	ECH_TO_CAL_prs_VEN,
	ECH_TO_CAL_prs_UF,
	ECH_TO_CAL_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum channelsToCalibrate_t	channelsToCalibrate_t;
#endif

/**
 * Gain and Offset Struct
 */
struct gainAndOffset_t
{
    float scaleGain[ESCALE_NUM] __attribute__((packed));
    short scaleOffset[ESCALE_NUM] __attribute__((packed));
    float prsGain[REAL_PRS_NUM] __attribute__((packed));
    short prsOffset[REAL_PRS_NUM] __attribute__((packed));
};
#ifdef __cplusplus
#else
typedef struct gainAndOffset_t 	gainAndOffset_t;
#endif

/**
 * Motor Current monitor
 */
enum motorCurrentMonitor_t
{
	EISENSE_MOT_ELECTROCLAMP_1,
	EISENSE_MOT_ELECTROCLAMP_2,
	EISENSE_MOT_SWT_CLAMP,
	EISENSE_MOT_VAC_PUMP,
	EISENSE_MOT_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum motorCurrentMonitor_t 	motorCurrentMonitor_t;
#endif

/**
 * Service power supply information
 */
struct servicePowerSupplyStruct_t
{
    int16_t pwrSupMonitor_C[EPWR_SUP_C_NUM] __attribute__((packed)); /*< Controller power supply monitor*/
    int16_t pwrSupMonitor_P[EPWR_SUP_P_NUM] __attribute__((packed)); /*< Protective power supply monitor*/

    int16_t scaleBoardPwrSupMonitor[ESCALE_PCB_monV_NUM] __attribute__((packed)); /*< Scale PCB power supply monitor*/
    int16_t prsBoardPwrSupMonitor[EPRS_PCB_monV_NUM] __attribute__((packed));   /*< Prs PCB power supply monitor*/

    int16_t grid_frequency __attribute__((packed));

#ifdef __cplusplus
    bool operator ==( const servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    servicePowerSupplyStruct_t()
    {
        for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    servicePowerSupplyStruct_t(const servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    servicePowerSupplyStruct_t& operator= (const servicePowerSupplyStruct_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
		return *this;
	}

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(servicePowerSupplyStruct_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
};
#ifdef __cplusplus
#else
typedef struct servicePowerSupplyStruct_t 	servicePowerSupplyStruct_t;
#endif

/**
 * Service pwm struct
 */
struct pwmStruct_t
{
	uint8_t vacumPump_Pwm;
	uint8_t vacumPump_Dir;
	uint8_t switchClamp_Pwm;
	uint8_t switchClamp_Dir;
	uint8_t electroClamp_Pwm;
	uint8_t electroClamp_En;

#ifdef __cplusplus
    bool operator ==( const pwmStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const pwmStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    pwmStruct_t()
    {
        for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    pwmStruct_t(const pwmStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    pwmStruct_t& operator= (const pwmStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(pwmStruct_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
};
#ifdef __cplusplus
#else
typedef struct pwmStruct_t 	pwmStruct_t;
#endif

/**
 * relays
 */
enum relays_t
{
    ERELAY_HEATER_DIAL = 0,
    ERELAY_HEATER_INF,
    ERELAY_SWITCH_CLAMP,
    ERELAY_PUMP_VACUM,
    ERELAY_PUMP_CITRATE,
    ERELAY_PUMP_HEPARIN,
    ERELAY_PUMP_BLOOD,
    ERELAY_PUMP_INF,
    ERELAY_PUMP_DIAL,
    ERELAY_PUMP_UF,
    ERELAY_ELECTROCLAMP,
    ERELAY_NUM
};
#ifdef __cplusplus
#else
typedef enum relays_t 	relays_t;
#endif

/**
  * Realys struct bit
  */
struct relays_bitF_t
{
    uint16_t heaterDial :1;
    uint16_t heaterInf :1;
    uint16_t switchClamp :1;
    uint16_t pumpVacum :1;
    uint16_t pumpCitrate :1;
    uint16_t pumpHeparin :1;
    uint16_t pumpBlood :1;
    uint16_t pumpInf :1;

    uint16_t pumpDial :1;
    uint16_t pumpUf :1;
    uint16_t electroclamp :1;
    uint16_t unused_11 : 1;
    uint16_t unused_12 : 1;
    uint16_t unused_13 : 1;
    uint16_t unused_14 : 1;
    uint16_t unused_15 : 1;

#ifdef __cplusplus
    bool operator ==( const relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    relays_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    relays_bitF_t(const relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    relays_bitF_t& operator= (const relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(relays_bitF_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct relays_bitF_t 	relays_bitF_t;
#endif

/**
 * Switch clamp possible position
 */
enum switch_clamp_pos_t
{
    ESWITCH_CLAMP_POS_OPEN,
    ESWITCH_CLAMP_POS_CLOSE_A,
    ESWITCH_CLAMP_POS_CLOSE_B,
    ESWITCH_CLAMP_POS_ERROR
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum switch_clamp_pos_t 	switch_clamp_pos_t;
#endif

/**
 * Electroclamp possible position
 */
enum electroclamp_pos_t
{
    EELECTROCLAMP_POS_CLOSE,
    EELECTROCLAMP_POS_OPEN
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum electroclamp_pos_t 	electroclamp_pos_t;
#endif

enum bag_syringe_change_t
{
    EBAG_SYRINGE_NONE,
    EBAG_ENTER,
    EBAG_END,
    ESYRINGE_ENTER,
    ESYRINGE_END,
    ECITRATE_ENTER,
    ECITRATE_END
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum bag_syringe_change_t 	bag_syringe_change_t;
#endif

enum buzzer_volume_t
{
	EBUZZER_VOLUME_UP = 0,
	EBUZZER_VOLUME_DOWN,
	EBUZZER_VOLUME_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum buzzer_volume_t 	buzzer_volume_t;
#endif

/**
 * Command response codes
 */
enum command_response_errors_t
{
    ECMD_RESP_OK = 0,
    ECMD_RESP_WRONG_NUM_PARAM,	// the number of parameters received from the communication master was wrong
    ECMD_RESP_NOT_EXC_PARAM_OUT_OF_RANGE, // the command received could not be executed because at least one of the parameters received was out of range
    ECMD_RESP_NOT_EXC_NOT_IMPLEMENTED, // the command received could not be executed because it is not implemented on this microcontroller
    ECMD_RESP_NOT_EXC_uC_STATE_MISMATCH, // the command received could not be executed because the state (phase) of the microcotroller does not allow the execution

    ECMD_RESP_NOT_EXC_NO_SET_BECAUSE_TSTATUS_NO_SET, // the treatment parameters can not be set because the treatment status was not set.
    ECMD_RESP_NOT_EXC_PHASE_CHANGE_NOT_ALLOWED, // the change of phase is not allowed
    ECMD_RESP_NOT_EXC_TREATMENT_PARA_NON_VALID,
    ECMD_RESP_NOT_EXC_ALARMS_ACTIVE,
    ECMD_RESP_SAVING_DATA_ERR,
    ECMD_RESP_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum command_response_errors_t 	command_response_errors_t;
#endif

#endif //#ifdef _CRRT_TYPE_

/*************************************************************************/
/******************************* SPS *************************************/
/*************************************************************************/
#ifdef _SSP_TYPES_
/**
 * SPS List of channels that can be calibrated
 */
enum sps_channelsToCalibrate_t
{
    ESPS_CH_TO_CAL_none = 0,
    ESPS_CH_TO_CAL_scale_LCB1,
    ESPS_CH_TO_CAL_scale_LCB2,
    ESPS_CH_TO_CAL_scale_SOL_A,
    ESPS_CH_TO_CAL_scale_SOL_B,
    ESPS_CH_TO_CAL_cond_SOL_A,
    ESPS_CH_TO_CAL_cond_SOL_AB,
    ESPS_CH_TO_CAL_TEMP1_SOL_A,
    ESPS_CH_TO_CAL_TEMP2_SOL_A,
    ESPS_CH_TO_CAL_TEMP1_SOL_AB,
    ESPS_CH_TO_CAL_TEMP2_SOL_AB,
    ESPS_CH_TO_CAL_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_channelsToCalibrate_t	sps_channelsToCalibrate_t;
#endif

/**
 * SPS_selector_IDs_t
 */
enum sps_selector_IDs_t
{
    ESPS_SELECTOR_ID_NONE = 0,
    ESPS_SELECTOR_ID_BAG_TO_FILL,
    ESPS_SELECTOR_ID_BAG_TO_EMPTY,
    ESPS_SELECTOR_ID_NUM
}__attribute__ ((packed));
#define SPS_SELECTOR_ID_NUM 	4
#ifdef __cplusplus
#else
typedef enum sps_selector_IDs_t 	sps_selector_IDs_t;
#endif

/**
 * Valve
 */
enum sps_valves_t
{
    ESPS_VAL_NONE = 0,
    ESPS_VAL_INLET,
    ESPS_VAL_WASTE,
    ESPS_VAL_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_valves_t 	sps_valves_t;
#endif

/**
 * Valve status
 */
enum sps_valve_status_t
{
    ESPS_VAL_ST_NONE = 0,
    ESPS_VAL_ST_CLOSE,
    ESPS_VAL_ST_OPEN,
    ESPS_VAL_ST_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_valve_status_t 	sps_valve_status_t;
#endif

/**
 * Selector status
 */
enum sps_selector_bag_to_fill_t
{
    ESPS_SEL_BAG_TO_FILL_WASTE = 0,
    ESPS_SEL_BAG_TO_FILL_A,
    ESPS_SEL_BAG_TO_FILL_B,
    ESPS_SEL_BAG_TO_FILL_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_selector_bag_to_fill_t 	sps_selector_bag_to_fill_t;
#endif

enum sps_selector_bag_to_empty_t
{
    ESPS_SEL_BAG_TO_EMPTY_A = 0,
    ESPS_SEL_BAG_TO_EMPTY_B,
    ESPS_SEL_BAG_TO_EMPTY_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_selector_bag_to_fill_t 	sps_selector_bag_to_fill_t;
#endif
/**
 * SPS Motors' IDs
 */
enum sps_motors_IDs_t
{
    ESPS_MOTOR_ID_NONE = 0,
    ESPS_MOTOR_ID_INLET,
    ESPS_MOTOR_ID_SOL_A,
    ESPS_MOTOR_ID_SOL_B,
    ESPS_MOTOR_ID_CRRT,
    ESPS_MOTOR_ID_WASTE,
    ESPS_MOTOR_ID_NUM
}__attribute__ ((packed));
#define SPS_MOTOR_ID_NUM 	6
#ifdef __cplusplus
#else
typedef enum sps_motors_IDs_t 	sps_motors_IDs_t;
#endif

/**
  * Motors' speed limits
  */
#define SPS_MOT_INLET_MAX_SPEED        200 /*< Inlet pump's max speed: 200 ml/min*/
#define SPS_MOT_INLET_MIN_SPEED        0   /*< Inlet pump's min speed: 0 ml/min*/

#define SPS_MOT_SOL_A_MAX_SPEED        200  /*< Sol. A pump's max speed: 200 0.1*ml/min*/
#define SPS_MOT_SOL_A_MIN_SPEED        0   /*< Sol. A pump's min speed: 0 ml/min*/

#define SPS_MOT_SOL_B_MAX_SPEED        200  /*< Sol. B pump's max speed: 200 0.1*ml/min*/
#define SPS_MOT_SOL_B_MIN_SPEED        0   /*< Sol. B pump's min speed: 0 ml/min*/

#define SPS_MOT_CRRT_MAX_SPEED         200 /*< To CRRT pump's max speed: 200 ml/min*/
#define SPS_MOT_CRRT_MIN_SPEED         0   /*< To CRRT pump's min speed: 0 ml/min*/

#define SPS_MOT_WASTE_MAX_SPEED        1200 /*< Waste pump's max speed: 200 ml/h*/
#define SPS_MOT_WASTE_MIN_SPEED        0    /*< Waste pump's min speed: 0 ml/h*/

/**
 * Phase SPS
 */
enum sps_phases_t
{
    ESPS_PHASE_init = 0,
    ESPS_PHASE_idle,
    ESPS_PHASE_preparation,
    ESPS_PHASE_production,
    ESPS_PHASE_service,
    ESPS_PHASE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_phases_t 		sps_phases_t;
#endif


/**
 * Sps Status...
 */
enum sps_status_t
{
    ESPS_STATUS_idle = 0,
    ESPS_STATUS_init,
    ESPS_STATUS_active,
    ESPS_STATUS_draining,
    ESPS_STATUS_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_status_t 		sps_status_t;
#endif

/**
 * SPS's scales
 */
enum sps_scales_t
{
    ESPS_SCALE_LCDB1 = 0,
    ESPS_SCALE_LCDB2,
    ESPS_SCALE_SOL_A,
    ESPS_SCALE_SOL_B,
    ESPS_SCALE_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_scales_t 	sps_scales_t;
#endif

/**
 * SPS's temperature
 */
enum sps_temperature_t
{
    ESPS_TEMP_INLET = 0,
    ESPS_TEMP_ENV_SOL_A,
    ESPS_TEMP_ENV_SOL_B,
    ESPS_TEMP_COND_A_T1,
    ESPS_TEMP_COND_A_T2,
    ESPS_TEMP_COND_AB_T1,
    ESPS_TEMP_COND_AB_T2,
    ESPS_TEMP_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_temperature_t 	sps_temperature_t;
#endif

/**
 * SPS's Conductivity
 */
enum sps_conductivity_t
{
    ESPS_COND_SOL_A = 0,
    ESPS_COND_SOL_AB,
    ESPS_COND_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_conductivity_t 	sps_conductivity_t;
#endif

/**
 * SPS Fatal errors
 */
enum sps_fatal_errors_t
{
	ESPS_FATAL_ERROR_NONE = 0,		// nessun allarme fatal error
	ESPS_FATAL_ERROR_01,			//
	ESPS_FATAL_ERROR_02,			//
	ESPS_FATAL_ERROR_03,			//
	ESPS_FATAL_ERROR_04,			// heater, ala
	ESPS_FATAL_ERROR_05,			// ala
	ESPS_FATAL_ERROR_06,			// ala

	ESPS_FATAL_ERROR_12 = 12,       // heater

	ESPS_FATAL_ERROR_16 = 16,       // pump
	ESPS_FATAL_ERROR_17,       		// pump

	ESPS_FATAL_ERROR_30 = 30, 		// read write calibration value

	ESPS_FATAL_ERROR_50 = 50,		// sps manager
	ESPS_FATAL_ERROR_51,			// sps manager
	ESPS_FATAL_ERROR_52,			// sps manager
	ESPS_FATAL_ERROR_53,			// sps manager
	ESPS_FATAL_ERROR_54,			// sps manager
	ESPS_FATAL_ERROR_55,			// sps manager
	ESPS_FATAL_ERROR_56,			// sps manager

	ESPS_FATAL_ERROR_70 = 70,		// finite state manager
	ESPS_FATAL_ERROR_77 = 77,		// finite state: init
	ESPS_FATAL_ERROR_78,			// finite state: service
	ESPS_FATAL_ERROR_79,			// finite state: preparation
	ESPS_FATAL_ERROR_80,			// finite state: production

	ESPS_FATAL_ERRO_90 = 90			// SPS_VALVE.cpp
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_fatal_errors_t 	sps_fatal_errors_t;
#endif

/**
 * SPS Calibration
 */
struct sps_gainAndOffset_t
{
    float scaleGain[ESPS_SCALE_NUM] __attribute__((packed));
    short scaleOffset[ESPS_SCALE_NUM] __attribute__((packed));
    float conductivityGain[ESPS_COND_NUM] __attribute__((packed));
    short conductivityOffset[ESPS_COND_NUM] __attribute__((packed));
    float tempGain[ESPS_TEMP_NUM] __attribute__((packed));
    short tempOffset[ESPS_TEMP_NUM] __attribute__((packed));

#ifdef __cplusplus
    bool operator ==( const sps_gainAndOffset_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const sps_gainAndOffset_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    sps_gainAndOffset_t()
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    sps_gainAndOffset_t(const sps_gainAndOffset_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    sps_gainAndOffset_t& operator= (const sps_gainAndOffset_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(sps_gainAndOffset_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct sps_gainAndOffset_t 	sps_gainAndOffset_t;
#endif

struct sps_alarms_t
{
    sps_alarms_bitf_t m_alarms;

#ifdef __cplusplus
    bool operator ==( const sps_alarms_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_bitf_t); i++)
			if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i)))
				return false;
		return true;
	}

	bool operator !=(const sps_alarms_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_bitf_t); i++)
			if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i)))
				return true;
		return false;
	}

	sps_alarms_t()
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_bitf_t); i++)
			*((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = 0;
	}

	sps_alarms_t(const sps_alarms_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_bitf_t); i++)
			*((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = *((uint8_t*)(((uint8_t*)(&__val.m_alarms))+i));
	}

	sps_alarms_t& operator= (const sps_alarms_t &__a)
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_t); i++)
		   *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = *((uint8_t*)(((uint8_t*)&__a.m_alarms)+i));
		return *this;
	}

	bool at(uint32_t _id)
	{
		uint32_t byteNum = _id / 8;
		uint32_t bitNum = _id % 8;
		return ((*(((uint8_t*)(&m_alarms))+byteNum) & (0x01 << bitNum)) > 0);
	}

	void setON(uint32_t _id)
	{
		uint32_t byteNum = _id / 8;
		uint32_t bitNum = _id % 8;
		*(((uint8_t*)(&m_alarms))+byteNum) |= (0x01 << bitNum);
	}

	void setOFF(uint32_t _id)
	{
		uint32_t byteNum = _id / 8;
		uint32_t bitNum = _id % 8;
		*(((uint8_t*)(&m_alarms))+byteNum) &= ~(0x01 << bitNum);
	}

	void reset()
	{
		for( uint16_t i = 0; i < sizeof(sps_alarms_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}

	void OR(const sps_alarms_t &__al1, const sps_alarms_t &__al2)
	{
		 for( uint16_t i = 0; i < sizeof(sps_alarms_bitf_t); i++)
		  *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) = (*((uint8_t*)(((uint8_t*)(&__al1.m_alarms))+i))) | ((*((uint8_t*)(((uint8_t*)(&__al2.m_alarms))+i))));
	}

	uint16_t getFirstBitSet()
	{
		uint16_t i, k;
		uint16_t firstBit = NUM_ALARMS_SPS;
		uint8_t bit;
		for( i = 0; i < sizeof(sps_alarms_bitf_t); i++)
		{
			if( *((uint8_t*)(((uint8_t*)(&m_alarms))+i)) != 0)
			{
				for(k = 0; k < 8; k++)
				{
					bit = (uint8_t) (0x01 << k);
					if( ( (*((uint8_t*)(((uint8_t*)(&m_alarms))+i))) & bit) == bit)
					{
						firstBit = i*8 +k;
						return firstBit;
					}
				}
			}
		}
		return firstBit;
	}
#endif
};
#ifdef __cplusplus
#else
typedef struct sps_alarms_t 		sps_alarms_t;
#endif

/**
  * SPS Switches bit fields struct
  * 16 bits (2 bytes)
  * uint16_t alignment
  */
struct sps_switches_bitF_t
{
    uint16_t selectorBagToFill : 2;
    uint16_t selectorBagToEmpty : 1;

    uint16_t inlet_valve : 1; //0=off; 1=on
    uint16_t waste_valve : 1; //0=off; 1=on

    uint16_t unused5_7 : 3;
    uint16_t unused8_15 : 8;

#ifdef __cplusplus
    sps_switches_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(sps_switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    sps_switches_bitF_t(const sps_switches_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    sps_switches_bitF_t& operator= (const sps_switches_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(sps_switches_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct sps_switches_bitF_t 		sps_switches_bitF_t;
#endif

/**
 * SPS status
 */
struct SPS_status_t
{
    sps_phases_t phase;
    sps_status_t status;
    sps_fatal_errors_t fatalError;
    uint16_t step __attribute__ ((packed));

    uint16_t inLetPumpSpeed __attribute__ ((packed)); // 0 - 200 ml/min
    uint16_t aSolPumpSpeed __attribute__ ((packed)); // 0 - 200 ml/min
    uint16_t bSolPumpSpeed __attribute__ ((packed)); // 0 - 200 ml/min
    uint16_t crrtPumpSpeed __attribute__ ((packed)); // 0 - 200 ml/min
    uint16_t wastePumpSpeed __attribute__ ((packed)); // 0 - 200 ml/min

    int16_t scale[ESPS_SCALE_NUM] __attribute__ ((packed)); //0 - 20000
    int16_t temp[ESPS_TEMP_NUM]	 __attribute__ ((packed));
    int16_t cond[ESPS_COND_NUM]	 __attribute__ ((packed));

    uint32_t totInlet __attribute__ ((packed));
    uint32_t totSolA __attribute__ ((packed));
    uint32_t totSolB __attribute__ ((packed));
    uint32_t totCrrt __attribute__ ((packed));
    uint32_t totWaste __attribute__ ((packed));

    sps_switches_bitF_t switchesStatus;
    sps_alarms_t alarms;

#ifdef __cplusplus
    bool operator ==( const sps_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const sps_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_status_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    SPS_status_t()
    {
        for( uint16_t i = 0; i < sizeof(SPS_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    SPS_status_t(const SPS_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(SPS_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    SPS_status_t& operator= (const SPS_status_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(SPS_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(SPS_status_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct SPS_status_t 		SPS_status_t;
#endif

/**
 * Service power supply information
 */
struct sps_servicePowerSupplyStruct_t
{
    int16_t pwrSupMonitor_C[EPWR_SUP_C_NUM] __attribute__((packed)); /*< Controller power supply monitor*/
    int16_t pwrSupMonitor_P[EPWR_SUP_P_NUM] __attribute__((packed)); /*< Protective power supply monitor*/

    int16_t scaleBoardPwrSupMonitor[ESCALE_PCB_monV_NUM] __attribute__((packed)); /*< Scale PCB power supply monitor*/
    int16_t cond_A_BoardPwrSupMonitor[ECOND_PCB_monV_NUM] __attribute__((packed));   /*< First Conductivity PCB power supply monitor*/
    int16_t cond_AB_BoardPwrSupMonitor[ECOND_PCB_monV_NUM] __attribute__((packed));   /*< First Conductivity PCB power supply monitor*/

    int16_t grid_frequency;

#ifdef __cplusplus
    bool operator ==( const sps_servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const sps_servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    sps_servicePowerSupplyStruct_t()
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    sps_servicePowerSupplyStruct_t(const sps_servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    sps_servicePowerSupplyStruct_t& operator= (const sps_servicePowerSupplyStruct_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(sps_servicePowerSupplyStruct_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct sps_servicePowerSupplyStruct_t 	sps_servicePowerSupplyStruct_t;
#endif

/**
 * relays
 */
enum sps_relays_t
{
    ESPS_RELAY_WATER = 0,
    ESPS_RELAY_PUMP_INLET,
    ESPS_RELAY_PUMP_SOL_A,
    ESPS_RELAY_PUMP_SOL_B,
    ESPS_RELAY_PUMP_CRRT,
    ESPS_RELAY_PUMP_WASTE,
    ESPS_RELAY_NUM
};
#ifdef __cplusplus
#else
typedef enum sps_relays_t 	sps_relays_t;
#endif

/**
  * Realys struct bit
  */
struct sps_relays_bitF_t
{
    uint16_t heaterWater :1;
    uint16_t pumpInlet :1;
    uint16_t pumpSolA :1;
    uint16_t pumpSolB :1;
    uint16_t pumpCrrt :1;
    uint16_t pumpWaste :1;
    uint16_t unused_06 :1;
    uint16_t unused_07 :1;

    uint16_t unused_08 :1;
    uint16_t unused_09 :1;
    uint16_t unused_10 :1;
    uint16_t unused_11 : 1;
    uint16_t unused_12 : 1;
    uint16_t unused_13 : 1;
    uint16_t unused_14 : 1;
    uint16_t unused_15 : 1;

#ifdef __cplusplus
    bool operator ==( const sps_relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const sps_relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
                return true;
        return false;
    }

    sps_relays_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    sps_relays_bitF_t(const sps_relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    sps_relays_bitF_t& operator= (const sps_relays_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(sps_relays_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct sps_relays_bitF_t 	sps_relays_bitF_t;
#endif

/**
 * Command response codes
 */
enum sps_command_response_errors_t
{
    ESPS_CMD_RESP_OK = 0,
    ESPS_CMD_RESP_WRONG_NUM_PARAM,	// the number of parameters received from the communication master was wrong
    ESPS_CMD_RESP_NOT_EXC_PARAM_OUT_OF_RANGE, // the command received could not be executed because at least one of the parameters received was out of range
    ESPS_CMD_RESP_NOT_EXC_NOT_IMPLEMENTED, // the command received could not be executed because it is not implemented on this microcontroller
    ESPS_CMD_RESP_NOT_EXC_uC_STATE_MISMATCH, // the command received could not be executed because the state (phase) of the microcotroller does not allow the execution

    ESPS_CMD_RESP_NOT_EXC_ALARMS_ACTIVE,
    ESPS_CMD_RESP_SAVING_DATA_ERR,
    ESPS_CMD_RESP_NUM
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef enum sps_command_response_errors_t 	sps_command_response_errors_t;
#endif

#endif //#ifdef _SPS_TYPES_

#endif
