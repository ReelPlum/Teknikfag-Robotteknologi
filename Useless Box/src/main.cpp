#include <global.h>
#include <Servo.h>
#include <StepMotor.h>
#include <random.h>

#include <StageOne.h>
#include <StageTwo.h>
#include <StageThree.h>
#include <StageFour.h>
#include <ElFinale.h>

/*
Ideer til stages:

1. Gå op og luk med det samme
2. Gå op, vent og luk hurtigt
3. Gå op, ned, op, ned, op og så luk
4. Gå op og luk langsomt. Gå langsomt ned
5. Gå op pause, sluk langsomt og gå langsomt ned.


*/

Servo SERVO_1;
StepMotor STEP_1;

const uint8_t SERVO_PIN;
const uint8_t SERVO_PWM;

const uint8_t STEPPER_DIR_PIN;
const uint8_t STEPPER_PULSE_PIN;

const uint8_t SWITCH_PIN;

TaskHandle_t GAMETASK_HANDLE;

const int32_t NUM_OF_STAGES = 4; 

int32_t Current_Stage = 0;
bool Started = false;
int32_t Wanted_Rounds = 0;
int32_t Current_Round = 0;

void start() {
    //Start uselessbox
    Current_Stage = 0;

    Wanted_Rounds = random_int_range(4, 7);
    Current_Round = 0;

    Started = true;
}

void stop(){
    //Stop useless box
    Started = false;
    Current_Stage = 0;

    //Return
}

void round(){
    //Run game task here
    if (not Started){
        //Tjek hvis switchen er slået til. Start spillet hvis knappen er til.
        if (digitalRead(SWITCH_PIN) == HIGH){
            //Switchen er slået til. Start boxen.
            start();
        }

        return;
    }
    if (digitalRead(SWITCH_PIN) == LOW){
        return;
    }

    if (Current_Round == Wanted_Rounds){
        //Next stage
        Current_Stage++;

        Wanted_Rounds = random_int_range(4, 7);
        Current_Round = 0;
    }
    if (Current_Stage > NUM_OF_STAGES-1){
        //End animation
        ElFinale(SERVO_1, STEP_1);

        //Stop box
        stop();
        return;
    }

    //Play stage
    if (Current_Stage == 0){
        //Stage 1
        StageOne(SERVO_1, STEP_1, Current_Round);
    }
    else if (Current_Stage == 1){
        //Stage 2
        StageTwo(SERVO_1, STEP_1, Current_Round);
    }
    else if (Current_Stage == 2){
        //Stage 3
        StageThree(SERVO_1, STEP_1, Current_Round);
    }
    else if (Current_Stage == 3){
        //Stage 4
        StageFour(SERVO_1, STEP_1, Current_Round);
    }

    Current_Round++;
    delay(500);
}

void game_task(void *args)
{
    double loop_time_s = 0.01;
    TickType_t xTimeIncrement = configTICK_RATE_HZ * loop_time_s;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        round();

        //Task delay
        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}

void setup() {
    Serial.begin(115200);

    SERVO_1.Setup(SERVO_PWM, SERVO_PIN);
    STEP_1.Setup(STEPPER_PULSE_PIN, STEPPER_DIR_PIN);

    pinMode(SWITCH_PIN, INPUT);

    xTaskCreatePinnedToCore(
    game_task,         
    "Game Task",        
    10000,               /* Stack size in words */
    NULL,                /* Task input parameter */
    2,                   /* Priority of the task from 0 to 25, higher number = higher priority */
    &GAMETASK_HANDLE,   
    1);                  /* core no */
}

void loop() {
    
}