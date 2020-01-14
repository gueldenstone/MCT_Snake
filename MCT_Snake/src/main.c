/**
  ******************************************************************************
  * @author  Lukas Güldenstein & Christopher Schultz
  * @version V1.0
  * @date    13. Januar 2020
  * @brief   Programm für das Spiel "Snake" auf einer 8x8 LED-Matrix
  * 			mit einem Analog-Stick
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "main.h"
#include "config.h"
#include "interrupts.h"

//Variablen
volatile int8_t randx, randy;								// Koordinaten für Zufallsmatrix
volatile int8_t x1, y1, x2, y2, x3, y3;						// Koordinaten für Körper der Schlange (Position)
volatile uint16_t t1, t2=66, adc1buffer[2];					// pseudo-random-Variablen, Buffer für die Analog-Digital-Wandlung
volatile _Bool position[8][8]={0}, zufall[8][8]={0};		// Matrizen für die Position der Schlange und den Zufallspunkt
volatile _Bool fail;										// Zustandsvariable (Spieler trifft den Rand der LED-Matrix)
volatile TypeDefDirection direction;						// Richtung der Schlangenbewegung

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
	ADC1->CR |= ADC_CR_ADSTART;		//
	resetposition();
	//Loop forever
	while(1){
		/* Spiel 2/3 */
		if(fail){
			TIM3->CR1 &= ~TIM_CR1_CEN;		// Schlangenbewegung stoppen
			resetposition();				// Spiel zurücksetzten
		}

		/* Spiel 1/2/3 */
		if(x1 == randx && y1 == randy){				// Punkt gefangen
			NVIC_DisableIRQ(DMA1_Channel1_IRQn);	// Auslesen des ADC stoppen
			randompoint();							// neuen Zufallspunkt erzeugen
			NVIC_EnableIRQ(DMA1_Channel1_IRQn);		// Auslesen des ADC wieder starten
		}
		__WFI();									// Zwischen Ein- bzw. Ausgabe "schlafen"
	}

}
void randompoint(void){
	zufall[randx][randy] = 0;		// aktuellen Punkt entfernen
	randx = t1%8;					// pseudo-random variablen auf 0...7 setzten
	randy = t2%8;
	zufall[randx][randy] = 1;		// neuen pseudo-random Punkt setzten
}

void delay(uint32_t usec){
	TIM7->ARR = usec-1;					// delay Zeit einstellen
	TIM7->CR1 |= TIM_CR1_CEN;			// Timer einschalten
	while(!(TIM7->SR & TIM_SR_UIF)){}	// warte auf so lange bis Update-Interrupt Flag gesetzt
	TIM7->SR &= ~TIM_SR_UIF;			// UIF-Flag zurücksetzten
}

/* Funktion setzt das Spiel auf Ursprünglichen Zustand zurück */
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
