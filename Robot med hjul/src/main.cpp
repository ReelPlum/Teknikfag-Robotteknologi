#include <Global.h>

DCMotor motorR(false, 17, 32, 33, 1, 4, 5, 2, 1, 19500, 12, DT, 4000, -100, 100, 100, 100000, 1990);
DCMotor motorL(false, 17, 26, 27, 1, 19, 18, 23, 2, 19500, 12, DT, 4000, -100, 100, 100, 100000, 1990);


TaskHandle_t DeadReckoningTaskHandle;

void DeadReckoningTask(void *args)
{
  TickType_t xTimeIncrement = configTICK_RATE_HZ * DT;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  { // loop tager mindre end 18us * 2
    double aR = CalculateAcceleration(motorR.get_acceleration(), WHEELRADIUS);
    double aL = CalculateAcceleration(motorL.get_acceleration(), WHEELRADIUS);
    double wR = CalculateVelocity(motorR.get_velocity(), WHEELRADIUS);
    double wL = CalculateVelocity(motorL.get_velocity(), WHEELRADIUS);
    GetPosition(&currentX, &currentY, &currentAngle, aR, aL, wR, wL, b, DT);

    //log_i("Accelerations are R: %f, L: %f and velocities are R: %f, L: %f",aR,aL,wR,wL);

    log_i("Position is %f, %f and angle is %f", currentX, currentY, currentAngle);

    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void setup()
{
  Serial.begin(115200);

  delay(100);

  motorR.init(2, 0, 0);
  motorL.init(2, 0, 0);
  
  xTaskCreatePinnedToCore(
      DeadReckoningTask,       /* Function to implement the task */
      "Dead Reckoning",     /* Name of the task */
      3000,           /* Stack size in words */
      NULL,           /* Task input parameter */
      3,              /* Priority of the task from 0 to 25, higher number = higher priority */
      &DeadReckoningTaskHandle, /* Task handle. */
      1);             /* Core where the task should run */
}

void loop()
{
  motorR.set_velocity(4000);
  motorL.set_velocity(4000);
  delay(5000);
  motorR.set_velocity(1000);
  motorL.set_velocity(1000);
  delay(5000);
}