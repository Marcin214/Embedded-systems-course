#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
void Automat(void);
//*************************************
#define DETEKTOR 0x00000400 // maska pinu P0.10
enum ServoState {CALLIB, IDLE, IN_PROGRESS};
enum DetectorState{ACTIVE,INACTIVE};

struct Servo{
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};
struct Servo eServo;

void DetectorInit(void); // ustawienie detektora jako wejscie
enum DetectorState eReadDetector(void); // zczytanie stanu DETEKTORA
void ServoInit(unsigned int uiServoFrequency);
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);
//****************************************************
void DetectorInit(void){
IO0DIR &= ~DETEKTOR;
}

enum DetectorState eReadDetector(void){
if(!(IO0PIN&DETEKTOR))
			return ACTIVE;
else 
	return INACTIVE;	
}

//*************************************
int main(){
	eServo.eState = CALLIB;
	LedInit();
	KeyboardInit();
	
	while(1){
		Timer0Interrupts_Init(20000,&Automat);
		}
	}
//*************************************
void Automat(void){
	
	switch(eServo.eState){
		
		case CALLIB:
				if(eReadDetector() == INACTIVE)
					LedStepLeft();	
				else if(eReadDetector() == ACTIVE){
					eServo.uiCurrentPosition = 0;
					eServo.uiDesiredPosition = 0;  
					eServo.eState = IDLE;
				}
			break;
			
		case IDLE:
			if(eKeyboardRead() == BUTTON_1)
				eServo.eState = CALLIB;
			else if(eKeyboardRead() == BUTTON_2){
				eServo.uiDesiredPosition = 12;
				eServo.eState = IN_PROGRESS;
				}
			else if(eKeyboardRead() == BUTTON_3){
				eServo.uiDesiredPosition = 24;
				eServo.eState = IN_PROGRESS;
				}
			else if(eKeyboardRead() == BUTTON_4){
				eServo.uiDesiredPosition = 36;
				eServo.eState = IN_PROGRESS;
				}
			else if(eKeyboardRead() == RELASED)
				eServo.eState = IDLE;
				
			break;
			
		case IN_PROGRESS:
		 if (eServo.uiCurrentPosition == eServo.uiDesiredPosition)
					eServo.eState = IDLE;
		 else if(eServo.uiCurrentPosition <= eServo.uiDesiredPosition){
					LedStepLeft();
					eServo.uiCurrentPosition ++;
			 eServo.eState = IN_PROGRESS;
				}
			else if(eServo.uiCurrentPosition >= eServo.uiDesiredPosition){
					LedStepRight();
					eServo.uiCurrentPosition --;  
				eServo.eState = IN_PROGRESS;
				}
			break;
		}
}
