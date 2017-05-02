/**
 * @file        md_bsp_explorer.h
 * @project		MD_F2833x_LIB
 *
 * @date        28 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Provides basic board support for ti's explorer kit F28335.
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
#ifndef MD_BSP_EXPLORER_H_
#define MD_BSP_EXPLORER_H_

/* -------------------------------  Includes  ------------------------------ */
#ifdef RTOS
 #include <xdc/std.h>
#endif

#include <stdint.h>
#include <stdbool.h>
#include <DSP28x_Project.h>
#include "md_globals.h"

/* ----------------------------  Private defines  -------------------------- */

/**
 * Board specific pin definitions
 */
#define LED1        GPIO9
#define LED2        GPIO11
#define LED3        GPIO34
#define LED4        GPIO49
#define LED_ERR		GPIO34

#define PB1		    GPIO17
#define PB1_BIT		17

#define PB1X        GPIO60
#define PB1X_BIT	60

/* Layout modified, PB2 wired as RESET button */
//#define PB2         GPIO48

/* ------------------------------------------------------------------------- */
/* ------------------  Encoder shadow structures  -------------------------- */
/* ------------------------------------------------------------------------- */
/**
 * Encoder GPIO macros
 */
#define  ENC_0    GPIO12
#define  ENC_1    GPIO13
#define  ENC_2    GPIO14
#define  ENC_3    GPIO15

#define  ENC_0    GPIO12
#define  ENC_1    GPIO13
#define  ENC_2    GPIO14
#define  ENC_3    GPIO15

#define  ENC_LSB  12

#define  ENC_BIT0_MASK    (uint32_t)(1 << 12)
#define  ENC_BIT1_MASK    (uint32_t)(1 << 13)
#define  ENC_BIT2_MASK    (uint32_t)(1 << 14)
#define  ENC_BIT3_MASK    (uint32_t)(1 << 15)

/**
 * Encoder GPIO bit mask (GPIO15...12)
 */
#define ENC_MASK 	(uint32_t)(ENC_BIT0_MASK | ENC_BIT1_MASK | ENC_BIT2_MASK | ENC_BIT3_MASK)
#define ENC_MASKv2 	(uint32_t)(0xf << 12)

/**
 * 4 bits to read hex encoded switch
 */
struct GPAENC_BITS {
   uint16_t ENC0:1;     // 0      GPIO12
   uint16_t ENC1:1;     // 1      GPIO13
   uint16_t ENC2:1;     // 2      GPIO14
   uint16_t ENC3:1;		// 3      GPIO15
};

union GPAENC_REG {
	uint16_t             all;
	struct GPAENC_BITS   bit;
};

struct GPAENC_DATA_REGS {
	union GPAENC_REG GPAENC;
};

/**
 * Global shadow register
 */
extern volatile struct GPAENC_DATA_REGS EncDataReg;

/**
 * Global encoder value
 */
extern volatile uint16_t encVal;

/* ---------------------  Private function prototypes  --------------------- */
#if defined(RTOS) || !defined(RTOS_ENC_XINT)
__interrupt
#endif
void XINT2_EncLsb_isr(void);

#if defined(RTOS) || !defined(RTOS_ENC_XINT)
__interrupt
#endif
void XINT3_Btn_isr(void);

void MD_BSP_EncInit(void);
void MD_BSP_BtnInit(const short pinNo);
void MD_BSP_XINT_EncInit(void);
void MD_BSP_XINT_BtnInit(const short pinNo);

uint16_t MD_BSP_EncValue(void);

/* ----------------------------  Private macros  --------------------------- */
#define MD_BSP_LedInit()     EALLOW; \
    GpioCtrlRegs.GPADIR.bit.LED1 = 1; \
    GpioCtrlRegs.GPADIR.bit.LED2 = 1; \
    GpioCtrlRegs.GPBDIR.bit.LED3 = 1; \
    GpioCtrlRegs.GPBDIR.bit.LED4 = 1; \
    _ledLvlA(LED1, 0); \
    _ledLvlA(LED2, 0); \
    _ledLvlB(LED3, 0); \
    _ledLvlB(LED4, 0); \
    EDIS

#define _ledLvlA(LEDx, LVL)   GpioDataRegs.GPADAT.bit.LEDx = LVL
#define _ledTglA(LEDx)        GpioDataRegs.GPATOGGLE.bit.LEDx = 1
#define _ledLvlB(LEDx, LVL)   GpioDataRegs.GPBDAT.bit.LEDx = LVL
#define _ledTglB(LEDx)        GpioDataRegs.GPBTOGGLE.bit.LEDx = 1
#define errorLed(LVL)		  _ledLvlB(LED_ERR, ~LVL)
#define errorLed_on()		  _ledLvlB(LED_ERR, 0)
#define errorLed_off()		  _ledLvlB(LED_ERR, 1)

/* TODO: GPA/GPB */
#define getButton(PBx) (GpioDataRegs.GPADAT.bit.PBx)

/* ---------------------------  Private typedefs  -------------------------- */
/* --------------------------  Private functions  -------------------------- */
/* --------------------------  Private variables  -------------------------- */

#endif /* MD_BSP_EXPLORER_H_ */
