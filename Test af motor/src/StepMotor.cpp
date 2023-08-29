#include <StepMotor.h>

void Setup_Stepper(uint8_t pulse_pin, uint8_t dir_pin)
{
    // ledcAttachPin(pin, pwm_channel);
    // ledcSetup(pwm_channel, PWM_FREQ_HZ_STEP, PWM_RES_BITS_STEP);
    pinMode(pulse_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void Move_Stepper(uint8_t pulse_pin, uint8_t dir_pin, boolean dir = 1, uint32_t steps = 200, uint32_t speed = 100)
{
    //Speed er steps / second
    
    digitalWrite(dir_pin, dir);

    for (int i = 0; i < steps; i++){
        digitalWrite(pulse_pin, HIGH);
        usleep(20);
        digitalWrite(pulse_pin, LOW);
        delay((1/speed) * 1000);
    }
}