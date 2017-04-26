/*
 * msp.h
 *
 *  Created on: Mar 19, 2012
 *      Author: RobG
 */

#ifndef MSP_H_
#define MSP_H_

#include "typedefs.h"
#include "config.h"
//
//
void initMSP430();
void setVcoreLevel(unsigned int level);
void setUpClock();
void portMapping();
//
void writeData(u_char data);
void writeCommand(u_char command);
void readDataBegin(u_char command);
u_char readData();
void readDataEnd();
u_int readID();

void delay(u_char x10ms);
//
#define COMMAND 0
#define DATA 1
//
#define portOut(p) portO(p)
#define portO(p) p ## OUT
#define portDir(p) portD(p)
#define portD(p) p ## DIR
#define portIn(p) portI(p)
#define portI(p) p ## IN
#define portSel(p) portS(p)
#if defined FR5969_TI
#define portS(p) p ## SEL1
#else
#define portS(p) p ## SEL
#endif
#if defined G2553_TI
#define portSel2(p) portS2(p)
#define portS2(p) p ## SEL2
#endif
//
// pins (USCI B by default, P1.0 Screen select, P1.4 D/C)
// clock
#ifndef LCD_SCLK_PIN
#define LCD_SCLK_PIN BIT5
#define LCD_SCLK_PORT P1
#endif
#define LCD_SCLK_OUT portOut(LCD_SCLK_PORT)
#define LCD_SCLK_DIR portDir(LCD_SCLK_PORT)
//
#define LCD_CLOCK LCD_SCLK_OUT |= LCD_SCLK_PIN; LCD_SCLK_OUT &= ~LCD_SCLK_PIN
#define LCD_SCLK_LO LCD_SCLK_OUT &= ~LCD_SCLK_PIN
#define LCD_SCLK_HI LCD_SCLK_OUT |= LCD_SCLK_PIN
// data out
#ifndef LCD_MOSI_PIN
#define LCD_MOSI_PIN BIT7
#define LCD_MOSI_PORT P1
#endif
#define LCD_MOSI_OUT portOut(LCD_MOSI_PORT)
#define LCD_MOSI_DIR portDir(LCD_MOSI_PORT)
//
#define LCD_MOSI_HI LCD_MOSI_OUT |= LCD_MOSI_PIN
#define LCD_MOSI_LO LCD_MOSI_OUT &= ~LCD_MOSI_PIN
#define LCD_MOSI_DIR_O LCD_MOSI_DIR |= LCD_MOSI_PIN
#define LCD_MOSI_DIR_I LCD_MOSI_DIR &= ~LCD_MOSI_PIN
// data in
#ifndef LCD_MISO_PIN
#define LCD_MISO_PIN BIT6
#define LCD_MISO_PORT P1
#endif
#define LCD_MISO portOut(LCD_MISO_PORT)
#define LCD_MISO_DIR portDir(LCD_MISO_PORT)
#define LCD_MISO_IN portIn(LCD_MISO_PORT)
//
#define LCD_MISO_READ LCD_MISO_IN & LCD_MISO_PIN
// chip select
#ifndef LCD_CS_PIN
#define LCD_CS_PIN BIT0 //5
#define LCD_CS_PORT P1 //2
#endif
#define LCD_CS_OUT portOut(LCD_CS_PORT)
#define LCD_CS_DIR portDir(LCD_CS_PORT)
//
#define LCD_SELECT LCD_CS_OUT &= ~LCD_CS_PIN
#define LCD_DESELECT LCD_CS_OUT |= LCD_CS_PIN
// data/control
#ifndef LCD_DC_PIN
#define LCD_DC_PIN BIT4 //3
#define LCD_DC_PORT P1
#endif
#define LCD_DC_OUT portOut(LCD_DC_PORT)
#define LCD_DC_DIR portDir(LCD_DC_PORT)
//
#define LCD_DC_LO LCD_DC_OUT &= ~LCD_DC_PIN
#define LCD_DC_HI LCD_DC_OUT |= LCD_DC_PIN
//
// hardware SPI
#define LCD_SCLK_SEL portSel(LCD_SCLK_PORT)
#if defined FR5969_TI
#define	LCD_IO_SEL portSel(LCD_MOSI_PORT)
#endif
#if defined G2955_ROBG || defined G2553_TI
#define LCD_SCLK_SEL2 portSel2(LCD_SCLK_PORT)
#endif
//

//
#endif /* MSP_H_ */

