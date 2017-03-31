/**
 * @file        main.c
 * @project		TEMPLATE_F28335_TI_RTOS
 *
 * @date        28 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Template project for TI-RTOS projects.
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
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "DSP28x_Project.h"

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;


/*
 * ===================== main of TEMPLATE_F28335_TI_RTOS =====================
 */
void main(void) {
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    System_printf("enter main()\n");
	
    BIOS_start();        /* enable interrupts and start SYS/BIOS */
}
