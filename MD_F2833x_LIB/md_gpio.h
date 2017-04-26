/**
 * @file        md_gpio.h
 * @project     MD_F2833x_LIB
 *
 * @date        25 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license     GNU GPL v3
 * 
 * @brief       Provides GPIO initialization and configuration functions.
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
 *
 * @note	In order to configure this GPIO library, one must enumerate GPIO
 * 			names as MD_GPIOName_t in md_globals.h
 *
   @verbatim
    ------------------------------------------------------------------------
	typedef enum {
		LED_RED_A         = 0,    //!< LED
		DBG_TIMING_PE2    = 1,    //!< Not used!
	} MD_GPIOName_t;
    ------------------------------------------------------------------------
   @endverbatim
 *
 *
 * @note	In addition, it's necessary to include a header file named
 * 			md_config.h which contains a MD_GPIO_TypeDef_t array like
 *
   @verbatim
    ------------------------------------------------------------------------
	MD_GPIO_TypeDef_t MD_Gpios[] = {
		{ LED_RED_A, 		MD_GPIO34, GPIO_Mode_OUT, GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, GPIO_OFF },
		{ DBG_TIMING_PE2, 	MD_GPIO1,  GPIO_Mode_IN,  GPIO_OType_OD, MD_GPIO_PuPd_UP, GPIO_OFF },
	};

	extern MD_GPIO_TypeDef_t MD_GPIO_TypeDef;
    ------------------------------------------------------------------------
   @endverbatim
 *
 *
 * @note	Include md_gpio.h from main.c:
 *
   @verbatim
    ------------------------------------------------------------------------
	@file        main.c...
	(...)
	#include <stdio.h>
	#include <stdint.h>
	#include "md_globals.h"
	#include "md_gpio.h"
    ------------------------------------------------------------------------
   @endverbatim
 *
 */
#ifndef MD_GPIO_H
#define MD_GPIO_H

/* C++ detection */
#ifdef __cplusplus
 extern "C" {
#endif

#include <DSP28x_Project.h>
#include "md_globals.h"

/**
 * @brief      GPIO Pins enumeration.
 *             For HAL drivers compatibility
 */
typedef enum {
    MD_GPIO0 = 0, MD_GPIO1,  MD_GPIO2,  MD_GPIO3,  MD_GPIO4,  MD_GPIO5,  MD_GPIO6,  MD_GPIO7,
    MD_GPIO8,  MD_GPIO9,  MD_GPIO10, MD_GPIO11, MD_GPIO12, MD_GPIO13, MD_GPIO14, MD_GPIO15,
    MD_GPIO16, MD_GPIO17, MD_GPIO18, MD_GPIO19, MD_GPIO20, MD_GPIO21, MD_GPIO22, MD_GPIO23,
    MD_GPIO24, MD_GPIO25, MD_GPIO26, MD_GPIO27, MD_GPIO28, MD_GPIO29, MD_GPIO30, MD_GPIO31,
    MD_GPIO32, MD_GPIO33, MD_GPIO34, MD_GPIO35, MD_GPIO36, MD_GPIO37, MD_GPIO38, MD_GPIO39,
    MD_GPIO40, MD_GPIO41, MD_GPIO42, MD_GPIO43, MD_GPIO44, MD_GPIO45, MD_GPIO46, MD_GPIO47,
    MD_GPIO48, MD_GPIO49, MD_GPIO50, MD_GPIO51, MD_GPIO52, MD_GPIO53, MD_GPIO54, MD_GPIO55,
    MD_GPIO56, MD_GPIO57, MD_GPIO58, MD_GPIO59, MD_GPIO60, MD_GPIO61, MD_GPIO62, MD_GPIO63,
    MD_GPIO64, MD_GPIO65, MD_GPIO66, MD_GPIO67, MD_GPIO68, MD_GPIO69, MD_GPIO70, MD_GPIO71,
    MD_GPIO72, MD_GPIO73, MD_GPIO74, MD_GPIO75, MD_GPIO76, MD_GPIO77, MD_GPIO78, MD_GPIO79,
    MD_GPIO80, MD_GPIO81, MD_GPIO82, MD_GPIO83, MD_GPIO84, MD_GPIO85, MD_GPIO86, MD_GPIO87
} MD_GPIOPin_t;

/**
 * @brief      GPIO Configuration Mode enumeration
 */
typedef enum {
    GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
    GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
    GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
    GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
} MD_GPIOMode_t;

/**
 * @brief      GPIO Output type enumeration
 */
typedef enum {
    GPIO_OType_PP = 0x00,
    GPIO_OType_OD = 0x01
} MD_GPIOOType_t;

/**
 * @brief      GPIO pull resistors enumeration
 */
typedef enum {
    MD_GPIO_PuPd_NOPULL = 0x00, /*!< No pull resistor */
    MD_GPIO_PuPd_UP = 0x01,     /*!< Pull up resistor enabled */
    MD_GPIO_PuPd_DOWN = 0x02    /*!< Pull down resistor enabled */
} MD_GPIOPuPd_t;

/**
 * @brief      GPIO Output Maximum frequency enumeration
 */
typedef enum {
    GPIO_Low_Speed     = 0x00, /*!< Low speed    */
    GPIO_Medium_Speed  = 0x01, /*!< Medium speed */
    GPIO_Fast_Speed    = 0x02, /*!< Fast speed   */
    GPIO_High_Speed    = 0x03  /*!< High speed   */
} GPIOSpeed_t;

/**
 * @brief      GPIO states enumeration.
 */
typedef enum {
    GPIO_OFF = 0,
    GPIO_ON
} MD_GPIOState_t;

/**
 * @brief      User level "per-GPIO" typedf to realize a "per GPIO" configuration array MDB_GPIO.
 * @todo       Add configuration of clock the GPIO modules clock.
 */
typedef struct MD_GPIO_TypeDef {
    MD_GPIOName_t   _GPIO_Name;       //!< GPIO name
    MD_GPIOPin_t    _GPIO_Pin;        //!< GPIO pin address.
    MD_GPIOMode_t   _GPIO_Mode;       //!< GPIO mode.
    MD_GPIOOType_t  _GPIO_OType;      //!< GPIO output type.
    MD_GPIOPuPd_t   _GPIO_PuPd;       //!< GPIO pull resistor.
    MD_GPIOState_t  _GPIO_InitVal;       //!< Initial GPIO state
} MD_GPIO_TypeDef_t;

//extern MD_GPIO_TypeDef_t MD_Gpios[];

void MD_GPIO_InitAll(void);
void MD_GPIO_Init(MD_GPIOName_t Name, MD_GPIOPin_t Pin, 
                  MD_GPIOMode_t Mode, MD_GPIOOType_t DriverType, 
                  MD_GPIOPuPd_t PuPd, MD_GPIOState_t InitVal);
void MD_GPIO_Switch(MD_GPIOName_t name, MD_GPIOState_t newState);
void MD_GPIO_On(MD_GPIOName_t name);
void MD_GPIO_Off(MD_GPIOName_t name);
void MD_GPIO_Toggle(MD_GPIOName_t name);

/**
 * @brief      Initializes GPIO pins(s) as alternate function
 * @note       This function also enables clock for GPIO port
 *
 * @param      GPIOx        Pointer to GPIOx port you will use for initialization
 * @param      GPIO_Pin     GPIO pin(s) you will use for initialization
 * @param      GPIO_OType   Select GPIO Output type. This parameter can be a value of @ref
 *                          MD_GPIO_OType_t enumeration
 * @param      GPIO_PuPd    Select GPIO pull resistor. This parameter can be a value of @ref
 *                          MD_GPIO_PuPd_t enumeration
 * @param      GPIO_Speed   Select GPIO speed. This parameter can be a value of @ref MD_GPIO_Speed_t
 *                          enumeration
 * @param      Alternate    Alternate function you will use
 */
// void MD_GPIO_InitAlternate(GPIO_t* GPIOx, uint16_t GPIO_Pin, MD_GPIO_OType_t GPIO_OType, 
//                            MD_GPIO_PuPd_t GPIO_PuPd, MD_GPIO_Speed_t GPIO_Speed, uint8_t Alternate);

/**
 * @brief      Deinitializes pin(s)
 * @note       Pins(s) will be set as analog mode to get low power consumption
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin as input
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them as input
 */
// void MD_GPIO_DeInit(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Sets pin(s) as input
 * @note       Pins HAVE to be initialized first using @ref MD_GPIO_Init() or @ref
 *             MD_GPIO_InitAlternate() function
 * @note       This is just an option for fast input mode
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin as input
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them as input
 */
// void MD_GPIO_SetPinAsInput(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Sets pin(s) as output
 * @note       Pins HAVE to be initialized first using @ref MD_GPIO_Init() or @ref
 *             MD_GPIO_InitAlternate() function
 * @note       This is just an option for fast output mode
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin as output
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them as output
 */
// void MD_GPIO_SetPinAsOutput(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Sets pin(s) as analog
 * @note       Pins HAVE to be initialized first using @ref MD_GPIO_Init() or @ref
 *             MD_GPIO_InitAlternate() function
 * @note       This is just an option for fast analog mode
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin as analog
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them as analog
 */
// void MD_GPIO_SetPinAsAnalog(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Sets pin(s) as alternate function
 * @note       For proper alternate function, you should first init pin using @ref
 *             MD_GPIO_InitAlternate() function. This functions is only used for changing GPIO mode
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin as alternate
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them as alternate
 */
// void MD_GPIO_SetPinAsAlternate(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Sets pull resistor settings to GPIO pin(s)
 * @note       Pins HAVE to be initialized first using @ref MD_GPIO_Init() or @ref
 *             MD_GPIO_InitAlternate() function
 *
 * @param      GPIOx       GPIOx PORT where you want to select pull resistor
 * @param      GPIO_Pin    Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                         them as output
 * @param      GPIO_PuPd   Pull resistor option. This parameter can be a value of @ref
 *                         MD_GPIO_PuPd_t enumeration
 * @retval     None        { description }
 */
// void MD_GPIO_SetPullResistor(GPIO_t* GPIOx, uint16_t GPIO_Pin, MD_GPIO_PuPd_t GPIO_PuPd);

/**
 * @brief      Sets pin(s) low
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin low
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them low
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_SetPinLow(GPIOx, GPIO_Pin)          ((GPIOx)->BSRRH = (GPIO_Pin))

/**
 * @brief      Sets pin(s) high
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin high
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them high
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_SetPinHigh(GPIOx, GPIO_Pin)         ((GPIOx)->BSRRL = (GPIO_Pin))

/**
 * @brief      Sets pin(s) value
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to set pin value
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to set
 *                        them value
 * @param      val        If parameter is 0 then pin will be low, otherwise high
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_SetPinValue(GPIOx, GPIO_Pin, val)   \
    ((val) ? MD_GPIO_SetPinHigh(GPIOx, GPIO_Pin) : MD_GPIO_SetPinLow(GPIOx, GPIO_Pin))

/**
 * @brief      Toggles pin(s)
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to toggle pin value
 * @param      GPIO_Pin   Select GPIO pin(s). You can select more pins with | (OR) operator to
 *                        toggle them all at a time
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_TogglePinValue(GPIOx, GPIO_Pin)     ((GPIOx)->ODR ^= (GPIO_Pin))

/**
 * @brief      Sets value to entire GPIO PORT
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx   GPIOx PORT where you want to set value
 * @param      value   Value for GPIO OUTPUT data
 * @retval     None    { description }
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_SetPortValue(GPIOx, value)          ((GPIOx)->ODR = (value))

/**
 * @brief      Gets input data bit
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to read input bit value
 * @param      GPIO_Pin   GPIO pin where you want to read value
 * @retval     1          in case pin is high, or 0 if low
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_GetInputPinValue(GPIOx, GPIO_Pin)   (((GPIOx)->IDR & (GPIO_Pin)) == 0 ? 0 : 1)

/**
 * @brief      Gets output data bit
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx      GPIOx PORT where you want to read output bit value
 * @param      GPIO_Pin   GPIO pin where you want to read value
 * @retval     1          in case pin is high, or 0 if low
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_GetOutputPinValue(GPIOx, GPIO_Pin)  (((GPIOx)->ODR & (GPIO_Pin)) == 0 ? 0 : 1)

/**
 * @brief      Gets input value from entire GPIO PORT
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx    GPIOx PORT where you want to read input data value
 * @retval     Entire   PORT INPUT register
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_GetPortInputValue(GPIOx)            ((GPIOx)->IDR)

/**
 * @brief      Gets output value from entire GPIO PORT
 * @note       Defined as macro to get maximum speed using register access
 *
 * @param      GPIOx    GPIOx PORT where you want to read output data value
 * @retval     Entire   PORT OUTPUT register
 * @return     { description_of_the_return_value }
 */
// #define MD_GPIO_GetPortOutputValue(GPIOx)           ((GPIOx)->ODR)

/**
 * @brief      Gets port source from desired GPIOx PORT
 * @note       Meant for private use, unless you know what are you doing
 *
 * @param      GPIOx        GPIO PORT for calculating port source
 * @retval     Calculated   port source for GPIO
 * @return     { description_of_the_return_value }
 */
// uint16_t MD_GPIO_GetPortSource(GPIO_t* GPIOx);

/**
 * @brief      Gets pin source from desired GPIO pin
 * @note       Meant for private use, unless you know what are you doing
 *
 * @param      GPIO_Pin     GPIO pin for calculating port source
 * @retval     Calculated   pin source for GPIO pin
 * @return     { description_of_the_return_value }
 */
// uint16_t MD_GPIO_GetPinSource(uint16_t GPIO_Pin);

/**
 * @brief      Locks GPIOx register for future changes
 * @note       You are not able to config GPIO registers until new MCU reset occurs
 *
 * @param      GPIOx      GPIOx PORT where you want to lock config registers
 * @param      GPIO_Pin   GPIO pin(s) where you want to lock config registers
 */
// void MD_GPIO_Lock(GPIO_t* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief      Gets bit separated pins which were used at least once in library and were not
 *             deinitialized
 *
 * @param      GPIOx   Pointer to GPIOx peripheral where to check used GPIO pins
 * @retval     Bit     values for used pins
 * @return     { description_of_the_return_value }
 */
// uint16_t MD_GPIO_GetUsedPins(GPIO_t* GPIOx);

/**
 * @brief      Gets bit separated pins which were not used at in library or were deinitialized
 *
 * @param      GPIOx   Pointer to GPIOx peripheral where to check used GPIO pins
 * @retval     Bit     values for free pins
 * @return     { description_of_the_return_value }
 */
// uint16_t MD_GPIO_GetFreePins(GPIO_t* GPIOx);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
