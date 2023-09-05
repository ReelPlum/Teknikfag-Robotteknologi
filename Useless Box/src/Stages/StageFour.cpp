#include <global.h>
#include <servo.h>
#include <StepMotor.h>

const int32_t sluk_knap_step = 100;
const int32_t home_step = 100;
int32_t counter = 0;

void stage_4(Servo servo, StepMotor step_motor)
{
    step_motor.Move(sluk_knap_step / 2, 100);
    delay(3000);
    step_motor.Move(sluk_knap_step / 2, 100);
    delay(500);
    step_motor.Move(home_step, 200);
}

void StageFour(Servo servo, StepMotor step_motor)
{
    while (counter == 0)
    {
        stage_4(servo,step_motor);
    }

    int32_t Tal = random(0, 3);

    if Tal == 0{

    }

}