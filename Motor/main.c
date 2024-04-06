#include "stm32l476xx.h"
#include "Motor.h"

static uint32_t pos;

void EXTI_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1; 
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;

	EXTI->RTSR1 |= EXTI_RTSR1_RT1;

	EXTI->IMR1 |= EXTI_IMR1_IM1;
	NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTI1_IRQHandler(void){
	if((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1){
		pos = pos + 1;
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
}

int main()
{
	pos = 0;
	EXTI_Init();
	prvMotorGPIO_Setup();
	while(1)
	{
		if(pos >= 528) // 48 * 11? I'm not sure why
		{
			GPIOC->ODR &= 0xFFFFFFF7;
		}
		else
		{
			GPIOC->ODR |= 0x00000008;
		}
	}
	return 0;
}