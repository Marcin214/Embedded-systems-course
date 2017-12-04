#include "uart.h"
#include "servo.h"
#include "watch.h"
#include "string.h"
#include "command_decoder.h"

#define MAX_SIZE 20

unsigned char fCalculatorChanged;
unsigned char fSendID;
unsigned char fSendNoCommand;
unsigned char fSendUnhandled;

extern Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern Watch sWatch;

char ucRxString[MAX_SIZE];
char ucTxString[MAX_SIZE];

int main(){
UART_InitWithInt(9600);
ServoInit(50);
WatchInit();

	while(1){

		if(eReciever_GetStatus() == READY){
			
			Reciever_GetStringCopy(ucRxString);
			DecodeMsg(ucRxString);
			
			if( ( ucTokenNr != 0 ) && ( asToken[0].eType == KEYWORD ) ){
         switch(asToken[0].uValue.eKeyword){
					 case ID:
					    fSendID = 1;
					 break;					 
           case CALC:			
							fCalculatorChanged = 1;
             break;    
					 case CALIB:
							ServoCallib();
						break;					
					 case GOTO:
					  	ServoGoTo(asToken[1].uValue.uiNumber);
					  break;
					 default:
						 fSendUnhandled = 1;
						 break;
					}		 
			}
			else{	
			  fSendNoCommand = 1;
			}
		}
		
		if(Transmiter_GetStatus()== FREE){
			
		 if(fSendID == 1 ){
			  CopyString("ID_Siri\n",ucTxString);	
				Transmiter_SendString(ucTxString);
				fSendID =0;
			}
			else if(fSendUnhandled== 1 ){
				CopyString("unhandled_command\n",ucTxString);
				Transmiter_SendString(ucTxString);
				fSendUnhandled =0;
			}
			else if(fSendNoCommand == 1 ){
				CopyString("no_command\n",ucTxString);
				Transmiter_SendString(ucTxString);
				fSendNoCommand =0;
			}
			else if(fCalculatorChanged ==1 ){
				asToken[1].uValue.uiNumber*= 2;
				CopyString("calc ",ucTxString);	
				AppendUIntToString(asToken[1].uValue.uiNumber,ucTxString);
				AppendString ("\n", ucTxString);				
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