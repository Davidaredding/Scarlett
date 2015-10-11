#include <SoftwareSerial.h>
#include "SwitchPanel.h"
//TODO: Rearrange these so we're not using all the analog pins for digital comms
//Setup for SPI

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


SoftwareSerial hm10(RX_S,TX_S); 
bool CommandMode = false;
char cmdBuffer[20];
short cmdBufferCnt = 0;
short hm10_buffer_size = 0;
short hm10_buffer_wait = 0;
short relayStatus = 0;
char  processor_Buffer[24];
short processor_Buffer_Index = 0;
void (* processor_ptr)(char s);
char inputs = 0;
unsigned long lastRead = 0;
SwitchPanel sp;


void setup(){
  

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the DATA rate for the SoftwareSerial port
  hm10.begin(9600);

  digitalWrite(CE,HIGH);
  pinMode(CE,OUTPUT);
  pinMode(DATA,INPUT); //shifts to input for load
  pinMode(CLOCK,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(LOAD,OUTPUT);

   
  digitalWrite(DATA,LOW);
  for (int i = 0; i < 8; ++i)
  {
    PulseClock();
  }
  LatchRegister();
  digitalWrite(CE,LOW);
  
  Serial.println("STARTED");
}

void loop(){
  uart_read();
  blueToothSerialRead();
  sp.Poll();
}

void uart_read()
{
  while(Serial.available() > 0)
  {
    char serialRead = Serial.read();
    if(processor_ptr == 0)
      setProcessor(serialRead);
    else
      processor_ptr(serialRead);
    //processCommandMode(c);
  }
}


void processCommandMode(char c){
  if(c == '^' && !CommandMode)
  {
    CommandMode = true;
    Serial.println("Entering Bluetooth Command mode, Sending 'AT'");
    hm10.print("AT");
  }

  if(c == '~' && CommandMode)
  {
    CommandMode = false;
    Serial.println("Exiting Command Mode");
    return;
  }



  //Recieve Commands

  //Use terminating characters because the normal buffering is to fast for human input
  if(c!='!')
  {
    cmdBuffer[cmdBufferCnt] = c;
    cmdBufferCnt ++;
    Serial.print(c);
  }
  else 
  {
    hm10.print(cmdBuffer);
    cmdBufferCnt = 0;
    Serial.print("\r\nCmd Sent: ");
    Serial.println(cmdBuffer);
    memset(cmdBuffer,0,20);
  }
}

void processCommandResponse(){
  short cnt = hm10.available();

  while(hm10.available() >0)
  {
    char c = hm10.read();
    Serial.print(c);
  }
}


void blueToothSerialRead(){
  while(hm10.available()>0)
  {
    char serialRead = hm10.read();
    if(CommandMode)
    {
      Serial.print(serialRead);
      continue;
    }
    
    if(processor_ptr == 0)
      setProcessor(serialRead);
    else
      processor_ptr(serialRead);

  }
}

void setProcessor(char serialRead){
  Serial.print("Received ");
  Serial.println(serialRead,BIN);
    if(serialRead == 1){
      Serial.println("Using All Relay Processor");
      processor_ptr = &all_relay_Processor;
    }
    else if(serialRead == 'I'){
      Serial.println("Using Individual Relay Processor");
      processor_ptr = &individual_relay_processor;
    }
    else{
      Serial.println("No Processor found");
    }
}

//Sets all relays at once.
void all_relay_Processor(char serialRead){

  //this processor only requires one byte of DATA 
  //so we dont' need to use the buffer
  Serial.println("Relay Processor working ...");

  Serial.println(serialRead,BIN);
  //Do work
  relayStatus = serialRead;
  WriteRelays();
    //Cleanup
  processor_ptr = 0;
}

//Modifies the existing relays, can affect one or all relays
void individual_relay_processor(char serialRead)
{

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
      //Serial.println("Writing Relays");
      WriteRelays();
      processorCleanup();
      
    }

}
void processorCleanup()
{
      memset(processor_Buffer,0,processor_Buffer_Index);
      processor_ptr = 0;
      processor_Buffer_Index = 0;
}
void toggleRelay(short relayMask)
{
  relayStatus = relayStatus ^ relayMask;
}

void turnRelayOn(short relayMask)
{
  relayStatus = relayStatus | relayMask;
}

void turnRelayOff(short relayMask)
{
  relayStatus = relayStatus & (~relayMask);
}

void WriteRelays(){
  char temp = relayStatus;
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(DATA,temp & 1);
    PulseClock();
    temp >>= 1;
  }
  LatchRegister();
}



void PulseClock(){
  digitalWrite(CLOCK,HIGH);
  digitalWrite(CLOCK,LOW);
}

void LatchRegister(){
  digitalWrite(LATCH,HIGH);
  digitalWrite(LATCH,LOW);
}



/**************** Processor Template *******************
 *  Used to create new processors and demonstrate the  *
 *  pattern to follow                                  *
 *******************************************************
void ProcessorTemplate(char serialRead){
  processor_Buffer[processor_Buffer_Index] = serialRead;
  processor_Buffer_Index ++;
  if(processor_Buffer_Index == 2)
  {
    //Do work
    //Cleanup
    memset(processor_Buffer,0,processor_Buffer_Index);
    processor_ptr = 0;
    processor_Buffer_Index = 0;
  }
  return;
}
*/












