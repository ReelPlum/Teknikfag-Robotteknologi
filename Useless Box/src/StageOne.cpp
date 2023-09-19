#include <StageOne.h>

const int32_t STEPS_TO_MOVE = 200;
const int32_t step_speed = 400;

void StageOne(Servo servo, StepMotor step_motor, int32_t current_round){
    //Choose Random Number
    step_motor.Move(STEPS_TO_MOVE, step_speed);
    delay(100);
    step_motor.Move(-STEPS_TO_MOVE, step_speed);
}