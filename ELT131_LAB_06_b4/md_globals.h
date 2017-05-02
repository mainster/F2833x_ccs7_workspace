/**
 * @file        md_globals.h
 * @project		ELT131_LAB_06_b4
 *
 * @date        29 Apr 2017
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
#ifndef MD_GLOBALS_H_
#define MD_GLOBALS_H_

/* -------------------------------  Includes  ------------------------------ */
#include <stdint.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Clock.h>


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

/* ---------------------------  Global typedefs  --------------------------- */

/* ----------------------------  Global macros  ---------------------------- */

/* ---------------------  Global function prototypes  ---------------------- */
void onEncLsb_changed(UArg arg);
void onBtn_pressed(void);

#define CONFIG_MATRIX_ROWS  1

typedef enum {
    MD_LED_RED = 0,         //!< Red LED at GPIO34
} MD_GPIO_Name_t;



#ifdef __cplusplus
 }
#endif

#endif
