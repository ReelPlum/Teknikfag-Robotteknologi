#include <StageFour.h>

const int32_t step_move = 100;
int32_t counter = 0;


void stage_4(Servo servo, StepMotor step_motor)
{
    step_motor.Move(step_move / 2, 100);
    delay(3000);
    step_motor.Move(step_move / 2, 100);
    delay(1000);
    step_motor.Move(-step_move, 200);
}

void StageFour(Servo servo, StepMotor step_motor)
{
    int32_t Tal = random(0, 3);
    int32_t Round = random(6,9);
    while (counter == 0)
    {
        stage_4(servo, step_motor);
        counter++;
    }

    while (1 < counter < Round)
    {     
        counter++; 
        if (Tal == 0)
        {
            StageOne(servo, step_motor);
        }

        if (Tal == 1)
            ;
        {
            step_motor.Move(step_move, 100);
            delay(1000);
            step_motor.Move(-step_move, 100);
        }

        if (Tal == 2)
            ;
        {
            step_motor.Move(step_move, 400);
            delay(1000);
            step_motor.Move(-step_move, 400);
        }

        if (Tal == 3)
        {
            stage_4(servo, step_motor);
        }
    }
}   