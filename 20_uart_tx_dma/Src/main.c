#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"
#include "uart.h"


#define GPIOAEN					(1U<<0) // Shifts 1 at position 0 => 0b 0000 0000 0000 0000 0000 0000 0000 0001
#define GPIOA_5					(1U<<5)
#define LED_PIN					GPIOA_5



static void dma_callback(void);

int main(void)
{
	//Source buffer as an array
	char message[31] = "Hello from Stm32 DMA transfer\n\r";


	//1. Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN; // |= Or operator changes only bit set at GPIOAEN to 1 =>friendly programming
	//2. Set PA5 as output pin
	GPIOA->MODER |= (1U<<10);	// Set bit 10 to 1
	GPIOA->MODER &=~(1U<<11);	// Set bit 11 to 0  &and operation with current register ~negate

	uart2_tx_init();
	dma1_stream6_init((uint32_t) message, (uint32_t) &USART2->DR, 31);

	while(1)
	{

	}

}

static void dma_callback(void)
{
	GPIOA->ODR |= LED_PIN;
}

void DMA1_Stream6_IRQHandler(void)
{
	//Check for transfer complete interrupt
	if(DMA1->HISR & HISR_TCIF6)
	{
		//Clear Flag
		DMA1->HIFCR |= HIFCR_CTCIF6;
		//Do Something
		dma_callback();
	}
}






