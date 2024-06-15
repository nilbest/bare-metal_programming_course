//Where is the led connected?
//Port:	A
//Pin:	5


#define PERIPH_BASE				(0x40000000UL) //UL => Hexadecimal
#define AHB1PERIPH_OFFSET		(0x00020000UL)
#define AHB1PERIPH_BASE			(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET			(0x0000UL)	//0x0000 0000 or 0x0 all the same

#define GPIOA_BASE				(AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET				(0x3800UL)
#define RCC_BASE				(AHB1PERIPH_BASE + RCC_OFFSET)

#define RCC_AHB1EN_R_OFFSET		(0x30UL)
#define RCC_AHB1EN_R			(*(volatile unsigned int *)(RCC_BASE + RCC_AHB1EN_R_OFFSET))
//(*(volatile unsigned int *) => tipecast and dereferenc it via *

#define MODE_R_OFFSET			(0x00UL)
#define GPIOA_MODE_R			(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET				(0x14UL)
#define GPIOA_OD_R				(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET)) //Output Data Register

#define GPIOAEN					(1U<<0) // Shifts 1 at position 0 => 0b 0000 0000 0000 0000 0000 0000 0000 0001

#define PIN5					(1U<<5)
#define LED_PIN_A				PIN5



int main (void)
{
	//1. Enable clock access to GPIOA
	RCC_AHB1EN_R |= GPIOAEN;	// |= Or operator changes only bit set at GPIOAEN to 1 =>friendly programming
	//2. Set PA5 as output pin
	GPIOA_MODE_R |= (1U<<10); 	// Set bit 10 to 1
	GPIOA_MODE_R &=~(1U<<11);	// Set bit 11 to 0  &and operation with current register ~negate

	while(1)
	{
		//3. Set PA5 high
		//GPIOA_OD_R |= LED_PIN_A;
		//4. Experiment 2 : toggle PA5
		GPIOA_OD_R ^= LED_PIN_A;
		for(int i=0;i<100000;i++){}
	}
}

