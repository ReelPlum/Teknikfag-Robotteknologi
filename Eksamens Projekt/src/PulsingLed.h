#pragma once

#include <Arduino.h>
#include <map.h>

class PulsingLed{
    public:
    PulsingLed(int32_t pwm_ch, int32_t pwm_resolution, int32_t pwm_freq, int32_t led_pin, double pulses_per_second);

    void init();

    private:
    static void pulseTask(void *args);

    int32_t pwm_ch;
    int32_t pwm_resolution;
    int32_t led_pin;
    int32_t pwm_freq;
    double pulses_per_second;
    TaskHandle_t TaskHandle;
};