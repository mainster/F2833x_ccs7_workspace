/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "DSP28x_Project.h"


interrupt void GPIO01_isr(void);

/**
 * Externals
 */
extern uint16_t RamfuncsLoadSize;

#undef FLASH
/*
 *  ======== main ========
 */
Void main() {
    InitSysCtrl();
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();
#endif
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;        // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;         // output
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;  // Xint1 is GPIO1
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //!< Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        //!< Enable XINT1
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
    IER |= M_INT1;                              // Enable CPU int1
    EINT;                                       // Enable Global Interrupts

	
    BIOS_start();        /* enable interrupts and start SYS/BIOS */
}

void GPIO01_isr(void) {
    unsigned int i;
    GpioDataRegs.GPASET.bit.GPIO2 = 1; // rising edge at GPIO2
    for(i=0;i<10;i++) asm(" NOP");
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; // falling edge at GPIO2
}
