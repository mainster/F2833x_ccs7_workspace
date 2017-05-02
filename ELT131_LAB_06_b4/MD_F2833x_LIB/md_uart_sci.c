/**
 * @file        md_uart_sci.c
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
#include <stdlib.h>
#include "md_uart_sci.h"

/**
 * Transmit and receive buffers for serial communication.
 * 
 * TODO: Implement UART transceiver buffers based on TI-RTOS Queue-system.
 *   - Inter-thread communication
 *   - Queue (03-04-2017, WHZ, ELT131)
 */
char sciaRxBuff [SCI_RX_BUFF_SIZ];
char sciaTxBuff [SCI_TX_BUFF_SIZ];
char *pUartRxBuff = &sciaRxBuff[0];
char *pUartTxBuff = &sciaTxBuff[0];

const char _EOL[] = { "\n" };

/**
 * @brief      Initialization of serial communication module A.
 *
 * @param[in]  F_CPU_MHZ   The SYSCLKOUT frequency in MHz.
 * @param[in]  baudrate    The UART baudrate in baud.
 */
void MD_SCIA_Init (const float F_CPU_MHZ, const uint32_t baudrate) {
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;        //!< GPIO28 alternate pin funtion: SCIA_Rx
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;        //!< GPIO29 alternate pin funtion: SCIA_Tx
	EDIS;

	/**
	 * SCI software reset (active low). Writing a 0 to this bit initializes the SCI state
	 * machines and operating flags (registers SCICTL2 and SCIRXST) to the reset condition
	 */
	SciaRegs.SCICTL1.bit.SWRESET = 0;           //!< Reset SCI registers
	SciaRegs.SCICTL1.bit.SWRESET = 1;           //!< Reset SCI registers

	/**
	 * SCICCR:   SCI Communication control register
	 * |     7     |    6     |     5      |      4       |     3     |  2 ... 0  |
	 * |-----------|----------|------------|--------------|-----------|-----------|
	 * | STOP BITS | EVEN/ODD | PARITY ENA | LOOPBACK ENA | ADDR/IDLE | SCICHARx  |
	 */
	SciaRegs.SCICCR.bit.STOPBITS = 1-1;         //!< Stop bit ctrl: 0: 1-Stop 1: 2-Stop
	SciaRegs.SCICCR.bit.PARITY = 1;             //!< Parity: Even
	SciaRegs.SCICCR.bit.PARITYENA = 0;          //!< Parity check: disable
	SciaRegs.SCICCR.bit.SCICHAR = 8-1;          //!< Character-length ctrl bits 2..0

	/**
	 * SCICTL1:   SCI Control register 1
	 * SCI receiver/transmitter enable/disable
	 */
	SciaRegs.SCICTL1.bit.RXENA = 1;             //!< Enable receiver to buffer transfer
	SciaRegs.SCICTL1.bit.TXENA = 1;             //!< Enable transmitter
	SciaRegs.SCICTL2.bit.TXINTENA = 1;          //!< Enable SCI_A Tx IRQ
	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;        //!< Enable SCI_A Rx IRQ

	/**
	 * LSPCLK   Low-speed peripheral clock.
	 * LOSPCP   Low-speed peripheral clock pre-scaler
	 *
	 * BRR = (LSPCLK[Hz] / (BAUDRATE * 8)) -1
	 * LSPCLK[Hz] = (SYSCLKOUT / LSPCLK_DIV)
	 *
	 * ATTENTION:   (SPRUFB0D.pdf, p39)
	 * --------------------------------
	 * LSPCLK_DIV = (SysCtrlRegs.LOSPCP.bit.LSPCLK << 1)
	 *
	 * BRR(BAUD, SYSCLKOUT) = (SYSCLKOUT/(LOSPCP * BAUD *8)) - 1
	 */
	uint16_t BRR =
			(F_CPU_MHZ * 1e6) /
			((SysCtrlRegs.LOSPCP.bit.LSPCLK << 1) * baudrate * 8) - 1;

	SciaRegs.SCIHBAUD = UINT8_C(BRR >> 8);      //!< Set the upper 8-Bits
	SciaRegs.SCILBAUD = UINT8_C(BRR & 0x00ff);  //!< Set the lower 8-Bits

	/**
	 * Initialize transmit and receive buffers
	 */
	pUartRxBuff = &sciaRxBuff[0];
	pUartTxBuff = &sciaTxBuff[0];

#if defined(SERVICE_TX_IRQ) && !defined(RTOS)
	EALLOW;
	PieVectTable.SCITXINTA = &MD_SCIA_Tx_irq;
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;  		//!< SCI-A-TX-irq
#endif

#if defined(SERVICE_RX_IRQ) && !defined(RTOS)
	EALLOW;
	PieVectTable.SCIRXINTA = &MD_SCIA_Rx_irq;
	EDIS;
#endif
}


/**
 * @brief      Simple digit to char converter.
 *
 * @param      number   The number 
 * @param[in]  base     The base
 * @return     character representation of number
 */
char makedigit (uint32_t *number, uint32_t base) {
	static char map[] = "0123456789";
	uint16_t ix;

	for (ix=0; *number >= base; ix++)
		*number -= base;

	return map[ix];
}

/**
 * @brief      Simple integral to string converter.
 *
 * @param[in]  number   The number
 * @return     Pointer to string representation of number.
 */
const char *int2str (const uint32_t number) {
	static char tmp[12] = { " " };
	char *p = &tmp[0];
	uint32_t _number = number;
	bool firstNonzero = false;

	if (! _number) {
		*p++ = '0';
		*p = '\0';
		return tmp;
	}

	for (uint32_t ddec = 1e9; ddec >= 1; ddec /= 10) {
		*p = makedigit(&_number, ddec);
		if ((*p != '0') || firstNonzero) {
			firstNonzero = true;
			p++;
		}
	}
	*p = '\0';
	return tmp;
}


retVal_t uartPutsp(char *str) {
	retVal_t ret = _uartPuts(str, "\n");
	str = NULL;
	return ret;
}

retVal_t uartPuts(const char *str) {
	return _uartPuts(str, "\n");
}

retVal_t uartPutsNoEOL(const char *str) {
	return _uartPuts(str, "");
}

retVal_t _uartPuts(const char *str, const char *EOL) {
	/* Return busy if pUartTxBuff doesnt match txBuff base */
	if (pUartTxBuff != &sciaTxBuff[0])
		return ret_busy;

	retVal_t retVal = ret_ok;

	/* Strip if string doesnt fit into tx buffer */
	if (strlen(str) >= SCI_TX_BUFF_SIZ) {
		strncpy(pUartTxBuff, str, SCI_TX_BUFF_SIZ-1);
		retVal = ret_striped;
	}
	else {
		strcpy(pUartTxBuff, str);
		strcat(pUartTxBuff, EOL);
	}

	/* Copy first char to SCI Tx register */
	SciaRegs.SCITXBUF = *pUartTxBuff++;

	return retVal;
}


char *_uartGets(const int maxChars) {
	pUartRxBuff = &sciaRxBuff[0];
	/**
	 * If less than strlen(pUartRxBuff) chars has been queried,
	 * strip rx buffer by manual placed '\0'.
	 */
	if ((maxChars > 0) && (maxChars < strlen(pUartRxBuff))) {
		if (maxChars < strlen(pUartRxBuff))
			*(pUartRxBuff + maxChars) = '\0';
	}
	uartSetRxed(0);
	return pUartRxBuff;
}

int16_t _uartRxedLine(int16_t state) {
	static int16_t line_received = 0;
	if (state > -1) {
		line_received = state;
		if (state == 0)
			/**
			 * Clear RX FIFO interrupt flag to unblock new incoming interrupts
			 */
			SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	}

	return line_received;
}


/**
 * @brief      SCIA ready to transmit IRQ handler 
 */
void MD_SCIA_Tx_irq(void) {
	if ((*pUartTxBuff != '\0') &&
			(pUartTxBuff <= &sciaTxBuff[SCI_TX_BUFF_SIZ]))
		SciaRegs.SCITXBUF = *pUartTxBuff++;
	else
		/*
		 * Reset pointer to buffer base address stops blocking of
		 * new uartPuts() calls.
		 */
		pUartTxBuff = &sciaTxBuff[0];

#if defined(SERVICE_TX_IRQ) && !defined(RTOS)
	/**
	 * Acknowledge group 9 IRQ
	 */
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
#endif
}

/**
 * @brief      SCIA data receive IRQ handler
 */
void MD_SCIA_Rx_irq(void) {
	/**
	 * If Rx buffer pointer equals Rf buffer base address,
	 * memset Rx buffer up to (and including) next '\0'
	 */
	if (pUartRxBuff == &sciaRxBuff[SCI_RX_BUFF_SIZ])
		memset(pUartRxBuff, ' ', strlen(pUartRxBuff)+1);

	/**
	 * If \n detected, close string in buffer by appending \0
	 */
	if ((SciaRegs.SCIRXBUF.all == '\n') ||
			(pUartRxBuff >= &sciaRxBuff[SCI_RX_BUFF_SIZ])) {
		*pUartRxBuff = '\0';
		uartSetRxed(1);
	}
	else {
		*pUartRxBuff++ = SciaRegs.SCIRXBUF.all;
		/**
		 * Clear RX FIFO interrupt flag to unblock new incoming interrupts
		 */
		SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	}
#if defined(SERVICE_RX_IRQ) && !defined(RTOS)
	/**
	 * Acknowledge group 9 IRQ
	 */
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
#endif

	//SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
}
