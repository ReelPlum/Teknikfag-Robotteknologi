#include <Servo.h>
#include <StepMotor.h>

const int32_t SERVO_1 = 0;

const uint8_t STEP_1_PULSE_pin = 23;
const uint8_t STEP_1_DIR_pin = 22;
int32_t count = 0;
StepMotor STEPPERMOTOR1;

void setup()
{
  Serial.begin(115200);
  STEPPERMOTOR1.Setup(STEP_1_PULSE_pin, STEP_1_DIR_pin);
}

void loop()
{
  if(count ==0){
    STEPPERMOTOR1.Move(50, 50);
    count++;
  }
  
}
