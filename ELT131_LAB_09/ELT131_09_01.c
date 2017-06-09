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
#include <ti/drivers/UART.h>
#include "md_uart_sci.h"

extern uint16_t RamfuncsLoadSize;

void HWI0_SCI_Rx_fxn(void);
void SWI0_SRVFB_nRTS_fxn(void);
void TskLed_refresh_fxn(void);

//void clock_500ms_fxn(void);

#define GPIO_INIT	EALLOW; \
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; \
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0x00; \
	EDIS;

#define SCI_BAUDRATE	230400U
#define SYSCLKOUT		150000000U

volatile uint32_t ctr;

#define UART_BUFFER_SIZE	32
char uartBuffer[UART_BUFFER_SIZE],
*pUartBuffer = &uartBuffer[0];


/*
 * ===================== main of ELT131_LAB_09 =====================
 */
void main(void) {
    InitSysCtrl();
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();                                    //!< Initialize the flash
	GPIO_INIT;

	UART_Handle uart;
	UART_Params uartParams;
//	Board_initUART();          // Calls UART_init for you
	MD_UART_SciaInit(150, 230400);

	/* Create a UART with data processing off. */
	UART_Params_init(&uartParams);
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readEcho = UART_ECHO_OFF;
	uart = UART_open(0, &uartParams);

	pUartBuffer = "ELT131_LAB_09_01";
	UART_write(uart, pUartRxBuff, UART_BUFFER_SIZE);

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
	uint16_t rxLedPattern = (uint16_t) uartGetc();
	Mailbox_post(LED_mbx, &rxLedPattern, BIOS_WAIT_FOREVER);
}

/**
 * @brief	Server feedback: Not ready to send.
 */
void SWI0_SRVFB_nRTS_fxn(void) {
	uartPuts("MAILBOX \"LED_mbx\" full --> nRTS!");
}

/**
 * @brief	Refresh LED byte value.
 */
void TskLed_refresh_fxn(void) {
	uint16_t rx_data;

	while (1) {
		Mailbox_pend(LED_mbx, &rx_data, BIOS_WAIT_FOREVER);
		uartPutsi("Set LED: ", (uint32_t) rx_data, "\n");
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


