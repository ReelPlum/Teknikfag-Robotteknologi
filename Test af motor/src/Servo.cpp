#include <Arduino.h>
#include <math.h>  
#include <Servo.h>


double map_float(double x, double x_min, double x_max, double y_min, double y_max){
    double a = (y_max-y_min)/(x_max-x_min);

    return a*x+(y_min-x_min*a);
}

void Setup_Servo(int32_t pwm_channel, uint8_t pin){
    pinMode(pin, OUTPUT);
    ledcAttachPin(pin, pwm_channel);
    ledcSetup(pwm_channel, PWM_FREQ_HZ_Servo, PWM_RES_BITS_Servo);
}

void Servo_Move(int16_t angle, int16_t max_angle, int32_t pwm_channel) {
      
    double percent = (angle / max_angle) * 100;
    double dc = map_float(percent, 0, 100, SERVO_T_LOW, SERVO_T_HIGH); // dc 2.5% -- 12.5%
    Serial.println(dc)
    int32_t value = map_float(dc, SERVO_T_LOW, SERVO_T_HIGH, 0, pow(2, PWM_RES_BITS_Servo)); // 0 - 2^8
    Serial.println(value)

    //int32_t value = SERVO_COUNTS*0.05 + (SERVO_COUNTS * percent * 0.05);

    ledcWrite(pwm_channel, value);
}


