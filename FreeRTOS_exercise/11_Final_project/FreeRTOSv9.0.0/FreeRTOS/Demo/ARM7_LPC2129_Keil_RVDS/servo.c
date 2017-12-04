#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#define MAX_SIZE_QUEUE     12

#define MAX_SIZE     10
#define DETEKTOR (1<<10)   // maska pinu P0.10


struct ServoStatus eServo;

QueueHandle_t xQueue;
QueueHandle_t xStatusQueue;

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
  Keyboard_Init();
//	Timer1Interrupts_Init(uiMicroseconds,&Automat);
}

void Automats(void *pvParameters){
	
	Data asDataToRecieve;
	unsigned int ucSpeed = 500;
	xQueue = xQueueCreate( MAX_SIZE_QUEUE , sizeof(Data) );
	xStatusQueue = xQueueCreate( 1 , sizeof(struct ServoStatus) );

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
							case _GOTO: eServo.uiDesiredPosition = asDataToRecieve.uiValue; break;
							case _WAIT: eServo.eState = WAIT;
						              xQueueOverwrite( xStatusQueue, &eServo );
													vTaskDelay(asDataToRecieve.uiValue); 
													break;
							case _SPEED: ucSpeed = (32/asDataToRecieve.uiValue); break;		
					}
						
				}
				else {
					eServo.eState = IN_PROGRESS;
				}
				break;

		case WAIT:
				eServo.eState = IDLE;			
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
	xQueueOverwrite( xStatusQueue, &eServo );
	vTaskDelay(ucSpeed);
	}
}

void Servo_Callib(void){
 	Data asDataToSend = {_CALLIB, 0};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_GoTo(unsigned int uiPosition){
	Data asDataToSend = {_GOTO, uiPosition};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_Wait(unsigned int uiTimeToWait){
	Data asDataToSend = {_WAIT, uiTimeToWait};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}

void Servo_Speed(unsigned int uiServoSpeed){
	Data asDataToSend = {_SPEED, uiServoSpeed};
	xQueueSendToBack( xQueue, &asDataToSend , 0 );
}
struct ServoStatus Servo_State(void){
	struct ServoStatus sServoStatus;
	xQueuePeek(xStatusQueue,&sServoStatus,portMAX_DELAY);
	return sServoStatus;
}