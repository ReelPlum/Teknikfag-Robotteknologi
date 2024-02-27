#include <DCMotor.h>
#include <SensorFusion.h>
#include <PID.h>

class Stabilizer {
public:
    Stabilizer(DCMotor RightMotor, DCMotor LeftMotor); // Constructor declaration

    void Init(DCMotor RightMotor, DCMotor LeftMotor);
    static void Update(void *arg);
    void SetExtraEngineSpeed(double right, double left);

    double ReadSensors();

private:
  double DT;
  double value;
  double wx;
  double acc;

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
};