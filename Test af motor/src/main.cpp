#include <Servo.h>
#include <StepMotor.h>

const int32_t SERVO_1 = 0;

const uint8_t STEP_1_PULSE_pin = 22;
const uint8_t STEP_1_DIR_pin = 23;

void setup()
{
  Serial.begin(115200);
  Setup_Servo(SERVO_1, 18);

  Setup_Stepper(STEP_1_PULSE_pin, STEP_1_DIR_pin);
}

void loop()
{

  Servo_Move(0, MAX_ANGLE, SERVO_1);
  delay(2000);
  Servo_Move(180, MAX_ANGLE, SERVO_1);
  delay(2000);

  Move_Stepper(STEP_1_PULSE_pin, STEP_1_DIR_pin, 1, 200, 50);
}
