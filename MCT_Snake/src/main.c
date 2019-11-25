/**
  ******************************************************************************
  * @author  Lukas Güldenstein & Christopher Schultz
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "main.h"
#include "config.h"

//Variablen
volatile int8_t i=0,j=0, randx=5, randy=5;
volatile uint32_t x,y;
volatile uint16_t t1, t2=66, dma[2];
volatile _Bool position[8][8]={0},zufall[8][8]={0}, output[8][8]={0}, output_flag;

int main(void)
{
	__disable_irq();				// Interrupts aus
	RCC_Config();					// Clock Config
	GPIO_Config(); 					// GPIO Config
	TIM_Config();					// Timer Configuration
	ADC_Config();					// ADC Config
	DMA_Config();					// DMA_Config
	EXTI_Config();
	__enable_irq();					// Interrupts an

	randompoint();
	ADC1->CR|= ADC_CR_ADSTART;

	//Loop forever
	while(1){
		/* Spiel 1 */
		if(x == randx && y == randy){
			NVIC_DisableIRQ(DMA1_Channel1_IRQn);
			position[x][y] = 0;
			zufall[randx][randy]=0;
			blinkall(5);
			randompoint();
			NVIC_EnableIRQ(DMA1_Channel1_IRQn);
		}
	}

}
void randompoint(void){
	zufall[randx][randy] = 0;
	randx = t1%8;
	randy = t2%8;
	zufall[randx][randy] = 1;
}

void allones(void){
	int a, b;
	for(a=7; a>=0; a--){
		for(b=7; b>=0; b--){
			zufall[a][b]=1;
		}
	}
}

void allzeroes(void){
	int a, b;
	for(a=7; a>=0; a--){
		for(b=7; b>=0; b--){
			zufall[a][b]=0;
		}
	}
}


void delay(uint32_t msec){
	TIM7->ARR = (msec*10)-1;
	TIM7->CR1 |= TIM_CR1_CEN;
	while(!(TIM7->SR & TIM_SR_UIF)){}
	TIM7->SR &= ~TIM_SR_UIF;
}

void blinkall(uint32_t blinks){
	for(int a = 0; a<=blinks; a++){
		allones();
		delay(200);
		allzeroes();
		delay(200);
	}
}
