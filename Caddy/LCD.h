#ifndef LCD_H
#define LCD_H
#include "stm32l476xx.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_DisplayString(uint32_t line, char *str, unsigned int length);
void putNibble(uint8_t c);
void pulse(void);
void LCD_WriteCom(uint8_t com);
void LCD_WriteData(uint8_t dat);

#endif