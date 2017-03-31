/**
 * @file        main.c
 * @project		ELT131_LAB_03_RTOS
 *
 * @date        24 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
   @verbatim

    ------------------------------------------------------------------------

    Copyright (C) 2016  Manuel Del Basso

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

#include "main.h"
#include "DSP28x_Project.h"
#include "ePWM1.h"


/**
 * Prototypes
 */
interrupt void GPIO01_isr(void);
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR);
void xInt1Init(void);

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;
//extern void ePWM1_Init(void);

/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; \
        GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
        EDIS;

#define F_CPU   150e6L   //!< SYSCLKOUT = 60MHz

/*
 * ===================== main of ELT131_LAB_03_RTOS =====================
 */
void main(void) {
    /**
     * Initialize the F28335-CPU and the FLASH
     */
//    MD_InitSysCtrl(2*F_CPU/30e6);       //!< Initialize System Control, SYSCLKOUT = 30MHz * 4/2 = 60MHz
    InitSysCtrl();

#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();
#endif
    InitPieCtrl();
    InitPieVectTable();


    //    o Enable pull-up for GPIO1
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
//    o Set GPIO02 as digital output
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
    EDIS;

//    o Enable GPIO01 as XINT – interrupt source with falling edge
    /**
     * Configure GPIO
     */
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         //!< Mux as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;          //!< Set to input
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;   //!< XINT1 is GPIO
    EDIS;

    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT1

    //    o Start signal ePWM1A
//    MD_ePwm1Init(2e3, F_CPU);
    BIOS_start();

}

/**
 * @brief     External GPIO1 IRQ callback handler
 */
void GPIO01_isr(void) {
    GpioDataRegs.GPASET.bit.GPIO2 = 1;          //!< Rising edge at GPIO2
    /* Handled by RTOS?! */
//    PieCtrlRegs.PIEACK.all = 1;                 //!< Ack interrupt service
    for (int i = 0; i<10; i++)
        asm("  NOP");
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;        //!< Falling edge at GPIO2
}

void xInt1Init(void) {
    /**
     * Enable Xint1 in the PIE: Group 1 interrupt 4
     */
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          //!< Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          //!< Enable PIE Group 1 INT4
    IER |= M_INT1;                              //!< Enable CPU int1

    /**
     * Configure GPIO
     */
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         //!< Mux as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;          //!< Set to input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;        //!< Xint1 Synch to SYSCLKOUT only
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0x08;   //!< Qual period = SYSCLKOUT/x
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;   //!< XINT1 is GPIO0
    EDIS;

    /**
     * Configure and enable XINT1
     */
    EALLOW;
//    PieVectTable.XINT1 = &xint1_isr;            //!< Register callback handler
    EDIS;
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT1
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
