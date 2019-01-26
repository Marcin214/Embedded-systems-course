#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"

#define UART_RX_BFFER_SIZE 20
char acBuffer[UART_RX_BFFER_SIZE];

void UartRx( void *pvParameters ){

	while(1){
		Uart_GetString(acBuffer) ;
		
		if (eCompareString(acBuffer, "zero") == EQUAL){
				LedToggle(0);
		}
		else if (eCompareString(acBuffer, "jeden") == EQUAL){
			LedToggle(1);
		}
	}
}


int main( void ){
	
	LedInit();
	UART_InitWithInt(9600);
	
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
