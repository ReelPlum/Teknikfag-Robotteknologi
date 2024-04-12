#include <Buzzer.h>


Buzzer::Buzzer(){

};

void Buzzer::init(int8_t io_pin, int32_t pwm_ch, int32_t initial_frequency){
    //Connect PWM to pin & channel with frequency
    this->pwm_ch = pwm_ch;
    this->pin = io_pin;
    this->frequency = initial_frequency;

    int32_t resolution = 6; //Resolution in bits

    ledcAttachPin(this->pin, this->pwm_ch);
    ledcSetup(this->pwm_ch, this->frequency, resolution);

    //Write tone with PWM to buzzer
    ledcWriteTone(this->pwm_ch, this->frequency);
};

void Buzzer::change_freq(int32_t frequency){
    //Change PWM frequency
    this->frequency = frequency;

    //Change written tone to buzzer
    ledcWriteTone(this->pwm_ch, this->frequency);
};