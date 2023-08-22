#include <Arduino.h>
#include <Servo.h>

const int32_t SERVO_1 = 0;
int32_t i = 1;

void setup()
{
  Setup_Servo(0, 18);
}

void loop()
{

  Servo_Move(0, MAX_ANGLE, SERVO_1);
  delay(2000);
  Servo_Move(180, MAX_ANGLE, SERVO_1);
  delay(2000);


}
