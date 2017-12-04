#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

TaskHandle_t xMyHandle;

typedef struct {
	unsigned char ucBlinkingFreq;
	unsigned char ucLedNumber;
}LedState;

//***********************************************************

void LedBlink( void *pvParameters ){
	LedState *sLedStateParameters = ((LedState*)pvParameters);
	while(1){
		LedToggle(sLedStateParameters->ucLedNumber);
		vTaskDelay(1000/(sLedStateParameters->ucBlinkingFreq));
		
	}
}

void LedCtrl( void *pvParameters ){

		while(1){
			((LedState*)pvParameters)->ucBlinkingFreq %=20;
			((LedState*)pvParameters)->ucBlinkingFreq +=2;
			vTaskDelay(1000);
			LedOff(((LedState*)pvParameters)->ucLedNumber);
			((LedState*)pvParameters)->ucLedNumber++;
			(((LedState*)pvParameters)->ucLedNumber)%=4;
			((LedState*)pvParameters)->ucBlinkingFreq %=20;
			((LedState*)pvParameters)->ucBlinkingFreq +=2;
			vTaskDelay(1000);
	}
}



void vSuspendTask( void *pvParameters ){
	TaskHandle_t* xHandle = ((TaskHandle_t*)pvParameters);

	while(1){
		
		vTaskSuspend(*xHandle);
		vTaskDelay(1000);
		vTaskResume(*xHandle);
		vTaskDelay(1000);
	}
}

	
//***********************************************************

int main( void ){
  
	LedState sLedState = {1, 0};
	LedInit();
	
	xTaskCreate(LedBlink, NULL , 100 , &sLedState, 2 , &xMyHandle  );
	xTaskCreate(LedCtrl, NULL , 100 , &sLedState, 2 , NULL );
	//xTaskCreate(vSuspendTask, NULL , 100 , &xMyHandle, 2 , NULL );
	vTaskStartScheduler();
	
	while(1);
}