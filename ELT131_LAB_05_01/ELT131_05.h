/**
 * @file        ELT131_LAB_03_RTOS_5.h
 * @project		ELT131_LAB_03_RTOS_5
 *
 * @date        5 Apr 2017
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
#ifndef _EL131_LAB_03_RTOS_H_
#define _EL131_LAB_03_RTOS_H_

/* -------------------------------  Includes  ------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include "DSP28x_Project.h"
#include "md_epwm.h"
#include "md_uart_sci.h"
#include "md_bsp_explorer.h"

/* ----------------------------  Configuration  ---------------------------- */
/* ---------------------------  Private typedefs  -------------------------- */
/* ----------------------------  Private defines  -------------------------- */

/* ----------------------------  Private macros  --------------------------- */
/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; 	/* Port mux: GPIO */ \
        GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; 		/* Data direction: output */ \
        GpioDataRegs.GPADAT.bit.GPIO0 = 0; 		/* Initial value: 0 */ \
        EDIS;

/**
 * Init external interrupts
 *
 * - GPIO1 Set as external interrupt src
 */
//#define gpioXintInit()    EALLOW; \
//        GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1; \
//        GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1; \
//        XIntruptRegs.XINT1CR.bit.ENABLE = 1; \
//        XIntruptRegs.XINT1CR.bit.POLARITY = 0; \
//		EDIS;

/* ---------------------  Private function prototypes  --------------------- */
//void xInt1Init(void);

/**
 * Remove compiler directive __interrupt from function declaration
 * and definition if registered as TI_RTOS irq callback.
 */
//#if  !defined(RTOS) || defined(ZERO_LATENCY_XINT1)
//interrupt
//#endif
//void XINT1_GPIO1_isr(void);

#endif /* MAIN_H_ */
