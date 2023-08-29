#include <Servo.h>
#include <StepMotor.h>

const int32_t SERVO_1 = 0;

const uint8_t STEP_1_PULSE_pin = 23;
const uint8_t STEP_1_DIR_pin = 22;

StepMotor STEPPERMOTOR1;
Servo SERVO1;

void setup()
{
  Serial.begin(115200);
  STEPPERMOTOR1.Setup(STEP_1_PULSE_pin, STEP_1_DIR_pin);

  SERVO1.Setup(0,18,SERVO_T_LOW,SERVO_T_HIGH,20*1000,PWM_FREQ_HZ_Servo,PWM_RES_BITS_Servo,MAX_ANGLE);

}

void loop()
{
  STEPPERMOTOR1.Move(200, 50);

  for (int i = 1; i < 181; i++)
  {
    SERVO1.Move(i);
    delay(20);
  }

  for (int i = 180; i > 0; i -= 1)
  {
    SERVO1.Move(i);
    delay(20);
  }
}
