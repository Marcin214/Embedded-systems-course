#include <LPC21xx.H>


#define Butt1   0x00000010
#define Butt2   0x00000040
#define Butt3   0x00000020
#define Butt4   0x00000080
#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000

enum LedState {LEFT,RIGHT};
enum KeyboardState {RELASED,BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

void LedStep(enum LedState Direction);
void LedOn(unsigned char ucLedIndeks);
void LedOff(unsigned char ucLedIndeks);

void LedInit(void);
void LedStepLeft(void);
void LedStepRight(void);
	
void KeyboardInit(void);
enum KeyboardState eKeyboardRead(void);
void Delay(unsigned long time);

int main(){

	enum KeyboardState ButtonState;
	
	LedInit();
	KeyboardInit();

	while(1){
		
		ButtonState = eKeyboardRead();
		
		switch(ButtonState){
			case BUTTON_1: 
				LedStepRight(); 
				break;
			case BUTTON_2: 
				LedStepLeft(); 
				break;
			case BUTTON_3: 
				break;
			case BUTTON_4: 
				break;
			case RELASED: 
				break;
			default:
				break;			
		}	
		Delay(50);
	}
}

void Delay(unsigned long time){
	for(time;time>0;time--){
		unsigned long i;
		for(i=11995;i>0;i--){}
	}
}



void LedStep(enum LedState Direction){
	static unsigned int i;
	switch(Direction){
		case LEFT: 
			i++;
			LedOn(i%4);
			break;
		case RIGHT: 
			i--;
			LedOn(i%4); 
			break;
		default:
			break;
			}
}

void LedStepLeft(void){
 LedStep(LEFT);
}

void LedStepRight(void){
 LedStep(RIGHT);
}

void LedInit(void){
	IO1DIR |= LED0_bm | LED1_bm | LED2_bm |LED3_bm;
	IO1SET =LED0_bm;
 
}

	void LedOn(unsigned char ucNumerDiody){
		
	IO1CLR = LED0_bm|LED1_bm|LED2_bm|LED3_bm;

	switch(ucNumerDiody){
		case 0: 
			IO1SET =LED0_bm; 
			break;
		case 1: 
			IO1SET =LED1_bm; 
			break;
		case 2: 
			IO1SET =LED2_bm; 
			break;
		case 3: 
			IO1SET =LED3_bm; 
			break;
		default:
			break;
	}
}

void LedOff(unsigned char ucLedIndeks){
	IO1CLR |= LED0_bm<<ucLedIndeks;
}



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
