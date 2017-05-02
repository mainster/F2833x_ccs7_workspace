/**
 * @file        md_config.h
 * @project		MD_F2833x_LIB
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

/* ---------------------------  Private typedefs  -------------------------- */
extern MD_GPIO_TypeDef_t MD_GPIO_TypeDef;

/* ----------------------------  Configuration  ---------------------------- */
MD_GPIO_TypeDef_t MD_Gpios[CONFIG_MATRIX_ROWS] = {
	{ LED_RED,			MD_GPIO34, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PUDis, GPIO_OFF },
//	{ ILI934x_WRx_PIN,	MD_GPIO0,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PUEna, GPIO_ON  },
//	{ ILI934x_nCS_PIN,	MD_GPIO19, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PUEna, GPIO_ON  },
//	{ ILI934x_SCK_PIN,	MD_GPIO18, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PUDis, GPIO_OFF },
//	{ ILI934x_SDI_PIN,	MD_GPIO17, GPIO_Mode_AF1, GPIO_OType_OD, GPIO_PUEna, GPIO_OFF },
//	{ ILI934x_SDO_PIN,	MD_GPIO16, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PUDis, GPIO_OFF },
};

/* ----------------------------  Private macros  --------------------------- */

/* --------------------------  Private functions  -------------------------- */

/* --------------------------  Private variables  -------------------------- */

/* ----------------------------  Private defines  -------------------------- */

/* ---------------------  Private function prototypes  --------------------- */

#ifdef __cplusplus
 }
#endif

#endif
