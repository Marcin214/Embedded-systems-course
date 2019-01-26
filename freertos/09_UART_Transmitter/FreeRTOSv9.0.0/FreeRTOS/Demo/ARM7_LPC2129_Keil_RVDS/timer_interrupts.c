#include <LPC21xx.H>
#include "timer_interrupts.h"

// TIMER
#define mCOUNTER_ENABLE  	(1<<0)
#define mCOUNTER_RESET   	(1<<1)

// CompareMatch TIMER0
#define mINTERRUPT_ON_MR0 (1<<0)
#define mRESET_ON_MR0     (1<<1)
#define mMR0_INTERRUPT    (1<<0)

// CompareMatch TIMER1
#define mINTERRUPT_ON_MR1 (1<<0)
#define mRESET_ON_MR1     (1<<1)
#define mMR1_INTERRUPT    (1<<0)

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER0_CHANNEL_NR 4
#define VIC_TIMER1_CHANNEL_NR 5

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE (1<<5)

void (*ptrTimer0InterruptFunction)(void); //zmienna globalna bedaca wskaznikiem do funkcji
void (*ptrTimer1InterruptFunction)(void); //zmienna globalna bedaca wskaznikiem do funkcji

/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(){

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	(*ptrTimer0InterruptFunction)();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod,void(*ptrInterruptFunction)()){ // microseconds

	ptrTimer0InterruptFunction = ptrInterruptFunction;
        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr0  =(unsigned long)Timer0IRQHandler; 	         // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T0MR0 = 15 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T0TCR |=  mCOUNTER_ENABLE; // start 

}
/**********************************************/

/**********************************************/
//(Interrupt Service Routine) of Timer 1 interrupt
__irq void Timer1IRQHandler(){

	T1IR=mMR1_INTERRUPT; 	// skasowanie flagi przerwania 
	(*ptrTimer1InterruptFunction)();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer1Interrupts_Init(unsigned int uiPeriod,void(*ptrInterruptFunction)()){ // microseconds

	ptrTimer1InterruptFunction = ptrInterruptFunction;
        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 1 interrupt channel 
	VICVectCntl2  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 1 and assign it to Timer 1 interrupt channel
	VICVectAddr2  =(unsigned long)Timer1IRQHandler; 	         // Set to Slot 1 Address of Interrupt Service Routine 

        // match module

	T1MR0 = 15 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR1 | mRESET_ON_MR1); // action 

        // timer

	T1TCR |=  mCOUNTER_ENABLE; // start 

}
/**********************************************/