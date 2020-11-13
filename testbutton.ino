//http://arduino.ru/forum/proekty/korpus-ot-drs-8300-v-arduino-proektakh

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


//0 - A
//1 - B
//2 - C
//3 - D
//4 - E
//5 - F
//6 - G
//7 - Colon

#define KEY_1   0x0800
#define KEY_2   0x0100
#define KEY_3   0x0008
#define KEY_4   0x0001

                      //0     1     2     3     4     5     6     7     8     9       
const char DISP[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};


uint8_t DIN = 4; //DIN
uint8_t CLK = 3; //CLK
uint8_t CS = 2; //CS

uint8_t DOUT = 5;


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
    shiftOut(DIN, CLK, LSBFIRST, 0b11000000); //C0
    for(int i=0; i<8; i++)
    {
      shiftOut(DIN, CLK, LSBFIRST, 0b00000000);
    }

    shiftOut(DIN, CLK, LSBFIRST, 0b11110000);//F0
    for(int i=0; i<8; i++)
    {
      shiftOut(DIN, CLK, LSBFIRST, 0b00000000);
    }

    shiftOut(DIN, CLK, LSBFIRST, 0b11111100);//FC
    for(int i=0; i<8; i++)
    {
      shiftOut(DIN, CLK, LSBFIRST, 0b00000000);
    }

    shiftOut(DIN, CLK, LSBFIRST, 0b11111111);//FF
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

void printDigit_raz1()
{
 int i;
  for (i=0; i<10; i++)
  {
    digitalWrite(CS,LOW);
    shiftOut(DIN, CLK, LSBFIRST, byte(0xC0));
    shiftOut(DIN, CLK, LSBFIRST, DISP[i]);
    digitalWrite(CS,HIGH);
    delay(1000);
  }
}

void printDigit_raz2()
{
 int i;
  for (i=0; i<10; i++)
  {
    digitalWrite(CS,LOW);
    shiftOut(DIN, CLK, LSBFIRST, byte(0xC2));
    shiftOut(DIN, CLK, LSBFIRST, DISP[i]);
    digitalWrite(CS,HIGH);
    delay(1000);
  }
}

void printDigit_raz3()
{
 int i;
  for (i=0; i<10; i++)
  {
    digitalWrite(CS,LOW);
    shiftOut(DIN, CLK, LSBFIRST, byte(0xC4));
    shiftOut(DIN, CLK, LSBFIRST, DISP[i]);
    digitalWrite(CS,HIGH);
    delay(1000);
  }
}

void printDigit_raz4()
{
 int i;
  for (i=0; i<10; i++)
  {
    digitalWrite(CS,LOW);
    shiftOut(DIN, CLK, LSBFIRST, byte(0xC6));
    shiftOut(DIN, CLK, LSBFIRST, DISP[i]);
    digitalWrite(CS,HIGH);
    delay(1000);
  }
}


void readKeyData(uint16_t * data) {
     
    digitalWrite(CS, LOW);  //
    digitalWrite(CLK, LOW);
    digitalWrite(DIN, LOW);
		shiftOut(DIN, CLK, LSBFIRST, 0x42);  // чтение данных
    delayMicroseconds(1);
    *data = shiftIn(DOUT, CLK, LSBFIRST) + (shiftIn(DOUT, CLK, LSBFIRST) << 8);
    digitalWrite(CS, HIGH);  //
    digitalWrite(CLK, HIGH);
    digitalWrite(DIN, HIGH);
}



void setup() {
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  
  pinMode(DOUT, INPUT);
  digitalWrite(DIN, HIGH);
  digitalWrite(CLK, HIGH);


  pinMode(DOUT, INPUT_PULLUP);  //кнопка 

Serial.begin(9600);
  command2();
  command3();
  command1();
  command4();

}


void loop() 
{
  static uint16_t keys = 0;
  uint16_t newKeys; // 2 байта
  readKeyData(& newKeys);
  if (keys != newKeys) {
    keys = newKeys;
    if (keys & KEY_1) 
    {
     // printDigit_raz1();
     Serial.println("Key1");
     
    }
    if (keys & KEY_2) 
    { 
      Serial.println("Key2");
    }
    if (keys & KEY_3) 
    { 
      Serial.println("Key3");
    }
   
  }
 // printDigit_raz1();
}

    
 /* digitalWrite(CS,LOW);
  shiftOut(DIN, CLK, LSBFIRST, byte(0xC0));
  shiftOut(DIN, CLK, LSBFIRST, 0b11111111);
  Serial.print("Code ON\n");
  digitalWrite(CS,HIGH);*/

   /* digitalWrite(CS,LOW);
  shiftOut(DIN, CLK, LSBFIRST, byte(0xC2));
  shiftOut(DIN, CLK, LSBFIRST, 0b11111111);
  Serial.print("Code ON\n");
  digitalWrite(CS,HIGH);*/

   /* digitalWrite(CS,LOW);
  shiftOut(DIN, CLK, LSBFIRST, byte(0xC4));
  shiftOut(DIN, CLK, LSBFIRST, 0b11111111);
  Serial.print("Code ON\n");
  digitalWrite(CS,HIGH);*/

   /* digitalWrite(CS,LOW);
  shiftOut(DIN, CLK, LSBFIRST, byte(0xC6));
  shiftOut(DIN, CLK, LSBFIRST, 0b11111111);
  Serial.print("Code ON\n");
  digitalWrite(CS,HIGH);*/


/*                    //0     1     2     3     4     5     6     7     8     9       
const char DISP[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};*/



// printDigit_raz1();
// printDigit_raz2();
// printDigit_raz3();
// printDigit_raz4();
