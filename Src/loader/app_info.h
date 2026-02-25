/**
@file		app_info.h
@brief		Application information file
@author		Andrea Pellegrini
@date		18/11/2015
@version	00.00
@source     Ashai_SPS_CTRL_CPU
*/

#ifndef _APP_INFO_H_
#define _APP_INFO_H_

#include "stdDataTypes.h"

#define APPLICATION_NAME_LENGTH_MAX		20

#define APPLICATION_REVISION_LENGTH_MAX 20

#define APPLICATION_DATE_LENGTH_MAX		20

#define BOARD_HW_VERSION_LENGTH_MAX		20

#define CRC_SECTION_MAX_NUM             6

#define SECTION_ADD_MAX_NUM            15


//! DEFINE SECONDA_POMPA ABILITA LA VERSIONE A DUE POMPE,
//!-------
//! ATTENZIONE - SE ABILITATA QUESTA DEFINE DEVO TENERE DISABILITATE LE DEFINE FEGATO E RENE
//! COMPARE INOLTRE IN SERVICE MENU DI SETUP PER LA CONFIGURAZIONE TRATTAMENTI POSSIBILI
//!-------
//! SE DISABILITATA DEVO ABILITARE ALMENO UNA DELLE DUE, FEGATO O RENE
//#define	SECONDA_POMPA

//! SE ABILITATA VEDO TRATTAMENTO FEGATO VENA PORTA, SINGOLA POMPA
//! ATTENZIONE - DEVE ESSERE DISABILITATA SE ABILITATA SECONDA_POMPA
#define FEGATO

//! SE ABILITATA VEDO TRATTAMENTO RENE, SINGOLA POMPA
//! ATTENZIONE - DEVE ESSERE DISABILITATA SE ABILITATA SECONDA_POMPA
//#define RENE


//! PER SICUREZZA
#ifdef SECONDA_POMPA
#undef FEGATO
#undef RENE
#endif

#ifdef FEGATO
#undef SECONDA_POMPA
#undef RENE
#endif

#ifdef RENE
#undef SECONDA_POMPA
#undef FEGATO
#endif

#if !defined(RENE) && !defined(FEGATO) && !defined(SECONDA_POMPA)
#error ----->DEFINIRE UNA DEFINE
#endif
//! PER SICUREZZA




#ifndef SECONDA_POMPA  //!////////////////////////////////////////

#define APPLICATION_NAME			"VitaSmart"
#define APPLICATION_NAME_LENGTH		9

#ifdef FEGATO
#define APPLICATION_REVISION "Rev:01.00.006"
#define APPLICATION_REVISION_LENGTH	13
#define APPLICATION_DATE "11-02-2026"
#define APPLICATION_DATE_LENGTH		10
#endif

#ifdef RENE
#define APPLICATION_REVISION "Rev:02.04.000"
#define APPLICATION_REVISION_LENGTH	13
#define APPLICATION_DATE "21-03-2025"
#define APPLICATION_DATE_LENGTH		10
#endif

#endif	//!////////////////////////////////////


#ifdef SECONDA_POMPA  //!////////////////////////////////////////

#define APPLICATION_NAME	 "VitaSmart"
#define APPLICATION_NAME_LENGTH		9
#define APPLICATION_REVISION "Rev:03.00.000"
#define APPLICATION_REVISION_LENGTH	13
#define APPLICATION_DATE "30-09-2025"
#define APPLICATION_DATE_LENGTH		10

#endif	//!////////////////////////////////////










/**
 * Memory sections where crc calculation is needed
 * ECRC_SECTION_NUM can be maximum CRC_SECTION_MAX_NUM
 */
enum memory_sections_crc_t
{
	ECRC_SECTION_TEXT = 0,
	ECRC_SECTION_NUM
};
#ifdef __cplusplus
#else
typedef enum memory_sections_crc_t 	memory_sections_crc_t;
#endif

/**
 * Memory sections' addresses
 * ESECTION_ADD_NUM can be maximum SECTION_ADD_MAX_NUM
 */
enum memory_sections_addresses_t
{
    ESECTION_ADD_TEXT = 0,
    ESECTION_ADD_RAMCODE,
    ESECTION_ADD_RAM,
    ESECTION_ADD_MAIN_STACK,
    ESECTION_ADD_PROCESS_STACK,
    ESECTION_ADD_THREAD_STACK,
    ESECTION_ADD_DATA,
    ESECTION_ADD_BSS,
    ESECTION_ADD_HEAP,
    ESECTION_ADD_XRAM,

	ESECTION_ADD_NUM
};
#ifdef __cplusplus
#else
typedef enum memory_sections_addresses_t 	memory_sections_addresses_t;
#endif

/**
 * Application information struct: this struct is saved into the flash memory, section application_info_section
 */
struct application_info_data_t
{
	uint32_t app_id;		 							/*< Application ID*/

	uint32_t app_crc_global;							/*< Application's global crc*/

	uint32_t mem_sections_crc_num;
	uint32_t mem_sections_crc[CRC_SECTION_MAX_NUM];		/*< Application's memory sections crc*/

	uint32_t mem_sections_addresses_num;
    uint32_t mem_sections_start_add[SECTION_ADD_MAX_NUM]; 	/*< Application's memory sections start addresses*/
    uint32_t mem_sections_end_add[SECTION_ADD_MAX_NUM]; 	/*< Application's memory sections end addresses*/

	char sSwName[APPLICATION_NAME_LENGTH_MAX];			/*< Application name*/
	uint32_t swNameLength;								/*< Application name string length*/

	char sSwRevision[APPLICATION_REVISION_LENGTH_MAX];	/*< Application revision */
	uint32_t swRevisionLength;							/*< Application revision string length*/

	char sSwDate[APPLICATION_DATE_LENGTH_MAX];			/*< Application date string*/
	uint32_t swDateLength;								/*< Application date string length*/

	char sHwVersion[BOARD_HW_VERSION_LENGTH_MAX];		/*< Board hardware revision*/
	uint32_t hwVersionLength;							/*< Board hardware revision string length*/

}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct application_info_data_t 	application_info_data_t;
#endif

/**
 * Application information struct + methods & operators
 */
struct application_info_t
{
	application_info_data_t data;

#ifdef __cplusplus
	application_info_t()
	{
		for( uint16_t i = 0; i < sizeof(application_info_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}

	application_info_t(const application_info_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(application_info_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
	}

	application_info_t(const application_info_data_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(application_info_data_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
	}

	application_info_t& operator= (const application_info_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(application_info_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
		return *this;
	}

	bool operator ==( const application_info_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(application_info_t); i++)
			if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)((( uint8_t*)&__val)+i)))
				return false;
		return true;
	}

	bool operator !=(const application_info_t &__val)
	{
		for( uint16_t i = 0; i < sizeof(application_info_t); i++)
			if( *((uint8_t*)(((uint8_t*)this)+i)) != *((uint8_t*)(((uint8_t*)&__val)+i)))
				return true;
		return false;
	}

    void clear()
    {
        for( uint16_t i = 0; i < sizeof(application_info_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
};
#ifdef __cplusplus
#else
typedef struct application_info_t 	application_info_t;
#endif

extern struct application_info_data_t const g_appInfo;

#endif /* _APP_INFO_H_ */
