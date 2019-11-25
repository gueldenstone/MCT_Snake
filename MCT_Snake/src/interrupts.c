/* Includes ------------------------------------------------------------------*/
#include "interrupts.h"

void SysTick_Handler(void){

}

void TIM6_DAC_IRQHandler(void){
	//Ausgabe
	output[i][j]= position[i][j] | zufall[i][j];
	GPIOA->ODR &= ~(0xFF<<4);								// Spalte zurücksetzten
	GPIOC->ODR |= 0xFF;										// Zeile zurücksetzten
	GPIOA->ODR |= (uint32_t)(output[i][j]<<(i+4));			// Spalte entsprechend Wert in matrix setzten
	GPIOC->ODR &= ~((uint32_t)(output[i][j]<<j));			// Zeile entsprechend Wert in matrix setzten
	i++;													// Zählvariable erhöhen, abfragen für zeile und spalte
	if(i>7){j++; i=0;}										// ggf. zurücksetzten der Indizes
	if(j>7){i=0; j=0; output_flag=1;
	}
	//Peusdo-random
	t1++;
	if(t1>=8000)t1=15;
	t2++;
	if(t2>=65525)t2=0;
	//ISR finished
	TIM6->SR &= ~(TIM_SR_UIF);
}

void DMA1_Channel1_IRQHandler(void){
	position[x][y] = 0;						// letzte Postion auf null setzten
	x = dma[0]/512;							// x, y bestimmen
	y = dma[1]/512;
	if(dma[0]>=2030 && dma[0]<=2060){		//Abfangen der "Mitte"-Position
		x=4;
	}
	if(dma[1]>=2030 && dma[1]<=2060){
		y=4;
	}
	else
	position[x][y] = 1;						//Falls nicht in der Mitte -> aktuelle Position setzten

	//ISR finished
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
}
void EXTI0_IRQHandler(void){
	randompoint();
	//ISR finished
	EXTI->PR |= EXTI_PR_PR0;
}
