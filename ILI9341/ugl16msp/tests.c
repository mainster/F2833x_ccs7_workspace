/*
 * tests.c
 *
 *  Created on: Sep 11, 2013
 *      Author: RobG
 */

#include "graphics.h"
#include "lcd.h"
#include "typedefs.h"
#include "tests.h"
#include <Math.h>

extern const unsigned int colors[];

u_char cc = 0;
u_char sx = 0;
u_char sy = 0;
u_char d = 0;
u_char o = 0;

const u_char starty[13] = { 30, 48, 150, 50, 40, 85, 38, 105, 66, 130, 35, 71,
		117 };
const u_char startx[17] = { 180, 30, 100, 96, 48, 50, 150, 70, 40, 38, 123, 132,
		80, 35, 110, 55, 99 };
const u_char dim[9] = { 20, 14, 5, 17, 10, 5, 3, 18, 12 };
const u_char offset[10] = { 6, 4, 1, 9, 7, 2, 8, 0, 3, 5 };

const u_char ti1[10] = { 94, 36, 109, 36, 97, 92, 82, 92, 94, 36 };
const u_char ti2[82] = { 0, 58, 41, 58, 41, 0, 80, 0, 80, 25, 88, 28, 84, 47,
		72, 47, 68, 65, 80, 65, 76, 84, 75, 95, 76, 103, 82, 110, 94, 112, 110,
		107, 115, 93, 106, 93, 104, 90, 110, 65, 122, 65, 126, 47, 114, 47, 118,
		29, 148, 32, 148, 54, 156, 65, 154, 95, 130, 106, 116, 120, 111, 134,
		114, 149, 98, 150, 80, 138, 62, 104, 54, 97, 46, 99, 38, 106, 30, 104,
		20, 88, 0, 58 };

///////////////////////////
// TI Logo
///////////////////////////
void drawTILogo(u_int x, u_int y, u_int color) {
	u_char sx = 0;
	u_char sy = 0;
	u_int ax = 0;
	u_int ay = 0;
	setColor(color);
	drawCircle(105 + x, 23 + y, 8);
	u_char i = 0;
	sx = ti1[i++] + x;
	sy = ti1[i++] + y;
	while (i < 10) {
		ax = ti1[i++] + x;
		ay = ti1[i++] + y;
		drawLine(sx, sy, ax, ay);
		sx = ax;
		sy = ay;
	}
	i = 0;
	sx = ti2[i++] + x;
	sy = ti2[i++] + y;
	while (i < 82) {
		ax = ti2[i++] + x;
		ay = ti2[i++] + y;
		drawLine(sx, sy, ax, ay);
		sx = ax;
		sy = ay;
	}
}

///////////////////////////
// spirograph
///////////////////////////
void drawSpirograph(u_int R, u_int r, u_int o) {

	u_int x = 120;
	u_int y = 180;
	u_int px = 0;
	u_int py = 0;
	u_int Rr = R + r;
	u_int Rrr = Rr / r;
	u_int Rrrt = 0;

//	x = (int) (R + r) * Math.cos(t) + o * Math.cos(((R + r) / r) * t);
//	y = (int) (R + r) * Math.sin(t) + o * Math.sin(((R + r) / r) * t);

	double t;

	for (t = 0.0; t < 100; t += 0.1) {
		Rrrt = Rrr * t;
//		x = (u_int) (R + r) * Math.cos(t) - (r + a) * Math.cos(((R + r) / r) * t);
//		y = (u_int) (R + r) * Math.sin(t) - (r + a) * Math.sin(((R + r) / r) * t);
		x = ((Rr) * cos(t)) - (o * cos(Rrrt));
		y = ((Rr) * sin(t)) - (o * sin(Rrrt));

	}

	drawLine(px, py, x, y);

	px = x;
	py = y;

//	x = (R + r) * cos(t) - O * cos(((R + r) / r) * t);
//	y = (R + r) * sin(t) - O * sin(((R + r) / r) * t);
//(moving circle outside the fixed circle)

//	x = (R - r) * cos(t) + O * cos(((R - r) / r) * t);
//	y = (R - r) * sin(t) - O * sin(((R - r) / r) * t);
//(moving circle inside the fixed circle)

}

///////////////////////////
//
///////////////////////////

void randomLines(u_int numberOfLines) {

//TODO

}

///////////////////////////
//
///////////////////////////
void drawLogicLines(u_char h) {
	u_char * addr = 0;
	addr += 0x1C00;
	u_int y = 0;
	u_char c = 1;
	while (y < getScreenHeight()) {
		setColor(colors[c]);
		drawLogicLine(1, y, getScreenWidth(), h, (u_char*) addr);
		y += (h + 2);
		c++;
		addr += getScreenWidth();
	}
}

u_int getColor() {
	cc++;
	if (cc == 43)
		cc = 0;
	return colors[cc];
}

u_char getX() {
	sx++;
	if (sx == 17)
		sx = 0;
	return startx[sx];
}

u_char getY() {
	sy++;
	if (sy == 13)
		sy = 0;
	return starty[sy];
}

u_char getD() {
	d++;
	if (d == 9)
		d = 0;
	return dim[d];
}

u_char getO() {
	o++;
	if (o == 10)
		o = 0;
	return offset[o];
}

void shesGotColors(u_int times) {

	u_char x;
	u_char y;
	u_char d;
	u_int repeatTimes = times;

	while (repeatTimes > 0) {
		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillCircle(x, y, d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y + d + getO(), x + d + getO(),
				y - d - getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawCircle(x, y, d + getO());

		repeatTimes--;
	}
}
/////////////////////////////////////////
// end of test
/////////////////////////////////////////
