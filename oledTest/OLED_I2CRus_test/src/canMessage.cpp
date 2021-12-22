#include "canMessage.h"

MCP2515 mcp2515(10); //CS pin
struct can_frame canMsg1; //for transcieving can messages
struct can_frame canMsg; //for receiving can messages
int flagLight; //for a led blink control

int stateOn = 1, stateOff = 1, stateOn_, stateOff_;	//for saving the state of the buttons Start/Stop
const int SYS_FLAG_WAIT = 1, SYS_FLAG_OP = 2, SYS_FLAG_FAULT = 8; //led flags of system modes(SYS_FLAG_CHARGE not used)
int ledState = LOW;
unsigned long currentMillis = 0, previousMillis = 0, previousMillisLed = 0; //for a control message and led blink

int valLcd[Last]; //array for saving lcd values 
int index; //index of received/transmitted can messages

/*Set and get lcd values*/
int set_values(int value, lcdValues param)
{
	valLcd[param] = value;
}
int get_value(lcdValues param)
{
	return valLcd[param];
}

/*Initialize can*/
void can_init()
{
	SPI.begin();
	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
	mcp2515.reset();
	mcp2515.setBitrate(CAN_250KBPS,MCP_16MHZ);
	mcp2515.setNormalMode();
}

/*Read can messages and save values for pages*/
void get_values_for_pages()
{
	mcp2515.readMessage(&canMsg);
	index = canMsg.can_id;
	switch (index) {
			case 0x181:
				set_values(canMsg.data[2] + (canMsg.data[3] << 8), ac_dc_FaultCode);
				break;
			case 0x281: 
				set_values((canMsg.data[2] + (canMsg.data[3] << 8)) >> 4, gen_CurrentAC);
				set_values((canMsg.data[0] + (canMsg.data[1] << 8)) >> 4, gen_VoltageAC);
				set_values((get_value(gen_VoltageAC) * get_value(gen_CurrentAC))/1000, gen_PowerAC);
				set_values((canMsg.data[5] + (canMsg.data[4] << 8)) >> 4, ac_dc_CurrentDC);
				set_values((canMsg.data[7] + (canMsg.data[6] << 8)) >> 4, ac_dc_VoltageDC);
				break;
			case 0x381:
				set_values(canMsg.data[6], ac_dc_TempratureDC);
				set_values((canMsg.data[2] + (canMsg.data[3] << 8)) >> 4, gen_Temperature);
			case 0x481: //Gen_Frequency
				set_values((canMsg.data[0] + (canMsg.data[1] << 8) + (canMsg.data[2] << 16) + (canMsg.data[3] << 24)), gen_Velocity);
				break;
			case 0x18f3e1a0: //DcDc_Out (dcControl_ActualValues)		
				set_values(0.1 * (canMsg.data[0] + (canMsg.data[1] << 8)), dc_VoltageDC);
				set_values(0.1 * (canMsg.data[2] + (canMsg.data[3] << 8)), dc_CurrentDC);
				set_values((get_value(dc_VoltageDC) * get_value(dc_CurrentDC))/1000, dc_PowerDC);
				set_values(canMsg.data[6], dc_HeatsingTemp);
				break;
			case 0x18f3e0e0: //DcAc_Out							
				set_values(canMsg.data[2], ac_VoltageAC);
				set_values(canMsg.data[3], ac_CurrentAC);
				set_values((get_value(ac_VoltageAC) * get_value(ac_CurrentAC))/1000, ac_PowerAC);
				break;
			case 0x18ff0027: //mcu_st  
				set_values((canMsg.data[7]>>0) & 1, mcu_st_ACDC_on);
				set_values((canMsg.data[7]>>1) & 1, mcu_st_ACDC_fault);		
				set_values((canMsg.data[7]>>2) & 1, mcu_st_DCDC_on);
				set_values((canMsg.data[7]>>3) & 1, mcu_st_DCDC_fault);
				set_values((canMsg.data[7]>>4) & 1, mcu_st_DCAC_on);
				set_values((canMsg.data[7]>>5) & 1, mcu_st_DCAC_fault);		
				set_values((canMsg.data[7]>>6) & 1, mcu_st_Iso_fault);	
				break;	
			case 0x18f3e0a0:
				set_values(canMsg.data[2] + (canMsg.data[3] << 8), dc_FaultCode);
				break;				
		}
}

/*Send control message to control unit*/
void send_control_message()
{
	canMsg1.can_id  = 0x18FF0028;
	canMsg1.can_dlc = 8;
	for (int j = 1; j < 8; j++) {
		canMsg1.data[j] = 0x00;
	}
	stateOn = digitalRead(buttonStart);
	stateOff = digitalRead(buttonStop);
	if (stateOn == 0 && stateOn_ == 1) {
		canMsg1.data[0] = 0x01;
	}
	if (stateOff == 1 && stateOff_ == 0) {
		canMsg1.data[0] = 0x00;
	}
	stateOff_ = digitalRead(buttonStop);
	stateOn_ = digitalRead(buttonStart);
	currentMillis = millis();
	if(currentMillis - previousMillis >= 500) {
		previousMillis = currentMillis;
		delay(10);
		mcp2515.sendMessage(&canMsg1);
	}
}

/*Receive can message with flagLight and control leds*/
void control_leds_message()
{
	mcp2515.readMessage(&canMsg);
	index = canMsg.can_id;
	if (index == 0x18FF0027) {
		flagLight = canMsg.data[7];
		switch(flagLight) {
			case SYS_FLAG_WAIT:  //1
				digitalWrite(buttonPinYellow, 1);
				digitalWrite(buttonPinRed, 0);
				break;
			case SYS_FLAG_FAULT: //8
				digitalWrite(buttonPinYellow, LOW);
				digitalWrite(buttonPinRed, HIGH);
				break;
			case SYS_FLAG_OP:  //2
				unsigned long currentMillisLed = millis();
				if (currentMillisLed - previousMillisLed >= 1000) {
					previousMillisLed = currentMillisLed;
					if (ledState == LOW) {
						ledState = HIGH;
						} 
					else 
						ledState = LOW;
					digitalWrite(buttonPinYellow, ledState);
				}
				digitalWrite(buttonPinRed, LOW);
				break;	}	
	}		
}


void can_change_page_7(int v_can, int st_can) {
  canMsg.can_id  = 0x36;
  canMsg.can_dlc = 8;
  canMsg.data[0] = 0x00;
  canMsg.data[1] = 0x01;
  canMsg.data[2] = 0x00;
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg);
}

