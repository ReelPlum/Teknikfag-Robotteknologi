#include <DCMotor.h>

class Stabilizer {
public:
    Stabilizer(DCMotor RightMotor, DCMotor LeftMotor); // Constructor declaration

    void Update();
    void ReadSensors();
};