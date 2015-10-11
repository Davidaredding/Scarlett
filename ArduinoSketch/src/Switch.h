#ifndef Switches_h
#define Switches_h

#include "Arduino.h"
class Switch
{
public:
	Switch();
	Switch(byte mask);
	bool getState();
	void setState(bool val);
	byte Mask;
	void (* switch_toggle_delegate)(Switch* sender);
private:
	bool _state;
};



#endif 