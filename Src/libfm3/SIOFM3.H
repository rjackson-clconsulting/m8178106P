//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                FUJILIB
//  Nome File:               SIOFM3.H
//  Descrizione:             Definizioni per modulo hw SIO per Fuji FM3/FM4
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2003/01/10 00:00:00 $
//
//---------------------------------------------------------------------------//

#ifndef _SIOFM3_H
#define _SIOFM3_H

#include "global.h"
#include "board_define.h"
#if (THIS_BOARD == FLEXIPER_CONTROLLER)
    #include "mb9bd10t.h"
#else
    #include "mb9bf50x.h"
#endif
#include "error.h"
#include "combuf.h"
//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
#define SIO_BASE_ERR		BASEMOD_SIOMA1					// numero di partenza per gli errori

#define SIO_BASEFREQ	(long) (CRYSTAL_FREQ * 10.0)		// stessa frequenza del quarzo
#define BRG_BASEFREQ	(long) (CRYSTAL_FREQ * 10.0)		// mezza frequenza del quarzo per
															// baud rate generator di uart e vl-csi

#define UART_SSR_ORE	BIT3		// Overrun error flag
#define UART_SSR_FRE 	BIT4		// Framing error flag
#define UART_SSR_PE 	BIT5		// Parity error flag
#define UART_SSR_REC	BIT7		// Received error flag clear

					//////////////////////////////////////////////////////////////////////////
					// definizione degli alias per i pin come "volatili" perchè altrimenti il
					// compilatore usa "troppo" i registri macchina !!
					// La problematica è sentita solamente nelle routines di "emulazione"
					// software delle seriali sincrone in cui viene generato il clock
					// "a mano"; in caso di semplici settaggi il problema non sussiste
					//////////////////////////////////////////////////////////////////////////

										// Pin usati per CSI0
#define _SDIN0	((volatile struct bitf *)&P4)->bit01		// SDIN
#define _SDOUT0	((volatile struct bitf *)&P4)->bit00		// SDOUT
#define _SCLK0	((volatile struct bitf *)&P4)->bit02       	// SCLK

										// Pin usati per CSI1
#define _SDIN1	((volatile struct bitf *)&P4)->bit04		// SDIN
#define _SDOUT1	((volatile struct bitf *)&P4)->bit03		// SDOUT
#define _SCLK1	((volatile struct bitf *)&P4)->bit05       	// SCLK

										// Pin usati per CSI2
#define _SDIN2	((volatile struct bitf *)&P3)->bit01		// SDIN
#define _SDOUT2	((volatile struct bitf *)&P3)->bit00		// SDOUT
#define _SCLK2	((volatile struct bitf *)&P3)->bit02       	// SCLK

										// Pin usati per CSI3 (non usato)
//#define _SDIN3	((volatile struct bitf *)&P2)->bit03		// SDIN
//#define _SDOUT3	((volatile struct bitf *)&P2)->bit04		// SDOUT
//#define _SCLK3	((volatile struct bitf *)&P2)->bit05       	// SCLK

										// Pin usati per CSI4
#define _SDIN4	PinMISO_PLD
#define _SDOUT4	PinMOSI_PLD
#define _SCLK4	PinSCLK_PLD

#define INT_DISABLE 0
#define INT_ENABLE  1
#define RELOAD_VALUE_CONSTANT 1
#define UART_MIN_RELOAD_VAL 4
#define CSI_MIN_RELOAD_VAL 3
#define MAX_RELOAD_VAL 29999

#define SIO_CHAN_TOT  8 //Number of MFSC device available on that chip



//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//
enum CSIOIntCause {
CSIOINT_NONE ,                  //No cause - Init Val
CSIOINT_TXDRE ,                 //Transmit Data Register is empty
CSIOINT_TXNODATA ,              //No data transmission
CSIOINT_TXFIFOE ,               //Transmit FIFO is empty
CSIOINT_TXINTNDCAUSE            //Not detected interrupt cause - Error
};


enum SIOErrors{ SIO_ERR_NONE,
				SIO_ERR_INV_CHAN=SIO_BASE_ERR,	// canale non valido
				SIO_ERR_INV_MODE, 		// modalità di funzionamento non valida
				SIO_ERR_INV_RATE, 		// modalità di funzionamento non valida
				SIO_ERR_INV_PRIO, 		// priorità irq non valida
				SIO_ERR_BASEFREQ, 		// i conti non tornano con la frequenza di base
				SIO_ERR_DATABIT, 		// i conti non tornano sulla lunghezza del frame
				SIO_ERR_STOPBIT, 		// i conti non tornano sui bit di stop
				SIO_ERR_INV_PARITY,     // i conti non tornano sulla parità
				SIO_ERR_DIRBIT, 		// i conti non tornano sulla direzione di shift
				SIO_ERR_CLKMODE,		// i conti non tornano sul tipo di clock
				SIO_ERR_XFR_MODE,		// modo trasferimento non valido
				SIO_ERR_XFR_NOTIMP,		// modo trasferimento non implementato
				SIO_ERR_INV_BUFFER,		// buffer proprio triste
   				SIO_ERR_BAUD_CHANGE		// cerca di cambiare baud rate su canale sincrono
			};

typedef enum SIOErrors SIOErrors;

enum SIOChan{ SIO_CHAN0,
			  SIO_CHAN1,
			  SIO_CHAN2,
			  SIO_CHAN3,
			  SIO_CHAN4,
              SIO_CHAN5,
			  SIO_CHAN6,
			  SIO_CHAN7,
			  SIO_CHAN_CLOSE,
              SIO_CHAN3_0 = SIO_CHAN3 + 0x4000,  //Used in Flexiper application with controller TYPE2 Micro
              SIO_CHAN3_1 = SIO_CHAN3 + 0x6000,  //Used in Flexiper application with controller TYPE2 Micro
              SIO_CHAN3_2 = SIO_CHAN3 + 0x8000,  //Used in Flexiper application with controller TYPE2 Micro
			  };


typedef enum SIOChan SIOChan;

									// tutte le modalità di configurazione
enum SIOMode{ SIO_CSI_MASTER_FULL,		// canale configurato come seriale sincrona master con tx & rx
			  SIO_CSI_MASTER_RXONLY,	// canale configurato come seriale sincrona master con solo tx
			  SIO_CSI_SLAVE_FULL,		// canale configurato come seriale sincrona slave con tx & rx
			  SIO_CSI_SLAVE_RXONLY,		// canale configurato come seriale sincrona slave con solo rx
			  SIO_UART, 				// canale configurato come uart
			  SIO_I2C, 					// canale configurato come IquadroC
			  SIO_SPI_MASTER_FULL ,      // MFS channel configured to work with SPI Mode Tranfer 1, Master mode with both RX and TX
			  SIO_SPI_MASTER_ONLY_TX,    // MFS channel configured to work with SPI Mode Tranfer 1, Master mode only TX feature
			  SIO_SPI_MASTER_ONLY_RX ,   // MFS channel configured to work with SPI Mode Tranfer 1, Master mode only RX feature
			  SIO_SPI_SLAVE_FULL  ,      // MFS channel configured to work with SPI Mode Tranfer 1, Slave mode with both RX and TX
			  SIO_SPI_SLAVE_ONLY_TX  ,   // MFS channel configured to work with SPI Mode Tranfer 1, Slave mode only TX feature
			  SIO_SPI_SLAVE_ONLY_RX     // MFS channel configured to work with SPI Mode Tranfer 1, Slave mode only RX feature

			  };



typedef enum SIOMode SIOMode;

									// tutte le velocità di cominicazione
enum SIORate{ SIO_RATE4800,			// meno di 4800baud fa scappare da ridere...
			  SIO_RATE9600,
			  SIO_RATE19200,
			  SIO_RATE38400,
			  SIO_RATE57600,
			  SIO_RATE76800,
			  SIO_RATE115200,		// new entry
			  SIO_RATE230400,		// last-new entry

			  SIO_RATE200K,
			  SIO_RATE400K,
			  SIO_RATE500K,
			  SIO_RATE1MEG,
			  SIO_RATE1_5MEG,
			  SIO_RATE2MEG,
			  SIO_RATE_EXT
			  };

typedef enum SIORate SIORate;

									// numero di bit dati da comunicare al mondo
enum SIODataBits{ SIO_7BIT,			// meno di 7bit fa proprio scappare da ridere
			  	  SIO_8BIT,
			  	  SIO_9BIT,
			  	  SIO_10BIT,
			  	  SIO_11BIT,
			  	  SIO_12BIT,
			      SIO_13BIT,
			      SIO_14BIT,
			      SIO_15BIT,
			      SIO_16BIT
			  };

typedef enum SIODataBits SIODataBits;


enum SIOStopBits{ SIO_1STOP,			// meno di 1bit di stop non ce la faccio
			  	  SIO_2STOP,
			  	  SIO_NOSTOP
			  };
typedef enum SIOStopBits SIOStopBits;

enum SIODirBits{  SIO_MSB_FIRST,		// per primo deve uscire l'MSB default
			  	  SIO_LSB_FIRST		// per primo deve uscire l'LSB variante
			  };
typedef enum SIODirBits SIODirBits;

			  							// tutti i controlli di parità
enum SIOParityBits{ SIO_NOPARITY,		// niente controllo parità
			  	  	SIO_ODDPARITY,		// parità dispari
			  	  	SIO_EVENPARITY,		// controllo su parità pari
			  	  	SIO_SPACEPARITY		// niente controllo su parità ma space aggiunto ad ogni carattere
			  };
typedef enum SIOParityBits SIOParityBits;

			  							// tutti le modalità di sincronizzazione/generazione col clock
enum SIOClkMode{ SIO_CLKHIGH,			// clock attivo alto: rise edge = shift out, fall edge = shift in
			  	 SIO_CLKLOW,			// clock attivo basso: fall edge = shift out, rise edge = shift in
			  	 SIO_CLKHIGH_REV,		// reverse mode clock attivo alto: fall edge = shift out, rise edge = shift in
			  	 SIO_CLKLOW_REV			// reverse mode clock attivo basso: rise edge = shift out, fall edge = shift in
			    };                      // _REV Macros are defined for SPI communication

typedef enum SIOClkMode SIOClkMode;

									// tutte le modalità di trasmissione
enum SIOXfrMode{ SIO_XFR_POLL,			// tutto a polling
			  	 SIO_XFR_INT_RX,		// solo la ricezione ad interrupt
			  	 SIO_XFR_INT_ALL,		// tutto ad irq (errori compresi)
			  	 SIO_XFR_DMA_RX,		// solo la ricezione con dma
			  	 SIO_XFR_DMA_ALL,		// sia trasmissione che ricezione con DMA
			  	 SIO_XFR_SOFT			// sia trasmissione che ricezione in emulazione software
			  };

typedef enum SIOXfrMode SIOXfrMode;

enum IRQPriority{					// priorità disponibili per i vari irq
		IRQ_PRIO0,						// priorità + alta
		IRQ_PRIO1,
		IRQ_PRIO2,
		IRQ_PRIO3,
		IRQ_PRIO4,
		IRQ_PRIO5,
		IRQ_PRIO6,
		IRQ_PRIO7 						// priorità + bassa
		};

typedef enum IRQPriority IRQPriority;

struct SioCfgParam{					// parametri di configurazione di un canale
	bool 			configured;			// flaggettino che segnala il passaggio per la funzione di config.
	byte			errCount;			// contatore errori rx uart
	byte			csiSoftBitCnt;		// bit count per csi in modalità softw
	word			csiSoftMsbMask;		// maschera bit msb per csi in modalità softw

    word			cntOverrunError;
	word			cntFrameError;
    word			cntParityError;

	SIOMode 		cfgMode;
	SIORate 		cfgRate;
	SIODataBits 	cfgDataBits;
	SIOStopBits 	cfgStopBits;
	SIOParityBits 	cfgParityBits;
	SIODirBits 		cfgDirBits;
	SIOClkMode 		cfgClkMode;
	SIOXfrMode 		cfgXfrMode;
	IRQPriority		cfgIrqPrio;			// livello di priorità interrupt richiesto

	ComRingBuf		*pComBuf;			// puntatore ai buffers di comunicazione
	void			(*pfPoller)(void);	// puntatore alla funzione di pollaggio della periferica
};

typedef struct SioCfgParam SioCfgParam;

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

void sio_init(void);
void sio_poll(void);

bool sio_cfgChan(SIOChan reqChan, CBufHandle hBuf, SIOMode reqMode, SIORate reqRate, SIODataBits reqDataBits,
				                  SIOStopBits reqStopBits, SIOParityBits reqParityBits,
				                  SIODirBits reqDirBits, SIOClkMode reqClkMode, SIOXfrMode reqXfrMode,
				                  IRQPriority reqPrio);

bool sio_changeUartSettings(SIOChan reqChan, SIORate reqRate, SIODataBits reqDataBits,
											 SIOStopBits reqStopBits, SIOParityBits reqParityBits);

#endif

