/* Includes ------------------------------------------------------------------*/
#include "rcc.h"

/* Code ------------------------------------------------------------------*/


void RCC_Config(void){

	/* PLL Config
	 *
	 */
//	RCC->CR &= ~(RCC_CR_PLLON); //PLL deaktivieren (zur Config)
//	RCC->CFGR |= RCC_CFGR_PLLMUL6;	//PLL Multi = 6
//	RCC->CFGR2 &= ~(RCC_CFGR2_PREDIV_0);		//No PreDIV
//	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_PREDIV;	//PLL Source -> HSI
//	RCC->CR |= RCC_CR_PLLON; //PLL aktivieren (zur Config)
//
//	/*SysClock anpassen
//	 *
//	 */
//	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //PreDiv für ABP1
//	RCC->CR |= RCC_CR_HSION;	//HSI ON
//	RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL set as SYSCLOCK

	/*Peripherie Clock
	 *
	 */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Clock to SYSCFG
	RCC->AHBENR |= RCC_AHBENR_ADC12EN; // Takt fuer ADC 1+2
	RCC->AHBENR |= RCC_AHBENR_ADC34EN; // Takt fuer ADC 3+4


	RCC->BDCR |= RCC_BDCR_RTCEN;
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;



}
