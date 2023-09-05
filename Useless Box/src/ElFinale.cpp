#include <ElFinale.h>
const int32_t step_move = 100;

void ElFinale(Servo servo, StepMotor step_motor)
{
    step_motor.Move(step_move / 2, 100);
    delay(2000);
    for (int i = 1; i < 136; i++)
    {
        servo.Move(i);
        delay(50);
    }
    delay(1000);
    step_motor.Move(step_move / 2, 100);
    delay(1000);
    step_motor.Move(-(step_move / 2), 100);
    delay(2000);
    for (int i = 135; i > 0; i -= 1)
    {
        servo.Move(i);
        delay(50);
    }
    delay(1000);
    step_motor.Move(-(step_move / 2), 100);
}