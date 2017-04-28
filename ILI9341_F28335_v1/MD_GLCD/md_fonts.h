/**
 * @file        md_fonts.h
 * @project		ILI9341_F28335_v1
 *
 * @date        26 Apr 2017
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
#ifndef MD_FONTS_H
#define MD_FONTS_H 100

/* C++ detection */
#ifdef __cplusplus
 extern C {
#endif

#include <stdint.h>

//#include "stm32f4xx.h"
//#include "defines.h"
//#include "string.h"

/**
 * @defgroup MD_LIB_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
	uint16_t FontWidth:8;    /*!< Font width in pixels */
	uint16_t FontHeight:8;   /*!< Font height in pixels */

	//	union {
//		uint16_t all;
//		uint16_t FontWidth:8;
//		uint16_t FontHeight:8;
//	} FontSize;

	const uint16_t *data; /*!< Pointer to data font data array */
} MD_FontDef_t;

/** 
 * @brief  String length and height 
 */
typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} MD_FONTS_SIZE_t;

/**
 * @}
 */

/**
 * @defgroup MD_FONTS_FontVariables
 * @brief    Library font variables
 * @{
 */

/**
 * @brief  7 x 10 pixels font size structure 
 */
extern MD_FontDef_t MD_Font_7x10;

/**
 * @brief  11 x 18 pixels font size structure 
 */
extern MD_FontDef_t MD_Font_11x18;

/**
 * @brief  16 x 26 pixels font size structure 
 */
extern MD_FontDef_t MD_Font_16x26;

/**
 * @}
 */
 
/**
 * @defgroup MD_FONTS_Functions
 * @brief    Library functions
 * @{
 */

/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref MD_FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref MD_FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* MD_FONTS_GetStringSize(char* str, MD_FONTS_SIZE_t* SizeStruct, MD_FontDef_t* Font);

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

