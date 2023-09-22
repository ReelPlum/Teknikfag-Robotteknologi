#include <PID.h>


void PID::Setup(int32_t kp, int32_t ki, int32_t kd, int64_t integral_limit, int32_t delta_time, double max_ctrl){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->integral_limit = integral_limit;
    this->delta_time = delta_time;
    this->max_ctrl = max_ctrl;
}

double PID::Squash(double n, double a, double b){
    if (n > a){
        return a;
    }else if (n < b){
        return b;
    }
    return n;
}

double PID::Calculate(int64_t req_val, int64_t act_val){
    double error = act_val - req_val;

    double value = 0;

    value += error * this->kp;
    
    if (fabs(error) < this->integral_limit){
        this->error_sum += error * this->delta_time;

        value += this->ki * this->error_sum;
    }

    value += this->kd * (this->last_err - error)/this->delta_time;
    this->last_err = error;

    return Squash(value, this->max_ctrl, -this->max_ctrl);
}