// *** CAN Adv ***
// Zadanie 1
// Doprowadzic do dzialania ponizszy program
// Stan ledów powinien odzwierciedlac stan licznika tylko w przypadku nacisnietego przycisku
// Uzyc Acceptance Filter

#include "led.h"
#include "keyboard.h"
#include "can.h"

#define ID 17

int main(){
	unsigned char ucMsgCtr=0;
	LedInit();
	Can1_InitAsTX();
	Can2_InitAsRX(ID);
  while(1){ 
		unsigned int 	uiID;
		if (eKeyboardRead()==BUTTON_1) uiID=ID; else uiID=0;
		
		if (ucCan1_TxFree())  Can1_SendByte(uiID,ucMsgCtr++);
		if (ucCan2_RxReady()) Led_ShowByte(ucCan2_ReceiveByte());
	}
}
