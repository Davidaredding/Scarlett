#include <Arduino.h>
#include <SoftwareSerial.h>
void setup();
void loop();
void serial_read();
void hm10_read();
void processCommandResponse();
void processRegister();
void WriteRegister(char c);
void PulseRegister();
void LatchRegister();
#line 1 "src/script.ino"
//#include <SoftwareSerial.h>


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




void setup()
{
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
  hm10.println("Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up!");
  
  
}


void loop() // run over and over
{
  serial_read();
  hm10_read();
}

void serial_read(){
  while(Serial.available() > 0)
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

    //Recieve Commands
    if(CommandMode)
    {
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
  }

}

void hm10_read()
{
  bool cnt = hm10_buffer_size < hm10.available();
  if(cnt)
  {
    hm10_buffer_wait = 0;    
    hm10_buffer_size = hm10.available();
  }
  else if(hm10.available() > 0)
  {
    hm10_buffer_wait ++;
  }

  if(hm10_buffer_wait >= 3)
  {
      hm10_buffer_wait = 0;
      hm10_buffer_size = 0;

      if(CommandMode)
        processCommandResponse();
      else
        processRegister();
      Serial.println(" ");
  }

}

void processCommandResponse()
{
  short cnt = hm10.available();

  while(hm10.available() >0)
  {
    char c = hm10.read();
    Serial.print(c);
  }
  
}

void processRegister()
{
  while(hm10.available()>0)
  {
    char i = hm10.read();
    Serial.print(i);
    WriteRegister(i);
  }
  LatchRegister();
}

//Clocks in the data, does not latch
void WriteRegister(char c){
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(data,c&0x01);
    PulseRegister();
    c >>= 1;
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



















