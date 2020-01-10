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
#include "led_matrix.h"
#include "analog_stick.h"
#include "snake.h"
#include "utility.h"


volatile int8_t x1, y1, x2, y2, x3, y3, randx, randy;;
volatile uint16_t t1, t2, adc1buffer[2];
volatile _Bool position[8][8]={0}, zufall[8][8]={0}, fail;
volatile TypeDefDirection direction;

int main(void)
{
	/* start values for variables */
	t1=0;
	t2=66;
	/* initialize all modules */
	__disable_irq();								// Interrupts aus
	Clock_Config();									// Clock Config mit 72Mhz
	Analog_Stick_Config(adc1buffer);				// use analog-stick
	LED_Matrix_Config();							// use LED-Matrix
	__enable_irq();									// Interrupts an

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

void Clock_Config(void){
	/* PLL Config */
	RCC->CR &= ~(RCC_CR_PLLON); 																	// Deactivate PLL
	while(RCC->CR & RCC_CR_PLLRDY){}																// Wait for PLL to stop
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_PREDIV | RCC_CFGR_PLLMUL9;										// Set PLL to input HSI, Multiplicator = 9
 	RCC->CR |= RCC_CR_PLLON; 																		// Activate PLL
 	while(!(RCC->CR & RCC_CR_PLLRDY)); 																// Wait for PLL to lock

 	/* FLASH wait states */
 	FLASH->ACR &= ~(FLASH_ACR_LATENCY_Msk);															// Reset Flash Wait states
 	FLASH->ACR |= 0b010 << FLASH_ACR_LATENCY_Pos;													// Set Flash wait states to 2

  	/* SysClock anpassen */
  	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; 																// PreDiv for ABP1 /2 (ABP1 36MHz max)
  	RCC->CFGR |= RCC_CFGR_SW_PLL;																	// Set PLL as Sysclock
  	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){} 										// Wait for switch to PLL as clock source

  	SystemCoreClockUpdate();																		//WICHTIG! updated die Variable SystemCoreClock, die als Berechnungsgrundlage für Timer gebraucht werden kann.
}
