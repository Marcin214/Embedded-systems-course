#include <asf.h>

#define LED    IOPORT_CREATE_PIN(PORTD, 5)
#define BUTTON IOPORT_CREATE_PIN(PORTE, 5)

int main (void)
{
	ioport_init();
	
	ioport_set_pin_dir(LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(BUTTON, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(BUTTON, IOPORT_MODE_PULLUP);
	
	bool value;


	while (1)
	{
			value = ioport_get_pin_level(BUTTON);
			if (0 == value)
			{
				ioport_toggle_pin_level(LED);
				delay_ms(100);
			}
			else{
				ioport_set_pin_level(LED, 0);	
			}
	}

	
}
