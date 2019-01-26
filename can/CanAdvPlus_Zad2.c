// CAN Adv Plus Zadanie 1
//
// Doprowadzic do dzialania ponizszy kod
// Wyjsc z programu "CAN Adv Plus Zadanie 1" (czyli praca bez identyfikatorów)
// Pozostac przy wysyBaniu 1 bajt/message
// Przeprowadzic równiez testy ngatywne czyli "zepsuc na chwile kod nadajnika/odbiornika"

#include "can.h"
#include "uart.h"

char acTxStr[30];
char acRxStr[30];

int main(){
	UART_Init(9600);
	Can1_InitAsTx(acTxStr);
	Can2_InitAsRx(acRxStr);
	while(1){
		if (eReciever_GetStatus()==READY) {
			Reciever_GetStringCopy(acTxStr);
			Can1_SendString();
		}
		if (ucCan2_RxReady()) {
			Transmiter_SendString(acRxStr);
		}
	}
}
