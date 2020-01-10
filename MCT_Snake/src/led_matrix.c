/*
 * led_matrix.c
 *
 *  Created on: 10.01.2020
 *      Author: lukas
 */

#include "led_matrix.h"

/*************************************************************************
Function: void LED_Matrix_Config(void)
Purpose:  setting up the GPIOs and Timer for the LED-Matrix
Input:    none
Returns:  none
Comment:  TIM6, GPIOA (4-11) and GPIOB (0-7) used
**************************************************************************/
void LED_Matrix_Config(void){

	/* ########## Clock Enable ########## */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //GPIO A
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //GPIO C
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	/* ########## GPIO MODES ##########
	 *GPIOx -> MODER
	 *
	 * GPIOx_MODER_INPUT
	 * GPIOx_MODER_OUTPUT
	 * GPIOx_MODER_ALT
	 * GPIOx_MODER_ANALOG
	 */

	// column outputs
	GPIOA->MODER |= GPIO_MODER_MODER4_0;	//1
	GPIOA->MODER |= GPIO_MODER_MODER5_0;	//2
	GPIOA->MODER |= GPIO_MODER_MODER6_0;	//3
	GPIOA->MODER |= GPIO_MODER_MODER7_0;	//4
	GPIOA->MODER |= GPIO_MODER_MODER8_0;	//5
	GPIOA->MODER |= GPIO_MODER_MODER9_0;	//6
	GPIOA->MODER |= GPIO_MODER_MODER10_0;	//7
	GPIOA->MODER |= GPIO_MODER_MODER11_0;	//8

	//line outputs
	GPIOC->MODER |= GPIO_MODER_MODER0_0;	//1
	GPIOC->MODER |= GPIO_MODER_MODER1_0;	//2
	GPIOC->MODER |= GPIO_MODER_MODER2_0;	//3
	GPIOC->MODER |= GPIO_MODER_MODER3_0;	//4
	GPIOC->MODER |= GPIO_MODER_MODER4_0;	//5
	GPIOC->MODER |= GPIO_MODER_MODER5_0;	//6
	GPIOC->MODER |= GPIO_MODER_MODER6_0;	//7
	GPIOC->MODER |= GPIO_MODER_MODER7_0;	//8

	/* Reset-State */
	GPIOA->ODR &= ~(0xFF<<4);	//Set PA4-PA11 = 0 	columns
	GPIOC->ODR |= 0xFF;			//Set PC0-PC7 =1	lines


	/* ########## TIMER ########## */

	//Timer 6, Timing for Dot-Matrix
	TIM6->PSC = (SystemCoreClock/1000000)-1; 	// prescaler, ARR in usec
	TIM6->ARR = 155; 							// set counter value T_LED = 10ms => T_matrix = 10ms/64 = 156,25usec
	TIM6->DIER |= TIM_DIER_UIE; 				// enable Interrupt on Update
	TIM6->CR1 |= TIM_CR1_CEN;					// enable Timer 6
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 				// enable NVIC on TIM6
}

/* Die ISR des Timers 6 bildet mit Hilfe der Indizes i und j eine Art for-Schleife,
 * die jeden Punkt der Dot-Matrix in einem festen Intervall anzeigt. Dazu werden die
 * Matrizen postion und zufall überlagert. */
void TIM6_DAC_IRQHandler(void){
	/* local variables */
	static volatile int8_t i=0,j=0;
	static _Bool output[8][8]={0};

	/* Output */
	output[i][j]= position[i][j] | zufall[i][j];
	GPIOA->ODR &= ~(0xFF<<4);								// Spalte zurücksetzten
	GPIOC->ODR |= 0xFF;										// Zeile zurücksetzten
	GPIOA->ODR |= (uint32_t)(output[i][j]<<(i+4));			// Spalte entsprechend Wert in matrix setzten
	GPIOC->ODR &= ~((uint32_t)(output[i][j]<<j));			// Zeile entsprechend Wert in matrix setzten
	i++;													// Zählvariable erhöhen, abfragen für zeile und spalte
	if(i>7){												// ggf. zurücksetzten der Indizes
		j++; i=0;
	}
	if(j>7){
		i=0; j=0;
	}

	/* pseudo-random */
	t1++;
	if(t1>=8000)t1=15;
	t2++;
	if(t2>=65525)t2=0;
	/* ISR finished */
	TIM6->SR &= ~(TIM_SR_UIF);
}




