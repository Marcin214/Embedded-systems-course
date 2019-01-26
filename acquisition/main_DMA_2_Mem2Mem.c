#include <asf.h>
#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define DMA_CHANNEL 0
#define SAMPLES_NR 5000
uint8_t SamplesSrc[SAMPLES_NR], SamplesDst[SAMPLES_NR];

int main (void) {
	for(int16_t i=0;i<SAMPLES_NR;i++) SamplesSrc[i]=i;
	// CLK
	sysclk_init();
	// UART
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 921600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	usart_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);

	 void dma_init(void){
		struct dma_channel_config dmach_conf;
		memset(&dmach_conf, 0, sizeof(dmach_conf));
		dma_channel_set_burst_length(&dmach_conf, DMA_CH_BURSTLEN_1BYTE_gc);
		dma_channel_set_transfer_count(&dmach_conf, SAMPLES_NR);
		dma_channel_set_src_reload_mode(&dmach_conf,DMA_CH_SRCRELOAD_TRANSACTION_gc);
		dma_channel_set_dest_reload_mode(&dmach_conf,DMA_CH_DESTRELOAD_TRANSACTION_gc);
		dma_channel_set_src_dir_mode(&dmach_conf, DMA_CH_SRCDIR_INC_gc);
		dma_channel_set_source_address(&dmach_conf,(uint16_t)(uintptr_t)SamplesSrc);
		dma_channel_set_dest_dir_mode(&dmach_conf, DMA_CH_DESTDIR_INC_gc);
		dma_channel_set_destination_address(&dmach_conf,(uint16_t)(uintptr_t)SamplesDst);
		dma_enable();
		dma_channel_write_config(DMA_CHANNEL, &dmach_conf);
		dma_channel_enable(DMA_CHANNEL);
	}
	dma_init();
	
	while(1) {
		
		uint8_t c;
		usart_serial_getchar(&USARTE0,&c);
		
		for(int16_t i=0;i<SAMPLES_NR;i++) SamplesSrc[i]=255-SamplesSrc[i];
		dma_channel_enable(DMA_CHANNEL);
		dma_channel_trigger_block_transfer(DMA_CHANNEL);
		
		while(dma_channel_is_busy(DMA_CHANNEL));
		usart_serial_write_packet(&USARTE0,SamplesDst, SAMPLES_NR);
		
	};
}