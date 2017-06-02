/**
 * @file        ELT131_05_01.c
 * @project		ELT131_LAB_05_01
 *
 * @date        6 Apr 2017
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
#include <ELT131_05.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <stdint.h>
#include "DSP28x_Project.h"
#include "md_bsp_explorer.h"

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;

void GPIOToggle_fxn(void);

#define F_CPU	150e6L

/*
 * ===================== main of ELT131_LAB_05_01 =====================
 */
void main(void) {
    InitSysCtrl();				//!< Initialize the F28335-CPU
#ifdef FLASH
	//!< Initialize the FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();
#endif
    gpioInit();
//    xInt1Init();
//    gpioXintInit();

    MD_BSP_LedInit();
//    MD_BSP_EncInit();

    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    System_printf("enter main()\n");
	
    BIOS_start();        /* enable interrupts and start SYS/BIOS */
}

void GPIOToggle_fxn(void) {
	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
}

//void XINT2_EncLsb_isr(void) {
//	PieCtrlRegs.PIEACK.bit.ACK5 = 1;            //!< Ack interrupt service
//
//	//!< Read-in actual encoder value
//	encVal = MD_BSP_EncValue();
//
//}
