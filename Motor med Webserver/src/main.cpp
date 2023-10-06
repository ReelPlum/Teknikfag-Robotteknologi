#include <global.h>
#include <WebServer.h>
#include <DCMotor.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  init_dc();
  init_web(*update);
}

void loop() {
  // put your main code here, to run repeatedly:
}