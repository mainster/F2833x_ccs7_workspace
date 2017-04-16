/**
 * @file        md_epwm.h
 * @project		MD_F2833x_LIB
 *
 * @date        31 Mar 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
 * @brief       Provides initialization and configuration methods for ePWM.
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
#ifndef _EPWM1_h_
#define _EPWM1_h_

/* -------------------------------  Includes  ------------------------------ */
#include <stdint.h>
#include "DSP28x_Project.h"
#include "md_bsp_explorer.h"
#include "md_globals_TEMPLATE.h"
/* ----------------------------  Configuration  ---------------------------- */
/* ---------------------------  Private typedefs  -------------------------- */
/* --------------------------  Private functions  -------------------------- */
/* --------------------------  Private variables  -------------------------- */
/* ----------------------------  Private defines  -------------------------- */

/* ----------------------------  Private macros  --------------------------- */
#define F_EPWM_MIN(_eDIV, _F_CPU)     ((uint32_t)(_F_CPU/_eDIV)/((uint32_t)2*0xffff))

/* ---------------------  Private function prototypes  --------------------- */
void MD_EPWM1_Init(const uint32_t f_ePwm1, const uint32_t _F_CPU);
void MD_EPWM1_freqConfig(const uint32_t f_ePwm1, const uint32_t _F_CPU);

/**
 * ePWM Submodule Configuration Parameters
 *
 * Submodule       Configuration Parameter or Option
 * Time-base (TB)  • Scale the time-base clock (TBCLK) relative to the system clock (SYSCLKOUT).
 *                 • Configure the PWM time-base counter (TBCTR) frequency or period.
 *                 • Set the mode for the time-base counter:
 *                     – count-up mode: used for asymmetric PWM
 *                     – count-down mode: used for asymmetric PWM
 *                     – count-up-and-down mode: used for symmetric PWM
 *                 • Configure the time-base phase relative to another ePWM module.
 *                 • Synchronize the time-base counter between modules through hardware or software.
 *                 • Configure the direction (up or down) of the time-base counter after a synchronization event.
 *                 • Configure how the time-base counter will behave when the device is halted by an emulator.
 *                 • Specify the source for the synchronization output of the ePWM module:
 *                     – Synchronization input signal
 *                     – Time-base counter equal to zero
 *                     – Time-base counter equal to counter-compare B (CMPB)
 *                     – No output synchronization signal generated.
 *
 * Counter-compare (CC)    • Specify the PWM duty cycle for output EPWMxA and/or output EPWMxB
 *                         • Specify the time at which switching events occur on the EPWMxA or EPWMxB output
 *
 * Action- (AQ)    • Specify the type of action taken when a time-base or counter-compare submodule event occurs:
 * qualifier           – No action taken
 *                     – Output EPWMxA and/or EPWMxB switched high
 *                     – Output EPWMxA and/or EPWMxB switched low
 *                     – Output EPWMxA and/or EPWMxB toggled
 *                 • Force the PWM output state through software control
 *                 • Configure and control the PWM dead-band through software
 *
 * Dead-band (DB)      • Control of traditional complementary dead-band relationship between upper and lower switches
 *                     • Specify the output rising-edge-delay value
 *                     • Specify the output falling-edge delay value
 *                     • Bypass the dead-band module entirely. In this case the PWM waveform is passed through 
 *                       without modification.
 *
 * PWM-chopper (PC)    • Create a chopping (carrier) frequency.
 *                     • Pulse width of the first pulse in the chopped pulse train.
 *                     • Duty cycle of the second and subsequent pulses.
 *                     • Bypass the PWM-chopper module entirely. In this case the PWM waveform is passed through 
 *                       without modification.
 *
 * Trip-zone (TZ)      • Configure the ePWM module to react to one, all, or none of the trip-zone pins .
 *                     • Specify the tripping action taken when a fault occurs:
 *                         – Force EPWMxA and/or EPWMxB high
 *                         – Force EPWMxA and/or EPWMxB low
 *                         – Force EPWMxA and/or EPWMxB to a high-impedance state
 *                         – Configure EPWMxA and/or EPWMxB to ignore any trip condition.
 *                     • Configure how often the ePWM will react to each trip-zone pins :
 *                         – One-shot
 *                         – Cycle-by-cycle
 *                     • Enable the trip-zone to initiate an interrupt.
 *                     • Bypass the trip-zone module entirely.
 *
 * Event-trigger (ET)  • Enable the ePWM events that will trigger an interrupt.
 *                     • Enable ePWM events that will trigger an ADC start-of-conversion event.
 *                     • Specify the rate at which events cause triggers (every occurrence or every second or third occurrence)
 *                     • Poll, set, or clear event flags
 *
 *
 *  EALLOW-Protected ePWMx Registers
 *  TZSEL TZCTL TZEINT TZCLR TZFRC HRCNFG
 *
 */
#endif
