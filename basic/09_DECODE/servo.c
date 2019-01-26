#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include "mystring.h"
#include "string.h"

#define MAX_SIZE 10
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
 eServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition){
	eServo.uiDesiredPosition=uiPosition;
}


void Automat(void){
	
	switch(eServo.eState){
		case CALLIB:
				if(eReadDetector() == ACTIVE){
					eServo.uiCurrentPosition = 0;
					eServo.uiDesiredPosition = 0;  
					eServo.eState = IDLE;
					}
				else
					LedStepLeft();	
				break;
			
		case IDLE:
				if (eServo.uiCurrentPosition == eServo.uiDesiredPosition)
					eServo.eState = IDLE;
				else 
					eServo.eState = IN_PROGRESS;
				break;
			
		case IN_PROGRESS:
				if(eServo.uiCurrentPosition==eServo.uiDesiredPosition){
            eServo.eState = IDLE;
        }
        else if(eServo.uiCurrentPosition<eServo.uiDesiredPosition){
            LedStepRight();
            eServo.uiCurrentPosition++;
        }
        else{
            LedStepLeft();
            eServo.uiCurrentPosition--;
        }
        break;
		}
}