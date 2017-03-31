/**
 * @file        md_gpio.h
 * @project		MD_BSP_EXPLORER_F28335
 *
 * @date        28 Mar 2017
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
#ifndef MD_GPIO_H_
#define MD_GPIO_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <DSP2833x_Device.h>
#include <DSP2833x_Gpio.h>
#include "md_globals.h"

/**
 * @addtogroup MD_APP
 * @{
 */

/**
 * @addtogroup APP_GPIO General Purpose I/O
 * @{
 */

/**
 * @addtogroup GPIO_Macros
 * @{
 */

/**
 * @brief      Define the number of enumerated pins in MD_GPIO_NAME_t
 */
#define   GPIO_COUNT   11

///**
// * @brief      Provides pin toggle functionality for timing related debugging
// *             purposes.
// */
//#define   DBG_PID_TIMING_TOG()    (MDB_GPIO_Toggle(DBG_TIMING_PE6))
//
///**
// * @brief      Provides Tic functionality for timing related debugging
// *             purposes.
// */
//#define   DBG_PID_TIMING_TIC()    (MDB_GPIO_On(DBG_TIMING_PE6))
//
///**
// * @brief      Provides Toc functionality for timing related debugging
// *             purposes.
// */
//#define   DBG_PID_TIMING_TOC()    (MDB_GPIO_Off(DBG_TIMING_PE6))
//
///**
// * @brief      Provides pin set/clear functionality for PID timing debugging
// *             purposes.
// *
// * @param      s     0: Set debug port to state "off"
// *                  >0: Set debug port to state "on"
// */
//#define   DBG_PID_TIMING(s)       ((s) > 0) ? (MDB_GPIO_On(DBG_TIMING_PE6)) : \
//                                              (MDB_GPIO_Off(DBG_TIMING_PE6))
//
///**
// * @brief      Provides pin toggle functionality for ADC timing related
// *             debugging purposes.
// */
//#define   DBG_ADC_TIMING_TOG( )   (MDB_GPIO_Toggle(DBG_TIMING_PE4))
//
///**
// * @brief      Provides pin set/clear functionality for ADC timing debugging
// *             purposes.
// *
// * @param      s     0: Set debug port to state "off"
// *                  >0: Set debug port to state "on"
// */
//#define   DBG_ADC_TIMING(s)       ((s) > 0) ? (MDB_GPIO_On(DBG_TIMING_PE4)) : \
//                                              (MDB_GPIO_Off(DBG_TIMING_PE4))

/** @} */

/**
 * @addtogroup GPIO_Typedefs
 * @{
 */

/**
 * @brief      GPIO port pin naming enumeration.
 */
typedef enum {
  LED_GREEN_A       = 0,    //!< LED3 on STM32F429-Discovery.
  LED_RED_A         = 1,    //!< LED4 on STM32F429-Discovery.
} MD_GPIO_NAME_t;

/**
 * @brief      GPIO states enumeration.
 */
typedef enum {
  GPIO_STATE_OFF = 0,
  GPIO_STATE_ON
} MD_GPIO_STATE_t;

/**
 * @brief      GPIO Port enumeration.
 */
typedef enum {
	GPIOPA = 0,		//!< GPIO  0...31
	GPIOPB,			//!< GPIO 32...63
	GPIOPC,			//!< GPIO 64...95
	GPIOPD,			//!< GPIO 96...127
} MD_GPIO_PORT_t;

///**
// * @brief      GPIO output driver state enumeration.
// *
// * @attention  This type is safety related. To prevent unwanted
// *             beam-enabled-states in case of misspelled argument invocations on
// *             MDB_GPIO_BeamPin.
// */
//typedef enum {
//  GPIO_DRIVER_OFF = 0,
//  GPIO_DRIVER_ON
//} MD_GPIO_DRIVER_STATE_t;

/**
 * @brief      User level "per-GPIO" typedf to realize a "per GPIO"
 *             configuration array MDB_GPIO.
 */
typedef struct {
    MD_GPIO_NAME_t          _GPIO_NAME;       //!< GPIO name
    MD_GPIO_PORT_t          _GPIO_PORT;       //!< GPIO port
    const uint16_t          _GPIO_PIN;        //!< GPIO pin number
//    const uint32_t          _GPIO_AHB_CLK;    //!< GPIO AHB peripheral clock source
    MD_GPIO_STATE_t		    _GPIO_INIT;       //!< Initial GPIO state
} MD_GPIO_t;

/** @} */

/**
 * @addtogroup GPIO_Variables
 * @{
 */

/** @} */

/**
 * @addtogroup GPIO_Functions
 * @{
 */

/**
 * @brief      Provides GPIO pin toggle functionality for MD_GPIO_NAME_t types
 *             defined in MDB_GPIO.
 *
 * @param[in]  name   Pass the GPIO name of type MD_GPIO_NAME_t.
 *
 * @note       Use in consumption with MD_GPIO.
 */
void MD_GPIO_Toggle(MD_GPIO_NAME_t name);

/**
 * @brief      Provides GPIO on/off functionality.
 *
 * @param[in]  name       Pass the GPIO name of type MD_GPIO_NAME_t.
 * @param[in]  newState   Pass the new state for GPIO name.
 */
void MD_GPIO_Switch(MD_GPIO_NAME_t name, MD_GPIO_STATE_t newState);

/**
 * @brief      { function_description }
 *
 * @param[in]  name   The name
 */
void MD_GPIO_On(MD_GPIO_NAME_t name);

//#define MDB_GPIO_On_m(name)     MDB_GPIO[name]._GPIO_PORT->BSRRL = MDB_GPIO[name]._GPIO_PIN
//#define MDB_GPIO_Toggle_m(name) MDB_GPIO[name]._GPIO_PORT->ODR ^= MDB_GPIO[name]._GPIO_PIN

//#define   MDB_GPIO_Toggle_m2(s)       ((s) > 0) ? (MDB_GPIO_On(DBG_TIMING_PE6))   : (MDB_GPIO_Off(DBG_TIMING_PE6))

#define   MDB_GPIO_Toggle_m2(s)       MDB_GPIO_Toggle(s)


void MD_GPIO_Off(MD_GPIO_NAME_t name);
void MD_GPIO_Init(void);
//void MD_GPIO_BeamPin(MD_GPIO_NAME_t name, MD_GPIO_DRIVER_STATE_t newState);


/** @} */

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* MD_GPIO_H_ */
