/**
 * @file        main.c
 * @project		MD_GPIO_LIB_F2833x
 *
 * @date        26 Apr 2017
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
#include <stdio.h>
#include <stdint.h>
#include "md_globals.h"
#include "md_gpio.h"


#undef FLASH
uint16_t ctr = 0;

/*
 * ===================== main of MD_GPIO_LIB_F2833x =====================
 */
void main(void) {
    InitSysCtrl();			//!< (...) SYSCLKOUT = 150MHz (F_CPU)
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
           (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1);
    InitFlash();
#endif
    InitGpio();
    MD_GPIO_InitAll();

    InitPieCtrl();          //!< Initialize PIE ctrl registers to POR state
    InitPieVectTable();     //!< Initialize PIE vector table
    InitCpuTimers();
    MD_ConfigCpuTimer(&CpuTimer0, 150, 50000);
    StartCpuTimer(0);

    EINT;

    while(1);;
}

void MD_ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period) {
    ConfigCpuTimer(Timer, Freq, Period);		//!< Heart beat 50ms
    EALLOW;
    PieVectTable.TINT0 = &CPU_TIM0_isr;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT1;
    EDIS;
}

/**
 * @brief      Timer0 IRQ callback handler
 */
void CPU_TIM0_isr(void) {
	if (++ctr > 10) {
		ctr = 0;
		MD_GPIO_Toggle(LED_RED);
	}

#ifndef RTOS_DISPATCHER
    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#endif
}

/**
 * @brief	Encoder LSB isr hook function
 */
void onEncLsb_changed(void) {
//	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}

void onBtn_pressed(void) {
//	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}
