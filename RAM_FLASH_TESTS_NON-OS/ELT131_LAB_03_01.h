/**
 * @file        ELT131_LAB_03_01.h
 * @project		ELT131_LAB_03_NON-OS
 *
 * @date        24 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       DESCRIPTION
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
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <stdbool.h>
#include "md_epwm.h"
#include "DSP28x_Project.h"
#include "md_bsp_explorer.h"
#include "md_globals.h"

/* ----------------------------  Configuration  ---------------------------- */
/* ---------------------------  Private typedefs  -------------------------- */
/* ----------------------------  Private defines  -------------------------- */

/* ----------------------------  Private macros  --------------------------- */
/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; \
        EDIS;

/* ---------------------  Private function prototypes  --------------------- */
/**
 * Remove compiler directive __interrupt from function declaration
 * and definition if registered as TI_RTOS irq callback.
 */
#ifndef RTOS
__interrupt
#endif
void XINT1_isr(void);

#ifndef RTOS
__interrupt
#endif
void XINT2_EncLsb_isr(void);

#ifndef RTOS
interrupt
#endif
void CPU_TIM0_isr(void);

void gpio1_ExtInt1Init();

#endif /* MAIN_H_ */
