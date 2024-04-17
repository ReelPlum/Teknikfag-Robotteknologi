#include <PulsingLed.h>


PulsingLed::PulsingLed(int32_t pwm_ch, int32_t pwm_resolution, int32_t pwm_freq, int32_t led_pin, double pulses_per_second){
    this->pwm_ch = pwm_ch;
    this->pwm_resolution = pwm_resolution;
    this->led_pin = led_pin;
    this->pwm_freq = pwm_freq;
    this->pulses_per_second = pulses_per_second;
};

void PulsingLed::init(){
    //Start PWM and pulse task
    log_i("Starting led");

    pinMode(this->led_pin, OUTPUT);
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


    double dt = .01;

    double pwm_max = pow(2, p->pwm_resolution) - 1;
    double x = 0;

    double i = 0;

    TickType_t xTimeIncrement = configTICK_RATE_HZ * dt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2

        i = -cos(2.0*x* p->pulses_per_second * PI)/2.0 + 0.5;

        ledcWrite(p->pwm_ch, pwm_max * i);

        if (x >= (1/p->pulses_per_second) * 1000) {
            x = x - (1/p->pulses_per_second) * 1000;
        }
        x += dt;

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};