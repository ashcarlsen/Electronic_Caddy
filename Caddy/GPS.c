#include "GPS.h"
#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>

/*============================
*	Pins:
*	C5: USART3 Rx
*============================*/

void GPS_Read_NMEA(char *buffer, uint32_t size)
	{
	USART3->CR1 |= USART_CR1_UE; //enable USART
	while((USART3->ISR & USART_ISR_REACK) == 0); //recieve enable flag
	unsigned int i = 0;
	char done = 0;
	char valid = 0;
	for(unsigned int k = 0; k < size; k++)	// Initialize buffer to nulls
	{
		buffer[k] = '\0';
	}
	for(int i = 0; i < size; i++)	//Read into the entire buffer
	{
		while(!(USART3->ISR & USART_ISR_RXNE));
		buffer[i] = USART3->RDR;
		if(buffer[i] == '$')
		{
			valid = 1;
		}
		else if(valid == 0)
		{
			i = 0;
			valid = 0;
		}
		else
		{
			if (strstr(buffer, "GPGGA") != NULL && buffer[i] == '\n')	// If we get enough data just stop reading
			{
				for(int j = i+1; j < size; j++)
				{
					buffer[j] = '\0';
				}
				i = size;
			}
		}
	}
	USART3->CR1 &= ~USART_CR1_UE; //disable USART
}

void GPS_Init(void){
	GPS_GPIO_Init();
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN; //Enable UART Clock
	USART3->CR1 &= ~USART_CR1_UE; //disable USART
	USART3->CR1 &= ~(USART_CR1_M); //set to 8-bit word, 1 start, n stop bits
	USART3->CR2 &= ~(USART_CR2_STOP); //set to 1 stop bit
	USART3->CR1 &= ~(USART_CR1_PCE); //disable parity control bits
	USART3->CR1 &= ~(USART_CR1_OVER8); //clear over8 bit to 16 oversampling
	USART3->BRR = 0x682; //9600 baud
	USART3->CR1 |= (USART_CR1_RE); //enable transmit and receive
}

void GPS_GPIO_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;	//enable clock on GPIOA
	GPIOC->MODER &= ~GPIO_MODER_MODE5;
	GPIOC->MODER |= GPIO_MODER_MODE5_1; //sets pin C5 to analog function
	GPIOC->AFR[0] |= 0x77 << (4*4);		// Sets pinC4-5 as analog if we want TX too
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD5_0); //Set as Pull-up
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5); //Set as high speed
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5);	// Push-pull output type
}