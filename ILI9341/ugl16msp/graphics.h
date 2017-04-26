/*
 * graphics.h
 *
 *  Created on: Mar 19, 2012
 *      Author: RobG
 */
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "typedefs.h"
//
// font sizes
#define FONT_SM		0
#define FONT_MD		1
#define FONT_LG		2
#define FONT_SM_BKG	3
#define FONT_MD_BKG	4
#define FONT_LG_BKG	5
//
void setColor(u_int color);
void setBackgroundColor(u_int color);
//
void clearScreen(u_char blackWhite);
void drawPixel(u_int x, u_int y);
void drawString(u_int x, u_int y, char type, char *string);
void drawCharLg(u_int x, u_int y, char c);
void drawCharMd(u_int x, u_int y, char c);
void drawCharSm(u_int x, u_int y, char c);
void drawCharLgBkg(u_int x, u_int y, char c);
void drawCharMdBkg(u_int x, u_int y, char c);
void drawCharSmBkg(u_int x, u_int y, char c);
void drawLine(u_int xStart, u_int yStart, u_int xEnd, u_int yEnd);
void drawRect(u_int xStart, u_int yStart, u_int xEnd, u_int yEnd);
void drawCircle(u_int x, u_int y, u_char radius);
//
void fillRect(u_int xStart, u_int yStart, u_int xEnd, u_int yEnd);
void fillCircle(u_int x, u_int y, u_char radius);
//
void drawLogicLine(u_int x, u_int y, u_int length, u_char height, u_char * data);
//
void drawImage(u_int x, u_int y, u_int w, u_int h, u_int * data);
void drawImageLut(u_int x, u_int y, u_int w, u_int h, u_char * data, u_int * lut);
void drawImageMono(u_int x, u_int y, u_int w, u_int h, u_char * data);
//
u_int getPixel(u_int x, u_int y);
void getPixels(u_int xStart, u_int yStart, u_int xEnd, u_int yEnd, u_int * data);
//
#endif /* GRAPHICS_H_ */

