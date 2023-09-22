#include <Arduino.h>
#include <math.h>

class PID{
    public:
    PID(){};
    void Setup(int32_t kp, int32_t ki, int32_t kd, int64_t integral_limit, int32_t delta_time);
    double Calculate(int64_t req_val, int64_t act_val);
    double Squash(double n, double a, double b);

    private:
    int32_t kp;
    int32_t ki;
    int32_t kd;
    int64_t last_err = 0;
    int64_t integral_limit;
    int32_t delta_time;
    int32_t error_sum;
    double max_ctrl;
};