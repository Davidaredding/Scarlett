 #include "Arduino.h"
 #include "Switch.h"

Switch::Switch(){}

Switch::Switch(byte mask)
{
	Mask = mask;
	_state = 0;
}

bool Switch::getState()
{
	return _state;
}

void Switch::setState(bool val)
{
	if(val == _state)
		return;
	_state = val;
	if(switch_toggle_delegate!=0)
		switch_toggle_delegate(this);
	
}