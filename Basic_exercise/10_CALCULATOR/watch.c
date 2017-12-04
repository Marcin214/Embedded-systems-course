#include "watch.h"

Watch sWatch;

void WatchUpdate(void){
	sWatch.ucSecconds++;
		
	if(sWatch.ucSecconds > 5){
			sWatch.ucSecconds = 0;
			sWatch.ucMinutes++;
			sWatch.fMinutesValueChanged = 1;
	}
	sWatch.fSeccondsValueChanged = 1;
}
