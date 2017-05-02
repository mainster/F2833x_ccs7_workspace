/**
 * @file        md_spi.c
 * @project		F2833x_SPI_loopback_ints
 * 
 * @date        26 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015 
 * @license		GNU GPL v3
 * 
 * @brief       TODO
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
#include "md_spi.h"

/* ---------------------  Private function prototypes  --------------------- */
void MD_SPI_FifoInit(const uint16_t BAUDVAL, MD_SPI_CharLen_t cLen);
void MD_SPI_DMA_SendHalfWord_fake (uint16_t color, uint16_t pixels_count);

#if defined(RTOS) || !defined(RTOS_ENC_XINT)
__interrupt
#endif
void SPI_TxFifo_isr(void);

#if defined(RTOS) || !defined(RTOS_ENC_XINT)
__interrupt
#endif
void SPI_RxFifo_isr(void);

/* ---------------------  Public function implementations  ----------------- */
void MD_SPI_Init(const uint16_t BAUDVAL, MD_SPI_CharLen_t cLen) {
	EALLOW;
	PieVectTable.SPITXINTA = &SPI_TxFifo_isr;	//!< Register TxFIFO callback handler
	PieVectTable.SPIRXINTA = &SPI_RxFifo_isr;	//!< Register RxFIFO callback handler
	EDIS;

	MD_SPI_FifoInit(BAUDVAL, cLen);			//!< Initialize the SPIA module

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;  //!< Enable the PIE block
	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;  //!< Enable PIE Group 6, INT 1
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1;  //!< Enable PIE Group 6, INT 2

	IER |= M_INT6;						//!< Enable CPU INT6
}

void MD_SPI_PutVar(const uint16_t txChar, MD_SPI_CharLen_t cLen) {
	/**
	 * If some single none-words e.g. 8-Bits commands must me txe'd,
	 * temporarily switch the SPI char width to 8-Bit
	 */
	while (SpiaRegs.SPISTS.bit.BUFFULL_FLAG);;
	if (cLen == MD_SPI_8_BIT) {
		SpiaRegs.SPICCR.bit.SPICHAR = 0x08;
		SpiaRegs.SPITXBUF = 0xff && txChar;
	}
	else
		SpiaRegs.SPITXBUF = txChar;

	if (cLen == MD_SPI_8_BIT)
		SpiaRegs.SPICCR.bit.SPICHAR = 0x0f;
}

void MD_SPI_Put(const uint16_t txChar) {
	while (SpiaRegs.SPISTS.bit.BUFFULL_FLAG);;
	SpiaRegs.SPITXBUF = txChar;
}

void MD_SPI_SetDataSize(MD_SPI_CharLen_t DataSize) {
	SpiaRegs.SPICCR.bit.SPICHAR = (uint16_t) DataSize;
}

/* ---------------------  Private function implementation  ----------------- */
void MD_SPI_FifoInit (const uint16_t BAUDVAL, MD_SPI_CharLen_t cLen) {
	SpiaRegs.SPICCR.bit.SPISWRESET    = 0;        //!< Block SPI logic via reset line
	SpiaRegs.SPICCR.bit.SPICHAR = (uint16_t)cLen; //!< 16-bit character
	SpiaRegs.SPICCR.bit.SPILBK        = 1;        //!< SPI loop back
	SpiaRegs.SPICCR.bit.CLKPOLARITY   = 0;        //!< SPI clock polarity = 0

	SpiaRegs.SPICTL.all               = 0x0000;   //!< Initial state
	SpiaRegs.SPICTL.bit.SPIINTENA     = 1;        //!< Interrupt enable
	SpiaRegs.SPICTL.bit.TALK          = 1;        //!< Master/Slave transmit enable
	SpiaRegs.SPICTL.bit.MASTER_SLAVE  = 1;        //!< Network control mode
	SpiaRegs.SPICTL.bit.CLK_PHASE     = 1;        //!< Clock phase select = 1
	SpiaRegs.SPICTL.bit.OVERRUNINTENA = 1;        //!< Overrun interrupt enable

	SpiaRegs.SPISTS.all               = 0x0000;   //!< Initial state
	SpiaRegs.SPIBRR                   = BAUDVAL;  //!< Baud rate

	SpiaRegs.SPIFFTX.all              = 0x0000;   //!< Initial state
	SpiaRegs.SPIFFTX.bit.TXFFIL       = 0x08;     //!< Interrupt level 8
	SpiaRegs.SPIFFTX.bit.TXFFIENA     = 1;        //!< Interrupt enable
	SpiaRegs.SPIFFTX.bit.SPIRST       = 1;        //!< Reset SPI
	SpiaRegs.SPIFFTX.bit.SPIFFENA     = 1;        //!< Enhancement enable
	SpiaRegs.SPIFFTX.bit.TXFIFO       = 0;        //!< FIFO reset

	SpiaRegs.SPIFFRX.all              = 0x0000;   //!< Initial state
	SpiaRegs.SPIFFRX.bit.RXFFIL       = 0x08;     //!< Set RX FIFO Int level to 8
	SpiaRegs.SPIFFRX.bit.RXFFIENA     = 0;        //!< Rx interrupt disable

	SpiaRegs.SPIFFCT.all              = 0x0000;   //!< FIFO control register

	SpiaRegs.SPIPRI.all               = 0x0000;   //!< Initial state
	SpiaRegs.SPIPRI.bit.FREE          = 1;        //!< Free emulation mode

	SpiaRegs.SPICCR.bit.SPISWRESET    = 1;        //!< Enable SPI (leave reset line)

	SpiaRegs.SPIFFTX.bit.TXFIFO       = 1;        //!< Tx FIFO reset ?? (*RESET)
	SpiaRegs.SPIFFRX.bit.RXFIFORESET  = 1;        //!< Rx FIFO reset
}

void MD_SPI_DMA_SendHalfWord_fake (uint16_t color, uint16_t pixels_count) {
	while (pixels_count--)
		MD_SPI_Put(color);
}

void SPI_TxFifo_isr(void) {
	onSpiTxFifo_irq();						//!< Invoke hook function TxFifo INT

#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_ENC_XINT))
//	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;	//!< Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK6 	= 1;   	//!< Issue PIE ACK for Group 6
#endif
}

void SPI_RxFifo_isr(void) {
	onSpiRxFifo_irq();						//!< Invoke hook function RxFifo INT

#if !defined(RTOS) || (defined(RTOS) && !defined(RTOS_ENC_XINT))
	  SpiaRegs.SPIFFRX.bit.RXFFOVFCLR = 1; 	//!< Clear Overflow flag
	  SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1; 	//!< Clear Interrupt flag
	  PieCtrlRegs.PIEACK.bit.ACK6 	  = 1;  //!< Issue PIE ACK for Group 6
#endif
}





