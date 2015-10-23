#ifndef Switches_h
#define Switches_h

#include "Arduino.h"
class Switch
{
public:
	short ID;
	Switch();
	Switch(byte mask, short id);
	bool getState();
	void setState(bool val);
	byte Mask;
	void (* switch_toggle_delegate)(Switch* sender);
private:
	bool _state;
};



#endif 