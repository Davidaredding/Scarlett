
#include "Arduino.h"
#include <EEPROM.h>
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
  switchBytes = {0,0,0,0,0,0,0};
  Main::instance = this;
}

void Main::Setup(){
  Serial.begin(9600);

  Serial.println("Reading values from EEPROM");

  for(int i =0; i<8; i++)
  {
    switchBytes[i] = EEPROM.read(i);
  }


  // Open serial communications and wait for port to open:
  
  // set the DATA rate for the SoftwareSerial port
  hm10->begin(9600);
  
  
  
  
  pinMode(DATA,OUTPUT); //shifts to input for load
  pinMode(CLOCK,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(LOAD,OUTPUT);
  pinMode(SP_DATA_PIN,INPUT);
   
  relayStatus = 0;
  writeRelays();
  
  SetupSwitchHandlers();

  Serial.println("STARTED IN MAIN");
}
int a = 0;
int cnt = 1;
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
    Serial.print("REceived Bluetooth -> ");

    char serialRead = hm10->read();
    Serial.println(serialRead,BIN);
    if(processor_ptr == 0)
      setProcessor(serialRead);
    else
      (this->*processor_ptr)(serialRead);

  }
}

void Main::setProcessor(char serialRead){
  Serial.print("Received ");
  Serial.println(serialRead,BIN);
    if(serialRead == 1 || serialRead == 'A'){
      Serial.println("Using All Relay Processor");
      processor_ptr = &Main::all_relay_processor;
    }
    else if(serialRead == 2 || serialRead == 'S'){
      Serial.println("Using Switch processor");
      processor_ptr = &Main::switch_processor;

    }
    else if(serialRead == 3 || serialRead == 'P'){
      Serial.println("Programming Mode - EEPROM");
      processor_ptr = &Main::eeprom_programming_processor;
    }
    else if(serialRead == 4 || serialRead == 'X'){
      Serial.println("Setting All Relays Off!");
      relayStatus = 0;
      writeRelays();
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

void Main::switch_processor(char serialRead)
{
  processor_Buffer[processor_Buffer_Index] = serialRead;
  processor_Buffer_Index ++;
  Serial.print(serialRead);

  if(serialRead =='\r'){
    int switch_id = processor_Buffer[0] - '0';
    bool state = processor_Buffer[1] - '0';

    if(switch_id <0 || switch_id > 7){
      Serial.print("Invalid switch id : ");
      Serial.println(switch_id);
    }
      
      ToggleSwitch(switch_id,state);
      processorCleanup();
  }
}

void Main::eeprom_programming_processor(char serialRead)
{
    processor_Buffer[processor_Buffer_Index] = serialRead;
    processor_Buffer_Index ++;
    Serial.print(serialRead);
 
    if(serialRead == '\r')
    {
      short s_id = processor_Buffer[0] - '0';
      processor_Buffer[0] = '0';

      Serial.println(processor_Buffer);

      short s_val = atoi(processor_Buffer);
      Serial.println(s_val,BIN);

      
      if(s_id <0 || s_id > 7){
        Serial.print("Rejecting invalid switch id : ");
        Serial.println(s_id);
      }
      else
      {
        Serial.println("Saved programming inputs");
        EEPROM.write(s_id,s_val);
        switchBytes[s_id] = s_val;
      }
  
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
  short temp = relayStatus;
  /*Serial.print("Writing ");
  Serial.print(temp,BIN);
  Serial.println(" to relays");*/
  
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(DATA,temp&1);
    pulseClock();
    temp >>= 1;
    Serial.println(temp,BIN);
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
  sp.Switches[0].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[1].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[2].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[3].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[4].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[5].switch_toggle_delegate = &Hardware_Switch_Toggle;
  sp.Switches[6].switch_toggle_delegate =Hardware_Switch_Toggle;
  sp.Switches[7].switch_toggle_delegate = &Hardware_Switch_Toggle;
}

void Main::ToggleSwitch(short id, bool state)
{
  if(id<0 || id > 7)
  {
    Serial.print("Invalid ID ");
    Serial.println(id,DEC);
    return;
  }

  Serial.print("Switch ");
  Serial.print(id);
  Serial.print(" turned ");
  if(state)
    Serial.println("ON");
  else
    Serial.println("OFF");



   short mask = Main::instance->switchBytes[id];
   Serial.print("Mask : ");
   Serial.println(mask,BIN);
   if(state)
   {
      Main::instance->relayStatus |= mask;
      writeRelays();
   }  
   else
   {
      Main::instance->relayStatus = relayStatus & ~mask;
      writeRelays();
   }

   Serial.print("Current Relay status : ");
   Serial.println(Main::instance->relayStatus,BIN);


}


void Main::Hardware_Switch_Toggle(Switch* sender)
{
  
    bool state = sender->getState();
    short id = sender->ID;
    Serial.print("Hardware switch ");
    Serial.print(id,DEC);
    Serial.print(" set ");
    if(state)
      Serial.println("ON");
    else
      Serial.println("OFF");
    Main::instance->ToggleSwitch(id,state);
  
}