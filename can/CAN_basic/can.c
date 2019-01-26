#include <LPC21xx.H>
#include "can.h"


void Can1_InitAsTX(void){

  PINSEL1			|= 0x00040000;
  C1MOD				=  0x00000001;
  C1BTR				=  0x001C001D;		
	C1MOD 			=  0x00000000;	
	
}
void Can2_InitAsRX(void){
	PINSEL1			|= 0x00014000;
  C2MOD				=  0x00000001;
  C2BTR				=  0x001C001D;		
	AFMR 				= 0x02;	
	C2CMR = 0x00000004;
  C2MOD				=  0x00;		
}

void Can1_SendByte(unsigned char ucValue){ 
	
	C1TFI1 = 0x00010000;			// Set DLC to 1 bytes 	
  C1TDA1 = ucValue;					// Copy  ucValue into firstbytes
  C1CMR  = 0x01;			// Transmit the message
}


unsigned char ucCan2_ReceiveByte(void){
 	unsigned char ucValue;
 
	ucValue = C2RDA; 
	C2CMR = 0x00000004;
  return ucValue;
}

unsigned char  ucCan1_TxFree(void){
	
 if(C1SR & 0x00000004)
   return 1;

 return 0;
	
}

unsigned char  ucCan2_RxReady(void){
	
 if (C2SR & 0x00000001)
	  return 1;
 return 0;
 }
