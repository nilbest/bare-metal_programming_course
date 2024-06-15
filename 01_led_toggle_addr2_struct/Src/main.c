//Where is the led connected?
//Port:	A
//Pin:	5

#include <stdint.h> //For including Int types

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

#define GPIOAEN					(1U<<0) // Shifts 1 at position 0 => 0b 0000 0000 0000 0000 0000 0000 0000 0001

#define PIN5					(1U<<5)
#define LED_PIN_A				PIN5


#define __IO volatile

//unint32_t because 32Bit registers
typedef struct
{
  volatile uint32_t DUMMY[12];		//Dummy for all 12 Registers before AHB1ENR
  volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
} RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
	volatile uint32_t DUMMY[4]; //Palceholder for each Element between MODER and ODR
	volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
} GPIO_TypeDef;

//make RCC and GPIO Pointer to the Adresses
#define RCC		((RCC_TypeDef*) RCC_BASE)
#define GPIO	((GPIO_TypeDef*) GPIOA_BASE)

int main (void)
{
	//1. Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN; // |= Or operator changes only bit set at GPIOAEN to 1 =>friendly programming
	//2. Set PA5 as output pin
	GPIO->MODER |= (1U<<10);	// Set bit 10 to 1
	GPIO->MODER &=~(1U<<11);	// Set bit 11 to 0  &and operation with current register ~negate

	while(1)
	{
		//4. Experiment 2 : toggle PA5
		GPIO->ODR ^= LED_PIN_A;
		for(int i=0;i<100000;i++){}
	}
}

