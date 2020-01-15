/* Includes ------------------------------------------------------------------*/
#include "interrupts.h"

/* Die ISR des Timers 6 bildet mit Hilfe der Indizes i und j eine Art for-Schleife,
 * die jeden Punkt der Dot-Matrix in einem festen Intervall anzeigt. Dazu werden die
 * Matrizen postion und zufall überlagert. */
void TIM6_DAC_IRQHandler(void){
	/* Globale Variablen (main.c) */
	extern volatile _Bool position[8][8], zufall[8][8];
	extern volatile uint16_t t1, t2;

	/* lokale Variablen */
	static int8_t i=0,j=0;
	static _Bool output[8][8]={0};

	/* Ausgabe */
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

	/* Pseudo-Random */
	t1++;
	if(t1>=8000)t1=15;
	t2++;
	if(t2>=65525)t2=0;

	/* ISR finished */
	TIM6->SR &= ~(TIM_SR_UIF);								// Pending Bit = 0
}

/* Jede Sekunde wird der Wert des von direction abgefragt und die Schlange bewegt
 * ein Feld in die entsprechende Richtung */
void TIM3_IRQHandler(void){
	/* Globale Variablen (main.c) */
	extern volatile int8_t x1,y1;
	extern volatile _Bool fail;
	extern volatile TypeDefDirection direction;
	extern volatile _Bool position[8][8];

	passthrough();				// Koordinaten verschieben
	switch(direction){			// neue Richtung auswählen
	case left:
		position[--x1][y1]=1;
		break;
	case right:
		position[++x1][y1]=1;
		break;
	case up:
		position[x1][++y1]=1;
		break;
	case down:
		position[x1][--y1]=1;
		break;
	default:
		break;
	}
	if(x1>7 || x1<0 || y1>7 || y1<0)		// Abfrage, ob sich die neue Position außerhalb der LED-Matrix befindet
		fail=1;

	/* ISR finished */
	TIM3->SR &= ~(TIM_SR_UIF);				// Pending Bit = 0
}
/* Nach erfolgreichem Datentransfer werden die ausgelesenen Werte des ADC auf den Bereich
 * der LED Matrix gemappt (Stufe 2) bzw. einer richtung zugewiesen (Stufe 3)*/
void DMA1_Channel1_IRQHandler(void){
	/* Globale Variablen (main.c) */
	extern volatile TypeDefDirection direction;
	extern volatile uint16_t adc1buffer[2];

/* Code für Stufe 3 */
	/* Mapping of ADC results to directions, middle position = no change */
	if(adc1buffer[0]<=1500 && adc1buffer[1]<=2500 && adc1buffer[1]>=1500)
		direction = left;
	if(adc1buffer[0]>=2500 && adc1buffer[1]<=2500 && adc1buffer[1]>=1500)
		direction = right;
	if(adc1buffer[1]<=1500 && adc1buffer[0]<=2500 && adc1buffer[0]>=1500)
		direction = down;
	if(adc1buffer[1]>=2500 && adc1buffer[0]<=2500 && adc1buffer[0]>=1500)
		direction = up;

/* Code für Stufe 2 */
//	position1[x][y] = 0;								// letzte Postion auf null setzten
//	x = adc1buffer[0]/512;								// x, y bestimmen
//	y = adc1buffer[1]/512;
//	if(adc1buffer[0]>=2030 && adc1buffer[0]<=2060){		//Abfangen der "Mitte"-Position
//		x=4;
//	}
//	if(adc1buffer[1]>=2030 && adc1buffer[1]<=2060){
//		y=4;
//	}
//	else
//	position[x][y] = 1;									//Falls nicht in der Mitte -> aktuelle Position setzten

	/* ISR finished */
	DMA1->IFCR |= DMA_IFCR_CTCIF1;						// Pending Bit = 0
}
void EXTI0_IRQHandler(void){
	/* Stufe 2 */
//	randompoint();						// Zufälligen punkt erzeugen

	/* Stufe 3 */
	TIM3->CR1 |= TIM_CR1_CEN;			// Enable Timer 3

	/* ISR finished */
	EXTI->PR |= EXTI_PR_PR0;			// Pending Bit = 0
}

/* passthrough coordinates */
void passthrough(void){
	extern volatile int8_t x1,y1, x2, y2, x3, y3;
	extern volatile _Bool position[8][8];
	position[x1][y1]=0;								// zurücksetzten
	position[x2][y2]=0;
	position[x3][y3]=0;
	x3=x2;											// x3/y3 wird zu x2/y2 und x2/y2 wird zu x1/y1
	x2=x1;
	y3=y2;
	y2=y1;
	position[x2][y2]=1;								// neue Position für den Schlangenkörper definieren
	position[x3][y3]=1;
}
