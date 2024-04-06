#include "Timer.h"
#include "stm32l476xx.h"

void SetupTIM1(void)
{
// Enable TIMER 1 clock 
RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; 
// Counting direction: e = Up-counting, 1 =Down-counting 
TIM1->CR1 &= ~TIM_CR1_DIR; // Select up-counting 
// PrescaLer, slow down the input clock by a factor of (1 + prescaler) 
TIM1->PSC = 39; // 16 MHz / (1 + 39) = 40,000 KHz
// Auto-reload 
TIM1->ARR = 39; // 1 KHz period for PWM 
// Clear output compare mode bits for channel 1 
TIM1->CCMR1 &= ~TIM_CCMR1_OC1M; 
// Select PWM Mode 1 output on channel 1 (OC1M = 110) 
TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; 
// Output 1 preload enable 
TIM1->CCMR1 |= TIM_CCMR1_OC1PE; 
// Select output polarity: 0 = Active high, 1 = Active Low 
TIM1->CCER &= ~TIM_CCER_CC1NP; // OC1N = OCREF + CC1NP 
// Enable complementary output of channel 1 (CH1N) 
TIM1->CCER |= TIM_CCER_CC1NE; 
// Main output enable (MOE): 0 = Disable, 1 Enable 
TIM1->BDTR |= TIM_BDTR_MOE; 
// Output Compare Register for channel 1 
TIM1->CCR1 = 20; // Initial duty cycle 50% 
// Enable counter 
TIM1->CR1 |= TIM_CR1_CEN; 
}

void initTIM1_GPIO(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	//enable clock on GPIOE
	GPIOA->MODER &= ~GPIO_MODER_MODE8;
	GPIOA->MODER |= GPIO_MODER_MODE8_1;
	GPIOA->AFR[1] |= 0x01; // Set pin A8 to alternate function 1
}
