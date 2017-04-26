/**
 * @file        md_ili9341.c
 * @project     ILI934x_F28335_v1
 *
 * @date        25 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license     GNU GPL v3
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
#include "md_ili934x.h"
#include "md_gpio.h"
#include "md_globals.h"

/**
 * @brief  Orientation
 * @note   Used private
 */
typedef enum {
    MD_ILI934x_Landscape,
    MD_ILI934x_Portrait
} MD_ILI934x_Orientation;

/**
 * @brief  LCD options
 * @note   Used private
 */
typedef struct {
    uint16_t width;
    uint16_t height;
    MD_ILI934x_Orientation orientation; // 1 = portrait; 0 = landscape
} MD_ILI931_Options_t;

/* Pin definitions */
#define ILI934x_nRST_SET            (1)
#define ILI934x_nRST_CLR            (0)
#define ILI934x_nCS_SET             MD_GPIO_Switch  (MD_Gpio[ILI934x_nCS_PIN], GPIO_ON)
#define ILI934x_nCS_CLR             MD_GPIO_Switch  (MD_Gpio[ILI934x_nCS_PIN], GPIO_OFF)
#define ILI934x_WRx_SET             MD_GPIO_Switch  (MD_Gpio[ILI934x_WRx_PIN], GPIO_ON)
#define ILI934x_WRx_CLR             MD_GPIO_Switch  (MD_Gpio[ILI934x_WRx_PIN], GPIO_OFF)

/* Private defines */
#define ILI934x_CMD_RESET               0x01
#define ILI934x_CMD_SLEEP_OUT           0x11
#define ILI934x_CMD_GAMMA               0x26
#define ILI934x_CMD_DISPLAY_OFF         0x28
#define ILI934x_CMD_DISPLAY_ON          0x29
#define ILI934x_CMD_COLUMN_ADDR         0x2A
#define ILI934x_CMD_PAGE_ADDR           0x2B
#define ILI934x_CMD_GRAM                0x2C
#define ILI934x_CMD_MAC                 0x36
#define ILI934x_CMD_PIXEL_FORMAT        0x3A
#define ILI934x_CMD_WDB                 0x51
#define ILI934x_CMD_WCD                 0x53
#define ILI934x_CMD_RGB_INTERFACE       0xB0
#define ILI934x_CMD_FRC                 0xB1
#define ILI934x_CMD_BPC                 0xB5
#define ILI934x_CMD_DFC                 0xB6
#define ILI934x_CMD_POWER1              0xC0
#define ILI934x_CMD_POWER2              0xC1
#define ILI934x_CMD_VCOM1               0xC5
#define ILI934x_CMD_VCOM2               0xC7
#define ILI934x_CMD_POWERA              0xCB
#define ILI934x_CMD_POWERB              0xCF
#define ILI934x_CMD_PGAMMA              0xE0
#define ILI934x_CMD_NGAMMA              0xE1
#define ILI934x_CMD_DTCA                0xE8
#define ILI934x_CMD_DTCB                0xEA
#define ILI934x_CMD_POWER_SEQ           0xED
#define ILI934x_CMD_3GAMMA_EN           0xF2
#define ILI934x_CMD_INTERFACE           0xF6
#define ILI934x_CMD_PRC                 0xF7

/* Pin functions */
uint16_t ILI934x_x;
uint16_t ILI934x_y;
MD_ILI931_Options_t ILI934x_Opts;
uint16_t ILI934x_INT_CalledFromPuts = 0;

/* Private functions */
void MD_ILI934x_InitLCD(void);
void MD_ILI934x_SendData(uint16_t data);
void MD_ILI934x_SendCommand(uint8_t data);
void MD_ILI934x_Delay(volatile unsigned int delay);
void MD_ILI934x_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void MD_ILI934x_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void MD_ILI934x_Init() {
    /* Init WRX pin */
    MD_GPIO_Init(ILI934x_WRX_PORT, ILI934x_WRX_PIN, MD_GPIO_Mode_OUT, MD_GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, MD_GPIO_Speed_Medium);

    /* Init CS pin */
    MD_GPIO_Init(ILI934x_CS_PORT, ILI934x_CS_PIN, MD_GPIO_Mode_OUT, MD_GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, MD_GPIO_Speed_Medium);

    /* Init RST pin */
    MD_GPIO_Init(ILI934x_nRST_PORT, ILI934x_nRST_PIN, MD_GPIO_Mode_OUT, MD_GPIO_OType_PP, MD_GPIO_PuPd_UP, MD_GPIO_Speed_Low);

    /* CS high */
    ILI934x_CS_SET;

    /* Init SPI */
    MD_SPI_Init(ILI934x_SPI, ILI934x_SPI_PINS);

    /* Init DMA for SPI */
    MD_SPI_DMA_Init(ILI934x_SPI);

    /* Init LCD */
    MD_ILI934x_InitLCD();

    /* Set default settings */
    ILI934x_x = ILI934x_y = 0;
    ILI934x_Opts.width = ILI934x_WIDTH;
    ILI934x_Opts.height = ILI934x_HEIGHT;
    ILI934x_Opts.orientation = MD_ILI934x_Portrait;

    /* Fill with white color */
    MD_ILI934x_Fill(ILI934x_COLOR_WHITE);
}

void MD_ILI934x_InitLCD(void) {
    /* Force reset */
    ILI934x_nRST_CLR;
    MD_ILI934x_Delay(20000);
    ILI934x_nRST_SET;

    /* Delay for RST response */
    MD_ILI934x_Delay(20000);

    /* Software reset */
    MD_ILI934x_SendCommand(ILI934x_CMD_RESET);
    MD_ILI934x_Delay(50000);

    MD_ILI934x_SendCommand(ILI934x_CMD_POWERA);
    MD_ILI934x_SendData(0x39);
    MD_ILI934x_SendData(0x2C);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x34);
    MD_ILI934x_SendData(0x02);
    MD_ILI934x_SendCommand(ILI934x_CMD_POWERB);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0xC1);
    MD_ILI934x_SendData(0x30);
    MD_ILI934x_SendCommand(ILI934x_CMD_DTCA);
    MD_ILI934x_SendData(0x85);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x78);
    MD_ILI934x_SendCommand(ILI934x_CMD_DTCB);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendCommand(ILI934x_CMD_POWER_SEQ);
    MD_ILI934x_SendData(0x64);
    MD_ILI934x_SendData(0x03);
    MD_ILI934x_SendData(0x12);
    MD_ILI934x_SendData(0x81);
    MD_ILI934x_SendCommand(ILI934x_CMD_PRC);
    MD_ILI934x_SendData(0x20);
    MD_ILI934x_SendCommand(ILI934x_CMD_POWER1);
    MD_ILI934x_SendData(0x23);
    MD_ILI934x_SendCommand(ILI934x_CMD_POWER2);
    MD_ILI934x_SendData(0x10);
    MD_ILI934x_SendCommand(ILI934x_CMD_VCOM1);
    MD_ILI934x_SendData(0x3E);
    MD_ILI934x_SendData(0x28);
    MD_ILI934x_SendCommand(ILI934x_CMD_VCOM2);
    MD_ILI934x_SendData(0x86);
    MD_ILI934x_SendCommand(ILI934x_CMD_MAC);
    MD_ILI934x_SendData(0x48);
    MD_ILI934x_SendCommand(ILI934x_CMD_PIXEL_FORMAT);
    MD_ILI934x_SendData(0x55);
    MD_ILI934x_SendCommand(ILI934x_CMD_FRC);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x18);
    MD_ILI934x_SendCommand(ILI934x_CMD_DFC);
    MD_ILI934x_SendData(0x08);
    MD_ILI934x_SendData(0x82);
    MD_ILI934x_SendData(0x27);
    MD_ILI934x_SendCommand(ILI934x_CMD_3GAMMA_EN);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendCommand(ILI934x_CMD_COLUMN_ADDR);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0xEF);
    MD_ILI934x_SendCommand(ILI934x_CMD_PAGE_ADDR);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x01);
    MD_ILI934x_SendData(0x3F);
    MD_ILI934x_SendCommand(ILI934x_CMD_GAMMA);
    MD_ILI934x_SendData(0x01);
    MD_ILI934x_SendCommand(ILI934x_CMD_PGAMMA);
    MD_ILI934x_SendData(0x0F);
    MD_ILI934x_SendData(0x31);
    MD_ILI934x_SendData(0x2B);
    MD_ILI934x_SendData(0x0C);
    MD_ILI934x_SendData(0x0E);
    MD_ILI934x_SendData(0x08);
    MD_ILI934x_SendData(0x4E);
    MD_ILI934x_SendData(0xF1);
    MD_ILI934x_SendData(0x37);
    MD_ILI934x_SendData(0x07);
    MD_ILI934x_SendData(0x10);
    MD_ILI934x_SendData(0x03);
    MD_ILI934x_SendData(0x0E);
    MD_ILI934x_SendData(0x09);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendCommand(ILI934x_CMD_NGAMMA);
    MD_ILI934x_SendData(0x00);
    MD_ILI934x_SendData(0x0E);
    MD_ILI934x_SendData(0x14);
    MD_ILI934x_SendData(0x03);
    MD_ILI934x_SendData(0x11);
    MD_ILI934x_SendData(0x07);
    MD_ILI934x_SendData(0x31);
    MD_ILI934x_SendData(0xC1);
    MD_ILI934x_SendData(0x48);
    MD_ILI934x_SendData(0x08);
    MD_ILI934x_SendData(0x0F);
    MD_ILI934x_SendData(0x0C);
    MD_ILI934x_SendData(0x31);
    MD_ILI934x_SendData(0x36);
    MD_ILI934x_SendData(0x0F);
    MD_ILI934x_SendCommand(ILI934x_CMD_SLEEP_OUT);

    MD_ILI934x_Delay(1000000);

    MD_ILI934x_SendCommand(ILI934x_CMD_DISPLAY_ON);
    MD_ILI934x_SendCommand(ILI934x_CMD_GRAM);
}

void MD_ILI934x_DisplayOn(void) {
    MD_ILI934x_SendCommand(ILI934x_CMD_DISPLAY_ON);
}

void MD_ILI934x_DisplayOff(void) {
    MD_ILI934x_SendCommand(ILI934x_CMD_DISPLAY_OFF);
}

void MD_ILI934x_SendCommand(uint8_t data) {
    ILI934x_WRX_RESET;
    ILI934x_CS_RESET;
    MD_SPI_Send(ILI934x_SPI, data);
    ILI934x_CS_SET;
}

void MD_ILI934x_SendData(uint16_t data) {
    ILI934x_WRX_SET;
    ILI934x_CS_RESET;
    MD_SPI_Send(ILI934x_SPI, data);
    ILI934x_CS_SET;
}

void MD_ILI934x_DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
    MD_ILI934x_SetCursorPosition(x, y, x, y);

    MD_ILI934x_SendCommand(ILI934x_CMD_GRAM);
    MD_ILI934x_SendData(color >> 8);
    MD_ILI934x_SendData(color & 0xFF);
}


void MD_ILI934x_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    MD_ILI934x_SendCommand(ILI934x_CMD_COLUMN_ADDR);
    MD_ILI934x_SendData(x1 >> 8);
    MD_ILI934x_SendData(x1 & 0xFF);
    MD_ILI934x_SendData(x2 >> 8);
    MD_ILI934x_SendData(x2 & 0xFF);

    MD_ILI934x_SendCommand(ILI934x_CMD_PAGE_ADDR);
    MD_ILI934x_SendData(y1 >> 8);
    MD_ILI934x_SendData(y1 & 0xFF);
    MD_ILI934x_SendData(y2 >> 8);
    MD_ILI934x_SendData(y2 & 0xFF);
}

void MD_ILI934x_Fill(uint32_t color) {
    /* Fill entire screen */
    MD_ILI934x_INT_Fill(0, 0, ILI934x_Opts.width - 1, ILI934x_Opts.height, color);
}

void MD_ILI934x_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint32_t pixels_count;

    /* Set cursor position */
    MD_ILI934x_SetCursorPosition(x0, y0, x1, y1);

    /* Set command for GRAM data */
    MD_ILI934x_SendCommand(ILI934x_CMD_GRAM);

    /* Calculate pixels count */
    pixels_count = (x1 - x0 + 1) * (y1 - y0 + 1);

    /* Send everything */
    ILI934x_CS_RESET;
    ILI934x_WRX_SET;

    /* Go to 16-bit SPI mode */
    MD_SPI_SetDataSize(ILI934x_SPI, MD_SPI_DataSize_16b);

    /* Send first 65535 bytes, SPI MUST BE IN 16-bit MODE */
    MD_SPI_DMA_SendHalfWord(ILI934x_SPI, color, (pixels_count > 0xFFFF) ? 0xFFFF : pixels_count);
    /* Wait till done */
    while (MD_SPI_DMA_Working(ILI934x_SPI));

    /* Check again */
    if (pixels_count > 0xFFFF) {
        /* Send remaining data */
        MD_SPI_DMA_SendHalfWord(ILI934x_SPI, color, pixels_count - 0xFFFF);
        /* Wait till done */
        while (MD_SPI_DMA_Working(ILI934x_SPI));
    }

    ILI934x_CS_SET;

    /* Go back to 8-bit SPI mode */
    MD_SPI_SetDataSize(ILI934x_SPI, MD_SPI_DataSize_8b);
}

void MD_ILI934x_Delay(volatile unsigned int delay) {
    for (; delay != 0; delay--);
}

void MD_ILI934x_Rotate(MD_ILI934x_Orientation_t orientation) {
    MD_ILI934x_SendCommand(ILI934x_CMD_MAC);
    if (orientation == MD_ILI934x_Orientation_Portrait_1) {
        MD_ILI934x_SendData(0x58);
    } else if (orientation == MD_ILI934x_Orientation_Portrait_2) {
        MD_ILI934x_SendData(0x88);
    } else if (orientation == MD_ILI934x_Orientation_Landscape_1) {
        MD_ILI934x_SendData(0x28);
    } else if (orientation == MD_ILI934x_Orientation_Landscape_2) {
        MD_ILI934x_SendData(0xE8);
    }

    if (orientation == MD_ILI934x_Orientation_Portrait_1 || orientation == MD_ILI934x_Orientation_Portrait_2) {
        ILI934x_Opts.width = ILI934x_WIDTH;
        ILI934x_Opts.height = ILI934x_HEIGHT;
        ILI934x_Opts.orientation = MD_ILI934x_Portrait;
    } else {
        ILI934x_Opts.width = ILI934x_HEIGHT;
        ILI934x_Opts.height = ILI934x_WIDTH;
        ILI934x_Opts.orientation = MD_ILI934x_Landscape;
    }
}

void MD_ILI934x_Puts(uint16_t x, uint16_t y, char *str, MD_FontDef_t *font, uint32_t foreground, uint32_t background) {
    uint16_t startX = x;

    /* Set X and Y coordinates */
    ILI934x_x = x;
    ILI934x_y = y;

    while (*str) {
        /* New line */
        if (*str == '\n') {
            ILI934x_y += font->FontHeight + 1;
            /* if after \n is also \r, than go to the left of the screen */
            if (*(str + 1) == '\r') {
                ILI934x_x = 0;
                str++;
            } else {
                ILI934x_x = startX;
            }
            str++;
            continue;
        } else if (*str == '\r') {
            str++;
            continue;
        }

        /* Put character to LCD */
        MD_ILI934x_Putc(ILI934x_x, ILI934x_y, *str++, font, foreground, background);
    }
}

void MD_ILI934x_GetStringSize(char *str, MD_FontDef_t *font, uint16_t *width, uint16_t *height) {
    uint16_t w = 0;
    *height = font->FontHeight;
    while (*str++) {
        w += font->FontWidth;
    }
    *width = w;
}

void MD_ILI934x_Putc(uint16_t x, uint16_t y, char c, MD_FontDef_t *font, uint32_t foreground, uint32_t background) {
    uint32_t i, b, j;
    /* Set coordinates */
    ILI934x_x = x;
    ILI934x_y = y;

    if ((ILI934x_x + font->FontWidth) > ILI934x_Opts.width) {
        /* If at the end of a line of display, go to new line and set x to 0 position */
        ILI934x_y += font->FontHeight;
        ILI934x_x = 0;
    }

    /* Draw rectangle for background */
    MD_ILI934x_INT_Fill(ILI934x_x, ILI934x_y, ILI934x_x + font->FontWidth, ILI934x_y + font->FontHeight, background);

    /* Draw font data */
    for (i = 0; i < font->FontHeight; i++) {
        b = font->data[(c - 32) * font->FontHeight + i];
        for (j = 0; j < font->FontWidth; j++) {
            if ((b << j) & 0x8000) {
                MD_ILI934x_DrawPixel(ILI934x_x + j, (ILI934x_y + i), foreground);
            }
        }
    }

    /* Set new pointer */
    ILI934x_x += font->FontWidth;
}


void MD_ILI934x_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
    /* Code by dewoller: https://github.com/dewoller */

    int16_t dx, dy, sx, sy, err, e2;
    uint16_t tmp;

    /* Check for overflow */
    if (x0 >= ILI934x_Opts.width) {
        x0 = ILI934x_Opts.width - 1;
    }
    if (x1 >= ILI934x_Opts.width) {
        x1 = ILI934x_Opts.width - 1;
    }
    if (y0 >= ILI934x_Opts.height) {
        y0 = ILI934x_Opts.height - 1;
    }
    if (y1 >= ILI934x_Opts.height) {
        y1 = ILI934x_Opts.height - 1;
    }

    /* Check correction */
    if (x0 > x1) {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1) {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    /* Vertical or horizontal line */
    if (dx == 0 || dy == 0) {
        MD_ILI934x_INT_Fill(x0, y0, x1, y1, color);
        return;
    }

    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = ((dx > dy) ? dx : -dy) / 2;

    while (1) {
        MD_ILI934x_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

void MD_ILI934x_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
    MD_ILI934x_DrawLine(x0, y0, x1, y0, color); //Top
    MD_ILI934x_DrawLine(x0, y0, x0, y1, color); //Left
    MD_ILI934x_DrawLine(x1, y0, x1, y1, color); //Right
    MD_ILI934x_DrawLine(x0, y1, x1, y1, color); //Bottom
}

void MD_ILI934x_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
    uint16_t tmp;

    /* Check correction */
    if (x0 > x1) {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1) {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    /* Fill rectangle */
    MD_ILI934x_INT_Fill(x0, y0, x1, y1, color);

    /* CS HIGH back */
    ILI934x_CS_SET;
}

void MD_ILI934x_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    MD_ILI934x_DrawPixel(x0, y0 + r, color);
    MD_ILI934x_DrawPixel(x0, y0 - r, color);
    MD_ILI934x_DrawPixel(x0 + r, y0, color);
    MD_ILI934x_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        MD_ILI934x_DrawPixel(x0 + x, y0 + y, color);
        MD_ILI934x_DrawPixel(x0 - x, y0 + y, color);
        MD_ILI934x_DrawPixel(x0 + x, y0 - y, color);
        MD_ILI934x_DrawPixel(x0 - x, y0 - y, color);

        MD_ILI934x_DrawPixel(x0 + y, y0 + x, color);
        MD_ILI934x_DrawPixel(x0 - y, y0 + x, color);
        MD_ILI934x_DrawPixel(x0 + y, y0 - x, color);
        MD_ILI934x_DrawPixel(x0 - y, y0 - x, color);
    }
}

void MD_ILI934x_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    MD_ILI934x_DrawPixel(x0, y0 + r, color);
    MD_ILI934x_DrawPixel(x0, y0 - r, color);
    MD_ILI934x_DrawPixel(x0 + r, y0, color);
    MD_ILI934x_DrawPixel(x0 - r, y0, color);
    MD_ILI934x_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        MD_ILI934x_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        MD_ILI934x_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        MD_ILI934x_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        MD_ILI934x_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}
