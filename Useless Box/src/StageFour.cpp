#include <StageFour.h>

const int32_t step_move = 100;

void StageFour(Servo servo, StepMotor step_motor, int32_t current_round)
{
    step_motor.Move(step_move / 2, 100);
    delay(3000);
    step_motor.Move(step_move / 2, 100);
    delay(1000);
    step_motor.Move(-step_move, 200);
}