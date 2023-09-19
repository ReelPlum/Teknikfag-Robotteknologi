#pragma once

#include <Arduino.h>
#include <math.h>
#include <map.h>


const int32_t MAX_ANGLE = 180;
const int32_t PWM_FREQ_HZ_Servo = 50;
const int32_t PWM_RES_BITS_Servo = 12;
const int32_t SERVO_T_LOW = 552; //MIcroseconds
const int32_t SERVO_T_HIGH = 2550; //Microseconds


class Servo {
    public:
    Servo(){};

    void Setup(uint8_t pwm_channel, uint8_t pin, double t_low = SERVO_T_LOW, double t_high = SERVO_T_HIGH, double servo_hz = 20*1000, int32_t pwm_freq_hz = PWM_FREQ_HZ_Servo, int32_t pwm_req_bits = PWM_RES_BITS_Servo, int32_t max_angle = MAX_ANGLE); 
    void Move(double angle);

    private:
    int32_t pwm_channel;
    uint8_t pin;
    double max_angle;
    double t_low;
    double t_high;
    double servo_hz;
    int32_t pwm_freq_hz;
    int32_t pwm_res_bits;
    
};