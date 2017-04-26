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
MD_GPIO_TypeDef_t MD_Gpios[] = {
	{ LED_RED,			MD_GPIO34, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuDIS, GPIO_OFF },
	{ ILI934x_WRx_PIN,	MD_GPIO0,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuDIS, GPIO_ON  },
	{ ILI934x_nCS_PIN,	MD_GPIO19, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PuENA, GPIO_ON  },
	{ ILI934x_SCK_PIN,	MD_GPIO18, GPIO_Mode_AF2, GPIO_OType_PP, GPIO_PuDIS, GPIO_OFF },
	{ ILI934x_SDI_PIN,	MD_GPIO13, GPIO_Mode_AF3, GPIO_OType_OD, GPIO_PuENA, GPIO_OFF },
	{ ILI934x_SDO_PIN,	MD_GPIO5,  GPIO_Mode_AF2, GPIO_OType_PP, GPIO_PuDIS, GPIO_OFF },
//	{ ILI934x_nCS_PIN,	MD_GPIO19, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PuENA, GPIO_ON  },
//	{ ILI934x_SCK_PIN,	MD_GPIO18, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PuDIS, GPIO_OFF },
//	{ ILI934x_SDI_PIN,	MD_GPIO17, GPIO_Mode_IN,  GPIO_OType_OD, GPIO_PuENA, GPIO_OFF },
//	{ ILI934x_SDO_PIN,	MD_GPIO16, GPIO_Mode_AF1, GPIO_OType_PP, GPIO_PuDIS, GPIO_OFF },
};


/**
 * @brief  This SPI pins are used on STM32F429-Discovery board
 */
//#ifndef ILI934x_SPI
//#define ILI934x_SPI           SPI5
//#define ILI934x_SPI_PINS      MD_SPI_PinsPack_1
//#endif
//
///**
// * @brief  CS PIN for SPI, used as on STM32F429-Discovery board
// */
//#ifndef ILI934x_CS_PIN
//#define ILI934x_CS_PORT       GPIOC
//#define ILI934x_CS_PIN        GPIO_PIN_2
//#endif
//
///**
// * @brief  WRX PIN for data/command, used as on STM32F429-Discovery board
// */
//#ifndef ILI934x_WRX_PIN
//#define ILI934x_WRX_PORT      GPIOD
//#define ILI934x_WRX_PIN       GPIO_PIN_13
//#endif
//
///**
// * @brief  RESET for LCD
// */
//#ifndef ILI934x_RST_PIN
//#define ILI934x_RST_PORT      GPIOD
//#define ILI934x_RST_PIN       GPIO_PIN_12
//#endif

/* ----------------------------  Private macros  --------------------------- */

/* --------------------------  Private functions  -------------------------- */

/* --------------------------  Private variables  -------------------------- */

/* ----------------------------  Private defines  -------------------------- */

/* ---------------------  Private function prototypes  --------------------- */

#ifdef __cplusplus
 }
#endif

#endif
