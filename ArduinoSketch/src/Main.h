#ifndef Main_H
#define Main_H

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "SwitchPanel.h"
#include "Switch.h"

//Bluetooth Software Serial
#define RX_S 4
#define TX_S 5

//74HC595 and 74HC165 Control pins
#define DATA 12
#define CLOCK 13

#define CE 11     //Specific To 595
#define LATCH 10

#define LOAD 9   //165's load register.

#define DEBUG true
#define POLLING_PERIOD 1000 //In MS

class Main
{
public:
	Main();
	static Main *instance;

	SoftwareSerial *hm10;
	short relayStatus;
	char processor_Buffer[24];
	short processor_Buffer_Index;
	void (Main::*processor_ptr)(char s);
	byte switchBytes[8];
	SwitchPanel sp;

	void Setup();
	void Loop();
	void uart_read();
	void blueToothSerialRead();
	void setProcessor(char serialRead);
	void all_relay_processor(char serialRead);
	void individual_relay_processor(char serialRead);
	void processorCleanup();
	void toggleRelay(short relayMask);
	void turnRelayOn(short relayMask);
	void turnRelayOff(short relayMask);
	void writeRelays();
	void pulseClock();
	void latchRegister();
	void SetupSwitchHandlers();
	


	static void Switch_0_Toggle(Switch* sender);
	static void Switch_1_Toggle(Switch* sender);
	static void Switch_2_Toggle(Switch* sender);
	static void Switch_3_Toggle(Switch* sender);
	static void Switch_4_Toggle(Switch* sender);
	static void Switch_5_Toggle(Switch* sender);
	static void Switch_6_Toggle(Switch* sender);
	static void Switch_7_Toggle(Switch* sender);


};

#endif