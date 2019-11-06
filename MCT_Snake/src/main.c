/**
  ******************************************************************************
  * @author  Lukas Güldemstein & Christopher Schultz
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "stm32f3xx.h"

//Variablen
static volatile int8_t i=0,j=0;
static volatile uint32_t x,y;
static volatile _Bool matrix[8][8]={0};
uint8_t k,l;

int main(void)
{
	__disable_irq();				//Interrupts aus
	RCC_Config();					//Clock Config
	GPIO_Config(); 					//GPIO Config
	//TIM_Config();					//Timer Configuration
	ADC_Config();					//ADC Config


	/*
	 * Matrix befüllen
	 */
//	matrix[0][0]=1;
//	matrix[1][1]=1;
//	matrix[2][2]=1;
//	matrix[3][3]=1;
//	matrix[4][4]=1;
//	matrix[5][5]=1;
//	matrix[6][6]=1;
//	matrix[7][7]=1;


	__enable_irq();					//Interrupts an
	//Timer aktivieren
	TIM6->CR1 |= TIM_CR1_CEN;


	//Loop forever
	while(1){
		matrix[x][y] = 0;
		ADC1->CR|= ADC_CR_ADSTART; 				//Starte Conversion
		while((ADC1->ISR & ADC_ISR_EOC) == 0){} //Warte bis Conversion abgeschlossen
		x = (ADC1->DR)/8;
		ADC3->CR|= ADC_CR_ADSTART; 				//Starte Conversion
		while((ADC3->ISR & ADC_ISR_EOC) == 0){} //Warte bis Conversion abgeschlossen
		y = (ADC3->DR)/8;
		matrix[x][y] = 1;
		GPIOA->ODR &= ~(0xFF<<4);							//Spalte zurücksetzten
		GPIOC->ODR |= 0xFF;									//Zeile zurücksetzten
		GPIOA->ODR |= (uint32_t)((matrix[i][j])<<(i+4));	//Spalte entsprechend Wert in matrix setzten
		GPIOC->ODR &= ~((uint32_t)matrix[i][j]<<j);			//Zeile entsprechend Wert in matrix setzten
		i++;
		if(i>7){j++; i=0;}
		if(j>7){i=0; j=0;}
	}

}


//void TIM_Config(void){
//	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //Enable Timer6 Clock
//	TIM6->ARR = 19; //set counter value
//	TIM6->PSC = 79;
//	TIM6->CR1 |= TIM_CR1_ARPE; //enable auto reload preload
//	//TIM6->CR1 |= TIM_CR1_URS; //Update source only at overflow
//	//TIM6->CR1 |= TIM_CR1_OPM; //Enable one-pulse Mode
//	TIM6->DIER |= TIM_DIER_UIE; //Enable Interrupt on Update
//	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Enable NVIC on TIM6
//	}

//void TIM6_DAC_IRQHandler(void){
//	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
//	TIM6->SR &= ~(TIM_SR_UIF);
//	GPIOA->ODR &= ~(0xFF<<4);							//Spalte zurücksetzten
//	GPIOC->ODR |= 0xFF;									//Zeile zurücksetzten
//	GPIOA->ODR |= (uint32_t)((matrix[i][j])<<(i+4));	//Spalte entsprechend Wert in matrix setzten
//	GPIOC->ODR &= ~((uint32_t)matrix[i][j]<<j);			//Zeile entsprechend Wert in matrix setzten
//	i++;
//	if(i>7){j++; i=0;}
//	if(j>7){i=0; j=0;}
//	}

void ADC_Config(void){
	ADC1_2_COMMON->CCR |= (0b01 << 16); 		// Synchroner ADC-Clock mit Vorteiler 1
	ADC3_4_COMMON->CCR |= (0b01 << 16); 		// Synchroner ADC-Clock mit Vorteiler 1
	/*
	 * ADC1 Config
	 */
	ADC1->SQR1 |= 0x2 << 6; 					// 1st conv. in regular sequence: Channel 2 (PA1)
	ADC1->SMPR1 |= 0b111 << 6; 					// Channel 2: Sample time 601.5 ns
	ADC1->CFGR |= 0b11 << ADC_CFGR_RES_Pos;		// Auflösung sind 6 Bit
	ADC1->CR &= ~(0b11 << 28); 					// Voltage regulator: Intermediate state
	ADC1->CR |= 0b01 << 28; 					// Voltage regulator: Enabled
	for (volatile int x = 0; x < 60; x++){}  	// Warte 10 us
	ADC1->CR |= ADC_CR_ADCAL;					// Kalibriere den ADC
	while ((ADC1->CR & ADC_CR_ADCAL) != 0){}  	// Warte bis Kalibrierung abgeschlossen
	ADC1->CR |= ADC_CR_ADEN; 					// Enable ADC
	while((ADC1->ISR & ADC_ISR_ADRD) == 0){}  	// Warte bis ADC bereit

	/*
	 * ADC3 Config
	 */
	ADC3->SQR1 |= 0x1 << 6; 					// 1st conv. in regular sequence: Channel 1 (PB1)
	ADC3->SMPR1 |= 0b111 << 3; 					// Channel 1: Sample time 601.5 ns
	ADC3->CFGR |= 0b11 << ADC_CFGR_RES_Pos;		// Auflösung sind 6 Bit
	ADC3->CR &= ~(0b11 << 28); 					// Voltage regulator: Intermediate state
	ADC3->CR |= 0b01 << 28; 					// Voltage regulator: Enabled
	for (volatile int x = 0; x < 60; x++){}  	// Warte 10 us
	ADC3->CR |= ADC_CR_ADCAL;					// Kalibriere den ADC
	while ((ADC3->CR & ADC_CR_ADCAL) != 0){}  	// Warte bis Kalibrierung abgeschlossen
	ADC3->CR |= ADC_CR_ADEN; 					// Enable ADC
	while((ADC3->ISR & ADC_ISR_ADRD) == 0){}  	// Warte bis ADC bereit
}

