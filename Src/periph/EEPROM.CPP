/**
@file		eeprom.cpp
@brief		EEPROM or FRAM memory on SPI bus (formerly developed by COMETA)
@author		Fernando Morani
@date		09/05/2017
@version	01.00
@source     M75-CPU Protective
*/
#include "eeprom.h"

/*****************************************
** STATIC VAR SECTION
******************************************/
static byte deviceAddress;
static word readAddress;
static word writeAddress;
static EEPErrors eepError;

/*****************************************
** GLOBAL VAR SECTION
******************************************/
byte readBuf[60];
FM3_MFS47_CSIO_TypeDef    *pCSI_EEP = FM3_MFS6_CSIO;

/*****************************************
** IMPLEMENTATION FUNCTION SECTION
******************************************/

/**
 ******************************************************************************
 ** \brief  Init the memory peripheral.
 ** \param  byte __deviceAddr: Address of the device
 ******************************************************************************/
void eep_init(byte __deviceAddr)
{
    deviceAddress = __deviceAddr;               // salva l'indirizzo richiesto.
    readAddress = 0;
    writeAddress = 0;
    eepError = EEP_ERR_NONE;
    eep_setupMem();
}

/**
 ******************************************************************************
 ** \brief  Returns corrent error and reset error
 ******************************************************************************/
int eep_errors(void)
{
int error = eepError;

    eepError = EEP_ERR_NONE;                    // reset errore

    return( error );
}

/**
 ******************************************************************************
 ** \brief  Set address device
 ** \param  byte __deviceAddr: Address of the device
 ******************************************************************************/
void eep_setDeviceAddress(byte __deviceAddr)
{
    deviceAddress = __deviceAddr;               // salva l'indirizzo richiesto.

    if( deviceAddress > 7 )
    {
        eepError = EEP_ERR_DEV_ADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_setDeviceAddress", eepError);        // aspetta che lo vengano a prendere
#endif
    }

}


/**
 ******************************************************************************
 ** \brief  Read from user memory.
 ** \param  void *__data_buf: Buffer to read in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \return False if error or result of eep_readMem
 ******************************************************************************/
bool eep_readUserMem(void *__data_buf, int __count, int __start_addr)
{
    if( __start_addr > EEP_LAST_USERADDR || __start_addr + __count > EEP_LAST_USERADDR )
    {
        eepError = EEP_ERR_USERADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_readUserMem", eepError);     // aspetta che lo vengano a prendere
#endif
        return(False);
    }


    return( eep_readMem(__data_buf, __count, __start_addr + EEP_FIRST_USERADDR) );          // scrive sommando l'offset dell'area "user"
}

/**
 ******************************************************************************
 ** \brief  Write in user memory.
 ** \param  void *__data_buf: Buffer to write in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \return False if error or result of eep_writeMem
 ******************************************************************************/
bool eep_writeUserMem(void *__data_buf, int __count, int __start_addr)
{
    if( __start_addr > EEP_LAST_USERADDR || __start_addr + __count > EEP_LAST_USERADDR )    // rapido check
    {
        eepError = EEP_ERR_USERADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_writeUserMem", eepError);        // aspetta che lo vengano a prendere
#endif
        return(False);
    }

    return( eep_writeMem(__data_buf, __count, __start_addr + EEP_FIRST_USERADDR) );         // scrive sommando l'offset dell'area "user"
}

/**
 ******************************************************************************
 ** \brief  Read parameters from memory.
 ** \param  void *__data_buf: Buffer to read in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \param  bool *__chkOk
 ** \return False if error or result of eep_readMem
 ******************************************************************************/
bool eep_readParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk)
{
bool chkMatch;
bool readOk;
bool writeOk;
word chkBlock;
word chkMem;
word chkBlock2;

    *__chkOk = False;                   // reset flag...

    if( __start_addr > EEP_LAST_PAR1ADDR || __start_addr + __count > EEP_LAST_PAR1ADDR )    // rapido check
    {
        eepError = EEP_ERR_PARADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_readParam", eepError);       // aspetta che lo vengano a prendere
#endif
        return(False);
    }

    readOk = eep_readMem(readBuf, 32, __start_addr + EEP_FIRST_PAR1ADDR);      // legge sommando l'offset dell'area "parametri 1"

/*
	for(chkMem=0; chkMem<32; chkMem++)
		readBuf[chkMem] = chkMem;

    writeOk = drv_writeMem(readBuf, 32, __start_addr + EEP_FIRST_PAR1ADDR);    // ricopia sommando l'offset dell'area "parametri 2"

	for(chkMem=0; chkMem<32; chkMem++)
		readBuf[chkMem] = chkMem + 10;

    readOk = drv_readMem(readBuf, 32, __start_addr + EEP_FIRST_PAR1ADDR);      // legge sommando l'offset dell'area "parametri 1"
*/

                                        // verifica sul primo blocco
    readOk = eep_readMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR1ADDR);      // legge sommando l'offset dell'area "parametri 1"
    chkBlock = eep_chkCalcBuf(__param_buf, __count);		// fa calcolare il crc

										// recupera il valore memorizzato
    chkMem = eep_chkRead(__start_addr + EEP_FIRST_PAR1ADDR, __count);

// risparmiamo una lettura: calcolando il crc sul buffer letto si evita di leggere due volte i dati !!
//    chkMatch = eep_chkVerify(__start_addr + EEP_FIRST_PAR1ADDR, __count, chkBlock);

    chkMatch = (chkMem == chkBlock);

    if( chkMatch == True )              // se il chk è ok
    {
        *__chkOk = True;

        writeOk = True;

        if( readOk == True )            // se la lettura è ok...
        {
                                        // verifica comunque sul secondo blocco
            chkBlock2 = eep_chkRead(__start_addr + EEP_FIRST_PAR2ADDR, __count);
            chkMatch = eep_chkVerify(__start_addr + EEP_FIRST_PAR2ADDR, __count, chkBlock2);

                                            // se nel secondo blocco il chk non è a posto .....
                                            // o comunque se i due chk non sono uguali
            if( !chkMatch || chkBlock2 != chkBlock )
            {
                writeOk = eep_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR2ADDR);    // ricopia sommando l'offset dell'area "parametri 2"
                writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR2ADDR, __count, &chkBlock);     // ricopia pure il chk
            }

            if( chkMatch == False )              // se il chk NON è ok
            {
                eepError = EEP_ERR_CHKPAR2;

#ifdef __DEBUG__
                    error_trace("EEPDRV", "eep_readParam", eepError);       // aspetta che lo vengano a prendere
#endif
            }
        }

        if( readOk == False || writeOk == False )
        {
            eepError = EEP_ERR_DEV_IO;

#ifdef __DEBUG__
            error_trace("EEPDRV", "eep_readParam", eepError);       // aspetta che lo vengano a prendere
#endif
        }


        return( readOk );
    }
    else
    {
        eepError = EEP_ERR_CHKPAR1;

#ifdef __DEBUG__
            error_trace("EEPDRV", "eep_readParam", eepError);       // aspetta che lo vengano a prendere
#endif
    }


                                        // verifica sul secondo blocco
    readOk = eep_readMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR2ADDR);          // legge sommando l'offset dell'area "parametri 2"
    chkBlock = eep_chkCalcBuf(__param_buf, __count);

										// recupera il valore memorizzato
    chkMem = eep_chkRead(__start_addr + EEP_FIRST_PAR2ADDR, __count);

										// confronta con quello calcolato
    chkMatch = (chkMem == chkBlock);

    if( chkMatch == True )              // se il chk del secondo blocco è ok
    {

        *__chkOk = True;

        writeOk = True;

        if( readOk == True )                    // se legge ok !!
        {
            writeOk = eep_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR1ADDR);    // scrive sommando l'offset dell'area "parametri 1"
            writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR1ADDR, __count, &chkBlock);     // ricopia pure il chk
        }

        if( readOk == False || writeOk == False )
        {
            eepError = EEP_ERR_DEV_IO;

#ifdef __DEBUG__
            error_trace("EEPDRV", "eep_readParam", eepError);       // aspetta che lo vengano a prendere
#endif
        }

        return( readOk );
    }

    eepError = EEP_ERR_CHKPARX;

#ifdef __DEBUG__
    error_trace("EEPDRV", "eep_readParam", eepError);               // aspetta che lo vengano a prendere
#endif

    *__chkOk = False;

    return( False );                    // non è andata proprio bene bene ....
}

/**
 ******************************************************************************
 ** \brief  Write parameters in memory.
 ** \param  void *__param_buf: Buffer to write in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \param  bool *__chkOk
 ** \return False if error
 ******************************************************************************/
 bool eep_writeParam(void *__param_buf, int __count, int __start_addr, bool *__chkOk)
{
word chkBlock;
bool writeOk = True;

    *__chkOk = False;

    if( __start_addr > EEP_LAST_PAR1ADDR || __start_addr + __count > EEP_LAST_PAR1ADDR )    // rapido check
    {
        eepError = EEP_ERR_PARADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_writeParam", eepError);      // aspetta che lo vengano a prendere
#endif
        return(False);
    }

    chkBlock = eep_chkCalcBuf(__param_buf, __count);

/*              **** utile solo in caso di crc polinomiale ..... e su eeprom reale (no fram) ****
                                                        // se non è cambiato...
    if( chkBlock == eep_chkRead(__start_addr + EEP_FIRST_PAR2ADDR, __count) )
    {
        *__chkOk = True;
        return( True );                                 // xchè mai scrivere ?
    }
    else
    {
        writeOk &= drv_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR1ADDR);   // scrive sommando l'offset dell'area "parametri 1"
        writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR1ADDR, __count, &chkBlock);     // aggiorna il chk

        writeOk &= drv_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR2ADDR);   // scrive sommando l'offset dell'area "parametri 2"
        writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR2ADDR, __count, &chkBlock);     // aggiorna il chk pure qui

        *__chkOk = True;
    }
*/

    writeOk &= eep_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR1ADDR);   // scrive sommando l'offset dell'area "parametri 1"
    writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR1ADDR, __count, &chkBlock);     // aggiorna il chk

    writeOk &= eep_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_PAR2ADDR);   // scrive sommando l'offset dell'area "parametri 2"
    writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_PAR2ADDR, __count, &chkBlock);     // aggiorna il chk pure qui


    if( writeOk == False )
    {
        eepError = EEP_ERR_DEV_IO;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_writeParam", eepError);      // aspetta che lo vengano a prendere
#endif
    }

    *__chkOk = True;

    return( writeOk );
}


/**
 ******************************************************************************
 ** \brief  Read block of data from user memory
 ** \param  void *__param_buf: Buffer to write in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \param  bool *__chkOk
 ** \return False if error
 ******************************************************************************/
bool eep_readUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk)
{
bool chkMatch;
bool readOk;
word chkBlock;

    *__chkOk = False;

    if( __start_addr > EEP_FIRST_USERADDR || __start_addr + __count > EEP_LAST_USERADDR )   // rapido check
    {
        eepError = EEP_ERR_USERADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_readUserChkBlock", eepError);        // aspetta che lo vengano a prendere
#endif
        return(False);
    }

                                        // verifica sul primo blocco
    chkBlock = eep_chkRead(__start_addr + EEP_FIRST_USERADDR, __count);
    chkMatch = eep_chkVerify(__start_addr + EEP_FIRST_USERADDR, __count, chkBlock);

    if( chkMatch == True )              // se il chk è ok
    {
        *__chkOk = True;

        readOk = eep_readMem(__param_buf, __count, __start_addr + EEP_FIRST_USERADDR);      // legge sommando l'offset dell'area "parametri 1"

        if( readOk == False )
        {
            eepError = EEP_ERR_DEV_IO;

#ifdef __DEBUG__
            error_trace("EEPDRV", "eep_readUserChkBlock", eepError);        // aspetta che lo vengano a prendere
#endif
        }

        return( readOk );
    }

    eepError = EEP_ERR_CHKUSER;

#ifdef __DEBUG__
    error_trace("EEPDRV", "eep_readUserChkBlock", eepError);        // aspetta che lo vengano a prendere
#endif
    return( False );                    // non è andata proprio bene bene ....
}

/**
 ******************************************************************************
 ** \brief  Write block of data in user memory
 ** \param  void *__param_buf: Buffer to write in
 ** \param  int __count:
 ** \param  int __start_addr
 ** \param  bool *__chkOk
 ** \return False if error
 ******************************************************************************/
bool eep_writeUserChkBlock(void *__param_buf, int __count, int __start_addr, bool *__chkOk)
{
word chkBlock;
bool writeOk = True;

    *__chkOk = False;

    if( __start_addr > EEP_FIRST_USERADDR || __start_addr + __count > EEP_LAST_USERADDR )   // rapido check
    {
        eepError = EEP_ERR_USERADDR;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_writeUserChkBlock", eepError);       // aspetta che lo vengano a prendere
#endif
        return(False);
    }

    writeOk &= eep_writeMem(__param_buf, __count, __start_addr + EEP_FIRST_USERADDR);   // scrive sommando l'offset dell'area "user"
    writeOk &= eep_chkWrite(__start_addr + EEP_FIRST_USERADDR, __count, &chkBlock);     // aggiorna il chk

    *__chkOk = True;

    if( writeOk == False )
    {
        eepError = EEP_ERR_DEV_IO;

#ifdef __DEBUG__
        error_trace("EEPDRV", "eep_writeUserChkBlock", eepError);       // aspetta che lo vengano a prendere
#endif
    }

    return( writeOk );
}


/**
 ******************************************************************************
 ** \brief  Checksum of requested block
 ** \param  void *__data_buf: Buffer to write in
 ** \param  int __count:
 ** \return calculated value
 ******************************************************************************/
word eep_chkCalcBuf(void *__data_buf, int __count)
{
register word i;
register byte *pd = (byte*) __data_buf;
word chkCalc;

    chkCalc = 0xFFFF;

    for(i=__count; i; i--)                      // somma tutto
    {
         chkCalc += *pd++;
    }

    return(chkCalc);
}
/**
 ******************************************************************************
 ** \brief  Write Checksum for requested block
 ** \param  word __blockAddr
 ** \param  word __blockSize
 ** \param  word *__chk
 ** \return calculated value
 ******************************************************************************/
// scrive il chk del blocco richiesto
bool eep_chkWrite(word __blockAddr, word __blockSize, word *__chk)
{
    return( eep_writeMem( (byte *)__chk, 2, __blockAddr + __blockSize) );
}

/**
 ******************************************************************************
 ** \brief  Read Checksum for requested block
 ** \param  word __blockAddr
 ** \param  word __blockSize
 ** \return  word *__chk
 ******************************************************************************/
word eep_chkRead(word __blockAddr, word __blockSize)
{
word chkLocal = 0;              // 0 -> valore non ammesso

    eep_readMem((byte *) &chkLocal, 2, __blockAddr + __blockSize);

    return( chkLocal );
}

/**
 ******************************************************************************
 ** \brief  Check and verify for parameters
 ** \param  word __blockAddr
 ** \param  word __blockSize
 ** \param  word __chk
 ** \return False if error
 ******************************************************************************/
bool eep_chkVerify(word __blockAddr, word __blockSize, word __chk)
{
register int i;
register byte *pd;
word chkCalc;
byte buf[32];
word bufAddr;
word bufSize;

    chkCalc = 0xFFFF;
    bufAddr = 0;
    memset(buf, 0 , sizeof(buf));               // reset buffer....

    while( __blockSize )
    {
        if( __blockSize >= 32 )
            bufSize = 32;
        else
            bufSize = __blockSize;

        __blockSize -= bufSize;                 // scala un attimo

                                                // ne legge un pochetto
        eep_readMem(buf, bufSize, __blockAddr + bufAddr);

        bufAddr += bufSize;                     // avanti l'indice al blocco

        pd = buf;                               // dall'inizio del buffer

        for(i=bufSize; i; i--)                  // somma tutto
             chkCalc += *pd++;
    }

    return( chkCalc == __chk );                 // torna a casa margherita
}


/**
 ******************************************************************************
 ** \brief  Setup Memory
 ******************************************************************************/
void eep_setupMem(void)
{
    MEM_CS = 1;       // fram disabilitata

    pCSI_EEP->SMR_f.MD1 = 1;       // seleziona modalità sincrona
    pCSI_EEP->SMR_f.SOE = 1;      // abilita la spi
    pCSI_EEP->SMR_f.SCKE = 1;
    pCSI_EEP->SMR_f.BDS = 1;
    pCSI_EEP->SMR_f.SCINV = 1;

    pCSI_EEP->SCR_f.TXE = 1;      // abilita sia TX che RX
    pCSI_EEP->SCR_f.RXE = 1;
    pCSI_EEP->SCR_f.SPI = 1;
    pCSI_EEP->SCR_f.MS = 0;

    pCSI_EEP->ESCR_f.L0 = 0;      // seleziona 8bit dati

    pCSI_EEP->BGR = 32;           // da CLKP1=32Mhz / 32 = 1Mhz spi

    pCSI_EEP->FCR = 0;            // disabilita la fifo ... non serve

}

/**
 ******************************************************************************
 ** \brief  Read from memory in user area
 ** \param  void *__data_buf
 ** \param  int __count
 ** \param  int __start_addr
 ** \return false if error
 ******************************************************************************/
bool eep_readMem(void *__data_buf, int __count, int __start_addr)
{
uint8_t *pBuf = (uint8_t *)__data_buf;
int cntRx;

	if( __start_addr > EEP_LAST_ADDR || __start_addr + __count > EEP_LAST_ADDR )
		return(False);

														// prima fase: selezione indirizzo
    MEM_CS = 0;       // parliamo con la fram

    eep_spi_putc(FM25_READ);                    // spara comando read mem
    eep_spi_putc(__start_addr >> 8);            // prima addr h
    eep_spi_putc(__start_addr);                 // poi addr l

    cntRx = 0;

    while( cntRx < __count )                    // legge di seguito
    {
        pBuf[cntRx++] = eep_spi_putc(0x18);     // reset receive flag
    }

	MEM_CS = 1;       // fine parlata con la fram

	return(True);										// trasferimento ok !!
}

/**
 ******************************************************************************
 ** \brief  write memory in user area
 ** \param  void *__data_buf
 ** \param  int __count
 ** \param  int __start_addr
 ** \return false if error
 ******************************************************************************/
bool eep_writeMem(void *__data_buf, int __count, int __start_addr)
{
uint8_t *pBuf = (uint8_t *)__data_buf;
int dummyRead = 0;
int cntWaitSpi = 0;
int cntTx = 0;

	if( __start_addr > EEP_LAST_ADDR || __start_addr + __count > EEP_LAST_ADDR )
		return(False);

														// prima fase: selezione indirizzo
    MEM_CS = 0;       // parliamo con la fram

    eep_spi_putc(FM25_WREN);                    // spara comando read mem

    MEM_CS = 1;       // fine comando

    eep_spi_putc(0);

    MEM_CS = 0;       // parliamo con la fram

    eep_spi_putc(FM25_WRITE);                   // spara comando write mem
    eep_spi_putc(__start_addr >> 8);            // prima addr h
    eep_spi_putc(__start_addr);                 // poi addr l

    cntTx = 0;

    while( cntTx < __count )             // legge di seguito
    {
        eep_spi_putc(pBuf[cntTx++]);
    }

	MEM_CS = 1;       // fine parlata con la fram

    eep_spi_putc(0);

    MEM_CS = 0;       // parliamo con la fram

    eep_spi_putc(FM25_WRDI);// spara comando write disable

	MEM_CS = 1;       // fine parlata con la fram

	return(True);										// trasferimento ok !!

}

/**
 ******************************************************************************
 ** \brief  Write char in SPI bus
 ** \param  int c: char to write
 ** \return read char
 ******************************************************************************/
int eep_spi_putc(int c)
{
int r;
int cntWaitSpi = 0;

   if( pCSI_EEP->SSR_f.RDRF )
        r = pCSI_EEP->RDR;

    pCSI_EEP->TDR = c;

    while( !pCSI_EEP->SSR_f.RDRF )            // piccola attesa dell'invio dati
        cntWaitSpi++;

    r = pCSI_EEP->RDR;

    return( r );
}

