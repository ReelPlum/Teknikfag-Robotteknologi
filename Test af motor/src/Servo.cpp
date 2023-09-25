#include <global.h>
#include <Servo.h>

void Servo::Setup(uint8_t pwm_channel, uint8_t pin, double t_low, double t_high, double servo_hz, int32_t pwm_freq_hz, int32_t pwm_res_bits, int32_t max_angle)
{
    this->pwm_channel = pwm_channel;
    this->pin = pin;
    this->servo_hz = servo_hz;
    this->t_low = (t_low / servo_hz) * 100;
    this->t_high = (t_high / servo_hz) * 100;
    this->pwm_freq_hz = pwm_freq_hz;
    this->pwm_res_bits = pwm_res_bits;
    this->max_angle = max_angle;

    ledcAttachPin(pin, pwm_channel);

    ledcSetup(pwm_channel, pwm_freq_hz, pwm_res_bits);
}

void Servo::Move(double angle)
{
    double percent = (angle / this->max_angle) * 100;
    double dc = map_double(percent, 0, 100, this->t_low, this->t_high);                 // map dc 2.5% -- 12.5%
    int32_t value = (int32_t)map_double(dc, 0, 100, 0, pow(2, this->pwm_res_bits) - 1); //

    ledcWrite(this->pwm_channel, value);
}