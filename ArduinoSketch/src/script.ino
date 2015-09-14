#include <SoftwareSerial.h>


#define data 16 //DS pin 14
#define clk 17 //SHCP Pin 11
#define latch 18 //STCP pin 12 
#define enabled 19 //OE pin 13 active low

SoftwareSerial hm10(14,15); // RX, TX

void setup()
{
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  hm10.begin(9600);

  pinMode(enabled,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(latch,OUTPUT);
  digitalWrite(enabled,LOW); 
  
  Serial.println("STARTED");
  //If we don't do this the BTModule may not respond after being powered down.
  hm10.println("Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up! Time to wake up! Wake up!");
  
  
}


void loop() // run over and over
{
  bluetooth_read();
}

void bluetooth_read(){

  while(hm10.available()>0)
  {
    short i = hm10.read();
    Serial.println(i);
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



















