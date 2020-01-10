/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx.h"
typedef enum{left, right, up, down}TypeDefDirection;

/* Exported functions prototypes ---------------------------------------------*/
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


#endif /* __MAIN_H */
