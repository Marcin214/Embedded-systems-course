#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
//*************************************

void Automat(void);

//*************************************
int main(){

	LedInit();
	KeyboardInit();
	
	while(1){
		Timer0Interrupts_Init(20000,&Automat);
		}
	}
//*************************************
void Automat(void){
	
	switch(eLedState){
		
		case LED_RIGHT:
			if(eKeyboardRead() == BUTTON_2)
				eLedState = LED_STOP;
			else{
				LedStepRight();
			}
			break;
			
		case LED_STOP:
			if(eKeyboardRead() == BUTTON_1)
				eLedState = LED_LEFT;
			else if(eKeyboardRead() == BUTTON_3)
				eLedState = LED_RIGHT;
			else
				eLedState = LED_STOP;
			break;
			
		case LED_LEFT:
			if(eKeyboardRead() == BUTTON_2)
				eLedState = LED_STOP;
			else{
				LedStepLeft();
			}
			break;
		}
}
