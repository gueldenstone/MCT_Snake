/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"
typedef enum{left, right, up, down}TypeDefDirection;

/* Globale Variablen ---------------------------------------------------------*/
volatile int8_t randx, randy;
volatile int8_t x1, y1, x2, y2, x3, y3;
volatile uint16_t t1, t2, adc1buffer[2];
volatile _Bool position[8][8], zufall[8][8], fail;
volatile TypeDefDirection direction;



/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void Clock_Config(void);

/* Defines -------------------------------------------------------------------*/
/* ########## GPIO MODES ########## */
#define GPIOx_MODER_INPUT		0b00
#define GPIOx_MODER_OUTPUT 		0b01
#define GPIOx_MODER_ALT	 		0b10
#define GPIOx_MODER_ANALOG 		0b11

/* ########## GPIO Output Types ########## */
#define GPIOx_OTYPER_PP			0b0
#define GPIOx_OTYPER_OD 		0b1

/* ########## GPIO Output Speed ########## */
#define GPIOx_OSPEEDR_LOW		0b00
#define GPIOx_OSPEEDR_MED		0b01
#define GPIOx_OSPEEDR_HIGH		0b11

/* ########## GPIO Pull-up/-down ########## */
#define GPIOx_PUPDR_NO		0b00
#define GPIOx_PUPDR_PU		0b01
#define GPIOx_PUPDR_PD		0b10

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
