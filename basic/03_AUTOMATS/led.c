#include <LPC21xx.H>
#include "led.h"

#define LED0_BM (1<<16)
#define LED1_BM (1<<17)
#define LED2_BM (1<<18)
#define LED3_BM (1<<19)

void LedStep(enum LedState eDirection);

void LedStep(enum LedState eDirection){
	static unsigned int i;
	switch(eDirection){
		case LED_LEFT: 
			i++;
			LedOn(i%4);
			break;
		case LED_RIGHT: 
			i--;
			LedOn(i%4); 
			break;
		default:
			break;
			}
}

void LedStepLeft(void){
 LedStep(LED_LEFT);
}

void LedStepRight(void){
 LedStep(LED_RIGHT);
}

void LedInit(void){
	IO1DIR |= LED0_BM | LED1_BM | LED2_BM |LED3_BM;
	IO1SET =LED0_BM;
 
}

	void LedOn(unsigned char ucNumerDiody){
		
	IO1CLR = LED0_BM|LED1_BM|LED2_BM|LED3_BM;

	switch(ucNumerDiody){
		case 0: 
			IO1SET =LED0_BM; 
			break;
		case 1: 
			IO1SET =LED1_BM; 
			break;
		case 2: 
			IO1SET =LED2_BM; 
			break;
		case 3: 
			IO1SET =LED3_BM; 
			break;
		default:
			break;
	}
}
