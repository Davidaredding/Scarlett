#include "Arduino.h"
#include "SwitchPanel.h"
#include "Switch.h"


SwitchPanel::SwitchPanel()
{
	lastRead = 0;
	current_switch_status = 0;
	previous_switch_status = 0;
	
	for(short i = 0; i < 8; i++)
	{
		byte b = 1<<i;
		Switch s(b,i);
		Switches[i] = s;
	}
	
}

void SwitchPanel::Poll()
{
	unsigned long t = millis();
	if((t - lastRead)<POLLING_PERIOD)
	  return;

	lastRead = t;
	pinMode(SP_DATA_PIN,INPUT);
	byte incoming = 0;
	LoadInputs();
	for(int i=0;i<8;i++)
	{
	  bool d = digitalRead(SP_DATA_PIN);
	  incoming = (incoming<<1)|d;
	  PulseClock();
	}

	/*#ifdef SP_DEBUG
	Serial.print("Switch input ");
	Serial.println(incoming, BIN);
	#endif*/

	current_switch_status = incoming;

	ProcessCurrentSwitchStatus();

	pinMode(SP_DATA_PIN,OUTPUT);
}

void SwitchPanel::ProcessCurrentSwitchStatus(){
	/*#ifdef SP_DEBUG
	Serial.print("Previous Status: ");
	Serial.println(previous_switch_status,BIN);
	Serial.print("Current Status: ");
	Serial.println(current_switch_status,BIN);
	Serial.println("--------------------");
	#endif*/

	if(previous_switch_status == current_switch_status)
		return;
		
	previous_switch_status = current_switch_status;
	for(int i=0; i< SWITCH_COUNT; i++)
	{
		Switches[i].setState(current_switch_status&Switches[i].Mask);
	}
}



void SwitchPanel::PulseClock(){
  digitalWrite(SP_CLOCK_PIN,HIGH);
  digitalWrite(SP_CLOCK_PIN,LOW);
}

void SwitchPanel::LoadInputs(){
  digitalWrite(SP_LOAD_PIN,LOW);
  digitalWrite(SP_LOAD_PIN,HIGH);
}