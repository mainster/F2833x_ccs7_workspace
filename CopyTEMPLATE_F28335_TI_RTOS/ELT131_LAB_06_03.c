/**
 * @file        ELT131_LAB_06_03.c
 * @project		CopyTEMPLATE_F28335_TI_RTOS
 *
 * @date        27 May 2017
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
#include <xdc/cfg/global.h> 		//!< header for statically defined objects/handles
#include <xdc/runtime/Log.h> 		//!< for Log_info() calls when UIA is added
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

void GPIO02_Pulse_fxn(void);
void XINT1_GPIO1_isr(void);

/*
 * ===================== main of CopyTEMPLATE_F28335_TI_RTOS =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    gpioInit();
    gpioXintInit();
    MD_EPWM1_Init(2e3, 150e6);    //!< Init/Start signal ePWM1A
	BIOS_start();
}

/**
 * @brief      GPIO2 Task
 */
void GPIO02_Pulse_fxn(void) {
	static int pulseCtr = 0;

	while(1) {
		Semaphore_pend(sem_GPIO01,BIOS_WAIT_FOREVER);
	    GpioDataRegs.GPASET.bit.GPIO2 = 1;      //!< Rising edge at GPIO2
	    for(int i=0;i<10;i++)
	        asm(" NOP");
	    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;    //!< falling edge at GPIO2
	    Log_info1("GPIO02 Pulse [%u] times", ++pulseCtr);
	}
}

/**
 * @brief      XINT1 GPIO1 toggle semaphore post
 */
void XINT1_GPIO1_isr(void) {
	Semaphore_post(sem_GPIO01);
}

