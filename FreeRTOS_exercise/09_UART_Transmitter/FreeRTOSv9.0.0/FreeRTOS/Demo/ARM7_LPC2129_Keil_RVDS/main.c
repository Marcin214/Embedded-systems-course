#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"

#define UART_RX_BFFER_SIZE 20
char acBuffer[UART_RX_BFFER_SIZE] = "0123456789\n";

void UartRx( void *pvParameters ){

	while(1){
		vTaskDelay(500);
		Uart_PutString(acBuffer);
		vTaskDelay(10);
		Uart_PutString(acBuffer);
		vTaskDelay(10);
		Uart_PutString(acBuffer);
	}
}


int main( void ){
	
	LedInit();
	UART_InitWithInt(9600);
	
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
