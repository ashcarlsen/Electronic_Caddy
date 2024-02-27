#include "stm32l476xx.h"
#include "flash.h"
#include <stdio.h>

int main()
{
	RCC->CR |= RCC_CR_HSION;            // enable HSI (internal 16 MHz clock)
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);
  RCC->CFGR |= RCC_CFGR_SW_HSI;    // make HSI the system clock
	SystemCoreClockUpdate();
	unlock_flash();
	flash_erase_page(1, 255);
	lock_flash();
	uint64_t eight = 160;
	uint64_t clubs = 0x0000000000000044;
	flash_program_double_word(0x0807F810, clubs);
	eight = (*(volatile uint64_t *)(0x0807F810));
	while(1);
}
