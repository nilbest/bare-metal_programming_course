#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include"systick.h"
#include"tim.h"

int timestamp = 0;
int timestamp_in_sec = 0;

//Set up: Connect a jumper wire from PA5 to PA6
int main(void)
{

	tim2_pa5_output_compare();	//Now this function toggle the LED PIN. There is no need to use the Output Data Register!!!
	tim3_pa6_input_capture();	//Enable Capture
	while(1)
	{
		//Wait until edge is captured
		while(!(TIM3->SR & SR_CC1IF)){}
		
		//Read captured Value
		timestamp = TIM3->CCR1;
		timestamp_in_sec = timestamp/1000;
	}
}



