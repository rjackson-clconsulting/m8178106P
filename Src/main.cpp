
/**
@file		main.cpp
@brief		main file
@author		Fernando Morani
@date		09/01/2018
@version	01.00.01
@source     VetSmart Protective Application
Notes on version 01.00.01
    Add support for loader s/w engine
*/

#include "main.h"

extern FM3_GPIO_TypeDef *g_pFM3_GPIO;
extern SystemStatus SysStatus;                 /* stato di funzionamento */
#ifdef LOADER_ENGINE
extern void (*g_fpJumpToLoader)(void);  //FERLOADER
extern uint32_t *g_JumpingCodeMemLocation;
extern loaderStatus_t g_loaderStatus;
#endif

/*************************
 * Main
 *************************/
int main(void)
{
    int iRetVal = 0;
    int i = 0;
    bool bLoaderStatus = false;



#ifdef LOADER_ENGINE

    bLoaderStatus = loader_status_checksumVerify();

    /**CheckSum calculation - Assigned to the SysStatus structure*/
    SysStatus.ChkCalc = g_loaderStatus.chkAppCalc;
    SysStatus.ChkLoader = g_loaderStatus.chkLoaderCalc;

    if(bLoaderStatus == false)
	{
		if (g_loaderStatus.loaderPlease == ELOADER_PLEASE_APP_JUMP){
            /**
            * The application firmware want to remain in loader.
            */
            __disable_irq();
            g_fpJumpToLoader = (void (*)()) LOADER_JUMP_ADDRESS;
            (*g_fpJumpToLoader)();
		}/**else{
		}*/

	}/**else{

	}*/


#endif

    Ala_Ini();

	boardInit();
	hwAppInit();

    /**
    Init the sw loader engine
    **/
    #ifdef LOADER_ENGINE
	iRetVal = loaderInit();
    if (iRetVal != ERROR_NONE){
        /**We have got an error*/
        writeF(); //Signalling a Fail on the 7Seg Display
        for (i=0;i<WAITING_FOR_CYCLE_TO_SHOW_FAIL;i++); //Just wait
        return iRetVal;  //It means Exit from the application
    }
    #endif

	PinSAFETY_BUSY = 0;
	PinBUZZER = 1;

        iRetVal = mainAppInit();
        if (iRetVal == ERROR_NONE){
            iRetVal = mainAppRun();
        }

	return iRetVal;
}







