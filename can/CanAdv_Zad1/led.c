#include <LPC21xx.H>
#include "led.h"

#define LED0_bm (1<<16)
#define LED1_bm (1<<17)
#define LED2_bm (1<<18)
#define LED3_bm (1<<19)
#define LED4_bm (1<<20)
#define LED5_bm (1<<21)
#define LED6_bm (1<<22)
#define LED7_bm (1<<23)

void LedStep(enum LedState eDirection);
void LedOff(unsigned char ucLedIndeks);


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
		IO1DIR |= LED0_bm | LED1_bm | LED2_bm |LED3_bm|LED4_bm|LED5_bm|LED6_bm|LED7_bm;
 
}

void Led_ShowByte(unsigned char ucByte){
	IO1CLR = LED0_bm | LED1_bm | LED2_bm |LED3_bm|LED4_bm|LED5_bm|LED6_bm|LED7_bm;
	IO1SET = (ucByte << 16);

}

	void LedOn(unsigned char ucNumerDiody){
		
	IO1CLR = LED0_bm|LED1_bm|LED2_bm|LED3_bm;

	switch(ucNumerDiody){
		case 0: 
			IO1SET =LED0_bm; 
			break;
		case 1: 
			IO1SET =LED1_bm; 
			break;
		case 2: 
			IO1SET =LED2_bm; 
			break;
		case 3: 
			IO1SET =LED3_bm; 
			break;
		default:
			break;
	}
}

void LedOff(unsigned char ucLedIndeks){
	IO1CLR |= LED0_bm<<ucLedIndeks;
}
