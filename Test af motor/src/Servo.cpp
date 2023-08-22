#include <Arduino.h>
#include <math.h>  

const int32_t MAX_ANGLE = 180;
const int32_t PWM_FREQ_HZ_Servo = 50;
const int32_t PWM_RES_BITS_Servo = 8;
const int32_t SERVO_T_LOW = 552; //MIcroseconds
const int32_t SERVO_T_HIGH = 2550; //Microseconds

double map_float(double x, double x_min, double x_max, double y_min, double y_max){
    double a = (y_max-y_min)/x_max;

    return a*x+x_min;
}

void Setup_Servo(int32_t pwm_channel, uint8_t pin){
    pinMode(pin, OUTPUT);
    ledcAttachPin(pin, pwm_channel);
    ledcSetup(pwm_channel, PWM_FREQ_HZ_Servo, PWM_RES_BITS_Servo);
}

void Servo_Move(int16_t angle, int16_t max_angle, int32_t pwm_channel) {
    double percent = (angle / max_angle) * 100;
    double dc = map_float(percent, 0, 100, SERVO_T_LOW, SERVO_T_HIGH);
    int32_t value = map_float(dc, SERVO_T_LOW, SERVO_T_HIGH, 0, pow(2, PWM_RES_BITS_Servo));

    //int32_t value = SERVO_COUNTS*0.05 + (SERVO_COUNTS * percent * 0.05);

    ledcWrite(pwm_channel, value);
}


