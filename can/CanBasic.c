// Doprowadzić do działania poniższy program
// Dodać funkcję Led_ShowByte do mudulu led (2 linijki kodu)
// Wykorzystać rejestry PINSEL, MOD, BTR, TFI1, AFMR, CMR.
// Użyć bittimingu z przykładu hitexa
// Wyłączyć filtrowanie komunikatów
// UWAGI: 
// - Nie dawać komentarzy w kodzie 
// - Nie używać przerwań
// - Należy pamiętać że nadajnik CAN sygnalizuje wysłanie komunikatu tylko wtedy kiedy któryś z odbiorników potwierdził odbiór.
// 
// Test1: Połączyć  CAN1 i CAN2 tego samego EVM-a
// Test2: Połaczyc CANy dwóch róznyh EVM-ów.
//        - które CAN-y ? 
//        - co i gdzie powinno sie wyswietlać ?
//        - co sie dzieje kiedy zatrzymamy nadajnik resetem ?
// Test3: To samo co poprzednio tylko z trzema EVM-ami

#include "can.h"
main (){
	unsigned char ucMsgCtr=0;
	Led_Init();
    Can1_InitAsTX();
    Can2_InitAsRX();
    while(1){
        if (ucCan1_TxFree())  Can1_SendByte(ucMsgCtr++);
        if (ucCan2_RxReady()) Led_ShowByte(ucCan2_ReceiveByte());
    }
}