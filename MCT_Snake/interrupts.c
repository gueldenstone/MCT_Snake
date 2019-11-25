/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <interrupts.h>
#include "stm32f3xx.h"


//DCC Interrupt Variablen
int8_t t=0, i=8;
TypeDefRecstate recstate;
_Bool bit;
uint8_t byte;

//Globale Variablen
extern volatile uint8_t package[3];
extern volatile _Bool received;


void SysTick_Handler(void){
}

void EXTI0_IRQHandler(void){
	TIM6->CR1 |= TIM_CR1_CEN;					//Timer 6 starten (87usec bis ISR)
	//ISR finished
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	EXTI->PR |= EXTI_PR_PR1;

}

void TIM6_DAC_IRQHandler(void){
	bit = !((_Bool)(GPIOA->IDR & 1));			//Status PA0 auslesen -> inverser Wert ist Bit
	switch(recstate){							//Recstate abfragen
		case WF_Preamble:						//Warten auf die Preambel
			if((bit==1) && (t<10)) t++;			//Preambel noch nicht vorbei
			if((bit==1) && (t>=10)){			//Preambel vorbei
				recstate=WF_Lead0;				//Nächster Status: Warten auf das Trennbit
			}
			if(bit==0) t=0;						//Synchronisierungsfehler, reset
			break;
		case WF_Lead0:							//Auf Trennbit warten
			if (bit==0) recstate=WF_Byte;		//Falls Trennbit erkannt, erstes Byte auslesen
			break;
		case WF_Byte:							//Auf Byte warten
			package[byte] |= bit<<(i-1);		//Bitstelle des Bytes auslesen und speichern
			i--;
			if(i<=0){							//Ende des Bytes erreicht
				recstate=WF_Trailer;			//Warten auf das nächste Trennbit
				byte++;							//nächstes Byte auswählen
				i=8;							//Bitstelle zurücksetzten
			}
			break;
		case WF_Trailer:						//Auf Trenn- oder Stopbit warten
			if(bit==0) recstate=WF_Byte;		//Trennbit erkannt, nächstes Byte auslesen
			if(bit==1){							//Stopbit erkannt, rücksetzten der Indizes
				received=1;						//main() signalisieren, dass package bereit steht
				byte=0;
				i=8;
				t=0;
				recstate=WF_Preamble;			//warten auf nächstes package
				}
			break;
	}
	//ISR finished
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
	TIM6->SR &= ~TIM_SR_UIF;
}
