/**
 * @file        ELT131_07_02.c
 * @project		ELT131_LAB_07
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
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Log.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

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

void Task1_fxn(void);
void XINT1_GPIO1_isr(void);

/**
 * Mailbox message typedef
 */
typedef struct MsgObj_t {
	unsigned int val;		//!< Value member
} MsgObj_t, *Msg;			//!< Object and pointer


/*
 * ===================== main of ELT131_LAB_07 =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();                                    //!< Initialize the flash
    gpioInit();
//    gpioXintInit();
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;
	EDIS;

    MD_EPWM1_Init(500, 150e6);    //!< Init/Start signal ePWM1A
	BIOS_start();
}

/**
 * @brief      Task 1
 */
void Task1_fxn(void) {
	MsgObj_t msg;
	msg.val = 1;

	while(1) {
		Semaphore_pend(sem_GPIO01, BIOS_WAIT_FOREVER);

		//!< Wait until GPIO2_Mbx has free msg slot then post the buffer msg.
		Mailbox_post(GPIO2_Mbx, &msg, BIOS_WAIT_FOREVER);
		Log_info1("msg.val=%u", msg.val);
		msg.val ^= 1;
		Log_info1("msg.val=%u", msg.val);
	}
}

/**
 * @brief      Task 2
 */
void Task2_fxn(void) {
	MsgObj_t msg;

	while(1) {
		Mailbox_pend(GPIO2_Mbx, &msg, BIOS_WAIT_FOREVER);
		GpioDataRegs.GPADAT.bit.GPIO2 = (msg.val & 0x01);
	}
}

/**
 * @brief      XINT1 GPIO1 toggle semaphore post
 */
void XINT1_GPIO1_isr(void) {
	Semaphore_post(sem_GPIO01);
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
