#include <Arduino.h>
#include <math.h>
#include <map.h>

void Setup_Stepper(uint8_t pulse_pin, uint8_t dir_pin);
void Move_Stepper(uint8_t pulse_pin, uint8_t dir_pin, boolean dir = 1, uint32_t steps = 200, uint32_t speed = 100);