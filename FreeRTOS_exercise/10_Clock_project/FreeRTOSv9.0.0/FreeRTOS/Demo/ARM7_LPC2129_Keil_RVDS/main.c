#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "watch.h"

#define MAX_SIZE 20

char ucTxString[MAX_SIZE];


void Button_Thread (void *pvParameters){
	
	while(1){
		
    switch(eKeyboard_Read()){
			case BUTTON_1:
				LedToggle(0);
				Uart_PutString("button 0x0001\n");			
				break;
			case BUTTON_2:
				LedToggle(1);
				Uart_PutString("button 0x0002\n");
				break;
			case BUTTON_3:
        Uart_PutString("button 0x0003\n");				
				break;
			case BUTTON_4:
        Uart_PutString("button 0x0004\n");				
				break;
			case RELASED:
				break;
			}
	}
}
		
void Clock_Thread (void *pvParameters){
	
	WatchEvent sWatchCurrent;
	
	while(1){
		
		sWatchCurrent = sWatch_Read();
    switch(sWatchCurrent.eTimeUnit){
			case SECCONDS:
				LedToggle(0);	
				CopyString("sec ",ucTxString);				
				AppendUIntToString (sWatchCurrent.TimeValue, ucTxString);
				AppendString ("\n", ucTxString);
				Uart_PutString(ucTxString);
				break;
			case MINUTES:
				LedToggle(1);	
			  CopyString("min ",ucTxString);				
				AppendUIntToString (sWatchCurrent.TimeValue, ucTxString);
				AppendString ("\n", ucTxString);
				Uart_PutString(ucTxString);
				break;	
		}
	}
}

int main( void ){
	
	Keyboard_Init();
	LedInit();
	Watch_Init();
	UART_InitWithInt(9600);

	
	xTaskCreate( Keyboard_Thread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Button_Thread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Clock_Thread, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
