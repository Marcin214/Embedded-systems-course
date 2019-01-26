#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12,ADC_REF_BANDGAP);
	 
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_POS_PIN5,8);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}


int main (void) {
	
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	
	
	adc_init();
	stdio_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	adc_enable(&MY_ADC);
	
	unsigned char c;
	uint16_t result;
	uint16_t oversampled_result;
	while(1) {
		
				c = getchar();
				
				if (c != '\0'){
					for (unsigned char ucCounter = 0; ucCounter < 16; ucCounter++){
						adc_start_conversion(&MY_ADC, MY_ADC_CH);
						adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
						result = adc_get_signed_result(&MY_ADC, MY_ADC_CH);
						oversampled_result += result;		
					}
					oversampled_result = (oversampled_result >>2);
					printf("%d\n",result);
				}
				else{
					printf("Nothing\n");
				}

	}
}
// bez usredniania 1148 - 1162 (+/- 6LSB)
// usredniania 1156 - 1165 (+/- 5LSB)
//-----------------------------------------
// zakres pomiarowy 1131-1536
// tryb róznicowy -70 +702 jeden czujnik, dwa czujniki -887 +702
// max gain 8, gain must be from range {0 (0.5x), 1, 2, 4, 8, 16, 32 or 64} 
