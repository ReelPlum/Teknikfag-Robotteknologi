#include <StageThree.h>

const int32_t STEPS_TO_MOVE = 100

void StageThree(Servo servo, StepMotor step_motor)
{
    int32_t tal = random(0, 2);
    if(tal == 0){
        StageOne(servo, step_motor);
    }

    else if(tal == 1){
        step_motor.Move(STEPS_TO_MOVE, 100);
        
        delay(1000);

        step_motor.Move(- STEPS_TO_MOVE, 100);
    }

    else if(tal == 2){
        step_motor.Move(STEPS_TO_MOVE, 400);
        
        delay(1500);

        step_motor.Move(STEPS_TO_MOVE, 300);

    }





}