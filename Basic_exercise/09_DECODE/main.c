#include "servo.h"
#include "uart.h"
#include "mystring.h"
#define MAX_SIZE 20

extern Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
char ucDestination[MAX_SIZE];

int main(){
	
UART_InitWithInt(9600);
ServoInit(50);

	
	while(1){
		if(eReciever_GetStatus() == READY){
			Reciever_GetStringCopy(ucDestination);
			DecodeMsg(ucDestination);
			
			if(ucTokenNr!=0&&asToken[0].eType==KEYWORD){
         switch(asToken[0].uValue.eKeyword){
           case CALLIB:
             ServoCallib();
             break;
            case GOTO:
             if(asToken[1].eType==NUMBER)
							ServoGoTo(asToken[1].uValue.uiNumber);
             break;
					}
			}	
		}
	}
}
