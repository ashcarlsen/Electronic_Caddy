#include "Timer.h"
#include "stm32l476xx.h"

static int s_count;

void SetupTIM4(void)
{
	s_count = 0;
	// SETUP TIM4 to count to 1ms
	// Enable Timer 4 peripheral clock
	RCC->APB1ENR1 |= 1UL << 2;
	// Set TIM4 to upcounting
	TIM4->CR1 &= 0xFFEF;
	// Enable update interrupts 
	TIM4->DIER |= TIM_DIER_UIE; 
	TIM4->SR &= 0xFFFFFE; 
	TIM4->PSC = 39;
	TIM4->ARR = 519;
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
	s_count = s_count + 1;
	TIM4->SR &= ~TIM_SR_UIF;
}

void delay_ms(int ms)
{
	s_count = 0;
	// Enable the timer to start counting.
	TIM4->CR1 |= 1;
	while(s_count <= ms);
	// Disable the timer to stop counting
	TIM4->CR1 &= 0xFFFFFFFE;
	s_count = 0;
}