#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include "mystring.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#define MAX_SIZE_QUEUE     12

#define MAX_SIZE     10
#define DETEKTOR (1<<10)   // maska pinu P0.10
QueueHandle_t xQueue;


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
	Timer1Interrupts_Init(uiMicroseconds,&Automat);
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

void Automats(void *pvParameters){
	
	Data asDataToRecieve;
	unsigned char ucSpeed = 10;
	xQueue = xQueueCreate( MAX_SIZE_QUEUE , sizeof(Data) );

	while(1){
		
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
				if (eServo.uiCurrentPosition == eServo.uiDesiredPosition){
					
					xQueueReceive( xQueue, &asDataToRecieve, portMAX_DELAY );
					
					switch(asDataToRecieve.eDataSource){
							case _CALLIB: eServo.eState = CALLIB; break;
							case _GOTO: eServo.uiDesiredPosition = asDataToRecieve.uDataValue.ucValue; break;
							case _WAIT: vTaskDelay(asDataToRecieve.uDataValue.uiValue); break;
							case _SPEED: ucSpeed = (32/asDataToRecieve.uDataValue.ucValue); break;		
					}
					
				}
				else {
					eServo.eState = IN_PROGRESS;
				}
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
	vTaskDelay(ucSpeed);
	}
}

void Servo_Callib(void){
 	Data asDataToSend = {_CALLIB, 0};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_GoTo(unsigned char ucPosition){
	Data asDataToSend = {_GOTO, ucPosition};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_Wait(unsigned int uiTimeToWait){
	Data asDataToSend = {_WAIT, uiTimeToWait};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_Speed(unsigned char ucServoSpeed){
	Data asDataToSend = {_SPEED, ucServoSpeed};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}