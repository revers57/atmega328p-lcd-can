#include "buttons.h"

/*Initialize buttons*/
void buttons_init()
{
	pinMode(buttonStart, INPUT_PULLUP); //On button
	pinMode(buttonStop, INPUT_PULLUP); //Off button
	pinMode(buttonF1, INPUT_PULLUP); //menu button 
	pinMode(buttonF2, INPUT_PULLUP); //menu button 	
	pinMode(buttonPinRed, OUTPUT);			//led red
	pinMode(buttonPinYellow, OUTPUT);		//led yellow
}



