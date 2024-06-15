#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include"systick.h"
#include"tim.h"

#define GPIOAEN					(1U<<0)
#define PIN5					(1U<<5)
#define LED						(PIN5)

int main(void)
{

	//Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//Set PA5 as output pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	uart2_tx_init();
	tim2_1hz_init();

	while(1)
	{
		//Wait for UIF
		while(!(TIM2->SR & SR_UIF)){}  
		//(TIM2->SR & SR_UIF) compares if SR is set to SR_UIF
		
		//Clear UIF Flag
		TIM2->SR &=~SR_UIF;  //sets SR to not SR_UIF
		printf("A Second has passed!\n\r");
		GPIOA->ODR ^= LED; 		//Toggles LED
	}
}



