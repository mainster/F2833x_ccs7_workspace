/**
 * @file        main.c
 * @path        ~/CODES_local/ccs7_workspace3/F28335_ram_led/main.c
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
 * Prototypes
 */
__interrupt void xint1_isr(void);
void delaySamps(uint16_t samps);
void MD_InitSysCtrl(uint16_t MD_DSP28_PLLCR);
void xInt1Init(void);
//void gpioInit(void);

/**
 * Macros
 */
#define LED1        GPIO9
#define LED2        GPIO11
#define LED3        GPIO34
#define LED4        GPIO49

#define ledInit()     EALLOW; \
        GpioCtrlRegs.GPADIR.bit.LED1 = 1; \
        GpioCtrlRegs.GPADIR.bit.LED2 = 1; \
        GpioCtrlRegs.GPBDIR.bit.LED3 = 1; \
        GpioCtrlRegs.GPBDIR.bit.LED4 = 1; \
        led12lvl(LED1, 0); \
        led12lvl(LED2, 0); \
        led34lvl(LED3, 0); \
        led34lvl(LED4, 0); \
        EDIS;

#define led12lvl(LED, LVL)  GpioDataRegs.GPADAT.bit.LED = LVL;
#define led34lvl(LED, LVL)  GpioDataRegs.GPBDAT.bit.LED = LVL;
#define led12tgl(LED)       GpioDataRegs.GPATOGGLE.bit.LED = 1;
#define led34tgl(LED)       GpioDataRegs.GPBTOGGLE.bit.LED = 1;

#define FLASH

/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; \
        GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
        EDIS;
/**
 * Global variables
 */
uint16_t ticksCtr = 0;

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
    InitSysCtrl();

    #ifdef FLASH
    memcpy( &RamfuncsRunStart,
            &RamfuncsLoadStart,
            (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1 );
    InitFlash();
    #endif

    InitPieCtrl();
    InitPieVectTable();

    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;   //!<  Re-map vector table
    EDIS;

    ledInit();

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 50000);

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    IER |= M_INT1;
    asm("  PUSH IER");
    asm("  POP DBGIER");
    EINT;                           //!< Enable global interrupt intm
    ERTM;                           //!< Enable global real time debug module

    CpuTimer0Regs.TCR.bit.TSS = 0;  //!< Timer Start/Stop

    while (1) {
        if ( ticksCtr > 15 )
            led34lvl(LED3, 1);
        if ( ticksCtr >= 20 ) {
            led34lvl(LED3, 0);
            ticksCtr = 0;
        }
    }
}

/**
 * @brief      Timer0 IRQ callback
 */
void cpu_timer0_isr(void) {
    PieCtrlRegs.PIEACK.all  = 1;            //!< Acknowledge PIE group 1
    ticksCtr++;
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

