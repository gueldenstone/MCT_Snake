/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"

/* Globale Variablen ---------------------------------------------------------*/

typedef enum{left, right, up, down}TypeDefDirection;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void randompoint(void);
void allones(void);
void allzeroes(void);
void delay(uint32_t time);
void blinkall(uint32_t blinks);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
