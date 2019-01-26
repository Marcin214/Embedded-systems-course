#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
#include "delay.h"


int main(){

	enum KeyboardState eButtonState;
	
	LedInit();
	KeyboardInit();

	while(1){
		
		eButtonState = eKeyboardRead();
		
		switch(eButtonState){
			case BUTTON_1: 
				LedStepRight(); 
				break;
			case BUTTON_2: 
				LedStepLeft(); 
				break;
			case BUTTON_3: 
				break;
			case BUTTON_4: 
				break;
			case RELASED: 
				break;
			default:
				break;			
		}	
		Delay(50);
	}
}
