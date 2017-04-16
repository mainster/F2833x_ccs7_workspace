/**
 * @file        ELT131_03_03.c
 * @project		TEMPLATE_RTOS_03_03
 *
 * @date        11 Apr 2017
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
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <DSP28x_Project.h>
#include "md_epwm.h"
#include "md_bsp_explorer.h"

#include <ti/sysbios/knl/Swi.h>
extern const Swi_Handle SWI_XINT1;

extern uint16_t RamfuncsLoadSize;

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

/**
 * ePWM frequency values
 */
uint32_t  epwmFreqs[] = { 100, 200, 2e3, 20e3, 200e3 };

/*
 * ===================== main of ELT131_LAB_04_01 =====================
 */
void main(void) {
    InitSysCtrl();									//!< Initialize the F28335-CPU
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
    		(unsigned long)(&RamfuncsLoadSize));
    InitFlash();									//!< Initialize the flash

    gpioInit();
    gpioXintInit();

    MD_BSP_LedInit();
    MD_BSP_EncInit();

    /* Read in initial encoder value */
    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;

    MD_EPWM1_Init(epwmFreqs[encVal], 150e6);	//!< Init/Start signal ePWM1A
    errorLed_off();
    BIOS_start();        						//!< ENI and start SYS/BIOS idle
}

/**
 * @brief      XINT1 GPIO1 IRQ handler
 */
void XINT1_GPIO1_isr(void) {
	Swi_post(SWI_XINT1);
}

/**
 * @brief      SWI XINT1 IRQ handler
 */
void SWI_XINT1_fxn(void) {
	GpioDataRegs.GPASET.bit.GPIO2 = 1; 		//!< Rising edge at GPIO2
	for(int i=0;i<10;i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 	//!< falling edge at GPIO2
}

/**
 * @brief		Hook function for encoder lsb xint2 irq.
 */
void onEncLsb_changed(void) {
	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}
