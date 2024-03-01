#include "Timer.h"
#include "keypad.h"
#include "stm32l476xx.h"

int main()
{
	RCC->CR |= RCC_CR_HSION;	// turn on HSI
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//wait till HSI is ready
	SetupTIM4();
	SetupKeypad();
	int press = 0;
	while(1)
	{
			press = keypadInt();
	}
}
