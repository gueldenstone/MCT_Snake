/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Prototypes -----------------------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI1_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void passthrough(void);

#endif

