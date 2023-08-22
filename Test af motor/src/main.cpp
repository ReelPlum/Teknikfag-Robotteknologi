#include <Arduino.h>
#include <Servo.cpp>


void setup() {
  Setup_Servo(0,18);
  
}

void loop() {
  Servo_Move(90,MAX_ANGLE,0);
  delay(2000);
  Servo_Move(180,MAX_ANGLE,0);
  delay(2000);
  Servo_Move(0,MAX_ANGLE,0);
}

