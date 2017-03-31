/**
 * @file        main.c
 * @project		ELT131_LAB_01
 *
 * @date        21 Mar 2017
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
#include "DSP28x_Project.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Prototypes
 */
interrupt void cpu_timer0_isr(void);
void delaySamps(uint16_t samps);
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR);
uint16_t cpuLoad(float load_in_percent);

/**
 * Virtual GPIO
 */
bool vGPIO = false;

/**
 * Macros
 */
#define testPinLvl(pinLevel)    GpioDataRegs.GPADAT.bit.GPIO0 = pinLevel
#define testPinTgl              GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
//#define testPinLvl(x)   vGPIO = x
//#define testPinTgl()    vGPIO ^= 0x01

/**
 * t_irq = (10*(LOAD-1)+15)/30e6
 * CPU_load[%] = 100* t_irq/50us
 *             = 100*(10*(LOAD-1)+15)/(30e6 * 50e-6)
 *             = 1e3/1.5e3*((LOAD-1)+1.5)
 *             = 2/3*((LOAD-1)+1.5)
 */
#define _cpuLoad(loadInPercent) ((uint16_t) ((float)loadInPercent * 3/2 - .5))

/* BSP related */
#define ledInit     GpioCtrlRegs.GPADIR.bit.GPIO9 = 1; \
                    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1

#define led1Lvl(x) GpioDataRegs.GPADAT.bit.GPIO9 = x        //!< GPA
#define led1Tgl() GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1      //!< GPA

#define led2Lvl(x) GpioDataRegs.GPATOGGLE.bit.GPIO11 = x    //!< GPA
#define led2Tgl() GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1     //!< GPA

#define MD_SYSCLKOUT 30     //!< [MHz]

#undef      FLASH
#undef      JITTER


/**
 * Global variables
 */
uint16_t LOAD = 0;

/*
 * ===================== main of ELT131_LAB_01 =====================
 */
int main(void) {
    /**
     * MCU frequency = crystal frequency * PLL multiplier {set by PLLCR.bit.DIV}
     *                  / PLL divider {set by PLLSTS.bit.DIVSEL}
     * For example:
     *      150MHz = 30MHz * 10 / 2
     *      30MHz = 30MHz * 2 / 2
     */
    MD_InitSysCtrl( 2 );

    #ifdef FLASH
    memcpy( &RamfuncsRunStart,
            &RamfuncsLoadStart,
            (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1 );
    InitFlash();
    #endif

    InitPieCtrl();
    InitPieVectTable();

    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    PieVectTable.TINT0 = &cpu_timer0_isr;   //!<  Re-map vector table
    ledInit;
    EDIS;

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, MD_SYSCLKOUT, 50);

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    IER |= M_INT1;
    asm("  PUSH IER");
    asm("  POP DBGIER");
    EINT;                           //!< Enable global interrupt intm
    ERTM;                           //!< Enable global real time debug module

    float loadInPercent = 50.3;
    LOAD = _cpuLoad(loadInPercent);

    CpuTimer0Regs.TCR.bit.TSS = 0;  //!< Timer Start/Stop

    while (1)
        asm("  NOP");
}

/**
 * @brief      Timer0 IRQ callback
 */
void cpu_timer0_isr(void) {
#ifndef JITTER
    GpioDataRegs.GPADAT.all = 1;            //!< Rising edge
#else
    GpioDataRegs.GPATOGGLE.bit.GPIO0= 1;    //!< Toggle rising edge
#endif
    PieCtrlRegs.PIEACK.all  = 1;            //!< Acknowledge PIE group 1

    int i = LOAD;
    do {
        asm("  NOP");
    } while (--i);

#ifndef JITTER
   GpioDataRegs.GPADAT.all = 0;             //!< Falling edge
#endif
}


/**
 * @brief      InitSysCtrl = f(MD_DSP28_PLLCR)
 *
 * @param[in]  MD_DSP28_PLLCR   The md dsp 28 pllcr
 */
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR) {
   // Disable the watchdog
   DisableDog();

   // Initialize the PLL control: PLLCR and DIVSEL
   // DSP28_PLLCR and DSP28_DIVSEL are defined in DSP2833x_Examples.h
   InitPll(MD_DSP28_PLLCR, DSP28_DIVSEL);

   // Initialize the peripheral clocks
   InitPeripheralClocks();
}

