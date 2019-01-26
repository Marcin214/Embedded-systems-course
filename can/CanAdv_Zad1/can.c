#include <LPC21xx.H>
#include "can.h"


void Can1_InitAsTX(void){

  PINSEL1			|= 0x00040000;
  C1MOD				=  0x00000001;
  C1BTR				=  0x001C001D;		
	C1MOD 			=  0x00000000;	
	
}
void Can2_InitAsRX(unsigned char ucID){
	unsigned int * StandardFilter0;
	StandardFilter0  = (unsigned int * )0xE0038000;
	
	PINSEL1			|= 0x00014000;
  C2MOD				=  0x00000001;
  C2BTR				=  0x001C001D;		
	C2CMR       = 0x00000004;
  C2MOD				=  0x00;		

	AFMR = 0x01;	
	* StandardFilter0 = ((ucID << 16) | 0x4000FFFF);
	SFF_sa = 0x00000000;
	SFF_GRP_sa = 0x00000004;
	EFF_sa = 0x00000004;
	EFF_GRP_sa = 0x00000004;
	ENDofTable = 0x00000004;
	AFMR = 0x00;	
}

void Can1_SendByte(unsigned char ucID, unsigned char ucValue){ 
	
	C1TFI1 = 0x00010000;	
	C1TID1 = ucID;
  C1TDA1 = ucValue;	
  C1CMR  = 0x01;			
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
