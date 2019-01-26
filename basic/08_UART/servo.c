#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include "delay.h"
extern char cOdebranyZnak;
#define DETEKTOR 0x00000400 // maska pinu P0.10

struct Servo eServo;

void DetectorInit(void){
IO0DIR &= ~DETEKTOR;
}

enum DetectorState eReadDetector(void){
if(!(IO0PIN&DETEKTOR))
			return ACTIVE;
else 
	return INACTIVE;	
}
void ServoInit(unsigned int uiServoFrequency){
	unsigned int uiMicroseconds = (1000000/uiServoFrequency);
	eServo.eState = CALLIB;
	LedInit();
	KeyboardInit();
	Timer0Interrupts_Init(uiMicroseconds,&Automat);
}

void ServoCallib(void){
		if(eReadDetector() == INACTIVE){
			LedStepLeft();	
			eServo.eState = CALLIB;
		}
		else if(eReadDetector() == ACTIVE){
			eServo.uiCurrentPosition = 0;
			eServo.uiDesiredPosition = 0;  
			eServo.eState = IDLE;
		}
				
}
void ServoGoTo(unsigned int uiPosition){
	eServo.uiDesiredPosition=eServo.uiDesiredPosition+uiPosition;
	cOdebranyZnak='0';
	eServo.eState = IN_PROGRESS;

}
void Automat(void){
	
	switch(eServo.eState){
		
		case CALLIB:
				ServoCallib();
			break;
			
		case IDLE:
			if(cOdebranyZnak == 'c')
				eServo.eState = CALLIB;
			else if (cOdebranyZnak == '1')
				ServoGoTo(12);
		break;
			
		case IN_PROGRESS:
			
		if (eServo.uiCurrentPosition == eServo.uiDesiredPosition)
					eServo.eState = IDLE;
		else if(eServo.uiCurrentPosition <= eServo.uiDesiredPosition){
					LedStepLeft();
					eServo.uiCurrentPosition ++;
				}
		else if(eServo.uiCurrentPosition >= eServo.uiDesiredPosition){
					LedStepRight();
					eServo.uiCurrentPosition --;  
				}
	
			break;
		}
}
/**************** Zadanie 3 ********************
void Automat(void){
	
	switch(eServo.eState){
		
		case CALLIB:
				ServoCallib();
			break;
			
		case IDLE:
			if((eKeyboardRead() == BUTTON_1)|(cOdebranyZnak == 'c'))
				eServo.eState = CALLIB;
			else if ((eKeyboardRead() == BUTTON_2)|(cOdebranyZnak == '1'))
				ServoGoTo(12);
			else if((eKeyboardRead() == BUTTON_3)|(cOdebranyZnak == '2'))
				ServoGoTo(24);	
			else if((eKeyboardRead() == BUTTON_4)|(cOdebranyZnak == '3'))
				ServoGoTo(36);
			else if(eKeyboardRead() == RELASED)
				eServo.eState = IDLE;
				
			break;
			
		case IN_PROGRESS:
			
		if (eServo.uiCurrentPosition == eServo.uiDesiredPosition)
					eServo.eState = IDLE;
		else if(eServo.uiCurrentPosition <= eServo.uiDesiredPosition){
					LedStepLeft();
					eServo.uiCurrentPosition ++;
				}
		else if(eServo.uiCurrentPosition >= eServo.uiDesiredPosition){
					LedStepRight();
					eServo.uiCurrentPosition --;  
				}
	
			break;
		}
}
***********************************************/

/**************** Zadanie 2 ********************
void Automat2(void){
	
	switch(eServo.eState){
		
		case CALLIB:
				ServoCallib();
			break;
			
		case IDLE:
			if(cOdebranyZnak == 'c')
				eServo.eState = CALLIB;
			else if (cOdebranyZnak == '1')
				ServoGoTo(12);
			else if(cOdebranyZnak == '2')
				ServoGoTo(24);	
			else if(cOdebranyZnak == '3')
				ServoGoTo(36);
			else if(eKeyboardRead() == RELASED)
				eServo.eState = IDLE;
				
			break;
			
		case IN_PROGRESS:
			
		if (eServo.uiCurrentPosition == eServo.uiDesiredPosition)
					eServo.eState = IDLE;
		else if(eServo.uiCurrentPosition <= eServo.uiDesiredPosition){
					LedStepLeft();
					eServo.uiCurrentPosition ++;
				}
		else if(eServo.uiCurrentPosition >= eServo.uiDesiredPosition){
					LedStepRight();
					eServo.uiCurrentPosition --;  
				}
	
			break;
		}
	Delay(100);
}
********************************************/

