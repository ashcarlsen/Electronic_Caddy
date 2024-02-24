#include "timer.h"
#include "stm32l476xx.h"

static int count;

void SetupTIM4(void)
{
	count = 0;
	// SETUP TIM4 to count to 1ms
	// Enable Timer 4 peripheral clock
	RCC->APB1ENR1 |= 1UL << 2;
	// Set TIM4 to upcounting
	TIM4->CR1 &= 0xFFEF;
	// Enable update interrupts 
	TIM4->DIER |= TIM_DIER_UIE; 
	TIM4->SR &= 0xFFFFFE; 
	TIM4->PSC = 39;
	TIM4->ARR = 499;
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
	count = count + 1;
	TIM4->SR &= ~TIM_SR_UIF;
}

void delay_ms(int ms)
{
	// Enable the timer to start counting.
	TIM4->CR1 |= 1;
	while(count <= ms);
	count = 0;
}