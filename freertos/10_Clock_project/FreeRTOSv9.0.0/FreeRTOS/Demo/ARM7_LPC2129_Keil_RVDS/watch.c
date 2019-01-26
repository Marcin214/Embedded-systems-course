#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "watch.h"
#include "timer_interrupts.h"


/********************* Global Variables ************************/
xSemaphoreHandle xBinarySemaphoreClock;

WatchEvent sSeccondsValue = {SECCONDS, 0};
WatchEvent sMinutesValue =  {MINUTES, 0};
/******************** Function Definition **********************/
void WatchUpdate(void){
	
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	
	sSeccondsValue.TimeValue++;
	xSemaphoreGiveFromISR(xBinarySemaphoreClock,&xHigherPriorityTaskWoken);
}

void Watch_Init(void){
	vSemaphoreCreateBinary(xBinarySemaphoreClock);
	xSemaphoreTake(xBinarySemaphoreClock,portMAX_DELAY);
	Timer1Interrupts_Init(500000, WatchUpdate);
}
	

WatchEvent sWatch_Read(void){
	
	xSemaphoreTake(xBinarySemaphoreClock,portMAX_DELAY);
	if(sSeccondsValue.TimeValue>5){
		sSeccondsValue.TimeValue = 0;
		sMinutesValue.TimeValue++;
		xSemaphoreGive(xBinarySemaphoreClock);
		return sMinutesValue;
	}
	return sSeccondsValue;
}


