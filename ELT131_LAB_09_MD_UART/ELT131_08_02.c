/**
 * @file        ELT131_08_02.c
 * @project		ELT131_LAB_08
 *
 * @date        28 May 2017
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

extern uint16_t RamfuncsLoadSize;

void clock_500ms_fxn(void);

#define GPIO_INIT	EALLOW; \
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; \
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x00; \
	EDIS;
rtos

/*
 * ===================== main of ELT131_LAB_08 =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();                                    //!< Initialize the flash
	GPIO_INIT;

	BIOS_start();
}

void clock_500ms_fxn(void) {
	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
	Semaphore_post(time_500ms);
}

//!< Priority: 3
void Consumer_fxn(void) {
	uint16_t rx_data;

	while (1) {
		Mailbox_pend(LED_mbx, &rx_data, BIOS_WAIT_FOREVER);
		//!< Invoke I2C_write8(&I2CA_BASE_ADDRESS, rx_data);
	}
}

//!< Priority: 1
void Producer_fxn(void) {
	uint16_t LED_data;

	while (1) {
		Semaphore_pend(time_500ms, BIOS_WAIT_FOREVER);
		Mailbox_post(LED_mbx, &LED_data, BIOS_WAIT_FOREVER);
		LED_data++;
	}
}


