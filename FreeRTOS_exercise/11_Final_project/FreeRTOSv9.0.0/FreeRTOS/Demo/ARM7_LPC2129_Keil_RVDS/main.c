#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "led.h"
#include "uart.h"
#include "string.h"
#include "servo.h"
#include "keyboard.h"
#include "command_decoder.h"

#define MAX_SIZE 20

char ucTxString[MAX_SIZE];
char ucRxString[MAX_SIZE];
extern Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

QueueHandle_t xQueueGeneral;

void Button_Thread (void *pvParameters){
	
	while(1){
		
    switch(eKeyboard_Read()){
			case BUTTON_1:
        xQueueSendToBack( xQueueGeneral, "button 0x0001", 0 );			
				break;
			case BUTTON_2:
				xQueueSendToBack( xQueueGeneral, "button 0x0002", 0 );	
				break;
			case BUTTON_3:
        xQueueSendToBack( xQueueGeneral, "button 0x0003", 0 );					
				break;
			case BUTTON_4:
        xQueueSendToBack( xQueueGeneral, "button 0x0004", 0 );					
				break;
			case RELASED:
				break;
			}
	}
}

void Receiver_Thread (void *pvParameters){
	
	while(1){
		Uart_GetString(ucRxString);
		xQueueSendToBack( xQueueGeneral, &ucRxString, 0 );	
	}
}

void Executor(void *pvParameters){
	
struct ServoStatus sServoStatus;
	
	while(1){
	xQueueReceive( xQueueGeneral, &ucRxString, portMAX_DELAY);
	DecodeMsg(ucRxString);
		if( ( ucTokenNr != 0 ) && ( asToken[0].eType == KEYWORD ) ){
			switch(asToken[0].uValue.eKeyword){
				case ID:
					Uart_PutString("id\n");
					break;
				case BUTTON:
					Servo_Wait(5000);
					Uart_PutString("wait 5sec\n");
					break;
				case CALIB:
					Servo_Callib();
					Uart_PutString("ok\n");
					break;					
				case GOTO:
					Servo_GoTo(asToken[1].uValue.uiNumber);
					Uart_PutString("ok\n");
					break;
				case STATE:
					sServoStatus = Servo_State();
					switch (sServoStatus.eState){
						case CALLIB: CopyString("state callib ",ucTxString); break;
						case IDLE : CopyString("state iddle ",ucTxString); break;
						case IN_PROGRESS : CopyString("state in_prog ",ucTxString); break;
						case WAIT : CopyString("state wait ",ucTxString); break;
						default:break;
					  }
					AppendUIntToString(sServoStatus.uiCurrentPosition,ucTxString);
					AppendString("\n",ucTxString);
					Uart_PutString(ucTxString);
					break;
			}
		}					
	}
}


int main( void ){
	
	Keyboard_Init();
	LedInit();
	UART_InitWithInt(9600);

	xQueueGeneral = xQueueCreate( 5, sizeof( char[20] ) );
  
	xTaskCreate( Automats, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Executor, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Keyboard_Thread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Button_Thread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Receiver_Thread, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	
	while(1);
}
