#include <Arduino.h>
#include <DCMotor.h>

DCMotor motorR(false, 17, 32, 33, 1, 4, 5, 2, 1, 19500, 12, .1, 4000, -100, 100, 100, 200, 1990);
DCMotor motorL(false, 17, 26, 27, 1, 19, 18, 23, 2, 19500, 12, .1, 4000, -100, 100, 100, 200, 1990);

void setup()
{
  Serial.begin(115200);

  delay(100);

  motorR.init(2, 0, 0);
  motorL.init(2, 0, 0);
}

void loop()
{
  motorR.set_velocity(4000);
  motorL.set_velocity(4000);
  delay(5000);
  motorR.set_velocity(1000);
  motorL.set_velocity(1000);
}