/**
 * @file        ELT131_06_01.c
 * @project		ELT131_LAB_06_01
 *
 * @date        26 May 2017
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
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Log.h>

#include "DSP28x_Project.h"
#include "md_epwm.h"

/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; \
        EDIS;

/**
 * Init external interrupts
 *
 * - GPIO1 Set as external interrupt src
 */
#define gpioXintInit()    EALLOW; \
        GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1; \
        GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1; \
        XIntruptRegs.XINT1CR.bit.ENABLE = 1; \
        XIntruptRegs.XINT1CR.bit.POLARITY = 0; \
        EDIS;

extern uint16_t RamfuncsLoadSize;

void GPIOToggleClk_fxn(void);

#define GPIO_INIT	EALLOW; \
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; \
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x00; \
	EDIS;

/*
 * ===================== main of ELT131_LAB_06_01 =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();                                    //!< Initialize the flash

    gpioInit();
    gpioXintInit();
    MD_EPWM1_Init(2e3, 150e6);    //!< Init/Start signal ePWM1A
	BIOS_start();
}

/**
 * @brief      XINT1 GPIO1 IRQ handler
 */
void XINT1_GPIO1_isr(void) {
    GpioDataRegs.GPASET.bit.GPIO2 = 1;      //!< Rising edge at GPIO2
    for(int i=0;i<10;i++)
        asm(" NOP");
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;    //!< falling edge at GPIO2
}

//void GPIOToggleClk_fxn(void) {
//	static int tglCtr = 0;
//	static unsigned long t0, t1, t2, start, stop, delta;
//
//	t0 = Timestamp_get32();
//	t1 = Timestamp_get32();
//	t2 = t1 - t0;
//
//	start = Timestamp_get32();
//	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//	stop = Timestamp_get32();
//	delta = stop - start -t2;
//
//	Log_info1("GPIO toggled [%u] times", ++tglCtr);
//	Log_info1("GPIO BENCHMARK: [%u] cycles", delta);
//}
