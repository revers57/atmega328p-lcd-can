#ifndef CANMESSAGE_H_
#define CANMESSAGE_H_
		#include <SPI.h>
		#include <mcp2515.h>
		#include <Wire.h>
		#include <Arduino.h>
		#include "buttons.h"
		#include "lcd.h"
				//variables
		enum lcdValues {gen_Velocity, gen_CurrentAC, gen_VoltageAC, gen_Temperature, gen_PowerAC,
		ac_VoltageAC, ac_CurrentAC, ac_PowerAC, ac_HeastingTemp,
		dc_VoltageDC, dc_CurrentDC, dc_PowerDC, dc_HeatsingTemp,
		ac_dc_CurrentDC, ac_dc_VoltageDC, ac_dc_TempratureDC,
		mcu_st_DCDC_on, mcu_st_DCDC_fault, dc_FaultCode,
		mcu_st_DCAC_on, mcu_st_DCAC_fault,
		mcu_st_ACDC_on, mcu_st_ACDC_fault, ac_dc_FaultCode,
		mcu_st_Iso_fault,
		Can_current_DC_change, Can_gen_moment,
		Last};
				//functions
		int array_of_values();
		int el_arrray(int x);
		int set_values(int value, lcdValues param);
		int get_value(lcdValues param);
		void can_init();
		void get_values_for_pages();
		void send_control_message();
		void control_leds_message();
		void can_change_page_7(int v_can, int st_can); 
#endif