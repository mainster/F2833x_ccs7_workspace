/**
 * @file        main.c
 * @project		MD_BSP_EXPLORER_F28335
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
#include "main.h"


void main(void) {
	InitSysCtrl();			//!< Core initialization
    InitPieCtrl();        	//!< Default status of PIE
    InitPieVectTable(); 	//!< Initialize PIE vector table

    ledsInit();
    MD_ENC_Init();

    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;

    InitCpuTimers();    // Function in: DSP2833x_CpuTimers.c

    // Configure CPU-Timer 0 to interrupt every 50 ms:
    // 150MHz CPU Freq, 50000 µseconds interrupt period
    ConfigCpuTimer(&CpuTimer0, 150, 50000);    // DSP2833x_CpuTimers.c

    MD_SCIA_Init(150, 115200);  // Initalize SCI

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;    // Cpu Timer 0 isr
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    // Enable CPU INT1 (CPU-Timer 0) and INT9 (SCIA-TX):
    IER = 0x101;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    CpuTimer0Regs.TCR.bit.TSS = 0;        // Start T0


    while(1) {
    	uint16_t encv = MD_ENC_Value();
    	char *tmp = makestring(encv);
        uartPuts(tmp);
        uartPuts("\n");

        while(CpuTimer0.InterruptCount < 5) {
            EALLOW;
            SysCtrlRegs.WDKEY = 0xAA;            // Service watchdog #2
            EDIS;
            if (uartHasRxed())
                uartPuts(&uartGets());
        }
        CpuTimer0.InterruptCount = 0;
    }
}

interrupt void cpu_timer0_isr(void) {
    CpuTimer0.InterruptCount++;        // increment time counter
    _ledTglB(LED3);
    // Service the watchdog every Timer 0 interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
