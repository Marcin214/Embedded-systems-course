#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "led.h"
#include "mystring.h"
#include "uart.h"
#include "keyboard.h"

//***********************************************************
xSemaphoreHandle xSemaphore;
//***********************************************************

char ucTxString[20];

void Rtos_Transmiter_SendString(char pcSource[]){
	TickType_t xExecutionTime;
	xExecutionTime = pdMS_TO_TICKS(xTaskGetTickCount());
	
	if( xSemaphoreTake(xSemaphore,NULL) == pdTRUE ){
		
		CopyString(pcSource,ucTxString);	
		AppendUIntToString(xExecutionTime,ucTxString);
		AppendString ("\n", ucTxString);
		
		Transmiter_SendString(ucTxString);
		while (Transmiter_GetStatus()!=FREE){}
			
		xSemaphoreGive(xSemaphore); 
		}
}
// 
void LettersTx (void *pvParameters){
	
	while(1){	
		Rtos_Transmiter_SendString("-ABCDEEFGH-:");	
		vTaskDelay(20);
	}
}

void KeyboardTx (void *pvParameters){
	while(1){			
		if(eKeyboardRead() != RELASED){	
				Rtos_Transmiter_SendString("-KeyboardTx-:");	
				vTaskDelay(20);
		}				
	}				
}

int main( void ){
	
	KeyboardInit();
	UART_InitWithInt(2400);
	
	 vSemaphoreCreateBinary( xSemaphore );
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 2, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 2, NULL );
	vTaskStartScheduler();
	
	while(1);
}
