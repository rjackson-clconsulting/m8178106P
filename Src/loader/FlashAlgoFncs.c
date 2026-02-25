
/**
 *******************************************************************************
 * @file       FlashAlgoFncs.c
 * @version    V0.4
 * @date       2011.08.04
 * @brief      Flash  Algorithm For STM32F10x
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 *
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */

#include <stdint.h>
#include "FlashAlgorithm.h"

#include "mb9bf50x.h"
#include "system_mb9bf50x.h"
#include "fm3_sysclk.h"

//#define FLASHING_INFO	1

/**
 *******************************************************************************
 * @brief      Initialize before Flash Programming/Erase Functions
 * @param[in]  baseAddr     Flash device base address.
 * @param[in]  clk     			Flash program clock.
 * @param[in]  operateFuc   Init for what operation
 														(FLASH_OPT_ERASECHIP/FLASH_OPT_ERASESECTORS/FLASH_OPT_PROGRAMPAGE).
 * @param[out] None
 * @retval     0   					All is OK.
 * @retval     others       Some error occurs.
 *
 * @par Description
 * @details    This function is called before flash programming/erase.
 * @note
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashInit (unsigned long baseAddr,
               unsigned long clk,
               unsigned long operateFuc)
{

	uint32_t delay;
	switch(operateFuc)
	{
		case FLASH_OPT_UNDEFINED       :                     // Undefined Operation
			for(delay=0; delay<1000; delay++)
			{
				__asm("nop");
			}
			//return TO_DRV_RET_ERROR;
			break;

		case FLASH_OPT_PROGRAMPAGE     :                     // Program Page
		case FLASH_OPT_VERIFY          :                     // Verify Page
		case FLASH_OPT_BLANKCHECK      :                     // Blank Check Page
		case FLASH_OPT_ERASECHIP       :                     // Erase Full Chip
		case FLASH_OPT_ERASESECTORS    :                     // Erase Sectors

			for(delay=0; delay<1000; delay++)
			{
				__asm("nop");
			}
			break;
        default:
            break;
		}

  return TO_DRV_RET_OK;
}


/**
 *******************************************************************************
 * @brief      Un-Init after Flash Programming/Erase Functions
 * @param[in]  operateFuc   Init for what operation
 														(FLASH_OPT_ERASECHIP/FLASH_OPT_ERASESECTORS/FLASH_OPT_PROGRAMPAGE).
 * @param[out] None
 * @retval     0   					All is OK.
 * @retval     others       Some error occurs.
 *
 * @par Description
 * @details    This function is called after flash programming/erase.
 * @note
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashUnInit (unsigned long operateFuc)
{
  // Lock Flash
  //FLASH->CR |=  FLASH_LOCK;

#ifdef FLASHING_INFO

#endif
	uint32_t delay;
	for(delay=0; delay<250000; delay++)
	{
		__asm("nop");
	}

	return TO_DRV_RET_OK;
}


/**
 *******************************************************************************
 * @brief      Erase the full chip.
 * @param[in]  None.
 * @param[out] None
 * @retval     0   					All is OK.
 * @retval     others       Some error occurs.
 *
 * @par Description
 * @details
 * @note
 *******************************************************************************
 */

__attribute__ (( section(".ramcode") ))
int FlashEraseChip(void)
{
	volatile int32_t  i32FlashFlag = 0;
	volatile uint32_t u32DummyRead;
	volatile uint16_t *pFlash = (volatile uint16_t*) 0;


#ifdef FLASHING_INFO

#endif

	FM3_FLASH_IF->FASZR &= 0xFFFD;      // ASZ[1:0] = 2'b01
	FM3_FLASH_IF->FASZR |= 1;
	u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

	if(u32DummyRead == 1) // to avoid warning
		__asm("nop");

	pFlash = (volatile uint16_t*) 0x00000000;

	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_1;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_2;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_3;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_4;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_5;
	*(FLASH_SEQ_1550) = 0x0010;

    if( pFlash == NULL )
	{
		return(TO_DRV_RET_ERROR);
	}
	else
	{
	    // sector erase timer ready?
	    while( (pFlash[0] & FLASH_DQ3) != FLASH_DQ3);
     }
     i32FlashFlag = 0;

	  while( i32FlashFlag == 0 )
	  {
		// Flash timeout?
		if((*pFlash & FLASH_DQ5) == FLASH_DQ5)
		{
			i32FlashFlag = FLASH_TIMEOUT_ERROR;
		}

		// Data correct?
		if((*pFlash & FLASH_DQ7) == FLASH_DQ7)
		{
			i32FlashFlag = FLASH_OK;
		}
	   }

	  FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
	  FM3_FLASH_IF->FASZR |= 0x2;
	  u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

	  if( i32FlashFlag != FLASH_OK )
	  {
		return(TO_DRV_RET_ERROR);
	  }

	return TO_DRV_RET_OK;


}

/**
 *******************************************************************************
 * @brief      Erase the select Sector.
 * @param[in]  sectorAddr   Sector's start address.
 * @param[out] None
 * @retval     0   					All is OK.
 * @retval     others       Some error occurs.
 *
 * @par Description
 * @details
 * @note
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashEraseSector (unsigned long sectorAddr)
{
	volatile int32_t  i32FlashFlag;
	volatile uint32_t u32DummyRead;
	volatile uint16_t *pFlash;

#ifdef FLASHING_INFO

#endif

	switch( sectorAddr )
	{
		case	0x0000:
			sectorAddr = FLASH_ADDRESS_SA5;
			break;
		case	0x2000:
			sectorAddr = FLASH_ADDRESS_SA4;
			break;
		case	0x4000:
			sectorAddr = FLASH_ADDRESS_SA7;
			break;
		case	0x6000:
			sectorAddr = FLASH_ADDRESS_SA6;
			break;
		case	0x8000:
			sectorAddr = FLASH_ADDRESS_SA9;
			break;
		case	0x14000:
			sectorAddr = FLASH_ADDRESS_SA8;
			break;
		case	0x20000:
			sectorAddr = FLASH_ADDRESS_SA11;
			break;
		case	0x30000:
			sectorAddr = FLASH_ADDRESS_SA10;
			break;
		case	0x40000:
			sectorAddr = FLASH_ADDRESS_SA13;
			break;
		case	0x50000:
			sectorAddr = FLASH_ADDRESS_SA12;
			break;
		case	0x60000:
			sectorAddr = FLASH_ADDRESS_SA15;
			break;
		case	0x70000:
			sectorAddr = FLASH_ADDRESS_SA14;
			break;
		default:
            break;
	}


#ifdef FLASHING_INFO

#endif

	pFlash = (volatile uint16_t*) sectorAddr;

	FM3_FLASH_IF->FASZR &= 0xFFFD;      // ASZ[1:0] = 2'b01
	FM3_FLASH_IF->FASZR |= 1;
	u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

	if(u32DummyRead == 1) // to avoid warning
		__asm("nop");

	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_1;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_2;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_3;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_4;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_5;

	if( pFlash == NULL )
	{
		return(TO_DRV_RET_ERROR);
	}
	else
	{
	  *pFlash = FLASH_SECTOR_ERASE_6;
    }
  // sector erase timer ready?
   while( (*pFlash & FLASH_DQ3) != FLASH_DQ3);

   i32FlashFlag = 0;

	while( i32FlashFlag == 0 )
	{
		// Flash timeout?
		if((*pFlash & FLASH_DQ5) == FLASH_DQ5)
		{
			i32FlashFlag = FLASH_TIMEOUT_ERROR;
		}

		// Data correct?
		if((*pFlash & FLASH_DQ7) == FLASH_DQ7)
		{
			i32FlashFlag = FLASH_OK;
		}
	}

	if( i32FlashFlag != FLASH_OK )
	{
		FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
		FM3_FLASH_IF->FASZR |= 0x2;

		u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR
		return(TO_DRV_RET_ERROR);
	}

	//////////////////////////////////
	// erase the complementary sector

#ifdef FLASHING_INFO

#endif

	switch( sectorAddr )
	{
		case	FLASH_ADDRESS_SA4:
			sectorAddr = FLASH_ADDRESS_SA5;
			break;
		case	FLASH_ADDRESS_SA5:
			sectorAddr = FLASH_ADDRESS_SA4;
			break;
		case	FLASH_ADDRESS_SA6:
			sectorAddr = FLASH_ADDRESS_SA7;
			break;
		case	FLASH_ADDRESS_SA7:
			sectorAddr = FLASH_ADDRESS_SA6;
			break;
		case	FLASH_ADDRESS_SA8:
			sectorAddr = FLASH_ADDRESS_SA9;
			break;
		case	FLASH_ADDRESS_SA9:
			sectorAddr = FLASH_ADDRESS_SA8;
			break;
		case	FLASH_ADDRESS_SA10:
			sectorAddr = FLASH_ADDRESS_SA11;
			break;
		case	FLASH_ADDRESS_SA11:
			sectorAddr = FLASH_ADDRESS_SA10;
			break;
		case	FLASH_ADDRESS_SA12:
			sectorAddr = FLASH_ADDRESS_SA13;
			break;
		case	FLASH_ADDRESS_SA13:
			sectorAddr = FLASH_ADDRESS_SA12;
			break;
		case	FLASH_ADDRESS_SA14:
			sectorAddr = FLASH_ADDRESS_SA15;
			break;
		case	FLASH_ADDRESS_SA15:
			sectorAddr = FLASH_ADDRESS_SA14;
			break;
        default:
            break;
	}


	pFlash = (volatile uint16_t*) sectorAddr;

	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_1;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_2;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_3;
	*(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_4;
	*(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_5;

	if( pFlash == NULL )
	{
		return(TO_DRV_RET_ERROR);
	}
	else
	{
	  *pFlash = FLASH_SECTOR_ERASE_6;
    }
	// sector erase timer ready?
	while( (pFlash[0] & FLASH_DQ3) != FLASH_DQ3);

	i32FlashFlag = 0;
	while( i32FlashFlag == 0 )
	{
		// Flash timeout?
		if((*pFlash & FLASH_DQ5) == FLASH_DQ5)
		{
			i32FlashFlag = FLASH_TIMEOUT_ERROR;
		}

		// Data correct?
		if((*pFlash & FLASH_DQ7) == FLASH_DQ7)
		{
			i32FlashFlag = FLASH_OK;
		}
	}

	FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
	FM3_FLASH_IF->FASZR |= 0x2;

	u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

	if(u32DummyRead == 1) // to avoid warning
		__asm("nop");

	if( i32FlashFlag == FLASH_OK )
		return(TO_DRV_RET_OK);
	else
		return(TO_DRV_RET_ERROR);

	return TO_DRV_RET_OK;
}


/**
 *******************************************************************************
 * @brief      Proram a page.
 * @param[in]  pageAddr   Page's start address.
 * @param[in]  size			  Page size
 * @param[in]  buf   			source point.
 * @param[out] None
 * @retval     0   					All is OK.
 * @retval     others       Some error occurs.
 *
 * @par Description
 * @details
 * @note
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashProgramPage (unsigned long pageAddr,
                      unsigned long size,
                      unsigned char *buf)
{
volatile int32_t  i32FlashFlag = 0;
volatile uint32_t u32DummyRead;
volatile uint16_t u16ProgamData;
volatile uint16_t *pwBuf;
volatile uint16_t *pFlash;

#ifdef FLASHING_INFO

#endif

  	size = (size + 1) & ~1;                       // Adjust size for Half Words
	pwBuf = (uint16_t *)buf;
	pFlash = (uint16_t *) (pageAddr + 0x0000);

	long progTimeout;

	while (size)
	{
	    u16ProgamData = *pwBuf;   			// Program Half Word

		FM3_FLASH_IF->FASZR &= 0xFFFD;      // ASZ[1:0] = 2'b01
		FM3_FLASH_IF->FASZR |= 1;
		u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

		if(u32DummyRead == 1) // to avoid warning
		__asm("nop");

		*(FLASH_SEQ_1550) = FLASH_WRITE_1;
		*(FLASH_SEQ_0AA8) = FLASH_WRITE_2;
		*(FLASH_SEQ_1550) = FLASH_WRITE_3;
		*pFlash = u16ProgamData;


		progTimeout = 20000;

		while(i32FlashFlag == 0 )
		{
			progTimeout--;

			if( progTimeout == 0 )
				break;

			// Flash timeout?
			if((pFlash[0] & FLASH_DQ5) == FLASH_DQ5)
			{
			  i32FlashFlag = FLASH_TIMEOUT_ERROR;
			}

			// Data correct?
			if((pFlash[0] & FLASH_DQ7) == FLASH_DQ7)
			{
			  i32FlashFlag = FLASH_OK;
			}
		}

		FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
		FM3_FLASH_IF->FASZR |= 0x2;
		u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR

		if(u32DummyRead == 1) // to avoid warning
			__asm("nop");

		if( i32FlashFlag != FLASH_OK )
  			return (TO_DRV_RET_ERROR);

		// Go to next Half Word
		pFlash++;
		pwBuf++;
		size -= 2;

	}

	return TO_DRV_RET_OK;
}

/**
 *******************************************************************************
 * @brief      Page Verify Function.
 * @param[in]  verifyAddr   Verify Start Address(Usually page start address).
 * @param[in]  size			Verify size
 * @param[in]  buf   		Source buf point.
 * @param[out] None
 * @retval     0   			Verify pass.
 * @retval     others       Some error occurs or verify failed..
 *
 * @par Description
 * @details   Optional function. When this function is absence,
 *            the link will read flash memory directly to do verify.
 * @note
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashVerify(unsigned long verifyAddr,unsigned long size,unsigned char *buf)
{
  int i;
  unsigned char* pageBuf = (unsigned char*)verifyAddr;

#ifdef FLASHING_INFO

#endif

  for(i = 0; i < size; i++)
  {
	if(pageBuf[i] != buf[i])
	{
		return (TO_DRV_RET_ERROR);
	}
  }

  return (TO_DRV_RET_OK);
}

/**
 *******************************************************************************
 * @brief      Page Blank Check Function.
 * @param[in]  checkAddr    Check Start Address(Usually page start address).
 * @param[in]  size			    Check size
 * @param[out] None
 * @retval     0   					Check pass.
 * @retval     others       Some error occurs or check failed.
 *
 * @par Description
 * @details   Optional function. When this function is absence,
 *            the link will read flash memory directly to do verify.
 * @note      Sector can be significant.We check the page rather than the sector,
 *            having regard to the actual size of the ram.
 *******************************************************************************
 */
__attribute__ (( section(".ramcode") ))
int FlashBlankCheck (unsigned long checkAddr,unsigned long size)
{
int i;
register uint16_t *pageBuf = (uint16_t *)checkAddr;

#ifdef FLASHING_INFO

#endif

	size /= 2;

	for(i = 0; i < size; i++)
	{
		if(pageBuf[0] != 0xFFFF)
		{
			return (TO_DRV_RET_ERROR);
		}
		pageBuf++;
	}

  return (TO_DRV_RET_OK);
}


