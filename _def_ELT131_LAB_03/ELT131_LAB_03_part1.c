/**
 * @file        main.c
 * @project		ELT131_LAB_03
 *
 * @date        24.03.2017
 * @author      Manuel Del Basso (mad16h09)
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
#include <stdio.h>
#include <stdlib.h>     //!< malloc()
#include <stdint.h>
//#include "main.h"

/**
 * BIOS header files
 */
//#include <xdc/std.h>
//#include <xdc/runtime/System.h>
//#include <xdc/runtime/Log.h>
//#include <xdc/cfg/global.h>
//#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/knl/Task.h>

/**
 * F28335 controlSUITE board support header files
 */
#include "DSP28x_Project.h"
//#include <DSP2833x_Gpio.h>
#include "ePWM1.h"

/**
 * Externals
 */
//extern uint16_t RamfuncsLoadSize;

/**
 * Prototypes
 */
//void ledToggle(void);
void delay500m(void);
void bubbleSort(uint16_t *pList, uint16_t n);
void sortRandomRam(void);
//void queryKey();
//void infDelay(void);

/**
 * Global variables
 */
volatile uint16_t i16ToggleCount = 0;

/**
 * Macro defines
 */
#define _ledToggle() GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1

#define getKey1 (GpioDataRegs.GPADAT.bit.GPIO17)

/*
 * ===================== main of ELT131_LAB_03 =====================
 */
void main(void) {
    InitSysCtrl();
#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (unsigned long)(&RamfuncsLoadSize));
    InitFlash();
#endif
    InitSysCtrl();
    InitPieCtrl();
    InitPieVectTable();
    ePwm1Init();

    while (1)
        asm("  NOP");

//    EALLOW;
//    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
//    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
//    EDIS;
//
//    BIOS_start();
}

/**
 * @brief   Led toggle function
 */
//void ledToggle(void) {
//    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;  //!< LD2 toggle
//    delay500m();
//    i16ToggleCount++;
//    Log_info1("Toggle LED the %u time", i16ToggleCount);
//}

/**
 * @brief      Queries key 1.
 *
 *             If key 1 pressed, this task creates dynamically a child task of
 *             sortRandomRam(). TODO: Is reentrant?
 */
//void queryKey() {
//    if (! getKey1) {
//        Task_Params taskParams;
//        Task_Handle taskHndl;
//        Task_Params_init(&taskParams);
//        taskParams.stackSize = 512;
//        taskParams.priority = 15;
//
//        taskHndl = Task_create((Task_FuncPtr)sortRandomRam, &taskParams, NULL);
//        if (taskHndl == NULL)
//            System_abort("Task create failed");
//        delay500m();            //!< Debounce
//    }
//}

#define nWords 100

/**
 * @brief      Sorts a uninitialized ram array of size nWords
 */
void sortRandomRam(void) {
    uint16_t *pMem = malloc(sizeof(uint16_t) * nWords);
    bubbleSort(pMem, nWords);
}

/**
 * @brief      Bubble sort algorithm.
 *
 * @param      pList   Pointer to an array which contents should be sorted.
 * @param[in]  n       sizeof array
 */
void bubbleSort(uint16_t *pList, uint16_t n) {
    long c, d, t;

    for (c = 0 ; c < ( n - 1 ); c++) {
        for (d = 0 ; d < n - c - 1; d++) {
            if (pList[d] > pList[d + 1]) {
                t         = pList[d];
                pList[d]   = pList[d + 1];
                pList[d + 1] = t;
            }
        }
    }
}

/**
 * @brief   Never ending delay function
 */
void infDelay(void) {
    while (1)
        delay500m();
}

/**
 * @brief   Delay function
 */
void delay500m(void) {
    for (uint32_t li = 0; li < 700000; li++)
        asm("  NOP");
}
