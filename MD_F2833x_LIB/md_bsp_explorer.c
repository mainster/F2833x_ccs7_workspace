/**
 * @file        md_bsp_explorer.c
 * @project		MD_BSP_EXPLORER_F28335
 *
 * @date        28 Mar 2017
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
//#include <DSP2833x_Gpio.h>
#include <stdint.h>
#include "md_bsp_explorer.h"

volatile struct GPAENC_DATA_REGS EncDataReg;

/**
 * Global value of last encoder query.
 */
volatile uint16_t  encVal = 0;

void XINT2_EncLsb_isr(void) {
	PieCtrlRegs.PIEACK.bit.ACK5 = 1;            //!< Ack interrupt service

	//!< Read-in actual encoder value
	encVal = MD_BSP_EncValue();

	onEncLsb_changed();
}

void MD_BSP_EncInit(void) {
	GpioCtrlRegs.GPADIR.all = ~0xf000;
	GpioCtrlRegs.GPAPUD.all = 0xf000;
	MD_BSP_XINT_EncInit();
}

uint16_t MD_BSP_EncValue(void) {
	EncDataReg.GPAENC.all = 0x0000000f & (GpioDataRegs.GPADAT.all >> 12);
	return (uint16_t) EncDataReg.GPAENC.all;
}


void MD_BSP_XINT_EncInit(void) {
	/**
	 * Enable XINT2 in the PIE: Group 1 interrupt 5
	 */
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          //!< Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          //!< Enable PIE Group 1 INT5
	IER |= M_INT2;                              //!< Enable CPU int2
	EALLOW;
	PieVectTable.XINT2 = &XINT2_EncLsb_isr;     //!< Register callback handler

	/**
	 * Configure GPIO
	 */
	GpioCtrlRegs.GPADIR.bit.ENC_Bit0 = 0;		//!< Set to input
	GpioCtrlRegs.GPAPUD.bit.ENC_Bit0 = 1;		//!< Enable pullup
	GpioCtrlRegs.GPAMUX1.bit.ENC_Bit0 = 0;      //!< Mux as GPIO
	GpioCtrlRegs.GPAQSEL1.bit.ENC_Bit0 = 0;     //!< Xint1 Synch to SYSCLKOUT only
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
	GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xff;	//!< Qual period = 2*0xff/F_CPU=3.4us
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 12;  //!< XINT2 is GPIO12
	EDIS;

	/**
	 * Configure edges and enable XINT2
	 */
	XIntruptRegs.XINT2CR.bit.POLARITY = 0x03;   //!< Edge interrupt
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;        //!< Enable XINT2
}



