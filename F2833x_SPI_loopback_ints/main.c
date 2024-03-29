/**
 * @file        main.c
 * @project		F2833x_SPI_loopback_ints
 *
 * @date        27 Apr 2017
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
#include "DSP28x_Project.h"
#include "md_globals.h"
#include "md_gpio.h"
#include "md_spi.h"
#include "md_config.h"

void delay_loop(void);
//void MD_SPI_FifoInit(void);
void error();

uint16_t txBuff[8]; 			//!< Send data buffer
uint16_t *pTx = &txBuff[0];		//!< Tx data word pointer
uint16_t txWord = 0;

Uint16 rdata[8];     // Receive data buffer
Uint16 rdata_point;  // Keep track of where we are
uint16_t ctr = 0;


void main(void) {
  InitSysCtrl();

  DINT;
  IER = 0x0000;
  IFR = 0x0000;

  InitPieCtrl();
  InitPieVectTable();

  MD_GPIO_InitAll(MD_Gpios, CONFIG_MATRIX_ROWS);
  MD_SPI_Init(0x0063);

  *pTx = 0x0000;
  *(pTx + 1) = 0x0001;
  *(pTx + 2) = 0x0010;
  *(pTx + 3) = 0x0100;
  *(pTx + 4) = 0x1000;
  *(pTx + 5) = 0x00ff;
  *(pTx + 6) = 0xff00;
  *(pTx + 7) = 0xffff;

  EINT;

  for (;;);
}

// Some Useful local functions
void delay_loop() {
  long      i;
  for (i = 0; i < 1000000; i++) {}
}

void error(void) {
  __asm("     ESTOP0");  //Test failed!! Stop!
  for (;;);
}

/* IRQ hook function for SPI TxFifo IRQ */
void onSpiTxFifo_irq(void) {
	SpiaRegs.SPITXBUF = *pTx++;    			//!< Send data
	if (pTx == &txBuff[8])
		pTx = &txBuff[0];
}

/* IRQ hook function for SPI RxFifo IRQ */
void onSpiRxFifo_irq(void) {
	asm("  NOP");
}


void MD_ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period) {
  ConfigCpuTimer(Timer, Freq, Period);    //!< Heart beat 50ms
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
  PieCtrlRegs.PIEACK.bit.ACK7 = 1;    //!< Ack interrupt service
#endif
}


//===========================================================================
// No more.
//===========================================================================

/**
 * @brief Encoder LSB isr hook function
 */
void onEncLsb_changed(void) {
//  MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}

void onBtn_pressed(void) {
//  MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}
