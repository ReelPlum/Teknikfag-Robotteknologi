#include <Arduino.h>

// put function declarations here:
int i = 0;

void setup()
{
  Serial.begin(115200);

  Serial.println("Setup exiting");
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.print("Counter: ");
  Serial.print(i);

  i = i + 1;

  delay(500);
}