/**
 * @file        md_gpio.c
 * @project		MD_BSP_EXPLORER_F28335
 *
 * @date        28 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Provides high level GPIO initialization and manipulation
 *              methods.
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
#include "md_gpio.h"
#include <DSP2833x_Gpio.h>

/**
 * @brief      Defines a list of used GPIO pins and their configurations. It
 *             provides clarity and simplifies initialization procedure. Needs
 *             to be ordered like in typedef GPIO_NAME_t
 */
MD_GPIO_t MD_GPIO[] = {
 { LED_GREEN_A, GPIOPA, (1 << 11), GPIO_STATE_OFF },
};


/**
 * @brief      Initialize all GPIOs from MD_GPIO.
 */
void MD_GPIO_Init(void) {
//	GPIO_InitTypeDef GPIO_InitStructure;
//	MD_GPIO_NAME_t name;
//
//	for (int idx = 0; idx < sizeof(MD_GPIO) / sizeof(MD_GPIO[0]); idx++) {
//
//		//!< Enable clock source.
//		RCC_AHB1PeriphClockCmd(MD_GPIO[idx]._GPIO_AHB_CLK, ENABLE);
//
//		GPIO_InitStructure.GPIO_Pin = MD_GPIO[idx]._GPIO_PIN;
//		GPIO_InitStructure.GPIO_Mode = MD_GPIO[idx]._GPIO_Mode;
//		GPIO_InitStructure.GPIO_OType = MD_GPIO[idx]._GPIO_OType;
//		GPIO_InitStructure.GPIO_PuPd = MD_GPIO[idx]._GPIO_PuPd;
//		GPIO_InitStructure.GPIO_Speed = MD_GPIO[idx]._GPIO_Speed;
//		GPIO_Init(MD_GPIO[idx]._GPIO_PORT, &GPIO_InitStructure);
//
//		//!< Default value
//		(MD_GPIO[idx]._GPIO_INIT ==GPIO_STATE_OFF)
//				? 	MD_GPIO_Off(idx)
//				: 	MD_GPIO_On(idx);
//
//	}
}

/**< Enable GPIO
 */
void MD_GPIO_Off(MD_GPIO_NAME_t name) {
	MD_GPIO[name]._GPIO_PORT->BSRRH = MD_GPIO[name]._GPIO_PIN;
}

/**< Disable GPIO
 */
void MD_GPIO_On(MD_GPIO_NAME_t name) {
	MD_GPIO[name]._GPIO_PORT->BSRRL = MD_GPIO[name]._GPIO_PIN;
}

/**< LED toggeln
 */
void MD_GPIO_Toggle(MD_GPIO_NAME_t name) {
	MD_GPIO[name]._GPIO_PORT->ODR ^= MD_GPIO[name]._GPIO_PIN;
}

//--------------------------------------------------------------
// LED ein- oder ausschalten
//--------------------------------------------------------------
void MD_GPIO_Switch(MD_GPIO_NAME_t name, MD_GPIO_STATE_t newState) {
	if (newState == GPIO_STATE_OFF) {
		MD_GPIO_Off(name);
	} else {
		MD_GPIO_On(name);
	}
}
