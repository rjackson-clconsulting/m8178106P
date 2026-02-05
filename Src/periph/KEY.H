//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                GENLIB
//  Nome File:               KEY.H
//  Descrizione:             Definizioni gestione buffer tastiera
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2002/03/22 00:00:00 $
//
//---------------------------------------------------------------------------//

#ifndef _KEY_H
#define _KEY_H


//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#include "board_define.h"
#include "global.h"


#define MAX_KEYBUF  100

#define F1          259
#define F2          260
#define F3          261
#define F4          262
#define F5          263
#define F6          264
#define F7          265
#define F8          266
#define F9          267
#define F10         268
#define F11         333
#define F12         334

#define CTR_F1      294
#define CTR_F2      295
#define CTR_F3      296
#define CTR_F4      297
#define CTR_F5      298
#define CTR_F6      299
#define CTR_F7      300
#define CTR_F8      301
#define CTR_F9      302
#define CTR_F10     303
#define CTR_F11     337
#define CTR_F12     338

#define CTR_SHF_F1   694
#define CTR_SHF_F2   695
#define CTR_SHF_F3   696
#define CTR_SHF_F4   697
#define CTR_SHF_F5   698
#define CTR_SHF_F6   699
#define CTR_SHF_F7   700
#define CTR_SHF_F8   701
#define CTR_SHF_F9   702
#define CTR_SHF_F10  703
#define CTR_SHF_F11  737
#define CTR_SHF_F12  738

#define ALT_F1      304
#define ALT_F2      305
#define ALT_F3      306
#define ALT_F4      307
#define ALT_F5      308
#define ALT_F6      309
#define ALT_F7      310
#define ALT_F8      311
#define ALT_F9      312
#define ALT_F10     313
#define ALT_F11     339
#define ALT_F12     340

#define ALT_1       320
#define ALT_2       321
#define ALT_3       322
#define ALT_4       323
#define ALT_5       324
#define ALT_6       325
#define ALT_7       326
#define ALT_8       327
#define ALT_9       328
#define ALT_10      329
#define ALT_11      330
#define ALT_12      331

#define SHF_F1      284
#define SHF_F2      285
#define SHF_F3      286
#define SHF_F4      287
#define SHF_F5      288
#define SHF_F6      289
#define SHF_F7      290
#define SHF_F8      291
#define SHF_F9      292
#define SHF_F10     293
#define SHF_F11     235
#define SHF_F12     236

#define BELL        7
#define ENTER       13
#define TAB         9
#define ESC         27
#define INS         282
#define HOME        271
#define PG_UP       273
#define PG_DN       281
#define END         279
#define DEL         283
#define BS          8
#define AR_UP       272
#define AR_DN       280
#define AR_RG       277
#define AR_LF       275

#define CTR_ENTER   10
#define CTR_TAB     348
#define CTR_ESC     27
#define CTR_INS     346
#define CTR_HOME    319
#define CTR_PG_UP   332
#define CTR_PG_DN   318
#define CTR_END     317
#define CTR_DEL     347
#define CTR_BS      127
#define CTR_AR_UP   341
#define CTR_AR_DN   345
#define CTR_AR_RG   316
#define CTR_AR_LF   315

#define ALT_ENTER   128
#define ALT_TAB     365
#define ALT_ESC     27
#define ALT_INS     362
#define ALT_HOME    351
#define ALT_PG_UP   353
#define ALT_PG_DN   361
#define ALT_END     359
#define ALT_DEL     363
#define ALT_BS      214
#define ALT_AR_UP   352
#define ALT_AR_DN   360
#define ALT_AR_RG   357
#define ALT_AR_LF   355

#define SHF_TAB     615

#define CTR_A       1    //
#define CTR_B       2    //
#define CTR_C       3    //
#define CTR_D       4	 //
#define CTR_E       5	 //
#define CTR_F       6    //
#define CTR_G       7    //
#define CTR_H       8	 //
#define CTR_I       9    //
#define CTR_J       10   // Non usare , codice di CTR_ENTER
#define CTR_K       11   //
#define CTR_L       12	 //
#define CTR_M       13	 // Non usare , codice di ENTER
#define CTR_N       14   //
#define CTR_O       15   //
#define CTR_P       16	 //
#define CTR_Q       17	 //
#define CTR_R       18   //
#define CTR_S       19   //
#define CTR_T       20	 //
#define CTR_U       21	 //
#define CTR_V       22   //
#define CTR_W       23   //
#define CTR_X       24   //
#define CTR_Y       25   //
#define CTR_Z       26   //

#define ALT_A       230  //
#define ALT_B       248  //
#define ALT_C       246  //
#define ALT_D       232  //
#define ALT_E       218  //
#define ALT_F       233  //
#define ALT_G       234  //
#define ALT_H       235  // non usare
#define ALT_I       223  //
#define ALT_J       236  //
#define ALT_K       237  //
#define ALT_L       238
#define ALT_M       250
#define ALT_N       249
#define ALT_O       224
#define ALT_P       225
#define ALT_Q       216
#define ALT_R       219
#define ALT_S       231
#define ALT_T       220
#define ALT_U       222
#define ALT_V       247
#define ALT_W       217
#define ALT_X       245
#define ALT_Y       221
#define ALT_Z       244


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//


#ifdef __cplusplus
extern "C" {
#endif

void key_init(void);
word getk(void);
word keypres(void);
word ungetk(word);
word ungetks(char *s);

void key_scan_selector(void);
void key_scan(void);

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern long cntEncSelector;		// conteggio tipo encoder del selettore	manuale


//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: KEY.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

#endif

