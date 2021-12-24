#include "canMessage.h"

MCP2515 mcp2515(10); //CS pin
struct can_frame canMsg1; //for transcieving/receiving can messages in show mode
struct can_frame canMsg; 
struct can_frame canMsgChange; //for transcieving/receiving can messages in change mode
struct can_frame canMsgChange1;
int flagLight; //for a led blink control

int stateOn = 1, stateOff = 1, stateOn_, stateOff_;	//for saving the state of the buttons Start/Stop
const int SYS_FLAG_WAIT = 1, SYS_FLAG_OP = 2, SYS_FLAG_FAULT = 8; //led flags of system modes(SYS_FLAG_CHARGE not used)
int ledState = LOW;
unsigned long currentMillis = 0, previousMillis = 0, previousMillisLed = 0; //for a control message and led blink

int valLcd[Last]; //array for saving lcd values 
int index; //index of received/transmitted can messages

/*Set and get lcd values*/
int set_values(int value, canValues param)
{
	valLcd[param] = value;
}
int get_value(canValues param)
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
			set_values(canMsg.data[2] + (canMsg.data[3] << 4), ac_dc_FaultCode);
			break;
		case 0x281: 
			set_values((0.0625 * (canMsg.data[2] + (canMsg.data[3] << 8))), gen_Voltage);
			set_values(0.0625 * (canMsg.data[0] + (canMsg.data[1] << 8)), gen_Current);
			set_values(((get_value(gen_Voltage) * get_value(gen_Current)) * 0.01) , gen_Power);
			break;
		case 0x381:
			set_values(canMsg.data[6], ac_dc_Temprature);
			set_values((canMsg.data[2] + canMsg.data[3] ), gen_Temperature);
			break;
		case 0x481: //Gen_Frequency
			set_values((canMsg.data[0] + canMsg.data[1] + canMsg.data[2] + canMsg.data[3]), gen_Velocity);
			break;
		case 0x18f3e1a0: //DcDc_Out (dcControl_ActualValues)		
			set_values((0.1 * (canMsg.data[0] + (canMsg.data[1] << 8))), dc_dc_Voltage);
			set_values(0.1 * (canMsg.data[2] + (canMsg.data[3] << 8)), dc_dc_Current);
			set_values((get_value(dc_dc_Voltage) * get_value(dc_dc_Current))/1000, dc_dc_Power);
			set_values(canMsg.data[6], dc_dc_HeatsingTemp);
			break;
		case 0x18f3e0e0: //DcAc_Out							
			set_values(canMsg.data[2], dc_ac_Voltage);
			set_values(canMsg.data[3], dc_ac_Current);
			set_values((get_value(dc_ac_Voltage) * get_value(dc_ac_Current))/1000, dc_ac_Power);
			break;
		case 0x18ff0027: //mcu_st  
			set_values(canMsg.data[0], uki_resistance);
			set_values((canMsg.data[7] >> 0) & 1, mcu_st_ACDC_on);
			set_values((canMsg.data[7] >> 1) & 1, mcu_st_ACDC_fault);		
			set_values((canMsg.data[7] >> 2) & 1, mcu_st_DCDC_on);
			set_values((canMsg.data[7] >> 3) & 1, mcu_st_DCDC_fault);
			set_values((canMsg.data[7] >> 4) & 1, mcu_st_DCAC_on);
			set_values((canMsg.data[7] >>5) & 1, mcu_st_DCAC_fault);		
			set_values((canMsg.data[7] >>6) & 1, mcu_st_Iso_fault);	
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
				digitalWrite(PinYellow, 1);
				digitalWrite(PinRed, 0);
				break;
			case SYS_FLAG_FAULT: //8
				digitalWrite(PinYellow, LOW);
				digitalWrite(PinRed, HIGH);
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
					digitalWrite(PinYellow, ledState);
				}
				digitalWrite(PinRed, LOW);
				break;	}	
	}		
}

/**/
void send_value_change(int v_can, int st_can) 
{
  canMsgChange.can_id  = 0x6A0;
  canMsgChange.can_dlc = 8;
  canMsgChange.data[0] = 0x00;
  canMsgChange.data[1] = 0x01;
  canMsgChange.data[2] = 0x00;
  canMsgChange.data[3] = 0x00;
  canMsgChange.data[4] = 0x00;
  canMsgChange.data[5] = 0x00;
  canMsgChange.data[6] = 0x00;
  canMsgChange.data[7] = 0x00;
  mcp2515.sendMessage(&canMsgChange);
}

/**/
int receive_value_change()
{
	mcp2515.readMessage(&canMsgChange1);
	index = canMsgChange1.can_id;
	while (index != 0x58A) {
		return 1;
	}
}