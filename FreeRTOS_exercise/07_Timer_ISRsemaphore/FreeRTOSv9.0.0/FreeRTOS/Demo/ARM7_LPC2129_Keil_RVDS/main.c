#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "led.h"
#include "timer_interrupts.h"

xSemaphoreHandle xBinarySemaphore;

void LedBlink( void *pvParameters ){

	while(1){
		xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
		LedToggle(0);	
	}
}
void vSemaphoreHandlerFromISR(void){
	
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	
	xSemaphoreGiveFromISR(xBinarySemaphore,&xHigherPriorityTaskWoken);
}

int main(void){
	
	LedInit();
	vSemaphoreCreateBinary(xBinarySemaphore);
	Timer1Interrupts_Init(500000, &vSemaphoreHandlerFromISR);
	
	xTaskCreate(LedBlink, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}

