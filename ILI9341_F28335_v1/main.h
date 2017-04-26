/**
 * @file        main.h
 * @project		ILI9341_F28335_v1
 * 
 * @date        25 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015 
 * @license		GNU GPL v3
 * 
 * @brief       TODO
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
#ifndef MAIN_H_
#define MAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* -------------------------------  Includes  ------------------------------ */
#include <DSP28x_Project.h>
#include <DSP2833x_Gpio.h>
#include <DSP2833x_GlobalPrototypes.h>
#include <DSP28x_Project.h>

#include "md_globals.h"
#include "md_gpio.h"

/* ----------------------------  Configuration  ---------------------------- */

/* ---------------------------  Private typedefs  -------------------------- */

/* ----------------------------  Private macros  --------------------------- */

/* --------------------------  Private functions  -------------------------- */

#ifndef RTOS
__interrupt
#endif
void XINT2_EncLsb_isr(void);

#ifndef RTOS
interrupt
#endif
void CPU_TIM0_isr(void);

/* --------------------------  Private variables  -------------------------- */

/* ----------------------------  Private defines  -------------------------- */

/* ---------------------  Private function prototypes  --------------------- */


#ifdef __cplusplus
 }
#endif

#endif
