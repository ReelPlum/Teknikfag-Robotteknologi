#include <Arduino.h>

// put function declarations here:
// int i = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(23, OUTPUT);

  Serial.println("Setup exiting");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Serial.print("Counter: ");
  // Serial.println(i);

  // i = i + 1;

  // delay(500);

  while (1)
  {
    digitalWrite(23, HIGH);

    digitalWrite(23, LOW);
  }
}