#include "watch.h"
#include "timer_interrupts.h"

Watch sWatch;

void WatchInit(void){
Timer1Interrupts_Init(1000000,WatchUpdate);
} 
	

void WatchUpdate(void){
	sWatch.ucSecconds++;
		
	if(sWatch.ucSecconds > 5){
			sWatch.ucSecconds = 0;
			sWatch.ucMinutes++;
			sWatch.fMinutesValueChanged = 1;
	}
	sWatch.fSeccondsValueChanged = 1;
}
