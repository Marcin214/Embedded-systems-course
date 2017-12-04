#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

typedef struct {
	unsigned char ucBlinkingFreq;
	unsigned char ucLedNumber;
}LedState;

TaskHandle_t xMyHandle;

//***********************************************************

void vLedBlink( void *pvParameters ){
	
	LedState *sLedStateParameters = ((LedState*)pvParameters);
	
	while(1){
		LedToggle(sLedStateParameters->ucLedNumber);
		vTaskDelay(1000/(sLedStateParameters->ucBlinkingFreq));	
	}
}
void vLedBlinkSuspend( void *pvParameters ){
	
	TaskHandle_t xHandle = *((TaskHandle_t*)pvParameters);
	
	while(1){
		vTaskDelay(1000);
		vTaskSuspend(xHandle);
		vTaskDelay(1000);
		vTaskResume(xHandle);
	}
}

//***********************************************************

int main( void ){
  
	LedState sLedState = {10, 0};
	LedInit();
	
	xTaskCreate(vLedBlink, NULL , 100 , &sLedState, 2 , &xMyHandle );
	xTaskCreate(vLedBlinkSuspend, NULL , 100 , &xMyHandle, 2 , NULL );
	vTaskStartScheduler();
	
	while(1);
}