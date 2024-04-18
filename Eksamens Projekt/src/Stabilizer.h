#pragma once

#include <DCMotor.h>
#include <SensorFusion.h>
#include <PID.h>
#include "ICM_20948.h"
#include <math.h>

typedef void (*angleChangeCallback)(double *angle);

class Stabilizer {
public:
    Stabilizer(){}; // Constructor declaration

    void init(DCMotor *RightMotor, DCMotor *LeftMotor, double kd, double k);
    void SetExtraEngineSpeed(double right, double left);
    void SetKD(double KD);
    void SetK(double k);
    void SetKP(double KP);
    void SetKI(double KI);
    void setWantedAngle(double WantedAngle);

    void SetExtraAngle(double angle);

    void ReadSensors();

    double getK();
    double getKD();
    double getKI();
    double getKP();
    double getCtrlAngle();
    double getTargetAngle();

    double current_angle;

    Pid* getPid();

private:
  static void BalanceTask(void *arg);
  static void AngleTask(void *arg);

  double DT;
  double value;
  double gyro;
  double acc;
  double wx;
  
  double extraAngle;

  double ki;
  double kp;
  double kd;
  double ctrl_angle;
  double wanted_angle = -0.1;

  double extraSpeedRight;
  double extraSpeedLeft;

  SensorFusion sensorFusion;
  Pid anglePID;
  TaskHandle_t balanceTaskHandle;
  TaskHandle_t angleTaskHandle;

  DCMotor *RightMotor;
  DCMotor *LeftMotor;
  ICM_20948_I2C myICM;
};