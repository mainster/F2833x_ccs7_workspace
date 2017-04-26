/*
 * main.c
 *
 *  Created on: May 6, 2013
 *      Author: RobG
 */
#include "msp430.h"
#include "msp.h"
#include "lcd.h"
#include "graphics.h"
#include "color.h"
#include "typedefs.h"
#include "tests.h"

u_char orientation = 0;

void main(void) {

	WDTCTL = WDTPW + WDTHOLD; // disable WDT

	initMSP430();

	_delay_cycles(160000); // wait

	initLCD();

	while (1) {
		_delay_cycles(1600000);

		clearScreen(1);

		setColor(COLOR_16_RED);
		drawString(5, 5, FONT_SM, "Texas Instruments");
		setColor(COLOR_16_WHITE);
		drawString(5, 20, FONT_MD, "2.2\" 320x240 BoosterPack");
		setColor(COLOR_16_BLUE);
		drawString(5, 40, FONT_MD, "& MSP430F5529 LaunchPad");
		setColor(COLOR_16_ORANGE);
		drawString(5, 60, FONT_MD, "RobG's graphics library");
		setColor(COLOR_16_PURPLE);
		drawString(5, 80, FONT_MD, "Works with:");
		setColor(COLOR_16_YELLOW);
		drawString(5, 100, FONT_LG, "F5172 F5510 F5529");
		setColor(COLOR_16_GREEN_YELLOW);
		drawString(5, 120, FONT_LG, "G2553 G2955 & more");

		_delay_cycles(32000000);

		drawTILogo(56, 56, COLOR_16_RED);
		drawTILogo(55, 56, COLOR_16_RED);
		drawTILogo(56, 55, COLOR_16_RED);
		drawTILogo(55, 55, COLOR_16_RED);
		drawTILogo(50, 50, COLOR_16_WHITE);

		_delay_cycles(40000000);
		shesGotColors(100);

		_delay_cycles(12000000);
//
		clearScreen(1);
//
//		drawLogicLines(8);
//
//		_delay_cycles(16000000);

		//drawSpirograph(40, 20, 15);
		//_delay_cycles(16000000);

		setOrientation(++orientation & 0x03);

	}
}

