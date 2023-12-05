#include <Arduino.h>
#include <DCMotor.h>
#include <DeadReckoning.h>

#define WHEELRADIUS 5
#define DT .1

DCMotor motorR(false, 17, 32, 33, 1, 4, 5, 2, 1, 19500, 12, DT, 4000, -100, 100, 100, 200, 1990);
DCMotor motorL(false, 17, 26, 27, 1, 19, 18, 23, 2, 19500, 12, DT, 4000, -100, 100, 100, 200, 1990);

double currentX = 0;
double currentY = 0;
double currentAngle = 0;

void DeadReckoningTask(void *args)
{
  TickType_t xTimeIncrement = configTICK_RATE_HZ * p->pidPos.get_dt();
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  { // loop tager mindre end 18us * 2
    double aR = CalculateAcceleration(motorR.get_acceleration(), WHEELRADIUS);
    double aL = CalculateAcceleration(motorL.get_acceleration(), WHEELRADIUS);

    double wR = CalculateVelocity(motorR.get_velocity(), WHEELRADIUS);
    double wL = CalculateVelocity(motorL.get_velocity(), WHEELRADIUS);

    GetPosition(&currentX, &currentY, &currentAngle, aR, aL, wR, wL, );
  }
}

void setup()
{
  Serial.begin(115200);

  delay(100);

  motorR.init(2, 0, 0);
  motorL.init(2, 0, 0);
}

void loop()
{
}