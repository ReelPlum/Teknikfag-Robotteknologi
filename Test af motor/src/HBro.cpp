#include <HBro.h>

void Setup_HBro(uint8_t pwm_channel, uint8_t pwm_pin, uint8_t a_pin, uint8_t b_pin)
{
    pinMode(a_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);

    ledcAttachPin(pwm_pin, pwm_channel);
    ledcSetup(pwm_channel, PWM_FREQUENCY_HBRO, PWM_RESOLUTION_BITS_HBRO);

    Change_Direction_Of_HBro(1, a_pin, b_pin);
}

void Change_Direction_Of_HBro(boolean dir, uint8_t a_pin, uint8_t b_pin)
{
    digitalWrite(a_pin, not dir);
    digitalWrite(b_pin, dir);
}

void Start_HBro(uint32_t pwm_channel, uint32_t power)
{
    int32_t value = map_double(power, 0, 100, 0, pow(PWM_RESOLUTION_BITS_HBRO, 2)-1);

    ledcWrite(pwm_channel, value);
}

void Stop_HBro(uint32_t pwm_channel)
{

}