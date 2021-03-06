#include <asf.h>

#define UART_TXPIN              IOPORT_CREATE_PIN(PORTE, 3)
#define MY_ADC                  ADCA
#define MY_ADC_CH               ADC_CH0

#define GAIN                    1
#define SAMPLES_PER_MEASUREMENT 1
#define REF_VOLTAGE_mV          1000.0

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_BANDGAP);
	
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SWEEP, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_POS_PIN5, GAIN);
	
	//adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, GAIN);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

static void evsys_init(void){
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	//EVSYS.CH3MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}

int main (void) {
	
	sysclk_init();
	evsys_init();
	
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
	
	uint8_t c;
	uint16_t result;
	

	while(1) {
		
		c = getchar();
		
		if (c != '\0'){
			result = 0;		
			
			EVSYS_STROBE = 0x01;
				
			//adc_start_conversion(&MY_ADC, MY_ADC_CH);
			adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
			result = adc_get_result(&MY_ADC, MY_ADC_CH);
			printf("%d\n",result);
			adc_clear_interrupt_flag(&MY_ADC, MY_ADC_CH );
		}
		
		else{
			printf("Nothing\n");
		}

	}
}