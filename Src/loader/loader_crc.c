/**
@file		loader_crc.c
@brief		Loader crc calculation and initialization
@author		Fernando Morani
@date		05/03/2018 (inherited from Nicola Molinazzi - Novembre 2011)
@version	01.01.01
Notes on version 01.01.01
    Add macros LOADER_START_ADD, LOADER_END_ADD to allow loader cheksum calculation
*/

#include "loader_crc.h"

#define FIRMWARE_ID 	0x71111111

/**
 * Loader status
 */
loaderStatus_t g_loaderStatus;

/**
 * Application fw entry point
 */
void (*g_fpJumpToApplication)(void);

extern uint32_t *g_JumpingCodeMemLocation;

void loader_status_init()
{
	uint16_t i;

	/**
	 * Loader information Initialization
	 */
	for(i = 0; i< APPLICATION_REVISION_LENGTH_MAX; i++)
		g_loaderStatus.swVersLoader[i] = g_appInfo.sSwRevision[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_NAME_LENGTH_MAX; i++)
		g_loaderStatus.swNameLoader[i] = g_appInfo.sSwName[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_DATE_LENGTH_MAX; i++)
		g_loaderStatus.swDateLoader[i] = g_appInfo.sSwDate[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_DATE_LENGTH_MAX; i++)
			g_loaderStatus.hwVersLoader[i] = g_appInfo.sHwVersion[i];// numero di revisione del loader

	/**
	 * Application information Initialization
	 */
	for(i = 0; i< APPLICATION_REVISION_LENGTH_MAX; i++)
		g_loaderStatus.swVersApp[i] = g_appInfo.sSwRevision[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_NAME_LENGTH_MAX; i++)
		g_loaderStatus.swNameApp[i] = g_appInfo.sSwName[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_DATE_LENGTH_MAX; i++)
		g_loaderStatus.swDateApp[i] = g_appInfo.sSwDate[i];// numero di revisione del loader

	for(i = 0; i< APPLICATION_DATE_LENGTH_MAX; i++)
			g_loaderStatus.hwVersApp[i] = g_appInfo.sHwVersion[i];// numero di revisione del loader

	/**
	 * Reset all other fields
	 */
	g_loaderStatus.chkAppProg = 0;
	g_loaderStatus.chkAppCalc = 0;
	g_loaderStatus.chkLoaderProg = 0;
	g_loaderStatus.chkLoaderCalc = 0;

	g_loaderStatus.maxScanTick = 0;
	g_loaderStatus.actScanTick = 0;
	g_loaderStatus.systemTickCount = 0;

	g_loaderStatus.cntCommErrTotal = 0;
	g_loaderStatus.cntCommErrWrongOpCode = 0;
	g_loaderStatus.cntCommErrBadStx = 0;
	g_loaderStatus.cntCommErrBadEtx = 0;
	g_loaderStatus.cntCommErrTimeoutRx = 0;
	g_loaderStatus.cntCommErrTimeoutTx = 0;
	g_loaderStatus.cntCommErrBadCrc = 0;
	g_loaderStatus.cntCommErrDataBufOverflow = 0;
	g_loaderStatus.cntCommErrBadEncoding = 0;
	g_loaderStatus.cntUartHwError = 0;

	g_loaderStatus.cntComTx	= 0;
	g_loaderStatus.cntComRx = 0;
	g_loaderStatus.cntFail = 0;

	g_loaderStatus.loaderPlease = ELOADER_PLEASE_NONE;// flag per richiesta stop nel loader
	g_loaderStatus.loader_error = 0;

	g_fpJumpToApplication = (void (*)()) APPLICATION_START_ADD;
}

/**
 * Verifies application checksums' consistency.
 * Returns True is the checksums are correct.
 */
bool loader_status_checksumVerify()
{
	const uint32_t *pFlash; 						//pointer to flash memory
	uint32_t startAdd;	   						//start address to calculate checksum
	uint32_t endAdd;	   						//end address to calculate checksum
	const application_info_data_t *pAppInfo;	//pointer to the application information struct
	uint32_t chkCalc = 0;						//calculate checksum
	uint32_t chkGlobalCalc = 0;					//calculate checksum global
	uint32_t totalSegments;						//application firmware's total memory segment
	int i;
	/**
	 * Verify loader consistency and calculate its checksum
	 */
	pAppInfo = &g_appInfo;
	startAdd = LOADER_START_ADD; /*pAppInfo->mem_sections_start_add[ESECTION_ADD_TEXT];*/
	endAdd = LOADER_END_ADD; /*pAppInfo->mem_sections_end_add[ESECTION_ADD_TEXT];*/

	totalSegments = pAppInfo->mem_sections_crc_num;

	if(endAdd >= APPLICATION_START_ADD)//Verifying loader flash memory end address consistency..
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_LOADER_TEXT_AREA_END;
		g_loaderStatus.chkLoaderCalc = 0xEEEEEEEE;
		g_loaderStatus.chkAppCalc = 0xFFFFFFFF;
		return False;
	}

	if(pAppInfo->app_id != FIRMWARE_ID)//Verifying loader ID..
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_LOADER_ID;
		g_loaderStatus.chkLoaderCalc = 0xDDDDDDDD;
		g_loaderStatus.chkAppCalc = 0xFFFFFFFF;
		return False;
	}

	if(totalSegments < 1 || totalSegments > CRC_SECTION_MAX_NUM) //Verifying number of segments
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_LOADER_SEGMENTS_NUM;
        g_loaderStatus.chkLoaderCalc = 0xCCCCCCCC;
		g_loaderStatus.chkAppCalc = 0xFFFFFFFF;
		return False;
	}

	chkCalc = 0;
	pFlash = (uint32_t *)startAdd;
	while(startAdd < endAdd)
	{
		chkCalc += *pFlash++;
		startAdd += 4;
	}
	g_loaderStatus.chkLoaderCalc = chkCalc; //loader calculated checksum

	/**
	 * Verify application consistency
	 */
	pAppInfo = (application_info_data_t*)APPLICATION_INFO_TABLE_ADD;

	totalSegments = pAppInfo->mem_sections_crc_num;

	if(pAppInfo->app_id != g_appInfo.app_id)//Verifying application ID..
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_APP_ID;
		g_loaderStatus.chkAppCalc = 0xEEEEEEEE;
		return False;
	}

	if(totalSegments < 1 || totalSegments > CRC_SECTION_MAX_NUM) //Verifying number of segments..
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_APP_SEGMENTS_NUM;
		g_loaderStatus.chkAppCalc = 0xDDDDDDDD;
		return False;
	}

	/**
	 * Verify application checksum
	 */
	chkGlobalCalc = 0;
	chkCalc = 0;
	for(i = 0; i < totalSegments; i++)
	{
		startAdd = pAppInfo->mem_sections_start_add[i];
		endAdd = pAppInfo->mem_sections_end_add[i];

		chkCalc = 0;
		pFlash = (uint32_t *)startAdd;
		while(startAdd < endAdd)
		{
			chkCalc += *pFlash++;
			startAdd += 4;
		}

		if(i == 0) //-- ESECTION_ADD_TEXT_START to ESECTION_ADD_TEXT_END checksum
		{
			chkCalc -= pAppInfo->mem_sections_crc[i];	//I have to subtract the text area checksum..
		}

		chkCalc -= pAppInfo->app_crc_global;			//I have to subtract the global checksum..
		if(chkCalc != pAppInfo->mem_sections_crc[i])//verifying crc..
		{
			g_loaderStatus.chkAppCalc = chkCalc;
			g_loaderStatus.chkAppProg = pAppInfo->mem_sections_crc[i];
			g_loaderStatus.loader_error = i;
			g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_APP_CHECKSUM;
			g_loaderStatus.chkAppCalc = 0xCCCCCCCC;
			return False;
		}
		chkGlobalCalc += chkCalc; //section checksum is added to the global crc
	}

	g_loaderStatus.chkAppCalc = chkGlobalCalc;
	g_loaderStatus.chkAppProg = pAppInfo->app_crc_global;
	if(pAppInfo->app_crc_global != chkGlobalCalc) //verifying global crc
	{
		g_loaderStatus.loader_error = i;
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_WRONG_APP_CHECKSUM;
		g_loaderStatus.chkAppCalc = 0xBBBBBBBB;
		return False;
	}

	/**
	 * Verifing if the application firmware want to remain in loader.
	 */
	if((FM3_CRC->CRCIN == STAY_IN_LOADER) || (*g_JumpingCodeMemLocation == STAY_IN_LOADER))
	{
		g_loaderStatus.loaderPlease = ELOADER_PLEASE_APP_JUMP;
		return False;
	}

	return True;
}
