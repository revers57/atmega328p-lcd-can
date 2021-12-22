#ifndef lcd_h_
#define lcd_h_
		#include <Arduino.h>
		#include <Wire.h>
		#include <LiquidCrystal_I2C_OLED.h>
		#include "buttons.h"
					//variables
		enum lcdValues_ {gen_Velocity_, gen_CurrentAC_, gen_VoltageAC_,gen_Temperature_, gen_PowerAC_,
		ac_VoltageAC_, ac_CurrentAC_, ac_PowerAC_, ac_HeastingTemp_,
		dc_VoltageDC_, dc_CurrentDC_, dc_PowerDC_, dc_HeatsingTemp_, 
		ac_dc_CurrentDC_, ac_dc_VoltageDC_, ac_dc_TempratureDC_, 
		mcu_st_DCDC_on_, mcu_st_DCDC_fault_, dc_FaultCode_,
		mcu_st_DCAC_on_, mcu_st_DCAC_fault_,
		mcu_st_ACDC_on_, mcu_st_ACDC_fault_, ac_dc_FaultCode_,
		mcu_st_Iso_fault_,
		current_DC_change = 5, gen_moment = 5,
		Last_};
					//functions
		int array_of_values_();
		int el_arrray(int x);
		int set_values_(int value, lcdValues_ param);
		int get_value_(lcdValues_ param);
		void lcd_init();
		void switch_page();
		void show_current_values();
		void page1();
		void page2();
		void page3();
		void page4();
		void page5();
		void page6();
		void page0();//3s preface screen
		void page7();//change_values page
		void values_page_7();
		void values_page_1();
		void values_page_2();
		void values_page_3();
		void values_page_4();
		void values_page_5();
		void values_page_6();
		void print_value(int col, int row, int b);
		void print_value_en(int col, int row, char a[]);
		void print_value_rus(int col, int row, char a[]);
		void lcd_change_page_7(int v_lcd, int st_lcd);
#endif