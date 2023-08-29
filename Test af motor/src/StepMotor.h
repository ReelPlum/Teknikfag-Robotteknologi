#include <Arduino.h>
#include <math.h>
#include <map.h>

void Setup_Stepper(uint8_t pulse_pin, uint8_t dir_pin);
void Move_Stepper(uint8_t pulse_pin, uint8_t dir_pin, uint8_t dir, uint32_t steps, uint32_t speed);