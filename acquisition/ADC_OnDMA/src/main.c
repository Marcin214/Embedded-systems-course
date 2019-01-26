#include <asf.h>
#include <math.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTD, 3)
#define DMA_CHANNEL 0
#define MY_ADC                  ADCA
#define MY_ADC_CH               ADC_CH0
#define GAIN                    1
#define LED                     IOPORT_CREATE_PIN(PORTD, 5)
#define TRIGGER                 IOPORT_CREATE_PIN(PORTA, 7)


#define SAMPLES_NR 1200
#define PI          3.14
#define AMP_MVOLT   125             //amplitude 1V
#define OFFSET      125            //offset 0,5V
#define T           200 

uint8_t Samples[SAMPLES_NR];
uint8_t probe[SAMPLES_NR];
uint16_t counter;

int main (void) {
	
	for(counter=0;counter<SAMPLES_NR;counter++)
	{
		probe[counter] = (uint8_t)OFFSET + AMP_MVOLT*sin(counter*2.0*PI/T);
	}

	// CLK
	sysclk_init();
	ioport_init();	
	ioport_set_pin_dir(LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(TRIGGER, IOPORT_DIR_OUTPUT);
	
	// UART
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	 usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 921600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	usart_serial_init(&USARTD0, &USART_SERIAL_OPTIONS);
    
	// ADC
	 void adc_init(void)
	{
		struct adc_config adc_conf;
		struct adc_channel_config adcch_conf;
		
		adc_read_configuration(&MY_ADC, &adc_conf);
		adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
		
		adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_AREFA);
		adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);
		adc_set_clock_rate(&adc_conf, 1000000UL);
		adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, GAIN);
	
		adc_write_configuration(&MY_ADC, &adc_conf);
		adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
		
	}
	
	adc_init();
	adc_enable(&MY_ADC);
	// DMA
 void dma_init(void){
	 
	 struct dma_channel_config dmach_conf;
	 memset(&dmach_conf, 0, sizeof(dmach_conf));
	 dma_channel_set_burst_length(&dmach_conf, DMA_CH_BURSTLEN_1BYTE_gc);
	 dma_channel_set_transfer_count(&dmach_conf, SAMPLES_NR);
	
	 dma_channel_set_src_reload_mode(&dmach_conf, DMA_CH_SRCRELOAD_BURST_gc);
	 dma_channel_set_dest_reload_mode(&dmach_conf,DMA_CH_DESTRELOAD_TRANSACTION_gc);
	 dma_channel_set_src_dir_mode(&dmach_conf, DMA_CH_SRCDIR_INC_gc);
	 dma_channel_set_dest_dir_mode(&dmach_conf, DMA_CH_DESTDIR_INC_gc);
	 dma_channel_set_source_address(&dmach_conf,(uint16_t)(uintptr_t)&ADCA.CH0RES);
	 dma_channel_set_destination_address(&dmach_conf,(uint16_t)(uintptr_t)Samples);
	 dma_channel_set_trigger_source(&dmach_conf, DMA_CH_TRIGSRC_ADCA_CH0_gc);
	 dma_channel_set_single_shot(&dmach_conf);
	 dma_enable();
	 //dma_set_callback(DMA_CHANNEL, dma_transfer_done);
	 dma_channel_set_interrupt_level(&dmach_conf, DMA_INT_LVL_LO);
	 dma_channel_write_config(DMA_CHANNEL, &dmach_conf);
	// dma_channel_enable(DMA_CHANNEL);
 }
		 dma_init();
	// Main loop
	


	while(1) {
	
		uint8_t c;
		usart_serial_getchar(&USARTD0,&c);
		dma_channel_enable(DMA_CHANNEL);
		ioport_toggle_pin_level(LED);
		ioport_set_pin_level(TRIGGER, 1);
		delay_ms(1);
		ioport_set_pin_level(TRIGGER, 0);
		while(dma_channel_is_busy(DMA_CHANNEL));
		usart_serial_write_packet(&USARTD0, probe, SAMPLES_NR);
	};
}