#include "drv8871.h"
#include <map.h>


void Drv8871::begin(int32_t pin_in1, int32_t pin_in2,
                    int32_t freq_hz, int32_t resolution_bit, int32_t pwm_ch,
                    int32_t max_ctrl_value)
{
    this->pin_in1 = pin_in1;
    this->pin_in2 = pin_in2;
    this->freq_hz = freq_hz;
    this->resolution_bit = resolution_bit;
    this->pwm_ch = pwm_ch;
    this->pwm_max_value = pow(2, resolution_bit) - 1;
    this->max_ctrl_value = max_ctrl_value;

    log_d("Max ctrl val: %d", this->pwm_max_value);

    ledcAttachPin(pin_in1, pwm_ch);
    ledcAttachPin(pin_in2, pwm_ch+1);
    
    ledcSetup(pwm_ch, freq_hz, resolution_bit);
    ledcSetup(pwm_ch+1, freq_hz, resolution_bit);
}

void Drv8871::set_pwm(int32_t value)
{
    if (value < 0)
    {
        value = -value;

        double pwm_value = map_double(value, 0, max_ctrl_value, 0, pwm_max_value);
        ledcWrite(pwm_ch, pwm_max_value-pwm_value);
        ledcWrite(pwm_ch+1, pwm_max_value);
    }
    else
    {
        double pwm_value = map_double(value, 0, max_ctrl_value, 0, pwm_max_value);
        ledcWrite(pwm_ch+1, pwm_max_value-pwm_value);
        ledcWrite(pwm_ch, pwm_max_value);
    }
}