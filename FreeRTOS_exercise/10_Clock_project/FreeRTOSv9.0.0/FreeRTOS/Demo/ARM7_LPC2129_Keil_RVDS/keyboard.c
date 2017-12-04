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


void Keyboard_Init(void){
xQueueKEYBOARD = xQueueCreate( 1, sizeof(enum KeyboardState) );
IO0DIR &= ~( BUTT1|BUTT2|BUTT3|BUTT4);

}


void Keyboard_Thread (void *pvParameters){
	
	enum KeyboardState eCurrentValue;
	enum KeyboardState ePreviousValue;
	
	while(1){
		
		eCurrentValue =eReadButtons();
		
		if((ePreviousValue != eCurrentValue) && (eCurrentValue != RELASED)){
			xQueueSendToBack( xQueueKEYBOARD , &eCurrentValue, NULL);
			}
		ePreviousValue = eCurrentValue;
	  vTaskDelay(100);
	}
}

enum KeyboardState eKeyboard_Read(void){
	enum KeyboardState  eKeyboardStateFromQueue;
	xQueueReceive( xQueueKEYBOARD, &eKeyboardStateFromQueue, portMAX_DELAY);
	return eKeyboardStateFromQueue;
}
