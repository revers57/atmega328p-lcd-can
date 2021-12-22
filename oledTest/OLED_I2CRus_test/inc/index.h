#ifndef INDEX_H_
#define INDEX_H_
		#include "lcd.h"
		#include "canMessage.h"
				//variables
				//functions
		void modify_and_show_display();
		void enterMode_show_values();
		void enterMode_change_values();
		void choose_changed_value();
		void modify_value(int v, int st); 
#endif