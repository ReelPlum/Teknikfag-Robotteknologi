#include <Arduino.h>
#include <math.h>
#include <map.h>

const double MAX_ANGLE = 180;
const int32_t PWM_FREQ_HZ_STEP = 200;
const int32_t PWM_RES_BITS_STEP = 12;
const int32_t STEP_T_LOW = 552; //MIcroseconds
const int32_t STEP_T_HIGH = 2550; //Microseconds

void Setup_Stepper(uint8_t pulse_pin, uint8_t dir_pin);
