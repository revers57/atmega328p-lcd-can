#include "index.h"

int stateF1 = 1, stateF1_ = 1, stateF2 = 1, stateF2_ = 1, stateStart = 1, stateStart_ = 1, stateStop = 1, stateStop_ = 1;
long millis_held, secs_held;
unsigned long firstTimeF1 = 0, firstTimeF2 = 0, millis_held_F1, millis_held_F2; //for a long button press
int currentMode = 0; //for switching main modes
int currentMode_Under = 0; //for switching modes, where concrete value is changed
int i = 0; //for showing page 7 only one time
long blinkTime, blinkTime2, blinkSpeed_ms = 1000 ; //for a blinking "_" on page 7
int chooseVal = 0; //for a switch between values in "Change values" mode


/****************** MODIFY ****************/
void enter_mode_change_values()		/*Enter*/
{
	if ((stateF1 == 0) && (stateF2 == 0)) {					
		if(millis() - firstTimeF2 >= 2000)
		currentMode = 1;
		if (i == 0) {
			page7();
			i = 1;	}
	}
	else
	firstTimeF2 = millis();
}

void access_values()		/*Make possible to modify a desired changed value*/
{
	if (currentMode == 1) {													
		if ((stateStart == 0) && (stateStart_ == 1)) {
			currentMode = 2;
			stateStart_ = 0; 
		}
	}	
}

void modify_value(int v, int st)	/*Modify concrete value*/
{
	lcd_change_page_7(v, st);
	can_change_page_7(v, st);
}

void modify_chosen_value()		/*Modify chosen value*/
{
	switch(currentMode_Under) {
		case 0: //val 0
				print_value_rus(16, 1, "_");
			if ((stateF1_ == 0) && (stateF1 == 1))
				modify_value(1, 0);
			if ((stateF2_ == 0) && (stateF2 == 1))
				modify_value(1, 1);
			if ((stateStop == 0) && (stateStop_ == 1))
				currentMode_Under == 1;
			break;
		case 1: //val 1
				print_value_rus(16, 2, "_");
			if ((stateF1_ == 0) && (stateF1 == 1))
				modify_value(2, 0);
			if ((stateF2_ == 0) && (stateF2 == 1))
				modify_value(2, 1);
			if ((stateStop == 0) && (stateStop_ == 1))
				currentMode_Under == 0;
			break;
	}
	currentMode = 3;
}

void choose_changed_value()		/*Choose a value to be modified*/
{
	switch (chooseVal) {
		case 0:
			if ((millis() - blinkTime > blinkSpeed_ms) && (millis()  - blinkTime < blinkSpeed_ms * 2))
				print_value_rus(16, 1, "_");
			else if ((millis() - blinkTime > blinkSpeed_ms * 2))
				blinkTime = millis();
			else
				print_value_rus(16, 1, " ");
			if ((stateF1 == 0) && (stateF1_ == 1)) {
				print_value_rus(16, 1, " ");
				chooseVal = 1;	
			}
			break;
		case 1:
			if ((millis() - blinkTime > blinkSpeed_ms) && (millis()  - blinkTime < blinkSpeed_ms * 2))
				print_value_rus(16, 2, "_");
			else if ((millis() - blinkTime > blinkSpeed_ms * 2))
				blinkTime = millis();
			else
				print_value_rus(16, 2, " ");
			if ((stateF1 == 0) && (stateF1_ == 1)) {
				print_value_rus(16, 2, " ");
				chooseVal = 0;	
			}
			break;
	}
	if ((stateStart == 0) && (stateStart_ == 1) && (chooseVal != 2)) {											    /*Verify your choice by pressing START*/
		switch (chooseVal) {
			case 0:
				currentMode_Under = 0;
				break;
			case 1:
				currentMode_Under = 1;
				break;		}
	stateStart_ = 0;
	currentMode = 3;
	}
}

void exit_mode_change_values()		/*Exit*/
{
	if (stateStop == 0 && stateStop_ == 1) {
		currentMode = 0;
		page1();
		i = 0;
	}
}

/****************** SHOW ******************/
void enter_mode_show_values()		/*Show a current screen and renewed values for it*/
{
	get_values_for_pages(); //can
	for (int y = 0; y < 33; y++)
		set_values_(get_value((lcdValues)y), (lcdValues_)y);
	show_current_values();
}	

void mode_show_page()
{
	if (currentMode == 0) {									
		enter_mode_show_values();
		if (stateF2 == 0 && stateF2_ == 1)
		switch_page();
	}
}


void modify_and_show_display() 
{
stateF2 = digitalRead(buttonF2);
stateF1 = digitalRead(buttonF1);
stateStart = digitalRead(buttonStart);
stateStop = digitalRead(buttonStop);
delay(10);	

enter_mode_change_values();		/****			Main mode 'MODIFY VALUES'			****/
access_values();
if (currentMode == 2) 
	choose_changed_value();		//Choose a value and
if (currentMode == 3)
	modify_chosen_value();		//modify it
exit_mode_change_values();
mode_show_page();				/****			Main mode 'SHOW VALUES'				****/

stateF2_ = stateF2;
stateF1_ = stateF1;
stateStart_ = stateStart;
stateStop_ = stateStop;
}


