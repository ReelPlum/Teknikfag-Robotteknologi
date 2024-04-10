#include <Arduino.h>

#include <PulsingLed.h>

PulsingLed led(1, 8, 19500, 19, 0.5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial)
    ;

  log_i("setup");
  led.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //log_i("Hello world");

  //delay(200);
}