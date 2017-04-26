/**
 * @file        gpio_config.h
 * @project		ILI9341_F28335_v1
 * 
 * @date        26 Apr 2017
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
#ifndef MD_CONFIG_H_
#define MD_CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* -------------------------------  Includes  ------------------------------ */

/* ----------------------------  Configuration  ---------------------------- */

/* ---------------------------  Private typedefs  -------------------------- */
//extern MD_GPIO_TypeDef_t MD_GPIO_TypeDef;

/* ----------------------------  Private macros  --------------------------- */

/* --------------------------  Private functions  -------------------------- */

/* --------------------------  Private variables  -------------------------- */
//typedef enum MD_GPIOName {
//	LED_RED_A         = 0,    //!< LED
//	DBG_TIMING_PE2    = 1,    //!< Not used!
//} MD_GPIOName_t;

extern MD_GPIO_TypeDef_t MD_GPIO_TypeDef;

MD_GPIO_TypeDef_t MD_Gpios[] = {
	{ LED_RED_A, 		MD_GPIO34, GPIO_Mode_OUT, GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, GPIO_OFF },
	{ DBG_TIMING_PE2, 	MD_GPIO1,  GPIO_Mode_IN,  GPIO_OType_OD, MD_GPIO_PuPd_UP, GPIO_OFF },
};


/* ----------------------------  Private defines  -------------------------- */

/* ---------------------  Private function prototypes  --------------------- */


#ifdef __cplusplus
 }
#endif

#endif
