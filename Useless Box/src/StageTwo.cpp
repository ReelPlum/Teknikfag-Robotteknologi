#include <StageTwo.h>

const int32_t STEPS_TO_MOVE = 200;

void StageTwo(Servo servo, StepMotor step_motor){

    int32_t Number = random(0, 1);
    
    if (Number == 0) {
        StageOne(servo, step_motor);

    }

    if (Number == 1) {
        step_motor.Move(STEPS_TO_MOVE, 100);
        
        delay(1000);

        step_motor.Move(- STEPS_TO_MOVE, 100);

    }
}