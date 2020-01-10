/*
 * utility.c
 *
 *  Created on: 10.01.2020
 *      Author: lukas
 */

#include "utility.h"
void delay(uint32_t usec){
	TIM7->ARR = usec-1;
	TIM7->CR1 |= TIM_CR1_CEN;
	while(!(TIM7->SR & TIM_SR_UIF)){}
	TIM7->SR &= ~TIM_SR_UIF;
}
void delay_Config(void){
	//Timer 7, Delay Timer
	TIM7->PSC = 7199; 							// Prescaler setzten, ARR in 0,1ms
	TIM7->CR1 |= TIM_CR1_OPM;					// One-Pulse-Mode
	TIM7->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update
}
