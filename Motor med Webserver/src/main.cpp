#include <global.h>
#include <WebServer.h>
#include <DCMotor.h>
#include <inversekinematic.h>
#include <Servo.h>

double l1 = 20;
double l2 = 13;
TaskHandle_t KinematikTaskHandle;


void kinematik(void *args){
  //Invers kinematik
  VectorZWEI c(get_pos('x'), get_pos('y'));

  JointAngle angles = calculate_joints2(l1, l2, c);
  double A = angles.A * (1416/(2*PI));
  //log_i("Hello %f %d", A, A);

  set_pos(A);
}

Servo s;

void updateMain(double *paramValue, char subtype)
{
  update(paramValue, subtype);

  //Set servo
  switch (subtype){
    case 't':
      //Set servo
      log_i("Servo %f", *paramValue);
      s.Move(*paramValue);

      break;
  };
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  s.Setup(1, 25);

  init_dc();
  init_web(*updateMain, *getData);

  // delay(1000);
  // xTaskCreatePinnedToCore(
  //     kinematik,
  //     "Kinematik loop",
  //     10000, /* Stack size in words */
  //     NULL,  /* Task input parameter */
  //     1,     /* Priority of the task from 0 to 25, higher number = higher priority */
  //     &KinematikTaskHandle,
  //     1); /* Core where the task should run */
}

void loop() {
}