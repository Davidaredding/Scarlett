
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Main.h"
#include "SwitchPanel.h"
#include "Switch.h"


//TODO: Rearrange these so we're not using all the analog pins for digital comms
//Setup for SPI

Main* Main::instance = NULL;

Main::Main(){
  hm10 = new SoftwareSerial(RX_S,TX_S);
  relayStatus = 0;
  processor_Buffer_Index = 0;
  switchBytes = {255,2,4,8,16,32,64,128};
  Main::instance = this;
}

void Main::Setup(){
  

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the DATA rate for the SoftwareSerial port
  hm10->begin(9600);

  digitalWrite(CE,HIGH);
  pinMode(CE,OUTPUT);
  pinMode(DATA,INPUT); //shifts to input for load
  pinMode(CLOCK,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(LOAD,OUTPUT);

   
  digitalWrite(DATA,LOW);
  for (int i = 0; i < 8; ++i)
  {
    pulseClock();
  }
  latchRegister();
  digitalWrite(CE,LOW);
  
  SetupSwitchHandlers();

  Serial.println("STARTED IN MAIN");
}

void Main::Loop(){
  uart_read();
  blueToothSerialRead();
  sp.Poll();
}

void Main::uart_read(){
  while(Serial.available() > 0)
  {
    char serialRead = Serial.read();
   if(processor_ptr == 0)
      setProcessor(serialRead);
    else
      (this->*processor_ptr)(serialRead);
  }
}


void Main::blueToothSerialRead(){
  while(hm10->available()>0)
  {
    char serialRead = hm10->read();
    if(processor_ptr == 0)
      setProcessor(serialRead);
    else
      (this->*processor_ptr)(serialRead);

  }
}

void Main::setProcessor(char serialRead){
  Serial.print("Received ");
  Serial.println(serialRead,BIN);
    if(serialRead == 1){
      Serial.println("Using All Relay Processor");
      processor_ptr = &Main::all_relay_processor;
    }
    else if(serialRead == 'I'){
      Serial.println("Using Individual Relay Processor");
      processor_ptr = &Main::individual_relay_processor;
    }
    else{
      Serial.println("No Processor found");
    }
}

//Sets all relays at once.
void Main::all_relay_processor(char serialRead){

  //this processor only requires one byte of DATA 
  //so we dont' need to use the buffer
  Serial.println("Relay Processor working ...");

  Serial.println(serialRead,BIN);
  //Do work
  relayStatus = serialRead;
  writeRelays();
    //Cleanup
  processor_ptr = 0;
}

//Modifies the existing relays, can affect one or all relays
void Main::individual_relay_processor(char serialRead){

  processor_Buffer[processor_Buffer_Index] = serialRead;
  processor_Buffer_Index ++;
  Serial.print(serialRead);
    //Do work
    if(serialRead == '\r'){
      Serial.println();
      
      char m = processor_Buffer[0];
      processor_Buffer[0] = '0';

      Serial.println(processor_Buffer);

      short val = atoi(processor_Buffer);
      Serial.println(val,BIN);

      switch(m)
      {
        case 'T':
          toggleRelay(val);
          break;
        case 'O':
          turnRelayOn(val);
          break;
        case 'X':
          turnRelayOff(val);
          break;
        default :
          Serial.println("Could not find a method with given input.");
          processorCleanup();
          return;
      }
      writeRelays();
      processorCleanup();
      
    }
}
void Main::processorCleanup(){
      memset(processor_Buffer,0,processor_Buffer_Index);
      processor_ptr = 0;
      processor_Buffer_Index = 0;
}
void Main::toggleRelay(short relayMask){
  relayStatus = relayStatus ^ relayMask;
}

void Main::turnRelayOn(short relayMask){
  relayStatus = relayStatus | relayMask;
}

void Main::turnRelayOff(short relayMask){
  relayStatus = relayStatus & (~relayMask);
}

void Main::writeRelays(){
  Serial.println("Writing to relays");
  char temp = relayStatus;
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(DATA,temp & 1);
    pulseClock();
    temp >>= 1;
  }
  latchRegister();
}

void Main::pulseClock(){
  digitalWrite(CLOCK,HIGH);
  digitalWrite(CLOCK,LOW);
}

void Main::latchRegister(){
  digitalWrite(LATCH,HIGH);
  digitalWrite(LATCH,LOW);
}

//************************************************//
//************* Toggle Switch Hanlders ***********//
void Main::SetupSwitchHandlers()
{
  sp.Switches[0].switch_toggle_delegate = &Switch_0_Toggle;
  sp.Switches[1].switch_toggle_delegate = &Switch_1_Toggle;
  sp.Switches[2].switch_toggle_delegate = &Switch_2_Toggle;
  sp.Switches[3].switch_toggle_delegate = &Switch_3_Toggle;
  sp.Switches[4].switch_toggle_delegate = &Switch_4_Toggle;
  sp.Switches[5].switch_toggle_delegate = &Switch_5_Toggle;
  //sp.Switches[6].switch_toggle_delegate = &Switch_6_Toggle;
  sp.Switches[7].switch_toggle_delegate = &Switch_7_Toggle;
}

void Main::Switch_0_Toggle(Switch* sender)
{
  bool state = sender->getState();
  short id = sender->ID;
  Main::instance->relayStatus = Main::instance->switchBytes[id];
  Main::instance->writeRelays();

}

void Main::Switch_1_Toggle(Switch* sender)
{

}

void Main::Switch_2_Toggle(Switch* sender)
{
 

}

void Main::Switch_3_Toggle(Switch* sender)
{


}

void Main::Switch_4_Toggle(Switch* sender)
{

}

void Main::Switch_5_Toggle(Switch* sender)
{

}

void Main::Switch_6_Toggle(Switch* sender)
{

}

void Main::Switch_7_Toggle(Switch* sender)
{
  
}