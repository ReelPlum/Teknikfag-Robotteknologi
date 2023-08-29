#pragma once

#include <Arduino.h>
#include <math.h>
#include <map.h>


const double MAX_ANGLE = 180;
const int32_t PWM_FREQ_HZ_Servo = 50;
const int32_t PWM_RES_BITS_Servo = 12;
const int32_t SERVO_T_LOW = 552; //MIcroseconds
const int32_t SERVO_T_HIGH = 2550; //Microseconds


void Setup_Servo(int32_t pwm_channel, uint8_t pin);
void Servo_Move(double angle, double max_angle, int32_t pwm_channel);