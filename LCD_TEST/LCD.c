/*======================================================
LCD GPIO Pins
GPIOB: LCD
	0: Data D4
	1: Data D5
	2: Data D6
	3: Data D7
GPIOC: LCD
	10: E
	11: RW
	12: RS
=======================================================*/
#include "Lcd.h"
#include "stm32l476xx.h"
#include "Timer.h"

void putNibble(unsigned char c) {
	GPIOB->ODR &= 0xFFFFFFF0; //0 out last character 
	GPIOB->ODR |= c;	// 0000 c
}

void pulse() {
	GPIOC->ODR |= 1<<10;
	delay_ms(1);
	GPIOC->ODR &= (unsigned int) ~(1<<10);
	delay_ms(1);
}


void LCD_WriteCom(unsigned char com) {
	unsigned char c;
	GPIOC->ODR &= (unsigned int)~(1<<12); //Set RS to 0
	c = com >> 4;
	putNibble(c); //put first nibble into D4-D7 on LCD
 	pulse();	//enable then disable E; Latch D4-D7 on LCD
	c = com & 0xF;
	putNibble(c); // put second nibble into D4-D7 on LCD
	pulse();
	
	GPIOC->ODR |= 1<<12; //Default RS to 1
}

void LCD_WriteData(unsigned char dat) {
	
	putNibble(dat >> 4); //put first nibble into D4-D7 on LCD
	pulse();	//enable then disable E; Latch D4-D7 on LCD
	
	putNibble(dat & 0x0F); // put second nibble into D4-D7 on LCD
	pulse();
}

void LCD_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;	//enable clock on GPIOB and GPIOC
	GPIOB->MODER &= 0xFFFFFF00;		//set pins 0-3 to 0 on GPIOB
	GPIOB->MODER |= 0x00000055;		//set pins 0-3 to output on GPIOB
	GPIOC->MODER &= 0xFC0FFFFF;     //set pins 10-12 to 0 on GPIOC
	GPIOC->MODER |= 0x01500000;		//set pins 10, 11, and 12 to output on GPIOC
	GPIOC->ODR &= 0xFFFFE3FF;  //0 output on GPIOC 10-12
	GPIOB->ODR &= 0xFFFFFFF0;  //0 output on GPIOB 0-3;
	delay_ms(30);
	LCD_WriteCom(0x2);	//return home
	delay_ms(2);
	LCD_WriteCom(0x28);	//set to 4-bit mode 2 lines and 5x8 font
	delay_ms(2);
	LCD_WriteCom(0x0c);	//turn off cursor and blink display on
	delay_ms(2);
	LCD_WriteCom(0x06);	//clear
	delay_ms(2);
	LCD_WriteCom(0x01);	//set to entry mode
	delay_ms(2);
	LCD_WriteCom(0x01);
	delay_ms(2);
}

void LCD_Clear(void){
	GPIOB->ODR &= 0xFFFFFFF0;
  LCD_WriteCom(0x01);		//clear
	delay_ms(2);
	LCD_WriteCom(0x01);
	delay_ms(2);
}

void LCD_DisplayString(unsigned int line, unsigned char *ptr) {
	unsigned char lineNum = 0x80;			// 1000 0000 is base command to write to 0x00. Need the 1 in that bit spot
	switch(line)
	{
		case 0: 
			{
				lineNum += 0x00;
				break;
			}
		case 1:
			{
				lineNum += 0x40;
				break;
			}
		case 2:
			{
				lineNum += 0x14;
				break;
			}
		case 3:
		{
			lineNum += 0x54;
			break;
		}
		default: break;
	}
	LCD_WriteCom(lineNum);					//send the command to move where the display memory is 
	delay_ms(2);
	int i;
	for(i = 0; ptr[i]!= '\0'; i++){	//for each character, write the data
		LCD_WriteData(ptr[i]);
		delay_ms(1);
	}
}
