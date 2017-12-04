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
char cCharacterToSend;
char cfLastCharacterTx;
QueueHandle_t xQueueRxUART;
QueueHandle_t xQueueTxUART;
xSemaphoreHandle xBinarySemaphoreRx;
xSemaphoreHandle xBinarySemaphoreTx;

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {

   BaseType_t xHigherPriorityTaskWoken;
	 xHigherPriorityTaskWoken = pdFALSE;
	 BaseType_t xTaskWokenByReceive = pdFALSE;
	
	
   unsigned int uiCopyOfU0IIR=U0IIR;     // odczyt U0IIR powoduje jego kasowanie

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
		 cOdebranyZnak = U0RBR; 

		 if(cOdebranyZnak == 0xD){
			  cOdebranyZnak = NULL;
			  xQueueSendToBackFromISR( xQueueRxUART,&cOdebranyZnak,&xHigherPriorityTaskWoken );
				xSemaphoreGiveFromISR(xBinarySemaphoreRx,&xHigherPriorityTaskWoken);
		 }
		 else{
			  xQueueSendToBackFromISR( xQueueRxUART,&cOdebranyZnak,&xHigherPriorityTaskWoken );
		 }
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
				if (xQueueReceiveFromISR( xQueueTxUART, &cCharacterToSend,&xTaskWokenByReceive)==pdTRUE){ 
						U0THR = cCharacterToSend;
				}
				else if(cfLastCharacterTx == 1){
					  cfLastCharacterTx = 0;
						cCharacterToSend = '\r';
						U0THR = cCharacterToSend;
						xSemaphoreGiveFromISR(xBinarySemaphoreTx,&xTaskWokenByReceive);
				}
		}

   VICVectAddr = 0; // Acknowledge Interrupt
}

void UART_InitWithInt(unsigned int uiBaudRate){
	
	xQueueRxUART = xQueueCreate( 20, sizeof(char) );
	xQueueTxUART = xQueueCreate( 20, sizeof(char) );
	vSemaphoreCreateBinary(xBinarySemaphoreRx);
	vSemaphoreCreateBinary(xBinarySemaphoreTx);
	xSemaphoreTake(xBinarySemaphoreRx,portMAX_DELAY);
	
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

}



void Uart_GetString(char * pcString){
	
  xSemaphoreTake(xBinarySemaphoreRx,portMAX_DELAY);
	while( xQueueReceive( xQueueRxUART, pcString, NULL) != pdFALSE){
		 pcString++;		
	}
}

void Uart_PutString(char * pcString){
    	
	 if(xSemaphoreTake(xBinarySemaphoreTx, portMAX_DELAY)==pdTRUE){
		  cfLastCharacterTx = 1;
      cCharacterToSend = *pcString;
		 
			while(*pcString != NULL){
				pcString++;
				xQueueSendToBack( xQueueTxUART , pcString, NULL);	 
			}
			
			U0THR = cCharacterToSend;
		}
}

