/**
 * @file        md_bsp_explorer.c
 * @project		MD_BSP_EXPLORER_F28335
 *
 * @date        28 Mar 2017
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

    Copyright (C) 2016  Manuel Del Basso

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
//#include <DSP2833x_Gpio.h>
#include <stdint.h>
#include "md_bsp_explorer.h"

volatile struct GPAENC_DATA_REGS EncDataReg;

uint16_t MD_BSP_EncValue(void) {
	EncDataReg.GPAENC.all = 0x0000000f & (GpioDataRegs.GPADAT.all >> 12);
	return (uint16_t) EncDataReg.GPAENC.all;
}

void MD_BSP_EncInit(void) {
	GpioCtrlRegs.GPADIR.all = ~0xf000;
	GpioCtrlRegs.GPAPUD.all = 0xf000;
}

