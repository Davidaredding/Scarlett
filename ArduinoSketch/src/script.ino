#include <SoftwareSerial.h>

#define data 16 //DS pin 14
#define clk 17 //SHCP Pin 11
#define latch 18 //STCP pin 12 
#define enabled 19 //OE pin 13 active low

SoftwareSerial hm10(14,15); // RX, TX
bool CommandMode = false;
char cmdBuffer[20];
short cmdBufferCnt = 0;

short hm10_buffer_size = 0;
short hm10_buffer_wait = 0;

short relayStatus = 0;

char  processor_Buffer[24];
short processor_Buffer_Index = 0;

void (* processor_ptr)(char s);




void setup(){
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  hm10.begin(9600);

  digitalWrite(enabled,HIGH);
  pinMode(enabled,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(latch,OUTPUT);
   
  digitalWrite(data,LOW);
  for (int i = 0; i < 8; ++i)
  {
    PulseRegister();
  }
  LatchRegister();
  digitalWrite(enabled,LOW);
  
  Serial.println("STARTED");
}

void loop(){
  uart_read();
  blueToothSerialRead();
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
    if(serialRead == 1){
      Serial.println("Using Relay Processor");
      processor_ptr = &all_relay_Processor;
    }
    if(serialRead == 'I'){
      Serial.println("Using Individual Relay Processor");
      processor_ptr = &individual_relay_processor;
    }
    else{
      Serial.println("No Processor found");
    }
}

//Sets all relays at once.
void all_relay_Processor(char serialRead){
  //this processor only requires one byte of data 
  //so we dont' need to use the buffer
  Serial.println("Relay Processor working ...");
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
          //turnRelayOn(val);
          break;
        case 'X':
          //turnRelayOff(val);
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
  Serial.println(relayMask,BIN);

  relayStatus = relayStatus ^ relayMask;
  
  Serial.println(relayStatus, BIN);
}

void turnRelayOn(char relayMask)
{
  relayStatus = relayStatus | relayMask;
}

void turnRelayOff(char relayMask)
{
  relayStatus = relayStatus & (~relayMask);
}

void WriteRelays()
{
  char temp = relayStatus;
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(data,temp & 0x01);
    PulseRegister();
    temp >>= 1;
  }
  LatchRegister();
}

void PulseRegister(){
  digitalWrite(clk,HIGH);
  digitalWrite(clk,LOW);
}

void LatchRegister(){
  digitalWrite(latch,HIGH);
  digitalWrite(latch,LOW);
}

/**************** Processor Template *******************
 *  Used to create new processors and demonstrate the  *
 *  pattern to follow                                  *
 *******************************************************

void ProcessorTemplate(char serialRead)
{
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












