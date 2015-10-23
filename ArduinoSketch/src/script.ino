#include "Main.h"

Main m;
void setup(){
	Serial.begin(9600);
  	m.Setup();
  
}

void loop(){
  m.Loop();
}











