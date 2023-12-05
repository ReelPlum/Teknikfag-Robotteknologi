#include <global.h>
#include <WebServer.h>
#include <DCMotor.h>
#include <inversekinematic.h>
#include <Servo.h>

double l1 = 20;
double l2 = 13;
TaskHandle_t KinematikTaskHandle;


Servo s;

void kinematik(void *args){
  TickType_t xTimeIncrement = 10/1000;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true){
    //Invers kinematik
    VectorZWEI c(get_pos('x'), get_pos('y'));

    JointAngle angles = calculate_joints2(l1, l2, c);
    double A = angles.A * (1416/(2*PI));
    log_i("Hello %f %d", A, A);

    set_pos(A);
    s.Move(angles.B);
    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void updateMain(double *paramValue, char subtype)
{
  update(paramValue, subtype);

  //Set servo
  switch (subtype){
    case 'u':
      //Set servo
      
      double angle = *paramValue / (1216.0) *360.0;
      if (*paramValue * -1 < 0){
        angle = 0;
      }

      //log_i("Servo %f", angle);
      s.Move(angle*-1);

      break;
  };
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  s.Setup(8, 25);

  s.Move(0);

  init_dc();
  init_web(*updateMain, *getData);

  // delay(1000);
  // xTaskCreatePinnedToCore(
  //     kinematik,
  //     "Kinematik loop",
  //     6000, /* Stack size in words */
  //     NULL,  /* Task input parameter */
  //     1,     /* Priority of the task from 0 to 25, higher number = higher priority */
  //     &KinematikTaskHandle,
  //     1); /* Core where the task should run */
}

void loop() {

}