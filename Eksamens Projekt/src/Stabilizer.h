#include <DCMotor.h>

class Stabilizer{
  public:
  Stabilizer(DCMotor RightMotor, DCMotor LeftMotor ); //And IMU sensors!!!

  void Setup();

  void Update();
  void ReadSensors();

  private:
}