#include <LPC21xx.H>
#include "can.h"
#include "led.h"
#include "delay.h"

#define NULL 0

unsigned char ucRxReady = 0;

char *ucRxString;
char *ucTxString;
		
		
void Can1_SendString(void){
	C1TFI1 = 0x00010000;			 	
  C1TDA1 = *ucTxString++;		
  C1CMR  = 0x30;		 	
}
		
void CAN1IRQ (void) __irq{
	
	if (NULL == *ucTxString){
		ucTxString = NULL;
	}
	else{
		C1TFI1 = 0x00010000;			 	
		C1TDA1 = *ucTxString++;		
		C1CMR  = 0x01;
	}
	
  VICVectAddr = 0x00000000;			 
}		
		
void CAN2IRQ (void) __irq{

	ucRxReady = 1;
	*ucRxString = C2RDA; 
	C2CMR = 0x00000004;
	
  VICVectAddr = 0x00000000;					// Signal the end of interrupt
}

void Can1_InitAsTX(char acTxBuffer[]){

	ucTxString = acTxBuffer;
	
  PINSEL1			|= 0x00040000;
  C1MOD				=  0x00000001;
  C1BTR				=  0x001C001D;	

  C1IER				    =  0x00000002;			  // Enable the Receive interrupt 
  VICVectCntl1		=  0x00000034;  	  	// Select a priority slot for a given interrupt
  VICVectAddr1		=  (unsigned)CAN1IRQ;	// Pass the address of the IRQ into the VIC slot
  VICIntEnable		=  0x00100000;		  	// Enable interrupt
	
	C1MOD 			=  0x00000000;	
	
}
void Can2_InitAsRX(char acRxBuffer[]){
	
	ucRxString = acRxBuffer;
	
	PINSEL1			|= 0x00014000;
  C2MOD				=  0x00000001;
  C2BTR				=  0x001C001D;		
	C2CMR       =  0x00000004;
	
	C2IER				    =  0x00000001;			  // Enable the Receive interrupt 
  VICVectCntl0		=  0x0000003B;  	  	// Select a priority slot for a given interrupt
  VICVectAddr0		=  (unsigned)CAN2IRQ;	// Pass the address of the IRQ into the VIC slot
  VICIntEnable		=  0x08000000;		  	// Enable interrupt
	
	AFMR 				= 0x02;	
  C2MOD				=  0x00;		
}

void Can1_SendByte(unsigned char ucValue){ 
	
	C1TFI1 = 0x00010000;			// Set DLC to 1 bytes 	
  C1TDA1 = ucValue;					// Copy  ucValue into firstbytes
  C1CMR  = 0x01;		      	// Transmit the message
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


 unsigned char ucCan2_RxReady(void){
	 
	unsigned char ucResult;
	ucResult=ucRxReady;
	if (ucResult == 1)
		ucRxReady=0;

	return ucResult;
};
 
//unsigned char  ucCan2_RxReady(void){
//	
// if (C2SR & 0x00000001)
//	  return 1;
// return 0;
// }
