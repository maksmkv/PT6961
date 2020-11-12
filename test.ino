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

void command1() //set display to 6x12 segments
{
    digitalWrite(CS, LOW);  //
    digitalWrite(CLK, LOW);
    digitalWrite(DIN, LOW);
      shiftOut(DIN, CLK, LSBFIRST, 0b00000011); // SET COMMAND 1
    digitalWrite(CS, HIGH);  //
    digitalWrite(CLK, HIGH);
    digitalWrite(DIN, HIGH);
    Serial.print("command1\n");
}  

void command2() {
    digitalWrite(CS, LOW);  //
    digitalWrite(CLK, LOW);
    digitalWrite(DIN, LOW);
    shiftOut(DIN, CLK, LSBFIRST, 0b01000000); // SET COMMAND 2 write
    digitalWrite(CS, HIGH);  //
    digitalWrite(CLK, HIGH);
    digitalWrite(DIN, HIGH);
    Serial.print("command2\n");
}

void command3() // Заполнение RAM нулями SET COMMAND 2
{
    digitalWrite(CS, LOW);  //
    digitalWrite(CLK, LOW);
    digitalWrite(DIN, LOW);
    shiftOut(DIN, CLK, LSBFIRST, 0b11000000);
      for(int i=0; i<8; i++)
      {
        shiftOut(DIN, CLK, LSBFIRST, 0b00000000);
      }
    digitalWrite(CS, HIGH);  //
    digitalWrite(CLK, HIGH);
    digitalWrite(DIN, HIGH);
    Serial.print("command3\n");
}

void command4() //display on, max brightness
{
    digitalWrite(CS, LOW);  //
    digitalWrite(CLK, LOW);
    digitalWrite(DIN, LOW);
    shiftOut(DIN, CLK, LSBFIRST, 0b10001111); // SET COMMAND 4
    digitalWrite(CS, HIGH);  //
    digitalWrite(CLK, HIGH);
    digitalWrite(DIN, HIGH);
    Serial.print("command4\n");
}


void setup() {
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);

Serial.begin(9600);
  command2();
  command3();
  command1();
  command4();

}

void loop() 
{
  digitalWrite(CS,LOW);
  shiftOut(DIN, CLK, LSBFIRST, byte(0xC0));
  shiftOut(DIN, CLK, LSBFIRST, 0b11111111);
  Serial.print("Code ON\n");
  digitalWrite(CS,HIGH);
}
