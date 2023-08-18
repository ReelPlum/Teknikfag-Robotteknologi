#include <Arduino.h>


//Define is a search and replace. Creates a constant
#define IO 23

//Constant variables are not a search and replace, but they cannot be changed. Types can be defined in constant variables.
const int Mom = 0;

// put function declarations here:
// int i = 0;

//Global variables start with uppercase
//All c-files has access to global variables in the whole program (If the correct procedure has been taken in the other programs)
int Counter = 0;

//Use static to keep it in this file.
static int Counter2 = 0;

int TEST = 23123;

int Pallse = 55352;

int magnus = 005352;
int plum = 2414;


// bool  
// short 16 bit
// char  8 bit

//signed char 8 bit -127 -> 128
//unsigned char 8 bit 0 -> 255

// int   32 bit heltal
// long  32 bit
// long long  64 bit
// float 7.1232 (32 bit)
// double 7.2144 (64 bit) Den er dog 32 bit på esp32

//For at få den samme størrelse på alle tingene kan vi bruge en h-fil der kan hjælpe med det. Her kan vi gøre således:
//int8_t -127->128
//int16_t
//int32_t
//int64_t
//
//uint8_t 0->255
//uint16_t
//uint32_t
//uint64_t
//
//double
//bool


void setup()
{
  //Local variables start with lowercase
  int i = 0;

  Serial.begin(115200);
  pinMode(IO, OUTPUT);

  Serial.println("Setup exiting");


  //You can limit a variables scope by creating a new block
  {
    int j = 0;

    Serial.println(j);
  }
  //I cannot call j here.
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Serial.print("Counter: ");
  // Serial.println(i);

  // i = i + 1;

  // delay(500);

  while (1)
  {
    digitalWrite(IO, HIGH);

    digitalWrite(IO, LOW);
  }
}