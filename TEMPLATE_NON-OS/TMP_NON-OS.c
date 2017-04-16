/**
 * @file        ELT131_LAB_03_01.c
 * @project		ELT131_LAB_03_NON-OS
 *
 * @date        10 Apr 2017
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
#include <TMP_NON-OS.h>
#include "md_bsp_explorer.h"

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;

/**
 * Prototypes
 */
__interrupt void xint1_isr(void);
__interrupt void CPU_TIM0_isr(void);

void delaySamps(uint16_t samps);
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR);

/**
 * ePWM frequency values
 */
uint32_t  epwmFreqs[] = { 100, 200, 2e3, 20e3, 200e3 };
#define	LEN_epwmFreqs 	sizeof(epwmFreqs)/sizeof(uint32_t)


#define F_CPU   150e6L       //!< SYSCLKOUT = 60MHz
#undef FLASH

/*
 * ===================== main of ELT131_LAB_03_NON-OS =====================
 */
int main(void) {
    InitSysCtrl();			//!< (...) SYSCLKOUT = 150MHz (F_CPU)
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
           (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1);
    InitFlash();
#endif
    InitPieCtrl();          //!< Initialize PIE ctrl registers to POR state
    InitPieVectTable();     //!< Initialize PIE vector table
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 50000);		//!< Heart beat 50ms
    PieVectTable.TINT0 = &CPU_TIM0_isr;

    gpioInit();
    gpio1_ExtInt1Init();

    MD_BSP_LedInit();
    MD_BSP_EncInit();
    MD_BSP_BtnInit();

    errorLed_off();

    //!< Read in initial encoder value
    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;

    MD_EPWM1_Init(epwmFreqs[encVal], F_CPU );	//!< Configure ePWM module
    CpuTimer0Regs.TCR.bit.TSS = 0;  			//!< Timer Start/Stop
    EINT;                   					//!< Enable Global Interrupts

    while (1)
        asm("  NOP");
}

/**
 * @brief      Timer0 IRQ callback handler
 */
void CPU_TIM0_isr(void) {
    int i = 0;
    do asm("  NOP");
    while (--i);

#ifndef RTOS_DISPATCHER
    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#endif
}

void onEncLsb_changed(void) {
	MD_EPWM1_freqConfig(epwmFreqs[encVal], F_CPU);
}

void onBtn_pressed(void) {
	errorLed(1);
}

/**
 * @brief     External GPIO1 IRQ callback handler
 */
void XINT1_isr(void) {
    GpioDataRegs.GPASET.bit.GPIO2 = 1;          //!< Rising edge at GPIO2
    PieCtrlRegs.PIEACK.bit.ACK4 = 1;            //!< Ack interrupt service

    for(int i=0;i<10;i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;        //!< Falling edge at GPIO2
}

void gpio1_ExtInt1Init(void) {
	/**
	 * Enable XINT1 in the PIE: Group 1 interrupt 4
	 */
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          //!< Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          //!< Enable PIE Group 1 INT4
	IER |= M_INT1;                              //!< Enable CPU int1
	EALLOW;
	PieVectTable.XINT1 = &XINT1_isr;     		//!< Register callback handler

	/**
	 * Configure GPIO
	 */
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;			//!< Set to input
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;			//!< Enable pullup
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;      	//!< Mux as GPIO
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;     	//!< XINT1 Synch to SYSCLKOUT only
	/**
	 * QUALPRDx	[8-bit]
	 * The sampling period is specified by the qualification period (QUALPRDn) bits in
	 * the GPxCTRL register. The sampling period is configurable in groups of 8 input
	 * signals. For example,
	 * GPIO0 to GPIO7 use GPACTRL[QUALPRD0] setting and
	 * GPIO8 to GPIO15 use GPACTRL[QUALPRD1]
	 *
	 * SamplingPeriod = 2 × GPxCTRL[QUALPRDn] × TSYSCLKOUT
	 */
	GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0x00;	//!< Qual period = 1/F_CPU
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL =  1;  //!< XINT1 is GPIO1
	EDIS;

	/**
	 * Configure edges and enable XINT1
	 */
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT2
}


/**
 * @brief      InitSysCtrl = f(MD_DSP28_PLLCR)
 *
 * @param[in]  MD_DSP28_PLLCR   The md dsp 28 pllcr
 */
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR) {
   // Disable the watchdog
   DisableDog();

   // Initialize the PLL control: PLLCR and DIVSEL
   // DSP28_PLLCR and DSP28_DIVSEL are defined in DSP2833x_Examples.h
   InitPll(MD_DSP28_PLLCR, DSP28_DIVSEL);

   // Initialize the peripheral clocks
   InitPeripheralClocks();
}

