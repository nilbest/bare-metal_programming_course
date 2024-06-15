#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include"systick.h"
#include"tim.h"



int main(void)
{

	tim2_pa5_output_compare(); //Now this function toggle the LED PIN. There is no need to use the Output Data Register!!!

	while(1)
	{
		
	}
}



