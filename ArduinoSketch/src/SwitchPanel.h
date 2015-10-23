#ifndef SwitchPanel_h
#define SwitchPanel_h

#include "Arduino.h"
#include "Switch.h"

#define SWITCH_COUNT 8

#define SP_DATA_PIN 12
#define SP_CLOCK_PIN 13
#define SP_LOAD_PIN 9
#define POLLING_PERIOD 50
#define SP_DEBUG true

class SwitchPanel
{
public:
	SwitchPanel();
	Switch Switches[SWITCH_COUNT];
	void Poll();

private:
	byte current_switch_status;
	byte previous_switch_status;
	//byte debounce_switch_status; not sure if we need this
	unsigned long lastRead;
	
	void PulseClock();
	void LoadInputs();
	void ProcessCurrentSwitchStatus();

};

#endif