#include <StageOne.h>

const int32_t STEPS_TO_MOVE = 200;

void StageOne(Servo servo, StepMotor step_motor){
    //Choose Random Number
    step_motor.Move(STEPS_TO_MOVE, 200);
    delay(1000);
    step_motor.Move(-STEPS_TO_MOVE, 200);
}