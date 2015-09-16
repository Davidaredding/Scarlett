#include <SoftwareSerial.h>


#define data 16 //DS pin 14
#define clk 17 //SHCP Pin 11
#define latch 18 //STCP pin 12 
#define enabled 19 //OE pin 13 active low

SoftwareSerial hm10(14,15); // RX, TX

bool CommandMode = false;
bool ServiceMode = false;

char cmdBuffer[20];
short cmdBufferCnt = 0;

short hm10_buffer_size = 0;
short hm10_buffer_wait = 0;

char relayStatus = 0;


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
  //If we don't do this the BTModule may not respond after being powered down.
  //hm10.print("Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up!");
}

void loop()
{
  serial_read();
  blueToothSerialRead();
}

void serial_read(){
  /*while(Serial.available() > 0)
  {
    char c = Serial.read();
    //Exit Command Mode
    if(c == '~' && CommandMode)
    {
      CommandMode = false;
      Serial.println("Exiting Command Mode");
    }

    //Enter Command Mode
    if(c == '^' && !CommandMode)
    {
      CommandMode = true;
      Serial.println("Entering Command mode, Sending 'AT'");
      hm10.print("AT");
    }
    else if(!CommandMode)
    {
      hm10.print(c);
    }



    //Recieve Commands
    if(CommandMode)
    {
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
        Serial.print("\r\nCmd Sent");
        Serial.println(cmdBuffer);
        memset(cmdBuffer,0,20);
      }
    }
  }*/
}


void processCommandResponse(){
  short cnt = hm10.available();

  while(hm10.available() >0)
  {
    char c = hm10.read();
    Serial.print(c);
  }
}

void PulseRegister(){
  digitalWrite(clk,HIGH);
  digitalWrite(clk,LOW);
}

void LatchRegister(){
  digitalWrite(latch,HIGH);
  digitalWrite(latch,LOW);
}


typedef void (*processor)(char serialRead);
processor currentProcessor = NULL;
char processorBuffer[10];
short processorBufferIndex = 0;

void blueToothSerialRead()
{
  while(hm10.available()>0)
  {
    char serialRead = hm10.read();
    Serial.print(serialRead);
    if(currentProcessor == NULL)
      setCurrentProcessor(serialRead);
    else
      currentProcessor(serialRead);
  }
}

void setCurrentProcessor(char serialRead)
{
  switch(serialRead){
    case 1:
      Serial.print("Using Relay Processor");
      currentProcessor = relayProcessor;
    default:
      Serial.print("No Processor found for " + serialRead);
  }
  Serial.println();
  Serial.flush();
}


//Processor Template
/*******************************************************
 *  Used to create new processors and demonstrate the  *
 *  pattern to follow                                  *
 *******************************************************

void ProcessorTemplate(char serialRead)
{
  processorBuffer[processorBufferIndex] = serialRead;
  processorBufferIndex ++;
  if(processorBufferIndex == [Expected bytes for this processor])
  {
    //Do work
    
    //Cleanup
    memset(processorBuffer,0,processorBufferIndex);
    currentProcessor = NULL;
    processorBufferIndex = 0;
  }
  return;
}
*/


void relayProcessor(char serialRead)
{
  //this processor only requires one byte of data 
  //so we dont' need to use the buffer
  
  //Do work
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(data,serialRead&0x01);
    PulseRegister();
    serialRead >>= 1;
  }
  LatchRegister();
    //Cleanup
  currentProcessor = NULL;
}











