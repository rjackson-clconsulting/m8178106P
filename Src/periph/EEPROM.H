/**
@file		eeprom.h
@brief		EEPROM or FRAM memory on SPI bus (formerly developed by COMETA)
@author		Fernando Morani
@date		09/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#ifndef _EEPDRV_H_
#define _EEPDRV_H_

#include "mb9bf50x.h"   //Include the relevant file for this micro (Fujitsu Family Fm3 - TYPE 0)
                        //In this sw the used struct is the same for TYPE0 and TYPE2
#include "board_define.h" //For the Chip Select PIN
#include "error.h"
#include "global.h"


/*****************************************
** DEFINE SECTION
******************************************/
#define EEP_BASE_ERR    1
#define	EEP_FIRST_ADDR			0x0000		// primo indirizzo (mem fisica)
#define	EEP_LAST_ADDR			0x7FFF		// ultimo indirizzo (mem fisica)

#define EEP_FIRST_USERADDR          (EEP_FIRST_ADDR + 0x0600)       // primo indirizzo (user)
#define EEP_LAST_USERADDR           (EEP_LAST_ADDR - 0x600)         // ultimo indirizzo (user)
#define EEP_FIRST_PAR1ADDR          (EEP_FIRST_ADDR + 0x0000)       // primo indirizzo (parameter block1)
#define EEP_LAST_PAR1ADDR           (EEP_FIRST_ADDR + 0x05FF)       // ultimo indirizzo (parameter block1)
#define EEP_FIRST_PAR2ADDR          (EEP_LAST_ADDR - 0x5FF)         // primo indirizzo (parameter block2)
#define EEP_LAST_PAR2ADDR           (EEP_LAST_ADDR - 0x000)         // ultimo indirizzo (parameter block2)

#define FM25_WREN   0b00000110      // Set write enable latch
#define FM25_WRDI   0b00000100      // Write disable
#define FM25_RDSR   0b00000101      // Read Status Register
#define FM25_WRSR   0b00000001      // Write Status Register
#define FM25_READ   0b00000011      // Read memory data
#define FM25_WRITE  0b00000010      // Write memory data

#define MEM_CS      PinCS_FRAM_n   //Memory chip select

/*****************************************
** TYPE SECTION
******************************************/
enum EEPErrors{ EEP_ERR_NONE,
                EEP_ERR_DEV_ADDR=EEP_BASE_ERR,      // device address errato
                EEP_ERR_DEV_IO,                     // problemi di input/output dal device
                EEP_ERR_USERADDR,                   // inidirizzo memoria user errato
                EEP_ERR_PARADDR,                    // inidirizzo memoria parametri errato
                EEP_ERR_CHKPAR1,                    // crc errato area par1
                EEP_ERR_CHKPAR2,                    // crc errato area par2
                EEP_ERR_CHKPARX,                    // crc errato area par1 e par2
                EEP_ERR_CHKUSER                     // crc errato area user
                };

typedef enum EEPErrors EEPErrors;

/*****************************************
** FUNCTION SECTION
******************************************/
#ifdef __cplusplus
extern "C" {
#endif

void eep_setupMem(void);
bool eep_readMem(void *__data_buf, int __count, int __start_ddr);	// legge dalla memoria dell'eep
bool eep_writeMem(void *__data_buf, int __count, int __start_addr);	// scrive nella memoria dell'eep
int eep_spi_putc(int c);

#ifdef __XTEXT__							// se utilizza il segmento xtext ....
void eep_init(byte __deviceAddr)										 						__attribute__ ((section (".xtext")));
int  eep_errors(void)						 							 						__attribute__ ((section (".xtext")));
void eep_setDeviceAddress(byte __deviceAddr)							 						__attribute__ ((section (".xtext")));
bool eep_readUserMem(void *__data_buf, int __count, int __start_ddr)							 __attribute__ ((section (".xtext")));       // legge dalla memoria dell'eep
bool eep_writeUserMem(void *__data_buf, int __count, int __start_addr)							 __attribute__ ((section (".xtext")));     // scrive nella memoria dell'eep
bool eep_readParam(void *__param_buf, int __count, int __start_ddr, bool *__chkOk)				__attribute__ ((section (".xtext")));     // legge i parametri dalla memoria dell'eep
bool eep_writeParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk)			__attribute__ ((section (".xtext")));   // scrive i parametri nella memoria dell'eep
bool eep_readUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk)		 __attribute__ ((section (".xtext")));
bool eep_writeUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk)		 __attribute__ ((section (".xtext")));
word eep_chkCalcBuf(void *__data_buf, int __count)							 					__attribute__ ((section (".xtext")));
bool eep_chkWrite(word __blockAddr, word __blockSize, word *__chk)							 	__attribute__ ((section (".xtext")));         // scrive il chk del blocco richiesto
word eep_chkRead(word __blockAddr, word __blockSize)							 				__attribute__ ((section (".xtext")));                       // legge il chk dei blocco richiesto
bool eep_chkVerify(word __blockAddr, word __blockSize, word __chk)							 	__attribute__ ((section (".xtext")));         // calcola e verifica il chk dei parametri
#else				// common .text section
void eep_init(byte __deviceAddr);
int  eep_errors(void);
void eep_setDeviceAddress(byte __deviceAddr);
bool eep_readUserMem(void *__data_buf, int __count, int __start_ddr);       // legge dalla memoria dell'eep
bool eep_writeUserMem(void *__data_buf, int __count, int __start_addr);     // scrive nella memoria dell'eep
bool eep_readParam(void *__param_buf, int __count, int __start_ddr, bool *__chkOk);     // legge i parametri dalla memoria dell'eep
bool eep_writeParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk);   // scrive i parametri nella memoria dell'eep
bool eep_readUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk);
bool eep_writeUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk);
word eep_chkCalcBuf(void *__data_buf, int __count);
bool eep_chkWrite(word __blockAddr, word __blockSize, word *__chk);         // scrive il chk del blocco richiesto
word eep_chkRead(word __blockAddr, word __blockSize);                       // legge il chk dei blocco richiesto
bool eep_chkVerify(word __blockAddr, word __blockSize, word __chk);         // calcola e verifica il chk dei parametri
#endif

#ifdef __cplusplus
}
#endif

#endif

