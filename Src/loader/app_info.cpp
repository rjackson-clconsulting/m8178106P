/**
@file		app_info.cpp
@brief		Application information file
@author		Andrea Pellegrini/Fernando Morani
@date		05/01/2018
@version	01.01
@source     VetSmart Protective
@ NOTE: tnoergaard 10/18/2025 sonar cube changes
*/

#include "app_info.h"

#ifndef SONAR_QUBE_EXCLUDE
#include <stdint.h>
#endif
/**
 * SPS CONTROLLER FIRMWARE'S ID
 */
#define ID_M75_VETSMART_PROT 		0x71111111			// M75 board starts from 0x70000000
#define APPLICATION_ID 				ID_M75_VETSMART_PROT

/**
Board's hardware revision.	Ma ha senso questa??????????????????????????????????????????????
*/
#define BOARD_HW_VERSION			"01.00.00" // Max length 20
#define BOARD_HW_VERSION_LENGTH		9



/**
 * Memory sections addresses (from linker)
 */
extern uint32_t _text;
extern uint32_t _etext;

extern uint32_t _ramcode_start;
extern uint32_t _ramcode_end;
extern uint32_t __ram_start__;
extern uint32_t __ram_end__;
extern uint32_t __main_stack_base__;
extern uint32_t __main_stack_end__;
extern uint32_t __process_stack_base__;
extern uint32_t __process_stack_end__;
extern uint32_t __main_thread_stack_base__;
extern uint32_t __main_thread_stack_end__;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern uint32_t __heap_base__;
extern uint32_t __heap_end__;

extern uint32_t _xram_start;
extern uint32_t _xram_used;

/**
 * Application information struct initialization
 */
__attribute__ ((section(".application_info_section")))
struct application_info_data_t const g_appInfo =
{
	(uint32_t)APPLICATION_ID,			/*< Application ID*/
	(uint32_t)0,						/*< Application's global crc*/
	(uint32_t)ECRC_SECTION_NUM,
	{									/*< Application's memory sections crc*/
		(uint32_t)0, 							// ECRC_SECTION_TEXT
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0
	},

	(uint32_t)ESECTION_ADD_NUM,
	{									/*< Application's memory sections start addresses*/
#ifdef SONAR_QUBE_EXCLUDE
		(uint32_t)&_text,						//ESECTION_ADD_TEXT_START
		(uint32_t)&_ramcode_start,				//ESECTION_ADD_RAMCODE_START
		(uint32_t)&__ram_start__,				//ESECTION_ADD_RAM_START
		(uint32_t)&__main_stack_base__,			//ESECTION_ADD_MAIN_STACK_START
		(uint32_t)&__process_stack_base__,		//ESECTION_ADD_PROCESS_STACK_START
		(uint32_t)&__main_thread_stack_base__,	//ESECTION_ADD_THREAD_STACK_START
		(uint32_t)&_data,						//ESECTION_ADD_DATA_START
		(uint32_t)&_bss_start,					//ESECTION_ADD_BSS_START
		(uint32_t)&__heap_base__,				//ESECTION_ADD_HEAP_START
		(uint32_t)&_xram_start,					//ESECTION_ADD_XRAM_START

#else
		(uintptr_t)&_text,						//ESECTION_ADD_TEXT_START
		(uintptr_t)&_ramcode_start,				//ESECTION_ADD_RAMCODE_START
		(uintptr_t)&__ram_start__,				//ESECTION_ADD_RAM_START
		(uintptr_t)&__main_stack_base__,			//ESECTION_ADD_MAIN_STACK_START
		(uintptr_t)&__process_stack_base__,		//ESECTION_ADD_PROCESS_STACK_START
		(uintptr_t)&__main_thread_stack_base__,	//ESECTION_ADD_THREAD_STACK_START
		(uintptr_t)&_data,						//ESECTION_ADD_DATA_START
		(uintptr_t)&_bss_start,					//ESECTION_ADD_BSS_START
		(uintptr_t)&__heap_base__,				//ESECTION_ADD_HEAP_START
		(uintptr_t)&_xram_start,					//ESECTION_ADD_XRAM_START
#endif

		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0
	},
	{
										/*< Application's memory sections end addresses*/
#ifdef SONAR_QUBE_EXCLUDE
		(uint32_t)&_etext,						//ESECTION_ADD_TEXT_END
		(uint32_t)&_ramcode_end,				//ESECTION_ADD_RAMCODE_END
		(uint32_t)&__ram_end__,					//ESECTION_ADD_RAM_END
		(uint32_t)&__main_stack_end__,			//ESECTION_ADD_MAIN_STACK_END
		(uint32_t)&__process_stack_end__,		//ESECTION_ADD_PROCESS_STACK_END
		(uint32_t)&__main_thread_stack_end__, 	//ESECTION_ADD_THREAD_STACK_END
		(uint32_t)&_edata,						//ESECTION_ADD_DATA_END
		(uint32_t)&_bss_end,						//ESECTION_ADD_BSS_END
		(uint32_t)&__heap_end__,					//ESECTION_ADD_HEAP_END
		(uint32_t)&_xram_used,					//ESECTION_ADD_XRAM_END
#else
		(uintptr_t)&_etext,						//ESECTION_ADD_TEXT_END
		(uintptr_t)&_ramcode_end,				//ESECTION_ADD_RAMCODE_END
		(uintptr_t)&__ram_end__,					//ESECTION_ADD_RAM_END
		(uintptr_t)&__main_stack_end__,			//ESECTION_ADD_MAIN_STACK_END
		(uintptr_t)&__process_stack_end__,		//ESECTION_ADD_PROCESS_STACK_END
		(uintptr_t)&__main_thread_stack_end__, 	//ESECTION_ADD_THREAD_STACK_END
		(uintptr_t)&_edata,						//ESECTION_ADD_DATA_END
		(uintptr_t)&_bss_end,						//ESECTION_ADD_BSS_END
		(uintptr_t)&__heap_end__,					//ESECTION_ADD_HEAP_END
		(uintptr_t)&_xram_used,					//ESECTION_ADD_XRAM_END
#endif

		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0
	},

	APPLICATION_NAME,					/*< Application name */
	APPLICATION_NAME_LENGTH,			/*< Application name string length*/
	APPLICATION_REVISION,				/*< Application revision*/
	APPLICATION_REVISION_LENGTH,		/*< Application revision string length*/
	APPLICATION_DATE,					/*< Application date*/
	APPLICATION_DATE_LENGTH,			/*< Application date string length*/
	BOARD_HW_VERSION,					/*< Board hardware revision*/
	BOARD_HW_VERSION_LENGTH				/*< Board hardware revision string length*/
};








