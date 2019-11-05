/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* Code ------------------------------------------------------------------*/

void GPIO_Config(void){
	// Clock Enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //GPIO A
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; //GPIO B
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //GPIO C


	/* Mode Config
	 * 00 = Input 		GPIO<port>->MODER &= ~(GPIO_MODER_MODER<pin>_Msk);
	 * 01 = Output		GPIO<port>->MODER |= GPIO_MODER_MODER<pin>_0;
	 * 10 = Alternate	GPIO<port>->MODER |= GPIO_MODER_MODER<pin>_1;
	 * 11 = Analog		GPIO<port>->MODER |= GPIO_MODER_MODER<pin>_Msk;
	 */

	//Outputs

	// Spalten Outputs
	GPIOA->MODER |= GPIO_MODER_MODER4_0;	//1
	GPIOA->MODER |= GPIO_MODER_MODER5_0;	//2
	GPIOA->MODER |= GPIO_MODER_MODER6_0;	//3
	GPIOA->MODER |= GPIO_MODER_MODER7_0;	//4
	GPIOA->MODER |= GPIO_MODER_MODER8_0;	//5
	GPIOA->MODER |= GPIO_MODER_MODER9_0;	//6
	GPIOA->MODER |= GPIO_MODER_MODER10_0;	//7
	GPIOA->MODER |= GPIO_MODER_MODER11_0;	//8

	//Zeilen Outputs
	GPIOC->MODER |= GPIO_MODER_MODER0_0;	//1
	GPIOC->MODER |= GPIO_MODER_MODER1_0;	//2
	GPIOC->MODER |= GPIO_MODER_MODER2_0;	//3
	GPIOC->MODER |= GPIO_MODER_MODER3_0;	//4
	GPIOC->MODER |= GPIO_MODER_MODER4_0;	//5
	GPIOC->MODER |= GPIO_MODER_MODER5_0;	//6
	GPIOC->MODER |= GPIO_MODER_MODER6_0;	//7
	GPIOC->MODER |= GPIO_MODER_MODER7_0;	//8

	//Inputs
	GPIOA->MODER &= ~(GPIO_MODER_MODER1_Msk);	//ADC1_1 	x-Achse (PA1)
	GPIOB->MODER &= ~(GPIO_MODER_MODER1_Msk);	//ADC3_1 	y-Achse (PB1)

	/* Ouput Type
	 * 0 = push/pull	GPIO<port>->OTYPER &= ~(GPIO_OTYPER_OT_<pin>);
	 * 1 = open drain	GPIO<port>->OTYPER |= GPIO_OTYPER_OT_<pin>;
	 */


	/* Output Speed
	 * x0 = Low			GPIO<port>->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR<pin>_Msk);
	 * 01 = Medium		GPIO<port>->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR<pin>_0);
	 * 11 = High		GPIO<port>->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR<pin>_Msk;
	 */

	/* Port pull-up/down
	 * 00 = No PU/PD	GPIO<port>->PUPDR &= ~(GPIO_PUPDR_PUPDR<pin>_Msk);
	 * 01 = PU			GPIO<port>->PUPDR |= GPIO_PUPDR_PUPDR<pin>_0;
	 * 10 = PD			GPIO<port>->PUPDR |= GPIO_PUPDR_PUPDR<pin>_1;
	 * 11 = reserved	GPIO<port>->PUPDR |= GPIO_PUPDR_PUPDR<pin>_Msk;
	 */



	/* Reset Pins to Default
	 * Port Reset		GPIO<port>->ODR &= ~(0xFFFF)
	 * Port Set			GPIO<port>->ODR |= 0xFFFF
	 * Pin Reset		GPIOA->ODR &= ~(GPIO_ODR_<pin>);
	 * Pin Set			GPIOA->ODR |= GPIO_ODR_<pin>;
	 */
	GPIOA->ODR &= ~(0xFF<<4);	//Set PA4-PA11 = 0 	Spalten
	GPIOC->ODR |= 0xFF;			//Set PC0-PC7 =1	Zeilen
}
