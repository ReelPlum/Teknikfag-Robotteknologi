#include <Arduino.h>

// put function declarations here:


int8_t pins[] = {4, 13, 16, 17, 21, 22, 26, 27, 23, 25, 18, 19, 32, 33};

void setup() {
  // put your setup code here, to run once:
  for (int8_t i : pins) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int8_t i : pins) {
    digitalWrite(i, HIGH);
  }

  usleep(10);
  
  for (int8_t i : pins) {
    digitalWrite(i, LOW);
  }
  
  usleep(10);
}