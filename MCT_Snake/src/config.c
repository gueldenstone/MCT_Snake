/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Functions -----------------------------------------------------------------*/
void RCC_Config(void){
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

	/* Peripherie Clock */

  	//Timer
  	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_TIM3EN; //Enable Timer 2, 6 & 7

	//GPIO
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //GPIO A
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; //GPIO B
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //GPIO C

	//Sonsitiges
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 	// Clock to SYSCFG
	RCC->AHBENR |= RCC_AHBENR_ADC12EN; 		// Takt fuer ADC 1+2
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;		// Takt für DMA1

}

void GPIO_Config(void){

	/* ########## GPIO MODES ##########
	 *GPIOx -> MODER
	 *
	 * GPIOx_MODER_INPUT
	 * GPIOx_MODER_OUTPUT
	 * GPIOx_MODER_ALT
	 * GPIOx_MODER_ANALOG
	 */
	//Inputs
	GPIOA->MODER |= GPIOx_MODER_ANALOG << GPIO_MODER_MODER0_Pos;	// PA0 x-Achse ADC1_IN1
	GPIOA->MODER |= GPIOx_MODER_ANALOG << GPIO_MODER_MODER1_Pos;	// PA1 y-Axhse ADC1_IN2
	GPIOB->MODER &= ~(GPIO_MODER_MODER0_Msk);						// PB0 als Input

	//Outputs
	// Spalten Outputs
	GPIOA->MODER |= GPIO_MODER_MODER4_0;	//1
	GPIOA->MODER |= GPIO_MODER_MODER5_0;	//2
	GPIOA->MODER |= GPIO_MODER_MODER6_0;	//3
	GPIOA->MODER |= GPIO_MODER_MODER7_0;	//4
	GPIOA->MODER |= GPIO_MODER_MODER8_0;	//5
	GPIOA->MODER |= GPIO_MODER_MODER9_0;	//6
	GPIOA->MODER |= GPIO_MODER_MODER10_0;	//7
	GPIOA->MODER |= GPIO_MODER_MODER11_0;	//8

	//Zeilen Outputs
	GPIOC->MODER |= GPIO_MODER_MODER0_0;	//1
	GPIOC->MODER |= GPIO_MODER_MODER1_0;	//2
	GPIOC->MODER |= GPIO_MODER_MODER2_0;	//3
	GPIOC->MODER |= GPIO_MODER_MODER3_0;	//4
	GPIOC->MODER |= GPIO_MODER_MODER4_0;	//5
	GPIOC->MODER |= GPIO_MODER_MODER5_0;	//6
	GPIOC->MODER |= GPIO_MODER_MODER6_0;	//7
	GPIOC->MODER |= GPIO_MODER_MODER7_0;	//8

	/* ########## GPIO Output Types ##########
	 * GPIOx -> OTYPER
	 *
	 * GPIOx_OTYPER_PP
	 * GPIOx_OTYPER_OD
	 */

	/* ########## GPIO Output Speed ##########
	 * GPIOx -> OSPEEDR
	 *
	 * GPIOx_OSPEEDR_LOW
	 * GPIOx_OSPEEDR_MED
	 * GPIOx_OSPEEDR_HIGH
	 */

	/* ########## GPIO Pull-up/-down ##########
	 * GPIOx -> PUPDR
	 *
	 * GPIOx_PUPDR_NO
	 * GPIOx_PUPDR_PU
	 * GPIOx_PUPDR_PD
	 */
	GPIOB->PUPDR |= GPIOx_PUPDR_PU << GPIO_PUPDR_PUPDR0_Pos;

	/* ########## Reset Pins to Default ##########
	 * Port Reset		GPIO<port>->ODR &= ~(0xFFFF)
	 * Port Set			GPIO<port>->ODR |= 0xFFFF
	 * Pin Reset		GPIOA->ODR &= ~(GPIO_ODR_<pin>);
	 * Pin Set			GPIOA->ODR |= GPIO_ODR_<pin>;
	 */
	GPIOA->ODR &= ~(0xFF<<4);	//Set PA4-PA11 = 0 	Spalten
	GPIOC->ODR |= 0xFF;			//Set PC0-PC7 =1	Zeilen
}

void TIM_Config(void){

	//Timer 6, Timing für Display
	TIM6->PSC = 71; 							// Prescaler setzten, ARR in usec
	TIM6->ARR = 155; 							// set counter value T_LED = 10ms => T_matrix = 10ms/64 = 156,25usec
	TIM6->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update
	TIM6->CR1 |= TIM_CR1_CEN;					// Enable Timer 6
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 				// Enable NVIC on TIM6

	//Timer 7, Delay Timer
	TIM7->PSC = 7199; 							// Prescaler setzten, ARR in 0,1ms
	TIM7->CR1 |= TIM_CR1_OPM;					// One-Pulse-Mode
	TIM7->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update


	//Timer 2, Timing für ADC Conv
	TIM2->PSC = 7199;							// Prescaler setzten, ARR in 0,1ms
	TIM2->ARR = 99; 							// set counter value, every 10msec ADC Conversion
	TIM2->CR2 |= 0b010 << TIM_CR2_MMS_Pos; 		// Trigger event on Update
	TIM2->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update
	TIM2->CR1 |= TIM_CR1_CEN;					// Enable Timer2

	/* Timer 3, Timing für Schlangenbewegung */
	TIM3->PSC = (SystemCoreClock/1000)-1;		// Prescaler setzten, ARR in 0,1ms
	TIM3->ARR = 10000;							// set counter Value every 1s move forward
	TIM3->DIER |= TIM_DIER_UIE; 				// Enable Interrupt on Update
//	TIM3->CR1 |= TIM_CR1_CEN;					// Enable Timer 3
	NVIC_EnableIRQ(TIM3_IRQn);					// Enable NVIC on TIM3
}

void ADC_Config(void){
	ADC1_2_COMMON->CCR |= (0b01 << 16); 		// Synchroner ADC-Clock mit Vorteiler 1

	/* ADC1 Config */
	ADC1->SQR1 |= 0x1;							// regular sequence length = 2
	ADC1->SQR1 |= 0x1 << ADC_SQR1_SQ1_Pos; 		// 1st conv. in regular sequence: Channel 1 (PA0)
	ADC1->SQR1 |= 0x2 << ADC_SQR1_SQ2_Pos;		// 2nd conv. in regular sequence: Channel 2 (PA1)
	ADC1->SMPR1 |= 0b111 << ADC_SMPR1_SMP0_Pos;	// Channel 1: Sample time 601.5
	ADC1->SMPR1 |= 0b111 << ADC_SMPR1_SMP1_Pos; // Channel 2: Sample time 601.5
//	ADC1->CFGR |= 0b00 << ADC_CFGR_RES_Pos;		// Auflösung sind 12 Bit
	ADC1->CFGR |= 0b1 << ADC_CFGR_EXTEN_Pos;	// External Trigger aktiviert (Rising Edge)
	ADC1->CFGR |= 0b1011 << ADC_CFGR_EXTSEL_Pos;// EXT11 selected for TIM2_TRGO event
	ADC1->CFGR |= ADC_CFGR_DMAEN;				// DMA einschalten
	ADC1->CFGR |= ADC_CFGR_DMACFG;				// DMA requests in circular mode
//	ADC1->IER |= ADC_IER_EOSIE;					// Interrupt enable for EOS

	/* ADC1 einschalten */
	ADC1->CR &= ~(0b11 << 28); 					// Voltage regulator: Intermediate state
	ADC1->CR |= 0b01 << 28; 					// Voltage regulator: Enabled
	for (volatile int x = 0; x < 540; x++){}  	// Warte 10 us
	ADC1->CR |= ADC_CR_ADCAL;					// Kalibriere den ADC
	while ((ADC1->CR & ADC_CR_ADCAL) != 0){}  	// Warte bis Kalibrierung abgeschlossen
	ADC1->CR |= ADC_CR_ADEN; 					// Enable ADC
	while((ADC1->ISR & ADC_ISR_ADRD) == 0){}  	// Warte bis ADC bereit
}

extern volatile uint16_t adc1buffer[2];
void DMA_Config(void){
	//DMA Channel 1 für ADC1
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;												// Interrupt after transfer
	DMA1_Channel1->CCR |= DMA_CCR_MINC;												// Memory increment mode enabled
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;												// Circular mode enabled
	DMA1_Channel1->CCR |= (0b1 << DMA_CCR_PSIZE_Pos) | (0b1 << DMA_CCR_MSIZE_Pos);	// Peripheral & Memory size = 16bit
	DMA1_Channel1->CNDTR = 0x2;														// number of data transfers = 2
	DMA1_Channel1->CMAR = (uint32_t)adc1buffer;										// Memory address setzten auf Array "adcresults"
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));									// peripheral address is ADC1.DR
	DMA1_Channel1->CCR |= DMA_CCR_EN;												// Enable DMA1
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);												// DMA Interrupt Handler
}

void EXTI_Config(void){
	/* EXTI 0 an PA0 */
	EXTI->IMR |= EXTI_IMR_IM0; 						// Maske Pin Interrupt PB0
	EXTI->FTSR |= EXTI_FTSR_FT0;					// Falling Edge Trigger ausgewählt
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB; 	// Connect EXTI0 mit Port B
	NVIC_EnableIRQ(EXTI0_IRQn); 					// Enable NVIC on EXTI0
}
