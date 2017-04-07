/**
 * @file        ePWM1.c
 * @project		ELT131_LAB_03
 *
 * @date        24.03.2017
 * @author      Manuel Del Basso (mad16h09)
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
//ti.platforms.tms320x28:TMS320F28335
#include "DSP28x_Project.h"

#define F_EPWM1     20e3
#define F_CPU       150e6

void ePwm1Init(void) {
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x01;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
    EPwm1Regs.TBCTL.bit.CTRMODE = 0x02;
    EPwm1Regs.AQCTLA.bit.ZRO = 0x02;        //!< Clear: force EPWMxA output low.
    EPwm1Regs.AQCTLA.bit.PRD = 0x01;        //!< Set: force EPWMxA output high.
    EPwm1Regs.TBPRD = (Uint16) ((float) 1/F_EPWM1)/((float)1/F_CPU);
    EDIS;
}
