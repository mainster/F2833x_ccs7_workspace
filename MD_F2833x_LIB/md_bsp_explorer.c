/**
 * @file        md_bsp_explorer.c
 * @project		MD_F2833x_LIB
 *
 * @date        28 Mar 2017
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
#include <stdint.h>
#include "md_bsp_explorer.h"

volatile struct GPAENC_DATA_REGS EncDataReg;

/**
 * Global value of last encoder query.
 */
volatile uint16_t  encVal = 0;


void XINT2_EncLsb_isr(void) {
	encVal = MD_BSP_EncValue();		//!< Read-in actual encoder value
#ifdef RTOS
//	onEncLsb_changed((UArg) 0);		//!< Invoke hook function for XINT2 Enc
#else
	onEncLsb_changed();		//!< Invoke hook function for XINT2 Enc
#endif

#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_ENC_XINT))
//??	PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;    //!< Ack interrupt service
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;    //!< Ack interrupt service
#endif
}

void XINT3_Btn_isr(void) {
//	onBtn_pressed();					//!< Invoke hook function for XINT2 Btn

#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_BTN_XINT))
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;    //!< Ack interrupt service
#endif
}


void MD_BSP_EncInit(void) {
	EALLOW;
	GpioCtrlRegs.GPADIR.all &= ~ENC_MASK;
	GpioCtrlRegs.GPAPUD.all |= ENC_MASK;
	EDIS;
	MD_BSP_XINT_EncInit();
}

void MD_BSP_BtnInit(const short pinNo) {
	EALLOW;
	GpioCtrlRegs.GPADIR.bit.PB1 = 0;
	GpioCtrlRegs.GPAPUD.bit.PB1 = 1;
	EDIS;
	MD_BSP_XINT_BtnInit(pinNo);
}

uint16_t MD_BSP_EncValue(void) {
	EncDataReg.GPAENC.all = 0x0000000f & (GpioDataRegs.GPADAT.all >> 12);
	return (uint16_t) EncDataReg.GPAENC.all;
}


void MD_BSP_XINT_EncInit(void) {
	/**
	 * Enable XINT2 in the PIE: Group 1 interrupt 5
	 */
#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_ENC_XINT))
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          //!< Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          //!< Enable PIE Group 1 INT5
	IER |= M_INT2;                              //!< Enable CPU int2
	EALLOW;
	PieVectTable.XINT2 = &XINT2_EncLsb_isr;     //!< Register callback handler
#endif
	/**
	 * Configure encoders lsb GPIO
	 */
	EALLOW;
	GpioCtrlRegs.GPADIR.bit.ENC_0 = 0;			//!< Set to input
	GpioCtrlRegs.GPAPUD.bit.ENC_0 = 1;			//!< Enable pullup
	GpioCtrlRegs.GPAMUX1.bit.ENC_0 = 0;      	//!< Mux as GPIO
	GpioCtrlRegs.GPAQSEL1.bit.ENC_0 = 0;     	//!< GPA Qualifier Select 1 Reg (GPIO0..15)
												//!< Xint1 Synch to SYSCLKOUT only
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
	GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xff;		//!< Qual period = 2*0xff/F_CPU=3.4us
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = ENC_LSB; //!< XINT2 is GPIO12
	EDIS;

	/**
	 * Configure edges and enable XINT2
	 */
	XIntruptRegs.XINT2CR.bit.POLARITY = 0x03;   //!< Edge interrupt
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;        //!< Enable XINT2
}

/**
 * @brief	Configure XINT3 to be used as PB1 XINT.
 *
 * @note	GPIO range: 32...63 (XINT3 multiplexer restriction)
 */
void MD_BSP_XINT_BtnInit(const short pinNo) {
	/**
	 * Enable XINT3 in the PIE: Group 12 INTx1 (Interconnect GPIO60 and TZ-6)
	 */
#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_ENC_XINT))
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          	//!< Enable the PIE block
	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;         	//!< Enable PIE Group 12 INT1
	IER |= M_INT12;                             	//!< Enable CPU int12
	EALLOW;
	PieVectTable.XINT3 = &XINT3_Btn_isr;     		//!< Register callback handler
#endif
	/**
	 * Configure pinNo as up pulled input
	 */
	EALLOW;
	GpioCtrlRegs.GPBDIR.all   &= ~(1 << pinNo);		//!< Set to input
	GpioCtrlRegs.GPBMUX2.all  &= ~(1 << pinNo);		//!< Mux as GPIO
	GpioCtrlRegs.GPBQSEL2.all &= ~(1 << pinNo);		//!< GPB Qualifier
	GpioCtrlRegs.GPBPUD.all   |= (1 << pinNo);		//!< Enable pullup

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
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0xff;		//!< Qual period = 2*0xff/F_CPU=3.4us
	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = pinNo;  	//!< XINT3 is GPIO60
	EDIS;

	/**
	 * Configure edges and enable XINT3
	 */
	XIntruptRegs.XINT3CR.bit.POLARITY = 0x0;   	//!< Falling edge interrupt
	XIntruptRegs.XINT3CR.bit.ENABLE = 1;        //!< Enable XINT3
}



