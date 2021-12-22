#include "main.h"

void setup()
{
	buttons_init();
	can_init();
	lcd_init();
}

void loop()
{
	send_control_message();				/*Send control message*/
	control_leds_message();				/*Control leds*/
 	modify_and_show_display();			/*Two modes display usage*/
}
