#include <Servo.h>


void Setup_Servo(int32_t pwm_channel, uint8_t pin)
{
    //pinMode(pin, OUTPUT);
    ledcAttachPin(pin, pwm_channel);
    ledcSetup(pwm_channel, PWM_FREQ_HZ_Servo, PWM_RES_BITS_Servo);
}

void Servo_Move(double angle, double max_angle, int32_t pwm_channel)
{

    double percent = (angle / max_angle);
    double dc = map_float(percent, 0, 1, (SERVO_T_LOW/1000)/20, (SERVO_T_HIGH/1000)/20); // dc 2.5% -- 12.5%
    int32_t value = (int32_t)map_float(dc, (SERVO_T_LOW/1000)/20, (SERVO_T_HIGH/1000)/20, 0, pow(2, PWM_RES_BITS_Servo)-1); //  
    Serial.printf("Percent: %.3f    DC: %.3f   value: %d\n", percent, dc, value);

    // int32_t value = SERVO_COUNTS*0.05 + (SERVO_COUNTS * percent * 0.05);

    ledcWrite(pwm_channel, value);
}
