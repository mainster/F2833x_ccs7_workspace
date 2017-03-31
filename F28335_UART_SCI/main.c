/**
 * @file        main.c
 * @path        ~/CODES_local/ccs7_workspace3/F28335_UART_SCI/main.c
 *
 * @date        
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license     GNU GPL v3
 *
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
 *
 */
#include <stdint.h>
#include <string.h>
#include "DSP2833x_Device.h"
#include "DSP28x_Project.h"
#include "md_uart_sci.h"


interrupt void cpu_timer0_isr(void);

void Gpio_select(void);

#define SYSCLKOUT   150e6L
#define BAUDRATE    115200L


/* BSP related */
#define ledInit     GpioCtrlRegs.GPADIR.bit.GPIO9 = 1; \
                    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1

#define led1Lvl(x) GpioDataRegs.GPADAT.bit.GPIO9 = x        //!< GPA
#define led1Tgl() GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1      //!< GPA

#define led2Lvl(x) GpioDataRegs.GPATOGGLE.bit.GPIO11 = x    //!< GPA
#define led2Tgl() GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1     //!< GPA

//#define MD_SYSCLKOUT 30     //!< [MHz]

#undef 		FLASH
#undef      JITTER



void main(void) {
    InitSysCtrl();    // Basic Core Initialization
#ifdef FLASH
    memcpy( &RamfuncsRunStart,
            &RamfuncsLoadStart,
            (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1 );
    InitFlash();
#endif

    // SYSCLK=150MHz, HISPCLK=75MHz, LSPCLK=37.5MHz
    EALLOW;
    SysCtrlRegs.WDCR= 0x00AF;          // Re-enable the watchdog
    EDIS;            // 0x00E8  to disable the Watchdog , Prescaler = 1
    // 0x00AF  to NOT disable the Watchdog, Prescaler = 64

    Gpio_select();    // GPIO9, GPIO11, GPIO34 and GPIO49 as output
    // to 4 LEDs at Peripheral Explorer

    InitPieCtrl();        // default status of PIE; in DSP2833x_PieCtrl.c

    InitPieVectTable(); // init PIE vector table; in DSP2833x_PieVect.c

    // re-map PIE - entry for Timer 0 Interrupt and SCI-A-TX
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
//    PieVectTable.SCITXINTA = &MD_SCIA_Tx_irq;
//    PieVectTable.SCIRXINTA=&MD_SCIA_Rx_irq;

    EDIS;

    InitCpuTimers();    // Function in: DSP2833x_CpuTimers.c

    // Configure CPU-Timer 0 to interrupt every 50 ms:
    // 150MHz CPU Freq, 50000 µseconds interrupt period
    ConfigCpuTimer(&CpuTimer0, 150, 50000);    // DSP2833x_CpuTimers.c

    MD_SCIA_Init(150, BAUDRATE);  // Initalize SCI

//    strcpy(pUartTxBuff, "The initial Tx buffer content!");

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;    // Cpu Timer 0 isr
    PieCtrlRegs.PIEIER9.bit.INTx1=1;
//    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;  // SCI-A-TX-isr

    // Enable the PIE.
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    // Enable CPU INT1 (CPU-Timer 0) and INT9 (SCIA-TX):
    IER = 0x101;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    CpuTimer0Regs.TCR.bit.TSS = 0;        // Start T0


    while(1) {
        uartPuts("This is the string!");

        while(CpuTimer0.InterruptCount < 20) {
            EALLOW;
            SysCtrlRegs.WDKEY = 0xAA;            // Service watchdog #2
            EDIS;
            if (uartHasRxed())
                uartPuts(&uartGets());
        }
        CpuTimer0.InterruptCount = 0;
    }
}

void Gpio_select(void) {
    EALLOW;
    GpioCtrlRegs.GPAMUX1.all = 0;            // GPIO15 ... GPIO0 = General Puropse I/O
    GpioCtrlRegs.GPAMUX2.all = 0;            // GPIO31 ... GPIO16 = General Purpose I/O



    GpioCtrlRegs.GPBMUX1.all = 0;            // GPIO47 ... GPIO32 = General Purpose I/O
    GpioCtrlRegs.GPBMUX2.all = 0;            // GPIO63 ... GPIO48 = General Purpose I/O
    GpioCtrlRegs.GPCMUX1.all = 0;            // GPIO79 ... GPIO64 = General Purpose I/O
    GpioCtrlRegs.GPCMUX2.all = 0;            // GPIO87 ... GPIO80 = General Purpose I/O

    GpioCtrlRegs.GPADIR.all = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;        // peripheral explorer: LED LD1 at GPIO9
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;        // peripheral explorer: LED LD2 at GPIO11

    GpioCtrlRegs.GPBDIR.all = 0;            // GPIO63-32 as inputs
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;    // peripheral explorer: LED LD3 at GPIO34
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1; // peripheral explorer: LED LD4 at GPIO49

    GpioCtrlRegs.GPCDIR.all = 0;            // GPIO87-64 as inputs

    //SL
    //    GpioCtrlRegs.GPAPUD.bit.GPIO28=0;
//    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 1;   // Disable internal pullup on GPIO28

    //  GPIO-28 - PIN FUNCTION = SCI-RX
    //    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;    // 0=GPIO,  1=SCIRX-A,  2=Resv,  3=Resv
    //    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;        // 1=OUTput,  0=INput
    //    GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;    // uncomment if --> Set Low initially
    //    GpioDataRegs.GPASET.bit.GPIO28 = 1;        // uncomment if --> Set High initially

    EDIS;
}


interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;        // increment time counter
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1; // toggle LED at GPIO34
    // Service the watchdog every Timer 0 interrupt
    EALLOW;
    SysCtrlRegs.WDKEY = 0x55;        // Service watchdog #1
    EDIS;
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//===========================================================================
// End of SourceCode.
//===========================================================================
