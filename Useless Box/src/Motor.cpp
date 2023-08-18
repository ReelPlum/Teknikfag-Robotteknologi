#include <Arduino.h>

const int32_t PWM_FREQ_HZ_Servo = 50;
const int32_t PWM_RES_BITS_Servo = 8;
const int32_t DLY_MS = 2;

//const int16_t MaxAngle = 170*2;
const int32_t SERVO_COUNTS = pow(2, PWM_RES_BITS_Servo);

void setup() {
    // ledcAttachPin(PIN_LED, PWM_CH);
    // ledcSetup(PWM_CH, PWM_FREQ_HZ, PWM_RES_BITS);
}

void Step_Move(){

}

void Setup_Servo(int32_t pwm_channel, uint8_t pin){
    ledcAttachPin(pin, pwm_channel);
    ledcSetup(pwm_channel, PWM_FREQ_HZ_Servo, PWM_RES_BITS_Servo);
}

void Servo_Move(int16_t angle, int16_t max_angle, int32_t pwm_channel) {
    double percent = (angle / max_angle);
    int32_t value = SERVO_COUNTS*0.05 + (SERVO_COUNTS * percent * 0.05);

    ledcWrite(pwm_channel, value);
}

void loop() {
}