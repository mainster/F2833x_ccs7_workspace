/**
 * @file        ELT131_LAB_03_RTOS_5.c
 * @project		ELT131_LAB_03_RTOS_5
 *
 * @date        5 Apr 2017
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
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <stdint.h>
#include <DSP28x_Project.h>
#include <ELT131_LAB_03_RTOS_5.h>
#include "DSP2833x_GlobalPrototypes.h"
#include "md_bsp_explorer.h"

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;

/**
 * ePWM frequency values
 */
uint32_t  epwmFreqs[] = { 100, 200, 2e3, 20e3, 200e3 };

/**
 * Global value of last encoder query.
 */
uint16_t  encVal = 0;


/*
 * ===================== main of ELT313_LAB_03_RTOS_v2 =====================
 * Predefined symbols:
 * 	- RTOS_DISPATCHER			//!< Removes all user access instructions to the PIE
 * 	- RTOS
 */
void main() {
    InitSysCtrl();				//!< Initialize the F28335-CPU
#ifdef FLASH
	//!< Initialize the FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();
#endif
    /**
 	 * SYS/BIOS expects to manage all the interrupt related hardware initializations.
 	 * User code should not manipulate the PIE registers, nor initialize the PIE vector
  	 * table. Furthermore, the user should not manually manipulate the IER and IFR
	 * registers. Interrupts are globally enabled within the BIOS_start() function
	 * called at the end of main(). Enabling global interrupts prior to that can
	 * lead to fatal application behavior
	 *
	 * InitPieCtrl();          //!< Initialize PIE control registers
	 * InitPieVectTable();     //!< Initialize PIE vector table
	 * EINT;                   //!< Enable Global Interrupts
	 */
    gpioInit();
    xInt1Init();
    gpioXintInit();

    MD_BSP_LedInit();
    MD_BSP_EncInit();

    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;			//!< Read in initial encoder value

    MD_EPWM1_Init(epwmFreqs[encVal], 150e6);	//!< Init/Start signal ePWM1A
    errorLed_off();
    BIOS_start();        						//!< ENI and start SYS/BIOS idle
}

/**
 * http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/
 * sysbios/6_33_04_39/exports/bios_6_33_04_39/docs/cdoc/index.html
 *
 * PIE interrupts
 * The peripheral interrupt expansion (PIE) block multiplexes 96 interrupts into
 * 12 CPU interrupts. The PIE vector table includes entries for each of these 96
 * interrupts. The relationship between Group number (x), IRQ number (y) and the
 * unique "interrupt number" (IRQ_ID), needed for TI-RTOS Hwi module is given as:
 *
 * IRQ_ID(x,y) = 8*(x-1) + 31 + y
 *
 * E.g. EPWM6_INT has INT(3,6):
 * IRQ_ID(EPWM6_INT) = 16 + 31 + 6 = 53
 *
 * ELT131_Praktikum.pdf
 * First assume that INT numbers 0-31 are “reserved” or “taken”. Now look at the
 * table on page 124 and start counting (from the right) at 32 with INT1.1.
 * INT1.2 would be 33…and so on…making INT1.4 = 35.
 */

/**
 * @brief      XINT1 GPIO1 IRQ handler
 */
void XINT1_GPIO1_isr(void) {
	GpioDataRegs.GPASET.bit.GPIO2 = 1; 		//!< Rising edge at GPIO2
	for(int i=0;i<10;i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 	//!< falling edge at GPIO2

#if !defined(RTOS_DISPATCHER) || defined(ZERO_LATENCY_XINT1)
    PieCtrlRegs.PIEACK.bit.ACK4 = 1;		//!< Ack interrupt service
#endif
}

/**
 * @brief      External interrupt 1 initialization
 */
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
    PieVectTable.XINT1 = &XINT1_GPIO1_isr;            //!< Register callback handler
    EDIS;
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT1
}

