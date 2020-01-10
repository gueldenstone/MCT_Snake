/*
 * snake.c
 *
 *  Created on: 10.01.2020
 *      Author: lukas
 */

#include "snake.h"

void randompoint(void){
	/* global variables */
	extern volatile _Bool zufall[8][8];
	extern volatile uint16_t t1, t2, randx, randy;
	extern volatile int8_t x1, y1, x2, y2, x3, y3;
	zufall[randx][randy] = 0;
	randx = t1%8;
	randy = t2%8;
	zufall[randx][randy] = 1;
	if(randx==(x1||x2||x3) && randy==(y1||y2||y3)){
		zufall[randx][randy] = 0;
		randx=(randx+1)%8;
		randy=(randy+1)%8;
		zufall[randx][randy] = 1;
	}
}

void resetposition(void){
	/* global variables */
	extern volatile _Bool position[8][8], fail;
	extern volatile TypeDefDirection direction;
	extern volatile int8_t x1, y1, x2, y2, x3, y3;
	position[x1][y1]=0;
	position[x2][y2]=0;
	position[x3][y3]=0;
	x1=4;
	y1=4;
	x2=4;
	y2=3;
	x3=4;
	y3=2;
	position[x1][y1]=1;
	position[x2][y2]=1;
	position[x3][y3]=1;
	fail=0;
	direction = up;
}

/* Jede Sekunde wird der Wert des von direction abgefragt und die Schlange bewegt
 * ein Feld in die entsprechende Richtung */
void TIM3_IRQHandler(void){
	/* global variables */
	extern volatile _Bool position[8][8], fail;
	extern volatile TypeDefDirection direction;
	extern volatile int8_t x1, y1, x2, y2, x3, y3;
	/* push snake body */
	position[x1][y1]=0;
	position[x2][y2]=0;
	position[x3][y3]=0;
	x3=x2;
	x2=x1;
	y3=y2;
	y2=y1;
	position[x2][y2]=1;
	position[x3][y3]=1;
	/* select next position for snake head */
	switch(direction){
	case left:
		position[--x1][y1]=1;
		break;
	case right:
		position[++x1][y1]=1;
		break;
	case up:
		position[x1][++y1]=1;
		break;
	case down:
		position[x1][--y1]=1;
		break;
	default:
		break;
	}
	if(x1>7 || x1<0 || y1>7 || y1<0)
		fail=1;
	if(x1==x2 && y1==y2)
		fail=1;

	/* ISR finished */
	TIM3->SR &= ~(TIM_SR_UIF);
}
