#include "lcd.h"

lcdValues_ values_[33];
int valLcd_[Last_];
int currentScreen = 0;

/*Initialize lcd; address, cols, rows*/
LiquidCrystal_I2C OLED1(0x3F,20,4); 

/*Set and get lcd values*/
int set_values_(int value, lcdValues_ param) {	//save value
	valLcd_[param] = value;
}
int get_value_(lcdValues_ param) {	//get saved value
	return valLcd_[param];
}

/*Simplify arduino print functions*/
void print_value_en(int col, int row, char a[66])
{//english
	OLED1.setCursor(col, row);
	OLED1.print(a);
}
void print_value_rus(int col, int row, char a[66])
{//russian
	OLED1.setCursor(col, row);
	OLED1.outStr(a);
}
void print_value(int col, int row, int b)
{//value
	OLED1.setCursor(col, row);
	OLED1.print(b);
}
void print_values_straight(int v, int unit, int row) { //for straight display of values when they change 
	int tens = unit - 1;
	int hundreds = unit - 2;
	if ((v % 10 == 0) && (v % 100 == 0))
		print_value(unit, row, v);
	else if ((v % 10 != 0) && (v % 100 == 0))
		print_value(tens, row, v);		
	else
		print_value(hundreds, row, v);	
}

/*Show text and values for pages*/
void values_page_1() {
	print_values_straight(get_value_(ac_VoltageAC_), 6, 0);
	print_values_straight(get_value_(ac_PowerAC_), 16, 0);
	print_values_straight(get_value_(dc_VoltageDC_), 9, 1);

	print_value(4, 1, get_value_(dc_VoltageDC_));	
	print_value(14, 1, get_value_(dc_PowerDC_));
	print_value(14, 2, get_value_(gen_PowerAC_));
	print_value(4, 2, get_value_(gen_Velocity_));
}

void values_page_2() {
	print_values_straight(get_value_(dc_VoltageDC_), 9, 1);
	print_values_straight(get_value_(dc_CurrentDC_), 9, 2);
	print_values_straight(get_value_(dc_HeatsingTemp_), 9, 3);
}

void values_page_3() { 
	print_values_straight(get_value_(ac_VoltageAC_), 9, 1);
	print_values_straight(get_value_(ac_CurrentAC_), 9, 2);
	print_values_straight(get_value_(ac_HeastingTemp_), 9, 3);
}

void values_page_4() {
	print_values_straight(get_value_(ac_dc_TempratureDC_), 9, 1);
	print_values_straight(get_value_(ac_dc_VoltageDC_), 9, 2);
	print_values_straight(get_value_(ac_dc_CurrentDC_), 9, 3);
}

void values_page_5() {
	print_values_straight(get_value_(gen_Temperature_), 9, 1);
	print_values_straight(get_value_(gen_VoltageAC_), 9, 2);
	print_values_straight(get_value_(gen_CurrentAC_), 9, 3);
}

void values_page_6() 
{
	print_value(8, 0, get_value_(dc_FaultCode_));	
	print_value(8, 0, get_value_(dc_FaultCode_));
	print_value(8, 2, get_value_(ac_dc_FaultCode_));	
	//int j;
	//j = get_value_(mcu_st_DCDC_on_); /*DC DC = 27B*/
	//switch (j) {
		//case 0:
			//print_value_en(5, 0, "-");
		//case 1:
			//print_value_en(5, 0, "+");
	//}
		//
	//j = get_value_(mcu_st_DCDC_fault_); //27В fault
	//case 0:
			//print_value_en(6, 0, " ");
	//case 1:
			//print_value_en(6, 0, "!");
		//
	//j = get_value_(mcu_st_DCAC_on_); /*DC AC = 220B*/
	//case 0:
			//print_value_en(5, 1, "-");
	//case 1:
			//print_value_en(5, 1, "+");
		//
	//j = get_value_(mcu_st_DCAC_fault_); //220В fault 
	//case 0:
			//print_value_en(6, 1, " ");
	//case 1:
			//print_value_en(6, 1, "!");
				//
	//j = get_value_(mcu_st_ACDC_on_); /*AC DC 400B*/
	//case 0:
			//print_value_en(5, 2, "-");
	//case 1:
			//print_value_en(5, 2, "+");
		//
	//j = get_value_(mcu_st_ACDC_fault_); //400В fault 
	//case 0:
			//print_value_en(6, 2, " ");
	//case 1:
			//print_value_en(6, 2, "!");
}

void values_page_7() {
{ 
	print_value(17, 2, get_value_(current_DC_change));
	print_value(17, 1, get_value_(gen_moment));
}

void lcd_change_page_7(int v_lcd, int st_lcd) //st_lcd: 0 - minus, 1 - plus || v_lcd: number of value on screen 7
{
	if (v_lcd == 1) {
		switch (st_lcd) {
			case 0:
			set_values_(current_DC_change + 1, current_DC_change);
			print_value(17, 2, get_value_(current_DC_change));
			case 1:
			set_values_(current_DC_change - 1, current_DC_change);
		print_value(17, 2, get_value_(current_DC_change)); }
	}
	if (v_lcd == 2) {
		switch (st_lcd) {
			case 0:
			set_values_(gen_moment + 1, gen_moment);
			print_value(17, 1, get_value_(gen_moment));
			case 1:
			set_values_(gen_moment -1, gen_moment);
		print_value(17, 1, get_value_(gen_moment)); 
		}
	}
}

void page1() 
{
	currentScreen = 1;
	OLED1.clear();
	print_value_en(0, 0, "AC");
	print_value_rus(9, 0, "В");								
	print_value_rus(17, 0, "кВт");
	print_value_en(0, 1, "DC");
	print_value_rus(9, 1, "В");
	print_value_rus(17, 1, "кВт");
	print_value_rus(0, 2, "Ген      об/м    кВт");
	print_value_rus(0, 3, "УКИ 6000 ом/В");
	values_page_1();
}

void page2()   //dc/dc
{
	OLED1.clear();
	print_value_rus(8, 0, "DC/DC");
	print_value_rus(0, 1, "Напр:");
	print_value_rus(11, 1, "В");
	print_value_rus(0, 2, "Ток:");
	print_value_rus(11, 2, "А");	
	print_value_rus(0, 3, "Темп:");
	print_value_rus(11, 3, "град");
}

void page3() //dc/ac
{ 
	OLED1.clear();
	print_value_rus(8, 0, "DC/AC");
	print_value_rus(0, 1, "Напр:");
	print_value_rus(11, 1, "В");
	print_value_rus(0, 2, "Ток:");
	print_value_rus(11, 2, "А");
	print_value_rus(0, 3, "Темп:");
	print_value_rus(11, 3, "град");
}

void page4() {  //invertor
	OLED1.clear();
	print_value_rus(8, 0, "Инвертор");
	print_value_rus(0, 1, "Напр:");
	print_value_rus(11, 1, "В");
	print_value_rus(0, 2, "Ток:");
	print_value_rus(11, 2, "А");
	print_value_rus(0, 3, "Темп:");
	print_value_rus(11, 3, "град");
}

void page5() //generator
{
	OLED1.clear();
	print_value_rus(8, 0, "Генератор");
	print_value_rus(0, 1, "Напр:");
	print_value_rus(11, 1, "В");
	print_value_rus(0, 2, "Ток:");
	print_value_rus(11, 2, "А");
	print_value_rus(0, 3, "Темп:");
	print_value_rus(11, 3, "град");
}

void page6() 
{  //flags of mistakes
	OLED1.clear();
	print_value_rus(0, 0, "27В");
	print_value_rus(0, 1, "220В");
	print_value_rus(0, 2, "400В");
	print_value_rus(0, 3, "УКИ");
	values_page_6();
} 

void page0()//3s preface screen
{
	OLED1.clear();
	print_value_rus(7, 0, "RUBRUKS");
	print_value_rus(7, 1, "Система");	
	print_value_rus(4, 2, "Энергопитания");
	print_value_rus(0, 3, "v_0.01");
}

void page7()
{//page of change mode 
	OLED1.clear();
	print_value_rus(2, 0, "Изменение настроек");
	print_value_rus(0, 1, "Ток DC/DC:");
	print_value_rus(0, 2, "Макс.момент ген:");
	print_value_rus(19, 1, "А");
	values_page_7();	
	currentScreen = 7;
}

/*Initialize lcd*/
void lcd_init()
{
	OLED1.init();
	OLED1.backlight();
	delay(500); //according to datasheet ws0100
	OLED1.clear();
	page0();
	currentScreen = 1;
	delay(3000);
	page1();
}

/*show current page on lcd screen with its renewed values*/
void show_current_values()
{                              
	switch (currentScreen) {
		case 1:
			values_page_1();
			break;
		case 2:
			values_page_2();
			break;
		case 3:
			values_page_3();
			break;
		case 4:
			values_page_4();
			break;
		case 5:
			values_page_5();
			break;
		case 6:
			values_page_6();
			break;
		//case 7:
			//values_page_7();
			//break;		
	}
}

/*Switch pages*/
void switch_page() {
	switch (currentScreen) {
		case 1:
			page2();
			currentScreen = 2;
			break;
		case 2:
			page3();
			currentScreen = 3;
			break;
		case 3:
			page4();
			currentScreen = 4;
			break;
		case 4:
			page5();
			currentScreen = 5;
			break;
		case 5:
			page6();
			currentScreen = 6;
			break;
		case 6:
			page1();
			currentScreen = 1;
			break;	
		//case 7:
			//page1();
			//currentScreen = 1;
			//break;
	}	
}

