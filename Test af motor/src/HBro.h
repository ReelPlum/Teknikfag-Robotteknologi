#include <Arduino.h>
#include <math.h>
#include <map.h>

const int32_t PWM_FREQUENCY_HBRO = 5000;
const int32_t PWM_RESOLUTION_BITS_HBRO = 12;

void Setup_HBro(uint32_t pwm_channel, uint8_t pwm_pin, uint8_t a_pin, uint8_t b_pin);

void Change_Direction_Of_HBro(boolean dir, uint8_t a_pin, uint8_t b_pin);

void Start_HBro(uint32_t pwm_channel, uint32_t power);

void Stop_HBro(uint32_t pwm_channel);