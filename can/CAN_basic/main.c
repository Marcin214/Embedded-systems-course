#include <LPC21xx.H>
#include "led.h"
#include "can.h"
#include "delay.h"

int main (){
	
	unsigned char ucMsgCtr=0;
	
	LedInit();
	Can1_InitAsTX();
	Can2_InitAsRX();
	
    while(1){
          if (ucCan1_TxFree())  Can1_SendByte(ucMsgCtr++);
					if (ucCan2_RxReady()) Led_ShowByte(ucCan2_ReceiveByte());	
		}
	
}

