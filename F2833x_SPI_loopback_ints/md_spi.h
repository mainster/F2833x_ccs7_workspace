/**
 * @file        md_spi.h
 * @project		F2833x_SPI_loopback_ints
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
#ifndef MD_SPI_H_
#define MD_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* -------------------------------  Includes  ------------------------------ */

/* ----------------------------  Configuration  ---------------------------- */

/* ---------------------------  Private typedefs  -------------------------- */

/* ----------------------------  Private macros  --------------------------- */

/* --------------------------  Private functions  -------------------------- */

/* --------------------------  Private variables  -------------------------- */

/* ----------------------------  Private defines  -------------------------- */

/* ---------------------  Public function prototypes  --------------------- */
 void MD_SPI_Init(const uint16_t BAUDVAL);



#ifdef __cplusplus
 }
#endif

#endif
