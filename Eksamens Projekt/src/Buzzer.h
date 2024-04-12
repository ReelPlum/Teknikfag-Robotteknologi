#pragma once


#include <Arduino.h>
#include <map.h>

class Buzzer{
    public:
    Buzzer();

    void init(int8_t io_pin, int32_t pwm_ch, int32_t initial_frequency);
    void change_freq(int32_t frequency);


    private:
    int32_t frequency;
    int8_t pin;
    int32_t pwm_ch;
};