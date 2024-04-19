#pragma once

#include <Arduino.h>
#include <Stabilizer.h>
#include <map.h>

class Buzzer{
    public:
    Buzzer();

    void init(int8_t io_pin, int32_t pwm_ch, int32_t max_frequency, int32_t min_frequency, Stabilizer *stabilizer);
    void change_freq(int32_t frequency);
    void toggle();

    void static Task(void *arg);

    private:
    int32_t frequency;
    
    double max_frequency;
    double min_frequency;

    int8_t pin;
    int32_t pwm_ch;

    bool enabled = false;

    Stabilizer *stabilizer;

    TaskHandle_t TaskHandle;
};