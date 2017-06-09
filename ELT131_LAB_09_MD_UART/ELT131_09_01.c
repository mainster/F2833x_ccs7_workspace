/**
 * @file        ELT131_09_01.c
 * @project		ELT131_LAB_09
 *
 * @date        7 Jun 2017
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

#include <ti/sysbios/knl/Swi.h>
//#include <ti/sysbios/hal/Hwi.h>

#include "DSP28x_Project.h"
#include "md_uart_sci.h"

extern uint16_t RamfuncsLoadSize;

void HWI0_SCI_Rx_fxn(void);
void SWI0_SRVFB_nRTS_fxn(void);
void TskLed_refresh_fxn(void);

//void clock_500ms_fxn(void);

/**
 * Board specific pin definitions
 */
#define LED1        GPIO9
#define LED2        GPIO11

#define SET_LED1(x)	GpioDataRegs.GPADAT.bit.GPIO9 = x;
#define SET_LED2(x)	GpioDataRegs.GPADAT.bit.GPIO11 = x;

#define GPIO_INIT	EALLOW; \
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; \
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x00; \
	EDIS;

#define SCI_BAUDRATE	230400U
#define SYSCLKOUT		150000000U

volatile uint32_t ctr;

/*
 * ===================== main of ELT131_LAB_09 =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();                                    //!< Initialize the flash
	GPIO_INIT;
	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
	EDIS;
	SET_LED1(0);
	SET_LED2(0);

	//!< Initialize SCIA and enable CPU INT9 (SCIA-TX)
	MD_SCIA_Init(150, SCI_BAUDRATE);
	uartPuts("ELT131_LAB_09_01");

	//!< CPU INTx configuration should be handled via BIOS
//	IER |= M_INT9;
//	EINT; ERTM;

//	while (1) {
//		uartPuts("Good morning, Sir!");
//		DELAY_US(100000);
//	}
	BIOS_start();
}

/**
 * @brief   SCIA data received HWI callback.
 */
void HWI0_SCI_Rx_fxn(void) {
	//	uint16_t rxLedPattern = uartGeti();
	char rxLedPatterns[32], *pRx = &rxLedPatterns[0];
	pRx = &uartGets();

	//	SET_LED1(rxLedPattern & 0x01);
	if (! Mailbox_post(LED_mbx, pRx, BIOS_NO_WAIT) )
		Swi_post(SWI0_SRVFB_nRTS);
}

/**
 * @brief	Server feedback: Not ready to send.
 */
void SWI0_SRVFB_nRTS_fxn(void) {
	SET_LED2(1);


//	uartPuts("MAILBOX \"LED_mbx\" full --> nRTS!");
}

/**
 * @brief	Refresh LED byte value.
 */
void TskLed_refresh_fxn(void) {
	uint16_t rx_data;

	while (1) {
		Mailbox_pend(LED_mbx, &rx_data, BIOS_WAIT_FOREVER);
		SET_LED1(rx_data & 0x01);

//		SET_LED1(((uint32_t) rx_data) & 0x01);

//		uartPutsi("Set LED: ", (uint32_t) rx_data, "\n");
		//!< Invoke I2C_write8(&I2CA_BASE_ADDRESS, rx_data);
	}
}

////!< Priority: 1
//void Producer_fxn(void) {
//	uint16_t LED_data;
//
//	while (1) {
//		Semaphore_pend(time_500ms, BIOS_WAIT_FOREVER);
//		Mailbox_post(LED_mbx, &LED_data, BIOS_WAIT_FOREVER);
//		LED_data++;
//	}
//}


