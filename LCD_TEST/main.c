#include "stm32l476xx.h"
#include "LCD.h"
#include "timer.h"

int main(void){
	RCC->CR |= RCC_CR_HSION;	// turn on HSI
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//wait till HSI is ready
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;	//enable clock on GPIOB and GPIOC
	SetupTIM4();
	LCD_Init();
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char *)"Latitude: \0");
	LCD_DisplayString(1, (unsigned char *)"Longitude: \0");
	LCD_DisplayString(2, (unsigned char*)"Altitude: \0");
	LCD_DisplayString(3, (unsigned char*)"Time: \0");
	while(1);
}