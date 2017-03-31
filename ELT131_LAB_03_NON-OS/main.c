/**
 * @file        main.c
 * @project		ELT131_LAB_03_NON-OS
 *
 * @date        24 Mar 2017
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
#include <main.h>

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

#define ledInit()     EALLOW; \
    GpioCtrlRegs.GPADIR.bit.LED1 = 1; \
    GpioCtrlRegs.GPADIR.bit.LED2 = 1; \
    ledLvl(LED1, 0); \
    ledLvl(LED2, 0); \
    EDIS

#define ledLvl(LEDx, LVL)   GpioDataRegs.GPADAT.bit.LEDx = LVL
#define ledTgl(LEDx)        GpioDataRegs.GPATOGGLE.bit.LEDx = 1

/**
 * Configure GPIOs
 */
#define gpioInit()    EALLOW; \
        GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; \
        GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
        EDIS;

#define F_CPU   60e6L   //!< SYSCLKOUT = 60MHz
#define FLASH

/*
 * ===================== main of ELT131_LAB_03_NON-OS =====================
 */
int main(void) {
    MD_InitSysCtrl(2*F_CPU/30e6);       //!< Initialize System Control, SYSCLKOUT = 30MHz * 4/2 = 60MHz
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,
           (unsigned long)(&RamfuncsLoadEnd - &RamfuncsLoadStart)+1);
    InitFlash();
#endif
    InitPieCtrl();                      //!< Initialize PIE control registers to their default state
    InitPieVectTable();                 //!< Initialize PIE vector table
    ledInit();

    MD_EPWM1_Init( 20e3, F_CPU );

    xInt1Init();
    gpioInit();

    EINT;                   //!< Enable Global Interrupts

    while (1)
        asm("  NOP");
}

/**
 * @brief     External GPIO1 IRQ callback handler
 */
void xint1_isr(void) {
    GpioDataRegs.GPASET.bit.GPIO2 = 1;          //!< Rising edge at GPIO2
    PieCtrlRegs.PIEACK.all = 1;                 //!< Ack interrupt service
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;        //!< Falling edge at GPIO2
}

void xInt1Init(void) {
    /**
     * Enable Xint1 in the PIE: Group 1 interrupt 4
     */
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          //!< Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          //!< Enable PIE Group 1 INT4
    IER |= M_INT1;                              //!< Enable CPU int1

    /**
     * Configure GPIO
     */
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         //!< Mux as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;          //!< Set to input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;        //!< Xint1 Synch to SYSCLKOUT only
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0x08;   //!< Qual period = SYSCLKOUT/x
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;   //!< XINT1 is GPIO0
    EDIS;

    /**
     * Configure and enable XINT1
     */
    EALLOW;
    PieVectTable.XINT1 = &xint1_isr;            //!< Register callback handler
    EDIS;
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT1
}


/**
 * @brief      Timer0 IRQ callback handler
 */
//void cpu_timer0_isr(void) {
//#ifndef JITTER
//    GpioDataRegs.GPADAT.all = 1;            //!< Rising edge
//#else
//    GpioDataRegs.GPATOGGLE.bit.GPIO0= 1;    //!< Toggle rising edge
//#endif
//    PieCtrlRegs.PIEACK.all  = 1;            //!< Acknowledge PIE group 1
//
//    int i = LOAD;
//    do {
//        asm("  NOP");
//    } while (--i);
//
//#ifndef JITTER
//   GpioDataRegs.GPADAT.all = 0;             //!< Falling edge
//#endif
//}


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

