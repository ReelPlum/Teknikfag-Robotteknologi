#include <DCMotor.h>
#include <SensorFusion.h>
#include <PID.h>
#include "ICM_20948.h"
#include <math.h>

typedef void (*angleChangeCallback)(double *angle);

class Stabilizer {
public:
    Stabilizer(){}; // Constructor declaration

    void init(DCMotor *RightMotor, DCMotor *LeftMotor);
    static void Update(void *arg);
    void SetExtraEngineSpeed(double right, double left);

    void ReadSensors();
    void RegisterAngleCallback(angleChangeCallback callback);

    double current_angle;

private:
  double DT;
  double value;
  double gyro;
  double acc;
  double wx;

  double ctrl_angle;
  double wanted_angle;
  double integration_threshold;

  double extraSpeedRight;
  double extraSpeedLeft;

  SensorFusion sensorFusion;
  Pid anglePID;
  TaskHandle_t updateTaskHandle;

  DCMotor *RightMotor;
  DCMotor *LeftMotor;
  ICM_20948_I2C myICM;

  angleChangeCallback angleCallbacks[5];
  int32_t angleCallbacksNum;
};