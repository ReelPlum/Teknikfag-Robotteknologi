#include <DCMotor.h>
#include <SensorFusion.h>
#include <PID.h>
#include "ICM_20948.h"
#include <math.h>

typedef void (*angleChangeCallback)(double *angle);

class Stabilizer {
public:
    Stabilizer(){}; // Constructor declaration

    void init(DCMotor *RightMotor, DCMotor *LeftMotor, double kd);
    void SetExtraEngineSpeed(double right, double left);
    void SetKD(double KD);
    void SetK(double k);
    void SetKP(double KP);
    void SetKI(double KI);
    void setTargetAngle(double WantedAngle);

    void SetExtraAngle(double angle);

    void ReadSensors();
    void RegisterAngleCallback(angleChangeCallback callback);

    double getK();
    double getKD();
    double getKI();
    double getKP();

    double getTargetAngle();

    double current_angle;

    Pid* getPid();

private:
  static void Update(void *arg);
  static void CallbackTask(void *arg);

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
  TaskHandle_t updateTaskHandle;

  DCMotor *RightMotor;
  DCMotor *LeftMotor;
  ICM_20948_I2C myICM;

  angleChangeCallback angleCallbacks[5];
  int32_t angleCallbacksNum;
};