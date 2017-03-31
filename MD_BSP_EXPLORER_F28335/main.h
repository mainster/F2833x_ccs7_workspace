/**
 * @file        main.h
 * @project		MD_BSP_EXPLORER_F28335
 *
 * @date        28 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Include file for main.c
 *
   @verbatim

    ------------------------------------------------------------------------

    Copyright (C) 2016  Manuel Del Basso

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
#include <DSP28x_Project.h>
#include <DSP2833x_GlobalPrototypes.h>
#include "md_bsp_explorer.h"
#include "md_globals.h"
#include "md_uart_sci.h"

interrupt void cpu_timer0_isr(void);

#endif /* MAIN_H_ */
