#include <Arduino.h>
#include <Web.h>

double UpdateCallback(char subtype){
  return 1.0;
};

void ChangeCallback(double *paramValue, char subtype)
{
};

void setup() {
  // put your setup code here, to run once:
  BeginParing(ChangeCallback, UpdateCallback);
};

void loop() {
  // put your main code here, to run repeatedly:
};