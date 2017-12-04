#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

//***********************************************************
xSemaphoreHandle xSemaphore;
//***********************************************************

void PulseLED0( void *pvParameters ){
	
	while(1){
		
		if( xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE ){
			LedOn(0);
			vTaskDelay(100);
			LedOff(0);
		}
	}
}

void PulseTrigger( void *pvParameters ){
	
	vTaskDelay(1000/3);   
	
	while(1){
		xSemaphoreGive(xSemaphore);  // jesli zmienimy priorytet tego tasku na nizszy, 
		vTaskDelay(1000/3);          // sygnal bedzie wyslany przed opóznieniem 
	}
}

void PulseLED1( void *pvParameters ){
	while(1){
		if( xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE ){
			LedOn(1);
			vTaskDelay(100);
			LedOff(1);
		}
	}
}
//***********************************************************
int main( void ){

	LedInit();
	
	vSemaphoreCreateBinary( xSemaphore );
	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	
	xTaskCreate(PulseTrigger, NULL , 100 , NULL, 2 , NULL );
	xTaskCreate(PulseLED0, NULL , 100 , NULL, 2 , NULL );
	xTaskCreate(PulseLED1, NULL , 100 , NULL, 2 , NULL );
	
	vTaskStartScheduler();
	
	while(1);
}