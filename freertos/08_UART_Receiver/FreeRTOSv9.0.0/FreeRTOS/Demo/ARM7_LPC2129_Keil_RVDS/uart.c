#include <LPC210X.H>
#include "uart.h"
#include "string.h"
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define TERMINATOR 0xD 

/*************************** UART ***************************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************************ INTERRUPTS ************************/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////////////// Zmienne globalne //////////////////////
RecieverBuffer sRecieverBuffer;
TransmiterBuffer sTransmiterBuffer;

char cOdebranyZnak;
QueueHandle_t xQueueUART;
xSemaphoreHandle xBinarySemaphore;

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {

   BaseType_t xHigherPriorityTaskWoken;
	 xHigherPriorityTaskWoken = pdFALSE;
	
	
   unsigned int uiCopyOfU0IIR=U0IIR;     // odczyt U0IIR powoduje jego kasowanie

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
		 cOdebranyZnak = U0RBR; 

		 if(cOdebranyZnak == '\r'){
			  cOdebranyZnak = NULL;
			  xQueueSendToBackFromISR( xQueueUART,&cOdebranyZnak,&xHigherPriorityTaskWoken );
				xSemaphoreGiveFromISR(xBinarySemaphore,&xHigherPriorityTaskWoken);
		 }
		 else{
			  xQueueSendToBackFromISR( xQueueUART,&cOdebranyZnak,&xHigherPriorityTaskWoken );
		 }
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
			if(Transmiter_GetStatus() == BUSY)
					U0THR=Transmiter_GetCharacterFromBuffer();
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}



void UART_InitWithInt(unsigned int uiBaudRate){
	
	xQueueUART = xQueueCreate( 5, sizeof(char) );
	vSemaphoreCreateBinary(xBinarySemaphore);
	xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                       // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT;    // dlugosc slowa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                         // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long )UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
	
	sRecieverBuffer.ucCharCtr = 0;
	sTransmiterBuffer.cCharCtr = 0;

}

//char cUart_GetChar(void){
//	char Recieve;
//	xQueueReceive( xQueueUART, &Recieve, portMAX_DELAY );
//	return Recieve;
//}


void Uart_GetString(char * pcString){
	
  xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
	while( xQueueReceive( xQueueUART, pcString, NULL) != pdFALSE){
		 pcString++;		
	}
}














char Transmiter_GetCharacterFromBuffer(void){

	if(1==sTransmiterBuffer.fLastCharacter){
		sTransmiterBuffer.fLastCharacter = 0;
		sTransmiterBuffer.eStatus = FREE;
		return NULL;
	}
	else if(NULL==sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr]){
		sTransmiterBuffer.fLastCharacter = 1;
		return TERMINATOR;
	}
	else{
		return sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
	}

}

void Transmiter_SendString(char cString[]){
	sTransmiterBuffer.cCharCtr = 0;
	strcpy(sTransmiterBuffer.cData,cString);
	U0THR = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
	sTransmiterBuffer.eStatus = BUSY;
}
eTransmiterStatus Transmiter_GetStatus(void){
	return sTransmiterBuffer.eStatus;
}
