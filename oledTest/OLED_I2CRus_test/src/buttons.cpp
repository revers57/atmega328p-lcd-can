#include "buttons.h"

/*Initialize pins of atmega328p*/
void pins_init()
{
	pinMode(buttonStart, INPUT_PULLUP); //On button
	pinMode(buttonStop, INPUT_PULLUP); //Off button
	pinMode(buttonF1, INPUT_PULLUP); //menu button 
	pinMode(buttonF2, INPUT_PULLUP); //menu button 	
	pinMode(PinRed, OUTPUT);		//led red
	pinMode(PinYellow, OUTPUT);		//led yellow
}



