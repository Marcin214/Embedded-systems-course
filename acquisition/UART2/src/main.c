#include <asf.h>
#include <math.h>

#define SAMPLES_NR  2000
#define PI          3.14
#define AMP_MVOLT   125           
#define OFFSET      125            
#define T           200

uint8_t probe[SAMPLES_NR];
uint16_t counter;

#define UART_TXPIN IOPORT_CREATE_PIN(PORTD, 3)
static usart_rs232_options_t USART_SERIAL_OPTIONS = {
	.baudrate = 921600,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = false
};
/*	
 static void my_callback(void)
 {
 usart_serial_write_packet(&USARTD0, probe, SAMPLES_NR);
 }*/
 uint16_t counter;
 uint8_t licznik_modulo;
int main (void) {

	pmic_init();
	sysclk_init();

	stdio_serial_init(&USARTD0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	
	/*tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, my_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 6250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	cpu_irq_enable();
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV256_gc);
*/	
		/*for(counter=0;counter<SAMPLES_NR;counter++)
		{
			probe[counter] = (uint8_t)OFFSET + AMP_MVOLT*sin(counter*2.0*PI/T);
		}*/
		for(counter=0;counter<SAMPLES_NR;counter++)
		{
			probe[counter] = licznik_modulo++;
		}
	unsigned int c = 0;
	while(1) {	
		usart_serial_getchar(&USARTD0,&c);
		usart_serial_write_packet(&USARTD0, probe, SAMPLES_NR);	
	}
}