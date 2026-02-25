/**
@file		irqManager.cpp
@brief		IRQ Services routine
@author		Fernando Morani
@date		05/05/2017
@version	01.10
@source     Flexiper Protective
@ Note: tnoergaard 10/17/2025, SonarQube updates, using local values to handle incrementing of volatiles.
*/

#include "irqManager.h"

/**
* EXTERNAL VARIABLES
*/

extern UWORD 	g_time_sec;


/**
*EXTERNAL ROUTINE
*/

/**
* GLOBAL VARIABLES
*/
long     g_cntNmi;
long     g_cntHardFault;
long     g_cntMemManage;
long     g_cntBusFault;
long     g_cntUsageFault;

volatile uint32_t g_globalTick;
volatile uint32_t 	g_time_msec;
static int cntMsec;

/**
 * @brief   system tick irq handler
 * @details
 * @note
 */
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(SysTickVector)
{
    uint32_t tempTickVal = g_globalTick; // Load
    uint32_t tempTimeMsec = g_time_msec;

    tempTickVal++; // Modify
	g_globalTick = tempTickVal; //Store
	cntMsec++;
	tempTimeMsec++;
	g_time_msec = tempTimeMsec;
    if( cntMsec >= 1000 )
    {
        cntMsec = 0;
        g_time_sec++;
    }
	g_globalTimer.clockTick(1, 0);
    adc_scan();
    if ( g_time_msec > ALA_DRV_GES_AIR_WAIT_TIME ){
        DrvAIR_Ges();
    }
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(NMIVector)
{
	g_cntNmi++;


}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(HardFaultVector)
{
	g_cntHardFault++;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(MemManageVector)
{
	g_cntMemManage++;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(BusFaultVector)
{
	g_cntBusFault++;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(UsageFaultVector)
{
	g_cntUsageFault++;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector1C)
{
	g_cntUsageFault = 0x1c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector20)
{
	g_cntUsageFault = 0x20;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector24)
{
	g_cntUsageFault= 0x24;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector28)
{
	g_cntUsageFault = 0x28;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector34)
{
	g_cntUsageFault = 0x34;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector40)
{
	g_cntUsageFault = 0x40;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector44)
{
	g_cntUsageFault = 0x44;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector48)
{
	g_cntUsageFault = 0x48;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector4c)
{
	g_cntUsageFault = 0x4c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector50)
{
	g_cntUsageFault = 0x50;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector54)
{
	g_cntUsageFault = 0x54;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector58)
{
	g_cntUsageFault = 0x58;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector5c)
{
	g_cntUsageFault = 0x5c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector6c)
{
	g_cntUsageFault = 0x6c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector70)
{
	g_cntUsageFault = 0x70;
}
#ifdef __cplusplus
}
#endif

#ifdef LOADER_ENGINE_INT
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector74)
{
    g_ComProtoUartSemaphore = COMPROTO_SEMAPHORE_BUSY;
    uartRxFromControllerIrq();
    g_ComProtoUartSemaphore = COMPROTO_SEMAPHORE_FREE;
}
#ifdef __cplusplus
}
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector7c)
{
	g_cntUsageFault = 0x7c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector84)
{
	g_cntUsageFault = 0x84;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector88)
{
	g_cntUsageFault = 0x88;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector8c)
{
	g_cntUsageFault = 0x8c;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector90)
{
	g_cntUsageFault = 0x90;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector9c)
{
	g_cntUsageFault = 0x9c;
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorA0)
{
	g_cntUsageFault = 0xA0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorA4)
{
	g_cntUsageFault = 0xA4;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorA8)
{
	g_cntUsageFault = 0xA8;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorAc)
{
	g_cntUsageFault = 0xAc;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorBc)
{
	g_cntUsageFault = 0xBc;
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorC0)
{
	g_cntUsageFault = 0xC0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorC4)
{
	g_cntUsageFault = 0xC4;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorC8)
{
	g_cntUsageFault = 0xC8;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorCc)
{
	g_cntUsageFault = 0xCc;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorD0)
{
	g_cntUsageFault = 0xD0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorD4)
{
	g_cntUsageFault = 0xD4;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorD8)
{
	g_cntUsageFault = 0xD8;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorDc)
{
	g_cntUsageFault = 0xDc;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorE0)
{
	g_cntUsageFault = 0xE0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorE4)
{
	g_cntUsageFault = 0xE4;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorE8)
{
	g_cntUsageFault = 0xE8;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorEc)
{
	g_cntUsageFault = 0xEc;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorF0)
{
	g_cntUsageFault = 0xF0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorF4)
{
	g_cntUsageFault = 0xF4;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorF8)
{
	g_cntUsageFault = 0xF8;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(VectorFc)
{
	g_cntUsageFault = 0xFc;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
_IRQ_HANDLER(Vector100)
{
	g_cntUsageFault = 0x100;
}
#ifdef __cplusplus
}
#endif
