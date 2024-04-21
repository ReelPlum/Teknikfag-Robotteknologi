#include <Buzzer.h>
#include <Global.h>

Buzzer::Buzzer(){};

void Buzzer::init(int8_t io_pin, int32_t pwm_ch, int32_t max_frequency, int32_t min_frequency, Stabilizer *stabilizer){
    //Connect PWM to pin & channel with frequency
    this->pwm_ch = pwm_ch;
    this->pin = io_pin;
    this->max_frequency = max_frequency;
    this->min_frequency = min_frequency;

    this->stabilizer = stabilizer;

    int32_t resolution = 6; //Resolution in bits

    ledcAttachPin(this->pin, this->pwm_ch);
    ledcSetup(this->pwm_ch, this->min_frequency, resolution);

    //Write tone with PWM to buzzer
    ledcWriteTone(this->pwm_ch, this->min_frequency);

    //Start Task
    xTaskCreatePinnedToCore(
        this->Task,
        "Buzzer Task",
        BuzzerStack,
        this, //< Pointer gets forwarded to the task
        BuzzerPriority,
        &(this->TaskHandle),
        BuzzerCore
    );
};

void Buzzer::change_freq(int32_t frequency){
    //Change PWM frequency
    //Change written tone to buzzer
    ledcWriteTone(this->pwm_ch, frequency);
};

void Buzzer::toggle(){
    this->enabled = !this->enabled;
}

void Buzzer::Task(void *arg){
    Buzzer *p = static_cast<Buzzer *>(arg);

    TickType_t xTimeIncrement = configTICK_RATE_HZ * BuzzerSpeed;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        if (p->enabled){
            double angle = p->stabilizer->getPid()->get_error();
            p->change_freq(map_double(fabs(angle), 0, 90, p->min_frequency, p->max_frequency));
        }
        else {
            p->change_freq(0);
        }

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}