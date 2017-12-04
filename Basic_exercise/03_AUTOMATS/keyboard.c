#include <LPC21xx.H>
#include "keyboard.h"
#include "delay.h"

#define Butt1   (1<<4)
#define Butt2   (1<<6)
#define Butt3   (1<<5)
#define Butt4   (1<<7)



enum KeyboardState eKeyboardRead(void){
	
  if(!(IO0PIN&Butt1)){
		Delay(80);
		if(!(IO0PIN&Butt1)) 
			return BUTTON_1;
	}
  else if(!(IO0PIN&Butt2)){
		Delay(80);
		if(!(IO0PIN&Butt2)) 
			return BUTTON_2;
	}
	
	else if(!(IO0PIN&Butt3)){
		Delay(80);
		if(!(IO0PIN&Butt3)) 
			return BUTTON_3;
	}
	else if(!(IO0PIN&Butt4)){
		Delay(80);
		if(!(IO0PIN&Butt4)) 
			return BUTTON_4;
	}
  return RELASED;
}

void KeyboardInit(void){
IO0DIR &= ~( Butt1|Butt2|Butt3|Butt4);
}
