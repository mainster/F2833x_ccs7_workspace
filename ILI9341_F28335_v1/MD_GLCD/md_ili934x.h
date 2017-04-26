/**
 * @file        md_ili9341.h
 * @project		ILI934x_F28335_v1
 *
 * @date        25 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Provides interface functions for ILI934x based GLCDs.
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
   @verbatim

	ILI934x			TMS320F2833x			DESCRIPTION
	----------------------------------------------------------------------
	SDO (MOSI)		GPIO16, SPI_SIMO_A		SPI master out
	SDI (MISO)		GPIO17, SPI_SOMI_A		SPI master in (not used yet)
	SCK (SCLK)		GPIO18, SPI_CLK_A		SPI clock line
	nCS (nCS)		GPIO19, SPI_STE_A		SPI slave transmit enable
	WRx (D/C)		GPIO0					Data/Command register select

	RESET        	10k HW pullup 			Reset ILI934x
	LED        		680R HW pullup 			Backlight
	VCC          	+3.3V					Positive power supply
	GND          	GND          			Ground

   @endverbatim
 *
 */
#ifndef MD_ILI934x_H
#define MD_ILI934x_H 100

//#include "stm32f4xx.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"
#include "md_globals.h"
#include "md_fonts.h"
//#include "md_gpio.h"
//#include "tm_stm32f4_spi.h"
//#include "tm_stm32f4_dma.h"
//#include "tm_stm32f4_spi_dma.h"

/**
 * @defgroup MD_ILI934x_Macros
 * @brief    Library defines
 * @{
 */

///**
// * @brief  This SPI pins are used on STM32F429-Discovery board
// */
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

/* LCD settings */
#define ILI934x_WIDTH        240
#define ILI934x_HEIGHT       320
#define ILI934x_PIXEL        76800

/* Colors */
#define ILI934x_COLOR_WHITE			0xFFFF
#define ILI934x_COLOR_BLACK			0x0000
#define ILI934x_COLOR_RED       0xF800
#define ILI934x_COLOR_GREEN			0x07E0
#define ILI934x_COLOR_GREEN2		0xB723
#define ILI934x_COLOR_BLUE			0x001F
#define ILI934x_COLOR_BLUE2			0x051D
#define ILI934x_COLOR_YELLOW		0xFFE0
#define ILI934x_COLOR_ORANGE		0xFBE4
#define ILI934x_COLOR_CYAN			0x07FF
#define ILI934x_COLOR_MAGENTA		0xA254
#define ILI934x_COLOR_GRAY			0x7BEF
#define ILI934x_COLOR_BROWN			0xBBCA

/* Transparent background, only for strings and chars */
#define ILI934x_TRANSPARENT			0x80000000

/**
 * @}
 */
 
/**
 * @defgroup MD_ILI934x_Typedefs
 * @brief    Library Typedefs
 * @{
 */


/**
 * @brief  Possible orientations for LCD
 */
typedef enum {
	MD_ILI934x_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	MD_ILI934x_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	MD_ILI934x_Orientation_Landscape_1, /*!< Landscape orientation mode 1 */
	MD_ILI934x_Orientation_Landscape_2  /*!< Landscape orientation mode 2 */
} MD_ILI934x_Orientation_t;

/**
 * @}
 */

/**
 * @defgroup MD_ILI934x_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes ILI934x LCD with LTDC peripheral
 *         It also initializes external SDRAM
 * @param  None
 * @retval None
 */
void MD_ILI934x_Init(void);

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  color: Color of pixel
 * @retval None
 */
void MD_ILI934x_DrawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief  Fills entire LCD with color
 * @param  color: Color to be used in fill
 * @retval None
 */
void MD_ILI934x_Fill(uint32_t color);

/**
 * @brief  Rotates LCD to specific orientation
 * @param  orientation: LCD orientation. This parameter can be a value of @ref MD_ILI934x_Orientation_t enumeration
 * @retval None
 */
void MD_ILI934x_Rotate(MD_ILI934x_Orientation_t orientation);

/**
 * @brief  Puts single character to LCD
 * @param  x: X position of top left corner
 * @param  y: Y position of top left corner
 * @param  c: Character to be displayed
 * @param  *font: Pointer to @ref MD_FontDef_t used font
 * @param  foreground: Color for char
 * @param  background: Color for char background
 * @retval None
 */
void MD_ILI934x_Putc(uint16_t x, uint16_t y, char c, MD_FontDef_t* font, uint32_t foreground, uint32_t background);

/**
 * @brief  Puts string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref MD_FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @retval None
 */
void MD_ILI934x_Puts(uint16_t x, uint16_t y, char* str, MD_FontDef_t *font, uint32_t foreground, uint32_t background);

/**
 * @brief  Gets width and height of box with text
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref MD_FontDef_t used font
 * @param  *width: Pointer to variable to store width
 * @param  *height: Pointer to variable to store height
 * @retval None
 */
void MD_ILI934x_GetStringSize(char* str, MD_FontDef_t* font, uint16_t* width, uint16_t* height);

/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  color: Line color
 * @retval None
 */
void MD_ILI934x_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void MD_ILI934x_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws filled rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void MD_ILI934x_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void MD_ILI934x_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief  Draws filled circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void MD_ILI934x_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief   Enables display
 * @note    After initialization, LCD is enabled and you don't need to call this function
 * @param   None
 * @retval  None
 */
void MD_ILI934x_DisplayOn(void);

/**
 * @brief   Disables display
 * @param   None
 * @retval  None
 */
void MD_ILI934x_DisplayOff(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

