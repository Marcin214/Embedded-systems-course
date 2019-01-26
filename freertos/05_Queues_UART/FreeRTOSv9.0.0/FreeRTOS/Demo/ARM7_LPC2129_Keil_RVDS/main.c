#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "led.h"
#include "mystring.h"
#include "uart.h"
#include "keyboard.h"

#define MAX_SIZE 20


char ucTxString[MAX_SIZE];

QueueHandle_t xQueue;


void Rtos_Transmiter_SendString(void *pvParameters){
	
	TickType_t xExecutionTime = pdMS_TO_TICKS(xTaskGetTickCount());
  BaseType_t xStatus;
	
	/* Declare the variable that will hold the values received from the queue. */
	char cValueToRecieve[MAX_SIZE];
	
	while(1){
		xStatus = xQueueReceive( xQueue, cValueToRecieve, portMAX_DELAY );
		
		if( xStatus == pdPASS ){
			CopyString(cValueToRecieve,ucTxString);	
			AppendUIntToString(xExecutionTime,ucTxString);
			AppendString ("\n", ucTxString);
		
			Transmiter_SendString(ucTxString);
			while (Transmiter_GetStatus()!=FREE){}
	  }
	
	}		
}




void LettersTx (void *pvParameters){
	
	char* cValueToSend;
  BaseType_t xStatus;
	
	cValueToSend = (( char* )pvParameters);
	
	while(1){	
		xStatus = xQueueSendToBack( xQueue, cValueToSend , 0 );
		
		if( xStatus != pdPASS ){
		LedToggle(0);
		}
	}
}

void KeyboardTx (void *pvParameters){
	
	char* cValueToSend;
  BaseType_t xStatus;
	
	cValueToSend = (( char* )pvParameters);
	
	while(1){			
		if(eKeyboardRead() != RELASED){	
				xStatus = xQueueSendToBack( xQueue, cValueToSend , 0 );
		}				
	}				
}

int main( void ){
	LedInit();
	KeyboardInit();
	UART_InitWithInt(2400);
	
	/* The queue is created to hold a maximum of 5 values, each of which is
  large enough to hold a variable of type int32_t. */
	 xQueue = xQueueCreate( 5, sizeof( char[20] ) );
	

	xTaskCreate(LettersTx, NULL, 128, "-ABCDEEFGH-:", 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, "-KeyboardTx-:", 1, NULL );
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	
	while(1);
}
