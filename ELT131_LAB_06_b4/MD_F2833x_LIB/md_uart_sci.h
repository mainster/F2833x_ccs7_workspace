/**
 * @file        md_uart_sci.h
 * @project		MD_F2833x_LIB
 * 
 * @date        29 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015 
 * @license		GNU GPL v3
 * 
 * @brief       Implementation of interrupt driven UART interface via
 *              TMS320F28335 SCI module. Provides low level functions
 *              for uart communication.
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
#ifndef MD_UART_SCI_H_
#define MD_UART_SCI_H_

#ifdef __cplusplus
 extern "C" {
#endif
    
/* -------------------------------  Includes  ------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "DSP2833x_Device.h"

#ifdef RTOS
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#endif
/* ----------------------------  Configuration  ---------------------------- */
#define SCI_RX_BUFF_SIZ   64
#define SCI_TX_BUFF_SIZ   64

#define SERVICE_RX_IRQ
#define SERVICE_TX_IRQ
//#undef    SERVICE_RX_IRQ
//#undef    SERVICE_TX_IRQ

/* --------------------------  External variables  ------------------------- */
extern const char _EOL[];

/* ---------------------------  Private typedefs  -------------------------- */
typedef enum RetVal {
    ret_ok,
    ret_busy,
    ret_fail,
    ret_striped,
} retVal_t;

/* ----------------------------  Private defines  -------------------------- */
#define MD_UART_SciaInit(__fcpuMhz, __baudrate)     MD_SCIA_Init(__fcpuMhz, __baudrate)

/* ----------------------------  Private macros  --------------------------- */
#define uartGets()      (*_uartGets(-1))
#define uartGetsn(n)    (*_uartGets(n))
#define uartHasRxed()   (_uartRxedLine(-1))     //!< Returns static state
#define uartSetRxed(x)  (_uartRxedLine(x))      //!< Updates and returns static state

/* ---------------------  Private function prototypes  --------------------- */
void MD_SCIA_Init (const float _F_CPU_MHZ, const uint32_t baudrate);
char *_uartGets(const int maxChars);
retVal_t _uartPuts(const char *str, const char *EOL);
retVal_t uartPuts(const char *str);
retVal_t uartPutsp(char *str);
retVal_t uartPutsNoEOL(const char *str);
int16_t _uartRxedLine(int16_t state);
const char *int2str (const uint32_t number);
char makedigit (uint32_t *number, uint32_t base);


/**
 * Remove compiler directive __interrupt from function declaration
 * and definition if used as TI_RTOS irq callback.
 */
//#ifndef RTOS
interrupt
//#endif
void MD_SCIA_Rx_irq(void);

//#ifndef RTOS
interrupt
//#endif
void MD_SCIA_Tx_irq(void);


#ifdef __cplusplus
 }
#endif

#endif
