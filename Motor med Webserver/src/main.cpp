#include <global.h>
#include <WebServer.h>
#include <DCMotor.h>
#include <inversekinematic.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  init_dc();
  init_web(*update, *getData);
}

double l1 = 20;
double l2 = 13;

void loop() {
  //Invers kinematik
  VectorZWEI c(get_pos('x'), get_pos('y'));

  JointAngle angles = calculate_joints2(l1, l2, c);
  double A = angles.A * (1416/(2*PI));
  //log_i("Hello %f %d", A, A);

  set_pos(A);
}