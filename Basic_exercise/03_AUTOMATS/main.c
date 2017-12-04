#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
#include "delay.h"



/*********** Zadanie nr 3*******************
enum LedState {LED_LEFT,LED_RIGHT};
enum LedState eLedState = LED_LEFT;
*******************************************/

/*********** Zadanie nr 4******************
enum LedState {LED_LEFT,LED_RIGHT,LED_STOP};
enum LedState eLedState = LED_STOP;
*******************************************/

/*********** Zadanie nr 5******************
enum LedState {LED_LEFT,LED_RIGHT,LED_STOP};
enum LedState eLedState = LED_RIGHT;
*******************************************/
unsigned int uiShiftCounter;
enum LedState eLedState = LED_STOP;
int main(){

	enum KeyboardState eButtonState;
	
	LedInit();
	KeyboardInit();
	
while(1){
	
	uiShiftCounter++;
	eButtonState = eKeyboardRead();
	
	switch(eLedState){
		
		case LED_RIGHT:
			if(eButtonState == BUTTON_2)
				eLedState = LED_STOP;
			else
				LedStepRight();
			break;
			
		case LED_STOP:
			if(eButtonState == BUTTON_1)
				eLedState = LED_LEFT;
			else if(eButtonState == BUTTON_3)
				eLedState = LED_RIGHT;
			else
				eLedState = LED_STOP;
			break;
			
		case LED_LEFT:
			if(eButtonState == BUTTON_2)
				eLedState = LED_STOP;
			else
				LedStepLeft();
			break;
		}
		
	Delay(100);
}
	
/*********** Zadanie nr 4******************
*******************************************/
	
/*********** Zadanie nr 5******************
	while(1){
	
	uiShiftCounter++;
	eButtonState = eKeyboardRead();
	
	switch(eLedState){
		case LED_RIGHT:
			if(eButtonState == BUTTON_1)
				eLedState = LED_STOP;
			else{
				LedStepRight();
				Delay(100);
			}
			break;
		case LED_STOP:
			if(eButtonState == BUTTON_2)
				eLedState = LED_RIGHT;
			else
				eLedState = LED_STOP;
			break;
		}
		
	Delay(50);
}
*******************************************/
	
/*********** Zadanie nr 4******************
	while(1){
	

	eButtonState = eKeyboardRead();
	
	switch(eLedState){
		case LED_RIGHT:
			if(uiShiftCounter%4 != 0){
				LedStepRight();
				uiShiftCounter++;
				Delay(200);
			}
			else
				eLedState = LED_STOP;		
			break;
		case LED_STOP:
			if(eButtonState == BUTTON_1){
				eLedState = LED_RIGHT;
				uiShiftCounter++;
			}
			else
				eLedState = LED_STOP;
			break;
		}

	Delay(100);
}
*******************************************/
	
/*********** Zadanie nr 3******************
	while(1){
	uiShiftCounter++;
	switch(eLedState){
		case LED_LEFT:
			if(uiShiftCounter%4 != 0)
				LedStepLeft();
			else{
				eLedState = LED_RIGHT;
				continue;
				}
			break;
		case LED_RIGHT:
			if(uiShiftCounter%4 != 0)
				LedStepRight();
			else{
				eLedState = LED_LEFT;
				continue;
				}
			break;
		}
	Delay(500);
}
*******************************************/
	
	
}
