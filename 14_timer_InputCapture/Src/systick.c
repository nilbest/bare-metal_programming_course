/*
 * systick.c
 *
 *  Created on: Jun 15, 2024
 *      Author: nilsb
 */
#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL				16000
//For getting 1ms (0.001s) delay via
//math: delay=N*(1/SYSCLK) SYSCLK for this chip is 16 000 000

#define CTRL_ENABLE						(1U<<0)
#define CTRL_CLKSRC						(1U<<2)
#define CTRL_COUNTFLAG					(1U<<16)



void systickDelayMs(int delay)
{
	//Configure systick
	//Reload with number of clocks per millisecond
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	//Clear systick current vale register
	SysTick->VAL = 0;

	//Enable systick and select internal clk src
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i=0; i<delay; i++)
	{
		//Wait until the countflag is set
		while((SysTick->CTRL & CTRL_COUNTFLAG)==0){} //Waits
	}
	SysTick->CTRL = 0;

}

