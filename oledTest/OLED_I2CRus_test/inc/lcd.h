#ifndef lcd_h_
#define lcd_h_
		#include <Arduino.h>
		#include <Wire.h>
		#include <LiquidCrystal_I2C_OLED.h>
		#include "buttons.h"
					//variables
		enum lcdValues_ {uki_resistance_, dc_ac_Voltage_, dc_ac_Current_, dc_ac_Power_,
		dc_dc_Voltage_, dc_dc_Current_, dc_dc_Power_, gen_Current_, gen_Voltage_,
		gen_Power_, gen_Velocity_, dc_dc_HeatsingTemp_, ac_dc_Voltage_, ac_dc_Current_,
		ac_dc_Power_, ac_dc_Temprature_, gen_Temperature_,
		mcu_st_DCDC_on_, mcu_st_DCDC_fault_, dc_FaultCode_,
		mcu_st_DCAC_on_, mcu_st_DCAC_fault_,
		mcu_st_ACDC_on_, mcu_st_ACDC_fault_, ac_dc_FaultCode_,
		mcu_st_Iso_fault_,
		current_DC_change_, gen_moment_,
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
		void lcd_change_page_7(int value_lcd, int minusOrPlus_lcd);
#endif