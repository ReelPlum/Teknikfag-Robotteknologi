#pragma once

#include <Arduino.h>
#include <math.h>
#include <map.h>

class StepMotor
{
    public:
    StepMotor(){};
    void Move(int32_t steps, uint32_t speed);
    void Setup(uint8_t pulse_pin, uint8_t dir_pin);

    private:
    uint8_t pulse_pin;
    uint8_t dir_pin;
};