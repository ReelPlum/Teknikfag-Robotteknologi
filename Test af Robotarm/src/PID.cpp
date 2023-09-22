#include <PID.h>


void PID::Setup(int32_t kp, int32_t ki, int32_t kd, int64_t integral_limit, int32_t delta_time){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->integral_limit = integral_limit;
    this->delta_time = delta_time;
}

int64_t PID::Calculate(int64_t req_val, int64_t act_val){
    int64_t error = act_val - req_val;

    int64_t value = 0;

    value += error * this->kp;
    
    if (error < this->integral_limit){
        value += this->ki * error * this->delta_time;
    }

    value += this->kd * (error - this->last_err)/this->delta_time;
    this->last_err = error;

    return value;
}