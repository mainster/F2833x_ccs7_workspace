/**
 * @file        ELT131_LAB_03_RTOS_3.c
 * @project		ELT131_LAB_03_RTOS_3
 *
 * @date        29 Mar 2017
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

/**
 * Idle message buffer
 */
char idleMsgBuff[32];

/**
 * Message buffer pointer
 */
char *pIdleMsg = NULL;

/**
 * String constants
 */
const char sEncMsg[] = { "New ENC value: " };
const char sEpwmFreq[] = { "ePWM frequency: " };

/*
 * ===================== main of ELT131_LAB_06_b33 =====================
 */
void main() {
    InitSysCtrl();							//!< Initialize the F28335-CPU
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
           (unsigned long)(&RamfuncsLoadSize));
    InitFlash();

    gpioInit();
    gpioXintInit();
    MD_UART_SciaInit(150, 115200);

    MD_BSP_LedInit();
    MD_BSP_EncInit();

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 50000);		//!< Heart beat 50ms

    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;			//!< Read in initial encoder value

    MD_EPWM1_Init(2e3, 150e6);					//!< Init/Start signal ePWM1A
//    CpuTimer0Regs.TCR.bit.TSS = 0;  			//!< Timer Start/Stop
    errorLed_off();
    BIOS_start();        						//!< ENI and start SYS/BIOS idle
}

/**
 * @brief      XINT1 GPIO1 IRQ handler
 */
void XINT1_GPIO1_isr(void) {
	Swi_post(SWI_XINT1);
	GpioDataRegs.GPASET.bit.GPIO2 = 1; 		//!< Rising edge at GPIO2
	for(int i=0;i<10;i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 	//!< falling edge at GPIO2

#ifndef RTOS_DISPATCHER
	PieCtrlRegs.PIEACK.bit.ACK4 = 1;		//!< Ack interrupt service
#endif
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

/**
 * @brief      SCIA TxFifo IRQ hook function
 */
void onSpiTxFifo_irq(void) {
#ifdef USE_TX_BUFFER
    SpiaRegs.SPITXBUF = *pTx++;
    if (pTx == &txBuff[8])
        pTx = &txBuff[0];
#endif
}

/**
 * @brief      SCIA RxFifo IRQ hook function
 */
void onSpiRxFifo_irq(void) {
}

/**
 * @brief      Idle task.
 *
 *             Idle task to query encoder GPIOs and reconfigure ePWM1 frequency,
 *             flagging an error LED if encoder value >sizeof(epwmFreqs)/
 *             sizeof(uint32_t) and to initiate Tx-IRQ driven SCIA transmission. 
 */
void tsk_idle_UartEnc(void) {
	uint32_t _encVal = MD_BSP_EncValue();
	if ((encVal != _encVal) &&
			(_encVal < sizeof(epwmFreqs)/sizeof(uint32_t))) {
		encVal = _encVal;
		MD_EPWM1_freqConfig(epwmFreqs[encVal], 150e6);

		pIdleMsg = strcpy(&idleMsgBuff[0], "epwm freq: ");
		strcat(pIdleMsg, int2str(epwmFreqs[encVal]));
	}

	errorLed( (encVal < sizeof(epwmFreqs)/sizeof(uint32_t) ? 0 : 1) );

	if (pIdleMsg != NULL) {
		uartPuts(pIdleMsg);
		pIdleMsg = NULL;
	}
}

void onEncLsb_changed(void) {
	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}

/**
 * @brief      Timer0 IRQ callback handler
 */
void CPU_TIM0_isr(void) {
    int i = 0;
    do asm("  NOP");
    while (--i);

    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#ifndef RTOS_DISPATCHER
    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#endif
}

/**
 * @brief      Debounce function
 */
void debounced(UArg arg) {
	errorLed_on();
}

#if defined(RTOS) && defined(RTOS_DYNAMIC_TASK)
void uartPutsByTask(char *str) {
	Task_Params taskParams;
	Task_Handle tskUartBusy;
	Error_Block eb;
	Error_init(&eb);

	/**
	 * Create a task with priority 10.
	 */
	Task_Params_init(&taskParams);
	taskParams.stackSize = 512;
	taskParams.priority = 10;

	tskUartBusy = Task_create((Task_FuncPtr)tskUartPuts(str), &taskParams, &eb);
	if (tskUartBusy == NULL)
		System_abort("Task create failed");
}

retVal_t tskUartPuts(const char *str) {
	/* DEADLOCK */
	while (_uartPuts(str, "\n") != ret_ok);;
}

#endif
