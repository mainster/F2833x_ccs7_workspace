/**
 * @file        md_globals.h
 * @project		MD_GPIO_LIB_F2833x
 *
 * @date        6 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015 
 * @license		GNU GPL v3
 * 
   @verbatim

	------------------------------------------------------------------------

	Copyright (C) 2017  Manuel Del Basso

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	------------------------------------------------------------------------

   @endverbatim
 *
 */
#ifndef MD_GLOBALS_H_
#define MD_GLOBALS_H_

/* -------------------------------  Includes  ------------------------------ */
#include <stdint.h>
#include <DSP28x_Project.h>
//#include "md_fonts.h"
//#include "md_ili934x.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* ----------------------------  Global defines  -------------------------- */
#define _ENABLE 	1
#define _DISABLE 	0

#define ON 			1
#define OFF 		0

/* --------------------------  Global variables  --------------------------- */

/* ----------------------------  Configuration  ---------------------------- */
#define CONFIG_MATRIX_ROWS	2

typedef enum {
	LED_RED = 0,    		//!< Red LED at GPIO34
//	ILI934x_WRx_PIN,
//	ILI934x_nCS_PIN,
//	ILI934x_SCK_PIN,
//	ILI934x_SDI_PIN,
//	ILI934x_SDO_PIN,
//	DBG_TIMING_PE2  		//!< Not used!
} MD_GPIO_Name_t;

/* ---------------------------  Global typedefs  --------------------------- */

/* ----------------------------  Global macros  ---------------------------- */
#define StartCpuTimer(x) CpuTimer##x.RegsAddr->TCR.bit.TSS = 0
#define StopCpuTimer(x) CpuTimer##x.RegsAddr->TCR.bit.TSS = 1

/* ---------------------  Global function prototypes  ---------------------- */
void MD_ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period);

/* ------------------------------------------------------------------------- */
/* IRQ hook function for button XINT IRQ (must be HW interconnected) */
void onBtn_pressed(void);

/* IRQ hook function for encoder LSB pin IRQs */
void onEncLsb_changed(void);

/* ------------------------------------------------------------------------- */
/* IRQ hook function for SPI TxFifo IRQ */
void onSpiTxFifo_irq(void);

/* IRQ hook function for SPI RxFifo IRQ */
void onSpiRxFifo_irq(void);

#ifndef RTOS
__interrupt
#endif
void XINT2_EncLsb_isr(void);

/* ------------------------------------------------------------------------- */

#ifndef RTOS
__interrupt
#endif
void CPU_TIM0_isr(void);

#ifdef __cplusplus
 }
#endif

#endif
