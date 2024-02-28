#include "stm32l476xx.h"
#include "flash.h"
#include <stdio.h>

static uint16_t clubs[12];

int main()
{
	RCC->CR |= RCC_CR_HSION;            // enable HSI (internal 16 MHz clock)
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);
  RCC->CFGR |= RCC_CFGR_SW_HSI;    // make HSI the system clock
	SystemCoreClockUpdate();
	
	clubs[0] = 0xFFFF;
	clubs[1] = 0xFFFF;
	clubs[2] = 230;
	clubs[3] = 205;
	clubs[4] = 190;
	clubs[5] = 175;
	clubs[6] = 160;
	clubs[7] = 150;
	clubs[8] = 130;
	clubs[9] = 100;
	clubs[10] = 70;
	clubs[11] = 55;
	writeClubs(clubs);
	
	while(1);
}
