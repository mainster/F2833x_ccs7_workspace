/**
 * @file        md_globals.h
 * @project		ELT131_LAB_03_NON-OS
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

#ifdef __cplusplus
 extern "C" {
#endif

/* ----------------------------  Global defines  -------------------------- */
#define _ENABLE 	1
#define _DISABLE 	0

#define ON 			1
#define OFF 		0

#define F_CPU	150e6
/* --------------------------  Global variables  --------------------------- */

/* ----------------------------  Configuration  ---------------------------- */
#define CONFIG_MATRIX_ROWS	1

typedef enum {
	MD_LED_RED = 0,    		//!< Red LED at GPIO34
} MD_GPIO_Name_t;
/* ---------------------------  Global typedefs  --------------------------- */

/* ----------------------------  Global macros  ---------------------------- */

/* ---------------------  Global function prototypes  ---------------------- */

#ifdef __cplusplus
 }
#endif

#endif
