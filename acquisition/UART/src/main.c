#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTD, 2)

int main (void) {

	sysclk_init();	
		
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	
	stdio_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	
	unsigned int c = 0;
	while(1) {
		
		//scanf("%s",ch);
		c = getchar();
		
		if (c == 'b'){
			printf("Binary\n");
		} 
		else if(c == 'v'){
			printf("Voltage\n");
		}
		else if(c == 't'){
			printf("Temperature\n");
		}
		else
		printf("Nothing\n");
	}
}