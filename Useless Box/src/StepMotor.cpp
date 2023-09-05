#include <global.h>
#include <StepMotor.h>

void StepMotor::Setup(uint8_t pulse_pin, uint8_t dir_pin)
{
    this->pulse_pin=pulse_pin;
    this->dir_pin=dir_pin;

    pinMode(pulse_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void StepMotor::Move(int32_t steps, uint32_t speed)
{
    //Speed er steps / second

    if (steps < 0){
        digitalWrite(this->dir_pin, 1);
        steps = -steps;
    }
    else{
        digitalWrite(this->dir_pin, 0);
    }

    for (int i = 0; i < steps; i++){
        digitalWrite(this->pulse_pin, HIGH);
        usleep(10);
        digitalWrite(this->pulse_pin, LOW);
        int32_t delay_ms = (1.0/speed) * 1000;

        delay(delay_ms);
    }
}