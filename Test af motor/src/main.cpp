#include <Arduino.h>
#include <Servo.cpp>

const int32_t SERVO_1 = 0;

void setup() {
  Setup_Servo(0,18);
  
}

void loop() {
  Servo_Move(90,MAX_ANGLE,SERVO_1);
  delay(2000);
  Servo_Move(180,MAX_ANGLE,SERVO_1);
  delay(2000);
  Servo_Move(0,MAX_ANGLE,SERVO_1);
}

