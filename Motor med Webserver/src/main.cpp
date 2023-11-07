#include <global.h>
#include <WebServer.h>
#include <DCMotor.h>
#include <inversekinematic.h>
#include <Servo.h>

double l1 = 20;
double l2 = 13;

Servo servo;

const int32_t SERVOPIN = 0;
const int32_t SERVOPWM = 0;

void kinematikLoop(void *arg){
  //Invers kinematik
  VectorZWEI c(get_pos('x'), get_pos('y'));

  JointAngle angles = calculate_joints2(l1, l2, c);
  double A = angles.A * (1416/(2*PI));
  //log_i("Hello %f %d", A, A);

  if (A > 200){
    A = 200;
  }else if (A < - 200){
    A = -200;
  }

  set_pos(A);
  servo.Move(angles.B);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  servo.Setup(SERVOPWM, SERVOPIN);

  TaskHandle_t KinematikTaskHandle;
  xTaskCreatePinnedToCore(
      kinematikLoop,       /* Function to implement the task */
      "Kinematik_task",     /* Name of the task */
      3000,           /* Stack size in words */
      NULL,           /* Task input parameter */
      1,              /* Priority of the task from 0 to 25, higher number = higher priority */
      &KinematikTaskHandle, /* Task handle. */
      1);             /* Core where the task should run */

  init_dc();
  init_web(*update, *getData);
}

void loop() {
}