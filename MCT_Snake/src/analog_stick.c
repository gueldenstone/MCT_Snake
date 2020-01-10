/*
 * analog_stick.c
 *
 *  Created on: 10.01.2020
 *      Author: lukas
 */
#include "analog_stick.h"


/*************************************************************************
Function: Analog_Stick_Config(uint16_t * adcbuffer)
Purpose:  setting up the inputs, ADC and DMA for using the analog-stick
Input:    uint16_t pointer to array with 2 values for adcbuffer
Returns:  none
Comment:  GPIOA (0-1), GPIOB 0, TIM2 and ADC1 (channel 1&2) used
**************************************************************************/
void Analog_Stick_Config(volatile uint16_t * dmatarget){
	/* ########## Clock Enable ########## */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/* ########## GPIO MODES ##########
	 *GPIOx -> MODER
	 *
	 * GPIOx_MODER_INPUT
	 * GPIOx_MODER_OUTPUT
	 * GPIOx_MODER_ALT
	 * GPIOx_MODER_ANALOG
	 */

	//Inputs
	GPIOA->MODER |= GPIOx_MODER_ANALOG << GPIO_MODER_MODER0_Pos;	// PA0 x-axis ADC1_IN1
	GPIOA->MODER |= GPIOx_MODER_ANALOG << GPIO_MODER_MODER1_Pos;	// PA1 y-axis ADC1_IN2
	GPIOB->MODER &= ~(GPIO_MODER_MODER0_Msk);						// PB0 as Input

	/* ########## GPIO Pull-up/-down ##########
	 * GPIOx -> PUPDR
	 *
	 * GPIOx_PUPDR_NO
	 * GPIOx_PUPDR_PU
	 * GPIOx_PUPDR_PD
	 */
	GPIOB->PUPDR |= GPIOx_PUPDR_PU << GPIO_PUPDR_PUPDR0_Pos;

	/* ########## TIMER ########## */
	//Timer 2, Timing für ADC Conv
	TIM2->PSC = (SystemCoreClock/10000)-1;		// Prescaler, ARR in ms
	TIM2->ARR = 99; 							// set counter value, every 10msec ADC Conversion
	TIM2->CR2 |= 0b010 << TIM_CR2_MMS_Pos; 		// Trigger event on Update
	TIM2->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update
	TIM2->CR1 |= TIM_CR1_CEN;					// Enable Timer2

	/* ########## ADC & DMA ########## */
	ADC1_2_COMMON->CCR |= (0b01 << 16); 		// synchronous clock for ADC1/2

	/* ADC1 Config */
	ADC1->SQR1 |= 0x1;							// regular sequence length = 2
	ADC1->SQR1 |= 0x1 << ADC_SQR1_SQ1_Pos; 		// 1st conv. in regular sequence: Channel 1 (PA0)
	ADC1->SQR1 |= 0x2 << ADC_SQR1_SQ2_Pos;		// 2nd conv. in regular sequence: Channel 2 (PA1)
	ADC1->SMPR1 |= 0b111 << ADC_SMPR1_SMP0_Pos;	// Channel 1: Sample time 601.5 Clock Cycles
	ADC1->SMPR1 |= 0b111 << ADC_SMPR1_SMP1_Pos; // Channel 2: Sample time 601.5 Clock Cycles
	ADC1->CFGR |= 0b00 << ADC_CFGR_RES_Pos;		// Resolution = 12 Bit
	ADC1->CFGR |= 0b1 << ADC_CFGR_EXTEN_Pos;	// External Trigger (Rising Edge)
	ADC1->CFGR |= 0b1011 << ADC_CFGR_EXTSEL_Pos;// EXT11 selected for TIM2_TRGO event
	ADC1->CFGR |= ADC_CFGR_DMAEN;				// DMA enable
	ADC1->CFGR |= ADC_CFGR_DMACFG;				// DMA requests in circular mode

	/* ADC1 enable */
	ADC1->CR &= ~(0b11 << 28); 					// Voltage regulator: Intermediate state
	ADC1->CR |= 0b01 << 28; 					// Voltage regulator: Enabled
	for (volatile int x = 0; x < 540; x++){}  	// Wait 10 us
	ADC1->CR |= ADC_CR_ADCAL;					// calibrate the ADC
	while ((ADC1->CR & ADC_CR_ADCAL) != 0){}  	// wait for calibration
	ADC1->CR |= ADC_CR_ADEN; 					// Enable ADC
	while((ADC1->ISR & ADC_ISR_ADRD) == 0){}  	// wait for ADC ready

	/* DMA Channel 1 für ADC1 */
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;												// interrupt after transfer
	DMA1_Channel1->CCR |= DMA_CCR_MINC;												// memory increment mode enabled
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;												// circular mode enabled
	DMA1_Channel1->CCR |= (0b1 << DMA_CCR_PSIZE_Pos) | (0b1 << DMA_CCR_MSIZE_Pos);	// peripheral & Memory size = 16bit
	DMA1_Channel1->CNDTR = 0x2;														// number of data transfers = 2
	DMA1_Channel1->CMAR = (uint32_t)dmatarget;										// memory address is input pointer
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));									// peripheral address is ADC1.DR
	DMA1_Channel1->CCR |= DMA_CCR_EN;												// enable DMA1
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);												// enable DMA Interrupt Handler

	/* EXTI 0 an PA0 (button pressed)*/
	EXTI->IMR |= EXTI_IMR_IM0; 						// mask Pin Interrupt PB0
	EXTI->FTSR |= EXTI_FTSR_FT0;					// falling edge trigger
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB; 	// connect EXTI0 to Port B
	NVIC_EnableIRQ(EXTI0_IRQn); 					// enable NVIC on EXTI0

}

void DMA1_Channel1_IRQHandler(void){
	extern volatile uint16_t adc1buffer[2];
	extern volatile TypeDefDirection direction;
/* Code für Stufe 3 */

	if(adc1buffer[0]<=1500 && adc1buffer[1]<=2500 && adc1buffer[1]>=1500)
		direction = left;
	if(adc1buffer[0]>=2500 && adc1buffer[1]<=2500 && adc1buffer[1]>=1500)
		direction = right;
	if(adc1buffer[1]<=1500 && adc1buffer[0]<=2500 && adc1buffer[0]>=1500)
		direction = down;
	if(adc1buffer[1]>=2500 && adc1buffer[0]<=2500 && adc1buffer[0]>=1500)
		direction = up;

/* Code für Stufe 2 */
//	position1[x][y] = 0;						// letzte Postion auf null setzten
//	x = adcresults[0]/512;					// x, y bestimmen
//	y = adcresults[1]/512;
//	if(adcresults[0]>=2030 && adcresults[0]<=2060){		//Abfangen der "Mitte"-Position
//		x=4;
//	}
//	if(adcresults[1]>=2030 && adcresults[1]<=2060){
//		y=4;
//	}
//	else
//	position[x][y] = 1;						//Falls nicht in der Mitte -> aktuelle Position setzten

	//ISR finished
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
}
void EXTI0_IRQHandler(void){
	/* Stufe 2 */
//	randompoint();						// Zufälligen punkt erzeugen
	/* Stufe 3 */
	TIM3->CR1 |= TIM_CR1_CEN;			// Enable Timer 3
	//ISR finished
	EXTI->PR |= EXTI_PR_PR0;
}
