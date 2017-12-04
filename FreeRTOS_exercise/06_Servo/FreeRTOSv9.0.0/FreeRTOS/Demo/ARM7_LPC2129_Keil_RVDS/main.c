#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "servo.h"
#include "led.h"
#include "mystring.h"
#include "keyboard.h"


int main( void ){
	LedInit();
	KeyboardInit();
 
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
	xTaskCreate(Automats, NULL, 128, NULL, 2, NULL );

	vTaskStartScheduler();
	
	while(1);
}
