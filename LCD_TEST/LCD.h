#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H

void putNibble(unsigned char c);
void pulse(void);
void LCD_WriteCom(unsigned char com);
void LCD_WriteData(unsigned char dat);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_DisplayString(unsigned int line, unsigned char *ptr);

#endif