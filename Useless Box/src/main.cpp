#include <global.h>
#include <Servo.h>
#include <StepMotor.h>
#include <random.h>

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

int32_t NUM_OF_STAGES = 7; 

int32_t Current_Stage = 0;
bool Started = false;

void start() {
    //Start uselessbox
    Current_Stage = 0;
    NUM_OF_STAGES = random_int_range(6, 12);
    Started = true;
}

void game_task(void *args)
{
double loop_time_s = 0.01;
  TickType_t xTimeIncrement = configTICK_RATE_HZ * loop_time_s;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for(;;)
  {
    //Run game task here
    if (not Started){
        //Tjek hvis switchen er slået til. Start spillet hvis knappen er til.
        if (digitalRead(SWITCH_PIN) == HIGH){
            //Switchen er slået til. Start boxen.
            start();
        }

        return;
    }

    int32_t stage_num = random_int_range(0, NUM_OF_STAGES-1);

    Current_Stage++;

    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void setup() {
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