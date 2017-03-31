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
#include "ePWM1.h"

/**
 * @brief      Initialize ePWM module and GPIO
 *
 * @param      f_ePwm1  PWM frequency in Hz
 */
void MD_ePwm1Init(uint32_t f_ePwm1, uint32_t _F_CPU) {
/**
 *
 * Time-base (TB)  • Scale the time-base clock (TBCLK) relative to the system clock (SYSCLKOUT).
 *                 • Configure the PWM time-base counter (TBCTR) frequency or period.
 *                 • Set the mode for the time-base counter:
 *                     – count-up mode: used for asymmetric PWM
 *                     – count-down mode: used for asymmetric PWM
 *                     – count-up-and-down mode: used for symmetric PWM
 *                 • Configure the time-base phase relative to another ePWM module.
 *                 • Synchronize the time-base counter between modules through hardware or software.
 *                 • Configure the direction (up or down) of the time-base counter after a synchronization event.
 *                 • Configure how the time-base counter will behave when the device is halted by an emulator.
 *                 • Specify the source for the synchronization output of the ePWM module:
 *                     – Synchronization input signal
 *                     – Time-base counter equal to zero
 *                     – Time-base counter equal to counter-compare B (CMPB)
 *                     – No output synchronization signal generated.
 *
 * EALLOW-Protected ePWMx Registers: TZSEL TZCTL TZEINT TZCLR TZFRC HRCNFG
 */
//    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x01;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
    EDIS;

    /**
     * TBCLK    Time-base clock.
     * This is a prescaled version of the system clock (SYSCLKOUT) and is used by all submodules within the
     * ePWM. This clock determines the rate at which time-base counter increments or decrements.
     *
     * TBCLK = (SYSCLKOUT / (TBCTL[HSPCLKDIV] * TBCTL[CLKDIV]))
     *
     * eDIV = TBCTL[HSPCLKDIV] * TBCTL[CLKDIV]
     * f_epwm_min(eDIV,SYSCLKOUT) = (SYSCLKOUT/eDIV)/(2*0xffff)       // factor 2: Because 16-Bit Up-Down
     *
     * eDIV                               1            2            4            8           16
     * ------------------------------------------------------------------------------------------
     * f_epwm_min(eDIV, 150MHz)      1144.4       572.21       286.11       143.05       71.527      // Hz
     * ceil()                          1145          573          287          144           72      // Hz
     *
     */
    while (1) {
        EPwm1Regs.TBCTL.bit.CTRMODE = 0x02;             //!< Up-Down counter

        if (f_ePwm1 >= F_EPWM_MIN(1, _F_CPU)) {
            EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
            EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
            break;
        };

        if (f_ePwm1 >= F_EPWM_MIN(2, _F_CPU)) {
            EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
            EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;
            break;
        };

        if (f_ePwm1 >= F_EPWM_MIN(4, _F_CPU)) {
            EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
            EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;
            break;
        };

        if (f_ePwm1 >= F_EPWM_MIN(8, _F_CPU)) {
            EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
            EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;
            break;
        };

        if (f_ePwm1 >= F_EPWM_MIN(16, _F_CPU)) {
            EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;
            EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;
            break;
        };
    }

    /**
     * Calculate resulting overall SYSCLKOUT presc. (eDIV)
     */
    uint16_t eDIV =
            (1 << EPwm1Regs.TBCTL.bit.HSPCLKDIV) *
            (1 << EPwm1Regs.TBCTL.bit.CLKDIV);

    /**
     * Factor 1/2: Because 16-Bit Up-Down
     */
    EPwm1Regs.TBPRD = 0.5 * _F_CPU/(eDIV * f_ePwm1);

    EPwm1Regs.AQCTLA.bit.ZRO = 0x02;        //!< Clear: force EPWMxA output low.
    EPwm1Regs.AQCTLA.bit.PRD = 0x01;        //!< Set: force EPWMxA output high.
}
