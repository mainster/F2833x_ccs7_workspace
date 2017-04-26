/*
 * config.h
 *
 *  Created on: Jul 12, 2013
 *      Author: RobG
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define ILI9340 // 2.2" 320x240

//
#define HARDWARE_SPI
#define ORIENTATION 0 // 0 90 180 270

//#define F5172_ROBG
//#define F5529_TI
//#define F5510_ROBG
#define G2553_TI
//#define G2955_ROBG
//#define FR5969_TI

// F5510_ROBG uses UCB1
#define UCBxTXBUF UCB0TXBUF
#define UCBxRXBUF UCB0RXBUF
#define UCBxSTAT UCB0STAT
#define UCBxBR0 UCB0BR0

#if defined F5529_TI

#define LCD_SCLK_PIN BIT2
#define LCD_SCLK_PORT P3
#define LCD_MOSI_PIN BIT0
#define LCD_MOSI_PORT P3
#define LCD_MISO_PIN BIT1
#define LCD_MISO_PORT P3
#define LCD_CS_PIN BIT5
#define LCD_CS_PORT P6
#define LCD_DC_PIN BIT6
#define LCD_DC_PORT P6

#elif defined FR5969_TI

#define LCD_SCLK_PIN BIT2
#define LCD_SCLK_PORT P2
#define LCD_MOSI_PIN BIT6
#define LCD_MOSI_PORT P1
#define LCD_MISO_PIN BIT7
#define LCD_MISO_PORT P1
#define LCD_CS_PIN BIT2
#define LCD_CS_PORT P4
#define LCD_DC_PIN BIT4
#define LCD_DC_PORT P2

#elif defined F5510_ROBG

#define LCD_SCLK_PIN BIT3
#define LCD_SCLK_PORT P4
#define LCD_MOSI_PIN BIT1
#define LCD_MOSI_PORT P4
#define LCD_MISO_PIN BIT2
#define LCD_MISO_PORT P4
#define LCD_CS_PIN BIT0
#define LCD_CS_PORT P6
#define LCD_DC_PIN BIT0
#define LCD_DC_PORT P4

// F5510_ROBG uses UCB1
#undef UCBxTXBUF
#undef UCBxRXBUF
#undef UCBxSTAT
#undef UCBxBR0
#define UCBxTXBUF UCB1TXBUF
#define UCBxTXBUF UCB1RXBUF
#define UCBxSTAT UCB1STAT
#define UCBxBR0 UCB1BR0

#elif defined G2955_ROBG

#define LCD_SCLK_PIN BIT3
#define LCD_SCLK_PORT P3
#define LCD_MOSI_PIN BIT1
#define LCD_MOSI_PORT P3
#define LCD_MISO_PIN BIT2
#define LCD_MISO_PORT P3
#define LCD_CS_PIN BIT0
#define LCD_CS_PORT P2
#define LCD_DC_PIN BIT0
#define LCD_DC_PORT P3

#else
// use default for F5172_ROBG and G2553_TI
#endif

#endif /* CONFIG_H_ */
