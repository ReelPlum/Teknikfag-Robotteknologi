#include <Servo.h>
#include <StepMotor.h>

const int32_t SERVO_1 = 0;

const uint8_t STEP_1_PULSE_pin = 22;
const uint8_t STEP_1_DIR_pin = 23;

void setup()
{
  Serial.begin(115200);
  Setup_Stepper(STEP_1_PULSE_pin, STEP_1_DIR_pin);
}

void loop()
{

  Move_Stepper(STEP_1_PULSE_pin, STEP_1_DIR_pin, 1, 200, 50);
}
