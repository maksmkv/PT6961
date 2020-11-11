#include <Arduino.h>

//    const static char _DISPLAY_6X12 = 0x02;
//    const static char _DISPLAY_7X11 = 0x03;
//    const static char _AUTO_INCREMENT = 0x40;
//    const static char _FIXED_ADDRESS = 0x44;
//    const static char _DISPLAY_OFF = 0x80;
//    const static char _DISPLAY_1_16 = 0x88;
//    const static char _DISPLAY_2_16 = 0x89;
//    const static char _DISPLAY_4_16 = 0x8A;
//    const static char _DISPLAY_10_16 = 0x8B;
//    const static char _DISPLAY_11_16 = 0x8C;
//    const static char _DISPLAY_12_16 = 0x8D;
//    const static char _DISPLAY_13_16 = 0x8E;
//    const static char _DISPLAY_14_16 = 0x8F;


uint8_t DIN = 4; //DIN
uint8_t CLK = 3; //CLK
uint8_t CS = 2; //CS
   

void setup() {
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  
  //страница 16 datasheet из 25
  delay(200); //задержка 200 мс
  
  pinMode(CS, LOW);  //
  shiftOut(DIN, CLK, LSBFIRST, 0x01000000); // SET COMMAND 2
  digitalWrite(CS, HIGH); //

  initialise(); // SET COMMAND 2
    
  pinMode(CS, LOW);  //
  shiftOut(DIN, CLK, LSBFIRST, char(0x02)); // SET COMMAND 1
  digitalWrite(CS, HIGH); //

  pinMode(CS, LOW);  //
  shiftOut(DIN, CLK, LSBFIRST, 0x00000001); // SET COMMAND 4
  digitalWrite(CS, HIGH); //
}

void loop() {

}


void initialise() // Заполнение RAM нулями SET COMMAND 2
{
  digitalWrite(CS,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0xC0);
    for(int i=0; i<8; i++)
    {
      shiftOut(dataPin, CLK, LSBFIRST, 0x00);
    }
  digitalWrite(CS,HIGH); 
}
 

void writePT()
{
}

void readPT()
{
  
}
