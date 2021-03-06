/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"

/* Globale Variablen ---------------------------------------------------------*/
volatile int8_t i,j, randx, randy;
volatile uint16_t t1, t2, adcresults[2];
volatile _Bool position1[8][8],zufall[8][8], output[8][8], output_flag;

typedef enum{left, right, up, down}TypeDefDirection;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void randompoint(void);
void allones(void);
void allzeroes(void);
void delay(uint32_t time);
void blinkall(uint32_t blinks);
void resetposition();

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
