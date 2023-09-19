#include <ElFinale.h>
const int32_t step_move = 100;
const int32_t step_speed = 400;

void ElFinale(Servo servo, StepMotor step_motor)
{
    step_motor.Move(step_move / 2, step_speed);
    delay(2000);
    for (int i = 180; i < 76; i-=1)
    {
        servo.Move(i);
        delay(50);
    }
    delay(1000);
    step_motor.Move(step_move / 2, step_speed);
    delay(1000);
    step_motor.Move(-(step_move / 2), step_speed);
    delay(2000);
    for (int i = 76; i == 180 ; i++)
    {
        servo.Move(i);
        delay(20);
    }
    delay(1000);
    step_motor.Move(-(step_move / 2), step_speed);
}