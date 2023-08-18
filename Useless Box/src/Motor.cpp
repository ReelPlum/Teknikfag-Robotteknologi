#include <Arduino.h>

const uint8_t PIN_LED = 17;
const int32_t PWM_CH = 0;
const int32_t PWM_FREQ_HZ = 50;
const int32_t PWM_RES_BITS = 8;
const int32_t DLY_MS = 2;

const int16_t MaxAngle = 170*2;
const int32_t Counts = pow(2, PWM_RES_BITS);

void setup() {
    // ledcAttachPin(PIN_LED, PWM_CH);
    // ledcSetup(PWM_CH, PWM_FREQ_HZ, PWM_RES_BITS);
}

void Step_Move(){
    
}

void Servo_Move(int16_t Angle, int32_t PWM_Channel) {
    double percent = (Angle / MaxAngle);
    int32_t value = Counts*0.05 + (Counts * percent * 0.05);

    ledcWrite(PWM_Channel, value);
}

void loop() {
}