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
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Clock.h>

#include <DSP28x_Project.h>
#include "md_epwm.h"
#include "md_bsp_explorer.h"
#include "md_uart_sci.h"

extern const Swi_Handle SWI_XINT1;
extern const Clock_Handle CLK_SINGLE;
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
        GpioCtrlRegs.GPADIR.bit.GPIO1 = 0; \
        GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1; \
        GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1; \
        XIntruptRegs.XINT1CR.bit.ENABLE = 1; \
        XIntruptRegs.XINT1CR.bit.POLARITY = 0; \
		EDIS;

volatile uint32_t theValue = 0;
volatile int16_t theDelta = 1;
/**
 * ePWM frequency values
 */
uint32_t  epwmFreqs[] = { 100, 200, 2e3, 20e3, 200e3 };

uint16_t swiDelay = 10;
volatile short multiplier = 20;


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
    MD_BSP_BtnInit(PB1X_BIT);

    /* Read in initial encoder value */
    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;

    MD_EPWM1_Init(2e3, 150e6);			//!< Init/Start signal ePWM1A
    errorLed_off();
    BIOS_start();        						//!< ENI and start SYS/BIOS idle
}


/**
 * @brief      XINT1 GPIO1 IRQ handler
 */
void XINT1_GPIO1_isr(void) {
//	for(int i=0; i<theValue; i++)
//		asm(" NOP");
	Swi_post(SWI_XINT1);
//	theValue++;
}

/**
 * @brief      SWI XINT1 IRQ handler
 */
void SWI_XINT1_fxn(void) {
	GpioDataRegs.GPASET.bit.GPIO2 = 1; 		//!< Rising edge at GPIO2
	for(int i=0; i<swiDelay + encVal * multiplier; i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 	//!< falling edge at GPIO2
}

void debounced(UArg arg) {
	errorLed_on();

}

/**
 * @brief		Hook function for encoder lsb XINT2 irq.
 */
void onEncLsb_changed(UArg arg) {
//	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
	_ledLvlA(LED1, encVal & 0x01);

	if (! arg)
		Clock_start(CLK_SINGLE);
	else {
		encVal = MD_BSP_EncValue();
		Log_info1("SWI_DELAY: %d\n", swiDelay + encVal * multiplier);
		errorLed_on();
	    uartPuts("Debounced\n");
	}
}

/**
 * @brief		Hook function for PB1 XINT3 irq.
 */
void onBtn_pressed(void) {
	if ((encVal & 0x01) && (swiDelay < /*SWI_DELAY_MAX*/1e3))
		swiDelay += 1;

	if (! (encVal & 0x01) && (swiDelay > 0))
		swiDelay -= 1;

	Log_info1("SWI_DELAY: %d\n", swiDelay);
}
