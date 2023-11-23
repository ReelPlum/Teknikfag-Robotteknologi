#include <Arduino.h>
#include <DCMotor.h>


DCMotor motor(false, 17, 26, 27, 1, 18, 19, 23, 1, 19500, 12, 0.1, 4000, -100, 100, 100, 200);

void setup(){
  Serial.begin(115200);

  log_i("AMEN NU MÅ DU KRAFTÆDMNE!!");
  motor.init();
}

void loop(){
}