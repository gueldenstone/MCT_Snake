/* Includes ------------------------------------------------------------------*/
#include "interrupts.h"


// Externe Variablen
extern volatile TypeDefDirection direction;
void SysTick_Handler(void){

}

/* Die ISR des Timers 6 bildet mit Hilfe der Indizes i und j eine Art for-Schleife,
 * die jeden Punkt der Dot-Matrix in einem festen Intervall anzeigt. Dazu werden die
 * Matrizen postion und zufall überlagert. */
void TIM6_DAC_IRQHandler(void){
	/* Ausgabe */
	output[i][j]= position1[i][j] | zufall[i][j];
	GPIOA->ODR &= ~(0xFF<<4);								// Spalte zurücksetzten
	GPIOC->ODR |= 0xFF;										// Zeile zurücksetzten
	GPIOA->ODR |= (uint32_t)(output[i][j]<<(i+4));			// Spalte entsprechend Wert in matrix setzten
	GPIOC->ODR &= ~((uint32_t)(output[i][j]<<j));			// Zeile entsprechend Wert in matrix setzten
	i++;													// Zählvariable erhöhen, abfragen für zeile und spalte
	if(i>7){j++; i=0;}										// ggf. zurücksetzten der Indizes
	if(j>7){i=0; j=0; output_flag=1;
	}
	/* Pseudo-Random */
	t1++;
	if(t1>=8000)t1=15;
	t2++;
	if(t2>=65525)t2=0;
	/* ISR finished */
	TIM6->SR &= ~(TIM_SR_UIF);
}

/* Jede Sekunde wird der Wert des von direction abgefragt und die Schlange bewegt ein Feld in die entsprechende Richtung */
void TIM3_IRQHandler(void){
	switch(direction){
	case left:
		position1[x][y]=0;
		position1[--x][y]=1;
		break;
	case right:
		position1[x][y]=0;
		position1[++x][y]=1;
		break;
	case up:
		position1[x][y]=0;
		position1[x][++y]=1;
		break;
	case down:
		position1[x][y]=0;
		position1[x][--y]=1;
		break;
	default:
		break;
	}
	if(x>8 || x<0 || y>8 || y<0){
		TIM3->CR1 &= ~TIM_CR1_CEN;
		position1[x][y]=0;
		zufall[randx][randy]=0;
//		blinkall(2);
		position1[4][4]=1;

	}
	/* ISR finished */
	TIM3->SR &= ~(TIM_SR_UIF);
}
/* Nach erfolgreichem Datentransfer werden die ausgelesenen Werte des ADC auf den Bereich*/
void DMA1_Channel1_IRQHandler(void){

/* Code für Stufe 3 */

	if(adcresults[0]<=1500 && adcresults[1]<=2500 && adcresults[1]>=1500) direction = left;
	if(adcresults[0]>=2500 && adcresults[1]<=2500 && adcresults[1]>=1500) direction = right;
	if(adcresults[1]<=1500 && adcresults[0]<=2500 && adcresults[0]>=1500) direction = down;
	if(adcresults[1]>=2500 && adcresults[0]<=2500 && adcresults[0]>=1500) direction = up;

/* Code für Stufe 2
*	position1[x][y] = 0;						// letzte Postion auf null setzten
	x = adcresults[0]/512;					// x, y bestimmen
	y = adcresults[1]/512;
	if(adcresults[0]>=2030 && adcresults[0]<=2060){		//Abfangen der "Mitte"-Position
		x=4;
	}
	if(adcresults[1]>=2030 && adcresults[1]<=2060){
		y=4;
	}
	else
	position1[x][y] = 1;						//Falls nicht in der Mitte -> aktuelle Position setzten*/

	//ISR finished
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
}
void EXTI0_IRQHandler(void){
	randompoint();							// Zufälligen punkt erzeugen
	TIM3->CR1 |= TIM_CR1_CEN;				// Enable Timer 3
	//ISR finished
	EXTI->PR |= EXTI_PR_PR0;
}
