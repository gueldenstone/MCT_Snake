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
volatile int8_t randx, randy;
volatile int8_t x1, y1, x2, y2, x3, y3;
volatile uint16_t t1, t2=66, adc1buffer[2];
volatile _Bool position[8][8]={0}, zufall[8][8]={0}, fail;
volatile TypeDefDirection direction;

int main(void)
{
	__disable_irq();				// Interrupts aus
	RCC_Config();					// Clock Config mit 72Mhz
	GPIO_Config(); 					// GPIO Config
	TIM_Config();					// Timer Configuration
	ADC_Config();					// ADC Config
	DMA_Config();					// DMA Config
	EXTI_Config();					// EXTI Config
	__enable_irq();					// Interrupts an

	randompoint();
	ADC1->CR |= ADC_CR_ADSTART;
	resetposition();
	//Loop forever
	while(1){
		/* Spiel 2 (Schlange) */
		if(fail){
			TIM3->CR1 &= ~TIM_CR1_CEN;
			resetposition();
		}

		/* Spiel 1 */
		if(x1 == randx && y1 == randy){
			NVIC_DisableIRQ(DMA1_Channel1_IRQn);
			zufall[randx][randy]=0;
			randompoint();
			NVIC_EnableIRQ(DMA1_Channel1_IRQn);
		}
		__WFI();
	}

}
void randompoint(void){
	zufall[randx][randy] = 0;
	randx = t1%8;
	randy = t2%8;
	zufall[randx][randy] = 1;
}

void delay(uint32_t usec){
	TIM7->ARR = usec-1;
	TIM7->CR1 |= TIM_CR1_CEN;
	while(!(TIM7->SR & TIM_SR_UIF)){}
	TIM7->SR &= ~TIM_SR_UIF;
}

void resetposition(void){
	position[x1][y1]=0;
	position[x2][y2]=0;
	position[x3][y3]=0;
	x1=4;
	y1=4;
	x2=4;
	y2=3;
	x3=4;
	y3=2;
	position[x1][y1]=1;
	position[x2][y2]=1;
	position[x3][y3]=1;
	fail=0;
	direction = up;
}
