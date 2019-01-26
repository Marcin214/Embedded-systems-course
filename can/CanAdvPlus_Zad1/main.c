// CAN Adv Plus Zadanie 1
//
// Doprowadzic do dzialania ponizszy kod
// Wyjsc z programu Can Basic (czyli praca bez identyfikatorów)
// Pozostac przy wysyBaniu 1 bajt/message
// Przeprowadzic równiez testy ngatywne czyli "zepsuc na chwile kod nadajnika/odbiornika"

#include "led.h"
#include "can.h"
#include "string.h"

char acTxStr[30],acRxStr[30];

int main(){
	LedInit();
	
	Can1_InitAsTX(acTxStr);
	Can2_InitAsRX(acRxStr);
	
	CopyString("12345",acTxStr);
	Can1_SendString(); 
  while(ucCan2_RxReady()==0){}
	if (eCompareString(acTxStr,acRxStr)==EQUAL) Led_ShowByte(0xF0);else Led_ShowByte(0x55);
		
	CopyString("bcdef",acTxStr);	
	while(ucCan2_RxReady()==0){}
	if (eCompareString(acTxStr,acRxStr)==EQUAL) Led_ShowByte(0xF0);else Led_ShowByte(0x01);
	while(1){}	
}
