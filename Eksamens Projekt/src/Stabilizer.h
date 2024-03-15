#include <DCMotor.h>
#include <SensorFusion.h>
#include <PID.h>
#include "ICM_20948.h"
#include <math.h>

class Stabilizer {
public:
    Stabilizer(); // Constructor declaration

    void init(DCMotor RightMotor, DCMotor LeftMotor);
    static void Update(void *arg);
    void SetExtraEngineSpeed(double right, double left);

    double ReadSensors();

private:
  double DT;
  double value;
  double gyro;
  double acc;
  double wx;

  double current_angle;
  double ctrl_angle;
  double wanted_angle;
  double integration_threshold;

  double extraSpeedRight;
  double extraSpeedLeft;

  SensorFusion sensorFusion;
  Pid anglePID;
  TaskHandle_t updateTaskHandle;

  DCMotor RightMotor;
  DCMotor LeftMotor;
  ICM_20948_I2C myICM;
};