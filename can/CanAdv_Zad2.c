// CAN Advanced
// Zadanie 2
// Doprowadzic do dzialania ponizsze programy
// Test przeprowadzic laczac 3 zestawy uruchomieniowe
// ID odbiornika okresla stan przycisku w trakcie resetu

#include "led.h"
#include "keyboard.h"
#include "can.h"

#define EVM_A_ID 17
#define EVM_B_ID 18

int main(){
	unsigned int 	uiID;
	unsigned char ucMsgCtr=0;
	
	Led_Init();
	Keyboard_Init();
	
	Can1_InitAsTx();
	
	if (eKeyboard_Read()==BUTTON_1) uiID=EVM_B_ID; else uiID=EVM_A_ID;
  Can2_InitAsRx(uiID);
	
  while(1){ 
		unsigned int 	uiID;
		
		if (eKeyboard_Read()==BUTTON_1) uiID=EVM_B_ID; else uiID=EVM_A_ID;
		if (ucCan1_TxFree()) Can1_SendByte(uiID,ucMsgCtr++);
		
		if (ucCan2_RxReady()) Led_ShowByte(ucCan2_ReceiveByte());		
}
}

