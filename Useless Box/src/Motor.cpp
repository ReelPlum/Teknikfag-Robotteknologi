#include <Arduino.h>

const uint8_t PIN_LED = 17;
const int32_t PWM_CH = 0;
const int32_t PWM_FREQ_HZ = 50;
const int32_t PWM_RES_BITS = 8;
const int32_t DLY_MS = 2;


void setup() {
    ledAttachPin(PIN_LED, PWM_CH);
    ledcSetup(PWM_CH, PWM_FREQ_HZ, PWM_RES_BITS);
}

void 

void loop() {
}