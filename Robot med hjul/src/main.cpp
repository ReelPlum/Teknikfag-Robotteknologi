#include <Global.h>

#include <DCMotor.h>
#include <DeadReckoning.h>
#include <WebServer.h>

const int32_t WHEELRADIUS = 5;
const double DT = .01;
const double b = 24.5;

double currentX = 0;
double currentY = 0;
double currentAngle = 0;

DCMotor motorR(false, 17, 32, 33, 1, 5, 4, 2, 1, 19500, 12, DT, 4000, -100, 100, 100, 100000, 1990);
DCMotor motorL(false, 17, 26, 27, 1, 18, 19, 23, 2, 19500, 12, DT, 4000, -100, 100, 100, 100000, 1990);

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

    //log_i("Position is %f, %f and angle is %f", currentX, currentY, currentAngle);

    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

double getData(char subtype){
  
  //If it asks for something weird for some stupid reason
  return 0.0;
}

double speedRY = 0;
double speedLY = 0;

double speedRX = 0;
double speedLX = 0;

double lastSpeedR = 0;
double lastSpeedL = 0;

void updateMain(double *paramValue, char subtype)
{
  log_i("%f", *paramValue);

  switch(subtype){
    case 'x':
      //rotate
      //log_i("X %d", *paramValue);

      if (*paramValue > 0){
        speedRX = -2000;
        speedLX = 2000;
      }else if (*paramValue < 0){
        speedRX = 2000;
        speedLX = -2000;
      }else {
        speedRX = 0;
        speedLX = 0;
      }

      //log_i("SpeedX %i, %i", speedRX, speedLX);

    case 'y':
      //forward
      //log_i("Y");

      //log_i("Y %d", *paramValue);

      if (*paramValue > 0){
        //log_i("<");
        speedRY = -2000;
        speedLY = -2000;
      }else if (*paramValue < 0){
        //log_i(">");
        speedRY = 2000;
        speedLY =   2000;
      }else {
        //log_i("0");
        speedRY = 0;
        speedLY = 0;
      }

      //log_i("SpeedY %i, %i", speedRY, speedLY);
  };
  
  log_i("%f, %f", speedRY, speedLY);
}

void setup()
{
  Serial.begin(115200);

  delay(100);

  motorR.init(1, 0, 0.05);
  motorL.init(1, 0, 0.05);

  init_web(updateMain, getData);

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
  // log_i("%i",speedRX + speedRY);
  // log_i("%i", speedLX + speedLY);

  // if (lastSpeedR != speedRX + speedRY){
  //   log_i("R %f",speedRX + speedRY );
  //   lastSpeedR = speedRX + speedRY;

  //   motorR.set_velocity(speedRX + speedRY);
  // }
  // if (lastSpeedL != speedLX + speedLY){
  //   log_i("L %f",speedLX + speedLY );
  //   lastSpeedL = speedLX + speedLY;

  //   motorL.set_velocity(speedLX + speedLY);
  // }

  motorR.set_velocity(2000);
  motorL.set_velocity(2000);

  delay(5000);
  motorR.set_velocity(0);
  motorL.set_velocity(0);

  delay(5000);
  motorR.set_velocity(-2000);
  motorL.set_velocity(-2000);

  delay(5000);
}