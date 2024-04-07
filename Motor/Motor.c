#include "stm32l476xx.h"
#include "Motor.h"

void prvMotorGPIO_Setup(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;	//enable clock on GPIOB and GPIOC
	GPIOC->MODER &= ~GPIO_MODER_MODE1;	//set C pins 1,2 to input modes
	GPIOC->MODER &= ~GPIO_MODER_MODE2;
	GPIOC->MODER &= ~GPIO_MODER_MODE3;
	GPIOC->MODER |= GPIO_MODER_MODE3_0; // set pin 3 to output
	GPIOC->MODER &= ~GPIO_MODER_MODE0;
	GPIOC->MODER |= GPIO_MODER_MODE0_0; // set pin 0 to output
}