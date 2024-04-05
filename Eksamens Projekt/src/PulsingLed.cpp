#include <PulsingLed.h>


PulsingLed::PulsingLed(int32_t pwm_ch, int32_t pwm_resolution, int32_t pwm_freq, int32_t led_pin, double pulses_per_second){
    this->pwm_ch;
    this->pwm_resolution;
    this->led_pin;
    this->pwm_freq;
    this->pulses_per_second;
};

void PulsingLed::init(){
    //Start PWM and pulse task
    ledcAttachPin(this->led_pin, this->pwm_ch);
    ledcSetup(this->pwm_ch, this->pwm_freq, this->pwm_resolution);

    xTaskCreate(
            this->pulseTask,
            "Pulse task",
            10000,
            this, //< Pointer gets forwarded to the task
            1,
            NULL);
};

void PulsingLed::pulseTask(void *args){
    PulsingLed *p = static_cast<PulsingLed *>(args);


    double dt = .05;

    double pwm_max = pow(2, p->pwm_resolution) - 1;
    double x = 0;

    TickType_t xTimeIncrement = configTICK_RATE_HZ * dt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        double i = -cos((PI* p->pulses_per_second*x)*2)/2 + 1/2;

        ledcWrite(p->pwm_ch, pwm_max * i);

        if (x >= (1/p->pulses_per_second) * 1000) {
            x = x - (1/p->pulses_per_second) * 1000;
        }
        x += dt;

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};