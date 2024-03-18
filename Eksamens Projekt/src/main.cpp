#include <Arduino.h>
#include <Stabilizer.h>

//#include <DCMotor.h>

#define DT .1

#define KI 1
#define KD 0
#define KP 0.05

DCMotor motorR(false, 17, 33, 32, 1, 5, 4, 2, 1, 19500, 12, DT, 4000, -100, 100, 6000, 100000, 1990);
DCMotor motorL(false, 17, 27, 26, 1, 18, 19, 23, 2, 19500, 12, DT, 4000, -100, 100, 6000, 100000, 1990);

Stabilizer stabilizer;

double UpdateCallback(char subtype){
  return 1.0;
};

void ChangeCallback(double *paramValue, char subtype)
{
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  motorR.init(KI, KD, KP);
  motorL.init(KI, KD, KP);

  stabilizer.init(&motorR, &motorL);
};

void loop() {
  // put your main code here, to run repeatedly:

};