//
// SSD1306 128x64 OLED library
//
// Required Libraries
// - I2C library from https://github.com/wagiminator/CH552-USB-OLED
//
// References
// - https://github.com/wagiminator/CH552-USB-OLED
//
// Apr 2023 by Li Mingjie
// - Email:  limingjie@outlook.com
// - GitHub: https://github.com/limingjie/
//

#pragma once
#include <stdint.h>

#define FONT_5x8           0
#define FONT_DOS_8x16      1
#define FONT_CHINESE_16x16 2

void OLED_init(void);
void OLED_clear(void);
void OLED_setFont(uint8_t font);
void OLED_setCursor(uint8_t row, uint8_t col);
void OLED_write(char c);
void OLED_print(const char* str);
