/**
 * @file        ELT131_LAB_03_RTOS_5.c
 * @project		ELT131_LAB_03_RTOS_5
 *
 * @date        10 Apr 2017
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

#include <ti/sysbios/family/c28/Hwi.h>

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
 * Idle message buffer + pointer
 */
char idleMsgBuff[32];
char *pIdleMsg = NULL;

/**
 * String constants
 */
const char sEncMsg[] = { "New ENC value: " };
const char sEpwmFreq[] = { "ePWM frequency: " };

/*
 * ===================== main of ELT131_LAB_03_RTOS_5 =====================
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
    gpioXintInit();

    MD_BSP_LedInit();
    MD_BSP_EncInit();

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 50000);		//!< Heart beat 50ms

    encVal = (sizeof(epwmFreqs)/sizeof(uint32_t) > MD_BSP_EncValue())
    		? MD_BSP_EncValue() : 2;			//!< Read in initial encoder value

    MD_EPWM1_Init(epwmFreqs[encVal], 150e6);	//!< Configure/Start signal ePWM1A

    /**
     * From http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/
     * sysbios/6_45_01_29/exports/bios_6_45_01_29/docs/cdoc/index.html
     *
     * DETAILS: plug hooks up the specified function as the branch target for a hardware interrupt
     * (fielded by the CPU) at the vector address corresponding to intNum. plug does not enable the
     * interrupt. Use Hwi_enableIER to enable specific interrupts. This API can plug the full set of
     * vectors supported by the PIE (0-127)
     */
    Hwi_plug(35,(Hwi_PlugFuncPtr) ZLI_XINT1_fxn);

    /**
     * Enable interrupts in a PIE group
     * DETAILS:
     * Atomically enable PIE interrupts in a single PIE group according to supplied PIEIER bitmask
     *
     * Bits16 Hwi_enablePIEIER(UInt groupNum, Bits16 pieMask);
     *
     * groupNum - PIE group number
     * pieMask - PIEIER enable mask for group
     *
     * RETURNS
     * Previous PIEIER settings bitmask
     *
	 * (Referenced from sprufb0b)
	 * Step a. Disable global interrupts (INTM = 1).
	 * Step b. Set the PIEIERx.y bits
	 * Step c. Wait 5 cycles (see explanation in code)
	 * Step d. Clear the CPU IFRx bits for the peripheral group
	 * Step e. Clear the PIEACKx bit for the peripheral group.
	 * Step f. Enable global interrupts (INTM = 0).
     */
    Hwi_enablePIEIER(1, 8);
    Hwi_enableInterrupt(1);

    errorLed_off();
    BIOS_start();        						//!< ENI and start SYS/BIOS idle
}

/**
 * @brief      Zero-latency interrupt callback handler for external IRQ.
 *
 * @note       From ELT131_Praktikum.pdf page 61: "Zero latency IER mask = 1 will allow all
 *             interrupts from Group INT1 to bypass SYSBIOS."
 */
interrupt void ZLI_XINT1_fxn(void) {
	GpioDataRegs.GPASET.bit.GPIO2 = 1; 			//!< Rising edge at GPIO2
	for(uint16_t i=0;i<10;i++)
		asm(" NOP");
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 		//!< Falling edge at GPIO2

	/**
	 * SYS/BIOS expects to manage all the interrupt related hardware initializations.
	 * User code should not manipulate the PIE registers, nor initialize the PIE vector
	 * table. Furthermore, the user should not manually manipulate the IER and IFR
	 * registers. Interrupts are globally enabled within the BIOS_start() function
	 * called at the end of main(). Enabling global interrupts prior to that can
	 * lead to fatal application behavior!
	 *
	 * In case of bypassing SYS/BIOS...
	 *     Hwi_enablePIEIER(1,8);
	 *     Hwi_enableInterrupt(1);
	 *
	 * ... the BIOS interrupt dispatcher doesn't take care about
	 * 		a) ... this specific IRQ?
	 * 		b) ... all IRQs generated from this GROUP?
	 */
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
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

#ifndef RTOS_DISPATCHER
    PieCtrlRegs.PIEACK.bit.ACK4 = 1;		//!< Ack interrupt service
#endif
}

/**
 * @brief		Hook function to implement private method that is invoked by
 * 				MD_BSP_EXPLORER IRQ handler XINT2_EncLsb_isr
 */
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

#ifndef RTOS_DISPATCHER
    PieCtrlRegs.PIEACK.bit.ACK7 = 1;		//!< Ack interrupt service
#endif
}

