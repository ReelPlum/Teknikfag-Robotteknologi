#include <global.h>
#include <Servo.h>
#include <StepMotor.h>

const int32_t SERVO_PIN = 21;

const uint8_t STEP_1_PULSE_pin = 22;
const uint8_t STEP_1_DIR_pin = 23;
int32_t count = 0;
Servo SERVO1;

void setup()
{
  Serial.begin(115200);
  delay(200);

  SERVO1.Setup(0, SERVO_PIN, SERVO_T_LOW, SERVO_T_HIGH, 20 * 1000, PWM_FREQ_HZ_Servo, PWM_RES_BITS_Servo, MAX_ANGLE);
}

void loop()
{
  for (int i = 145; i > 75; i -= 1)
    {
        SERVO1.Move(i);
        delay(50);
    }
  delay(2000);
  for (int i = 75; i < 145; i++)
    {
        SERVO1.Move(i);
        delay(50);
    }



  delay(5000);

}
