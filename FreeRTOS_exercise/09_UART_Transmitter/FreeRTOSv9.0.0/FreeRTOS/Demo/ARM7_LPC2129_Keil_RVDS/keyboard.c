#include <LPC21xx.H>
#include "keyboard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "servo.h"

#define BUTT1   (1<<4)
#define BUTT2   (1<<6)
#define BUTT3   (1<<5)
#define BUTT4   (1<<7)

QueueHandle_t xQueueKEYBOARD;

enum KeyboardState eReadButtons(void){

  if((IO0PIN&BUTT1) == 0){ 
			return BUTTON_1;
	}
  else if((IO0PIN&BUTT2) == 0){ 
			return BUTTON_2;
	}
	
 else if((IO0PIN&BUTT3) == 0){ 
			return BUTTON_3;
	}
 else if((IO0PIN&BUTT4) == 0){ 
			return BUTTON_4;
	}
  return RELASED;
}


void KeyboardInit(void){
xQueueKEYBOARD = xQueueCreate( 1, sizeof(enum KeyboardState ) );
IO0DIR &= ~( BUTT1|BUTT2|BUTT3|BUTT4);

}

void Keyboard (void *pvParameters){
	
	enum KeyboardState eCurrentValue;
	enum KeyboardState ePreviousValue;
	
	while(1){
		
		eCurrentValue =eReadButtons();
		
		if((ePreviousValue != eCurrentValue) && (eCurrentValue != RELASED)){
			
			switch(eCurrentValue){
					case BUTTON_1: Servo_Callib();
				break;
				case BUTTON_2: Servo_GoTo(12);
					break;
				case BUTTON_3: Servo_GoTo(24);
					break;
				case BUTTON_4:
					Servo_Speed(8);
          Servo_GoTo(12);
				  Servo_Wait(200);
					Servo_GoTo(0);
					Servo_Speed(4);
					Servo_GoTo(24);
				  Servo_Wait(400);
					Servo_GoTo(0);
				  Servo_Speed(2);
					Servo_GoTo(36);
				  Servo_Wait(600);
				  Servo_Speed(1);
				  Servo_GoTo(0);
					break;
				default:
					break;
				}	
			}
		ePreviousValue = eCurrentValue;
	  vTaskDelay(100);
	}
}

