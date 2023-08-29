#include <Servo.h>

const int32_t SERVO_1 = 0;
int32_t i = 1;

void setup()
{
  Serial.begin(115200);
  Setup_Servo(SERVO_1, 18);
}

void loop()
{

  Servo_Move(0, MAX_ANGLE, SERVO_1);
  delay(2000);
  Servo_Move(180, MAX_ANGLE, SERVO_1);
  delay(2000);

}
