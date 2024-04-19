#pragma once

#include <Arduino.h>
// #include "global.h"
#include <pid.h>
//#include <hbridge.h>
#include <drv8871.h>
#include <ESP32Encoder.h>
#include <Global.h>

class DCMotor
{
public:
    //Konstruer DC motor klassen. Her vil vi sætte den op i starten af koden for at gemme den givne data i Heap'en.
    DCMotor(int32_t enc_a_pin, int32_t enc_b_pin,
            int32_t hbridge_ina_pin, int32_t hbridge_inb_pin, int32_t hbridge_pwm_pin,
            int32_t pwm_channel, int32_t pwm_frequency_hz, int32_t pwm_resolution_bits, double dt, 
            double min_ctrl_value, double max_ctrl_value, double impulses_per_rotation, double EncoderGearing);

    void init();

    double get_velocity();

    double get_acceleration();
    double getEncoderPosition();
    void set_PWM(int32_t pwm);
    double calculate_degtovel(double val);

private:
    static void pidTask(void *args);

    int32_t enc_a_pin;
    int32_t enc_b_pin;
    int32_t hbridge_ina_pin;
    int32_t hbridge_inb_pin;
    int32_t hbridge_pwm_pin;
    int32_t pwm_channel;
    int32_t pwm_frequency_hz;
    int32_t pwm_resolution_bits;
    int32_t pwm_min;
    int32_t pwm_max;
    double dt;
    double min_ctrl_value;
    double max_ctrl_value;
    double impulses_per_rotation;

    volatile double acceleration;
    volatile int64_t current_pos;
    volatile double current_vel;
    double EncoderGearing;

    double FullEncoderRotation;

    ESP32Encoder encoder;
    Pid pidVel;
    Pid pidPos;
    //H_Bridge hbridge;
    Drv8871 hbridge;
    int32_t pinPidLoop;
    int32_t pinLimitSw;
    TaskHandle_t pidTaskHandle;
};