/**
 * @file        main.h
 * @project		ELT131_LAB_03_NON-OS
 *
 * @date        24 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
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
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Macros
 */
#define LED1        GPIO9
#define LED2        GPIO11

#define ledInit()     EALLOW; \
    GpioCtrlRegs.GPADIR.bit.LED1 = 1; \
    GpioCtrlRegs.GPADIR.bit.LED2 = 1; \
    ledLvl(LED1, 0); \
    ledLvl(LED2, 0); \
    EDIS

#define ledLvl(LEDx, LVL)   GpioDataRegs.GPADAT.bit.LEDx = LVL
#define ledTgl(LEDx)        GpioDataRegs.GPATOGGLE.bit.LEDx = 1




#endif /* MAIN_H_ */
