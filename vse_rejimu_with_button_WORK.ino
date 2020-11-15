//315 МГц
//Received 7652288 / 24bit Protocol: 1   - кнопка А
//Received 7652144 / 24bit Protocol: 1  - кнопка B
//Received 7652108 / 24bit Protocol: 1 - кнопка С
//Received 7652099 / 24bit Protocol: 1   - кнопка D


#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68

#include "OasisLED.h"
#include <Arduino.h>

#include <IRremote.h>
#include <RCSwitch.h>


#define KEY_1   0x0800
#define KEY_2   0x0100
#define KEY_3   0x0008
#define KEY_4   0x0001

static uint16_t keys = 0;
uint16_t newKeys; // 2 байта


int RECV_PIN = 11;

uint8_t DOUT = 5; //button
uint8_t dataPin = 4;//DIN
uint8_t clockPin = 3;//CLK
uint8_t strobePin = 2; //CS

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

OasisLED ledDisplay = OasisLED(clockPin, strobePin, dataPin);

IRrecv irrecv(RECV_PIN);
decode_results results;
RCSwitch mySwitch = RCSwitch();

int n=0;  // переключение режимов работы

byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}


void setDateDs1307(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));     
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}

void getDateDs1307(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{

  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f); 
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}


void receiveData(){
          
  if(irrecv.decode(&results))
  {
     //Serial.println(results.value, HEX); 
    
  if ((results.value == 0x37) || (results.value == 0x10037))
   {
       // Serial.println(results.value, HEX);
    ++n;
        //  Serial.println(n);
  if (n>=7) n=0;
   }
   
   
   
    switch(n) {
       case 1: 
           if ((results.value == 0x37) || (results.value == 0x10037)){  // переключение режимов работы
               mySwitch.send(7652288,24);}
           break;
       case 2: 
           if ((results.value == 0x37) || (results.value == 0x10037)){// переключение режимов работы
               mySwitch.send(7652288,24);}
           break;
           
       case 3: 
           if ((results.value == 0x37) || (results.value == 0x10037)){// переключение режимов работы
               mySwitch.send(7652144,24); }
           break;  
           
       case 4: 
           if ((results.value == 0x37) || (results.value == 0x10037)){// переключение режимов работы
               mySwitch.send(7652144,24); }
           break;  
           
       case 5: 
       if ((results.value == 0x37) || (results.value == 0x10037)){// переключение режимов работы
               mySwitch.send(7652288,24);
               delay(100);
               mySwitch.send(7652144,24); }
               break;  
    
       case 6: 
           if ((results.value == 0x37) || (results.value == 0x10037)){// переключение режимов работы
               mySwitch.send(7652288,24);
               delay(100);
               mySwitch.send(7652144,24); }
               break;  
              }
              
              
  if ((results.value == 0x6E) || (results.value == 0x1006E)){
    mySwitch.send(7652288,24);
  }
  
   if ((results.value == 0x70) || (results.value == 0x10070)){
    mySwitch.send(7652144,24);
  }
        
  delay (500);   
  irrecv.resume(); 
    }
  }


void printTime()
{
 // byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  ledDisplay.setValue(hour*100+minute);
  ledDisplay.toggleColon();
  delay (1000);  
  ledDisplay.toggleColon();
  delay(1000);
  ledDisplay.reset();
}


void readKeyData(uint16_t * data) {
     
    digitalWrite(strobePin, LOW);  //
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 0x42);  // чтение данных
    delayMicroseconds(1);
     *data = shiftIn(DOUT, clockPin, LSBFIRST) + (shiftIn(DOUT, clockPin, LSBFIRST) << 8);
    digitalWrite(strobePin, HIGH);  //
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, HIGH);
}

void hourUP(){
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f); 
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
  
  hour = hour + 1;
  if (hour>23) {
    hour = 0;
  }
  
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
}

  
void minuteUP(){
    getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f); 
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
  
  minute = minute + 1;
  if (minute>59) {
    minute = 0;
  }
  
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
  }
  
void setup()
{
  Wire.begin();
  ledDisplay.initialize();
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  mySwitch.enableTransmit(10);
  mySwitch.setPulseLength(237);
  
  pinMode(DOUT, INPUT);
  pinMode(DOUT, INPUT_PULLUP);  //кнопка 
  
  second = 00;
  minute = 02;
  hour = 14;
  dayOfWeek = 7;
  dayOfMonth = 15;
  month = 11;
  year = 20;
  //setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);   // установка даты
}



void loop()
{
 printTime();
 receiveData();
 readKeyData(& newKeys);
 if (keys != newKeys) {
    keys = newKeys;
    if (keys & KEY_1) 
    {
     hourUP();
    }
    if (keys & KEY_2) 
    { 
      minuteUP();
    }
  }
} 
