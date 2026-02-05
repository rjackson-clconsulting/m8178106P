/**
@file		board_functDeclare.h
@brief		Declaration section for board.cpp
@author		Fernando Morani
@date		07/03/2018
@version	01.01
@source     VetSmart - Protective Micro
*/

#ifndef _BOARD_FUNCTDECLARE_H_
#define _BOARD_FUNCTDECLARE_H_

void pin_port0_Set();
void pin_port1_Set();
void pin_port2_Set();
void pin_port3_Set();
void pin_port4_Set();
void pin_port5_Set();
void pin_port6_Set();
void pin_port7_Set();

void pin_SpecAde_Set();

void setEPFR02Reg();  //MFT Unit 1
void setEPFR07Reg();  //SIO Chanell setting
void setEPFR08Reg();  //SIO Chanell setting

void nvic_init();
void irqSelect();
#endif
