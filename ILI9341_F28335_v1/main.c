/**
 * @file        main.c
 * @project		ILI9341_F28335_v1
 *
 * @date        26 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       ILI9341 TFT/LCD interface
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
//#include "DSP2833x_Gpio.h"
#include "md_globals.h"
#include "md_gpio.h"
#include "md_spi.h"
#include "md_ili934x.h"

uint16_t txBuff[8]; 			//!< Send data buffer
uint16_t *pTx = &txBuff[0];		//!< Tx data word pointer
uint16_t txWord = 0;
uint16_t ctr = 0;

extern MD_GPIO_TypeDef_t MD_Gpios[];
#define F_CPU 150e6

/*
 * ===================== main of MD_GPIO_LIB_F2833x =====================
 */
void main(void) {
    InitSysCtrl();			//!< (...) SYSCLKOUT = 150MHz (F_CPU)
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
           (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1);
    InitFlash();
#endif
//    InitGpio();
    InitPieCtrl();          //!< Initialize PIE ctrl registers to POR state
    InitPieVectTable();     //!< Initialize PIE vector table
    InitCpuTimers();

    MD_ConfigCpuTimer(&CpuTimer0, 150, 50000);
//    StartCpuTimer(0);
    EINT;

    MD_GPIO_InitAll(MD_Gpios, CONFIG_MATRIX_ROWS);
    MD_SPI_Init(0x0063, MD_SPI_8_BIT);
//
//    MD_SPI_Put(0xaa);
//    MD_SPI_Put(0xaa);


    //!< Initialize ILI9341
    MD_ILI934x_Init();

    //!< Rotate LCD for 90 degrees
    MD_ILI934x_Rotate(MD_ILI934x_Orientation_Landscape_2);
	//FIll lcd with color
	MD_ILI934x_Fill(ILI934x_COLOR_MAGENTA);
	//Draw white circle
	MD_ILI934x_DrawCircle(60, 60, 40, ILI934x_COLOR_GREEN);
	//Draw red filled circle
	MD_ILI934x_DrawFilledCircle(60, 60, 35, ILI934x_COLOR_RED);
	//Draw blue rectangle
	MD_ILI934x_DrawRectangle(120, 20, 220, 100, ILI934x_COLOR_BLUE);
	//Draw black filled rectangle
	MD_ILI934x_DrawFilledRectangle(130, 30, 210, 90, ILI934x_COLOR_BLACK);
	//Draw line with custom color 0x0005
	MD_ILI934x_DrawLine(10, 120, 310, 120, 0x0005);

	//Put string with black foreground color and blue background with 11x18px font
	MD_ILI934x_Puts(65, 130, "STM32F4 Discovery", &MD_Font_11x18, ILI934x_COLOR_BLACK, ILI934x_COLOR_BLUE2);
	//Put string with black foreground color and blue background with 11x18px font
	MD_ILI934x_Puts(60, 150, "ILI934x LCD Module", &MD_Font_11x18, ILI934x_COLOR_BLACK, ILI934x_COLOR_BLUE2);
	//Put string with black foreground color and red background with 11x18px font
	MD_ILI934x_Puts(245, 225, "majerle.eu", &MD_Font_7x10, ILI934x_COLOR_BLACK, ILI934x_COLOR_ORANGE);

	while(1);;
}

/* IRQ hook function for SPI TxFifo IRQ */
void onSpiTxFifo_irq(void) {
	return;

	SpiaRegs.SPITXBUF = *pTx++;    			//!< Send data
	if (pTx == &txBuff[8])
		pTx = &txBuff[0];
}

/* IRQ hook function for SPI RxFifo IRQ */
void onSpiRxFifo_irq(void) {
	asm("  NOP");
}

void MD_ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period) {
    ConfigCpuTimer(Timer, Freq, Period);		//!< Heart beat 50ms
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
    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#endif
}

/**
 * @brief	Encoder LSB isr hook function
 */
void onEncLsb_changed(void) {
//	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}

void onBtn_pressed(void) {
//	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}
