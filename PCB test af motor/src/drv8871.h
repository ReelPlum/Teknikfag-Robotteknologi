#pragma once
#include <Arduino.h>

class Drv8871
{
public:
  Drv8871(){};

  void set_pwm(int32_t value);

  void begin(int32_t pin_in1, int32_t pin_in2,
             int32_t freq_hz, int32_t resolution_bit, int32_t pwm_ch,
             int32_t max_ctrl_value);

private:
  int32_t pin_in1;
  int32_t pin_in2;
  int32_t freq_hz;
  int32_t resolution_bit;
  int32_t pwm_ch;
  int32_t pwm_max_value;
  int32_t max_ctrl_value;
};
