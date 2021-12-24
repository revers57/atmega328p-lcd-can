#ifndef CANMESSAGE_H_
#define CANMESSAGE_H_
		#include <SPI.h>
		#include <mcp2515.h>
		#include <Wire.h>
		#include <Arduino.h>
		#include "buttons.h"
		#include "lcd.h"
				//variables
		enum canValues {uki_resistance, dc_ac_Voltage, dc_ac_Current, dc_ac_Power, 
		dc_dc_Voltage, dc_dc_Current, dc_dc_Power, gen_Current, gen_Voltage, 
		gen_Power, gen_Velocity, dc_dc_HeatsingTemp, ac_dc_Voltage, ac_dc_Current,
		ac_dc_Power, ac_dc_Temprature, gen_Temperature,
		mcu_st_DCDC_on, mcu_st_DCDC_fault, dc_FaultCode,
		mcu_st_DCAC_on, mcu_st_DCAC_fault,
		mcu_st_ACDC_on, mcu_st_ACDC_fault, ac_dc_FaultCode,
		mcu_st_Iso_fault,
		Can_current_DC_change, Can_gen_moment,
		Last};
				//functions
		int array_of_values();
		int el_arrray(int x);
		int set_values(int value, canValues param);
		int get_value(canValues param);
		void can_init();
		void get_values_for_pages();
		void send_control_message();
		void control_leds_message();
		void send_value_change(int v_can, int st_can); 
		int receive_value_change();
#endif