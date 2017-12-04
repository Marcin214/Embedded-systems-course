#include <LPC21xx.H>
#include "uart.h"
#include "watch.h"
#include "mystring.h"
#include "command_decoder.h"
#include "timer_interrupts.h"

#define MAX_SIZE 20

unsigned char fCalculatorChanged;

extern Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern TransmiterBuffer sTransmiterBuffer;
extern Watch sWatch;

char ucRxString[MAX_SIZE];
char ucTxString[MAX_SIZE];

int main(){
UART_InitWithInt(9600);
Timer0Interrupts_Init(1000000,WatchUpdate); //co sekunde przerwanie

	while(1){

		if(eReciever_GetStatus() == READY){
			Reciever_GetStringCopy(ucRxString);
			DecodeMsg(ucRxString);
			if(ucTokenNr!=0&&asToken[0].eType==KEYWORD){
         switch(asToken[0].uValue.eKeyword){
           case CALC:			
							asToken[1].uValue.uiNumber*= 2;
							CopyString("calc ",ucTxString);	
							AppendUIntToString(asToken[1].uValue.uiNumber,ucTxString);
							AppendString ("\n", ucTxString);
							fCalculatorChanged = 1;
             break;    
					}
			}	
		}
		
		if(Transmiter_GetStatus()== FREE){
			
			if(fCalculatorChanged ==1 ){
				Transmiter_SendString(ucTxString);
				fCalculatorChanged =0;
			}
			else if(sWatch.fMinutesValueChanged ==1 ){	
					sWatch.fMinutesValueChanged= 0;			
          CopyString("min ",ucTxString);				
					AppendUIntToString (sWatch.ucMinutes, ucTxString);
					AppendString ("\n", ucTxString);
					Transmiter_SendString(ucTxString);
				}
				else if(sWatch.fSeccondsValueChanged ==1 ){	
					sWatch.fSeccondsValueChanged = 0;			
					CopyString("sec ",ucTxString);						
					AppendUIntToString (sWatch.ucSecconds, ucTxString);	
					AppendString ("\n", ucTxString);				
					Transmiter_SendString(ucTxString);
				}
		}			
	}
}