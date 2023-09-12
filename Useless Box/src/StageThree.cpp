#include <StageThree.h>

const int32_t STEPS_TO_MOVE = 100;

void StageThree(Servo servo, StepMotor step_motor, int32_t current_round)
{
    step_motor.Move(STEPS_TO_MOVE, 400);

    delay(1500);

    step_motor.Move(STEPS_TO_MOVE, 300);
}