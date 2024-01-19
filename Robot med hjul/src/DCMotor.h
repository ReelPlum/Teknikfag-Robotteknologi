#include <Arduino.h>
// #include "global.h"
#include "pid.h"
#include "hbridge.h"
#include <ESP32Encoder.h>

class DCMotor
{
public:
    DCMotor(bool position_mode, int32_t pid_loop_pin, int32_t enc_a_pin, int32_t enc_b_pin, int32_t limit_sw_pin,
            int32_t hbridge_ina_pin, int32_t hbridge_inb_pin, int32_t hbridge_pwm_pin,
            int32_t pwm_channel, int32_t pwm_frequency_hz, int32_t pwm_resolution_bits, double dt, double pid_max_ctrl_value,
            double min_ctrl_value, double max_ctrl_value, double max_vel, double integration_threshold, double impulses_per_rotation);

    void init(double ki, double kd, double kp);

    void set_pos(double pos);

    double get_velocity();

    double get_acceleration();

    void set_velocity(double velocity);

    // Andre medlemsfunktioner og operationer, f.eks. setPos, waitMove, home, osv.
    // ...

private:
    void waitMove();
    void home();
    static void pidTask(void *args);

    bool position_mode;
    int32_t pid_loop_pin;
    int32_t enc_a_pin;
    int32_t enc_b_pin;
    int32_t limit_sw_pin;
    int32_t hbridge_ina_pin;
    int32_t hbridge_inb_pin;
    int32_t hbridge_pwm_pin;
    int32_t pwm_channel;
    int32_t pwm_frequency_hz;
    int32_t pwm_resolution_bits;
    int32_t pwm_min;
    int32_t pwm_max;
    double dt;
    double pid_max_ctrl_value;
    double min_ctrl_value;
    double max_ctrl_value;
    double max_vel;
    double integration_threshold;
    double impulses_per_rotation;

    volatile double acceleration;
    volatile double req_pos;
    volatile double req_vel;
    volatile double current_pos;
    volatile double current_vel;
    double ctrl_pos;
    double ctrl_vel;

    ESP32Encoder encoder;
    Pid pidVel;
    Pid pidPos;
    H_Bridge hbridge;
    int32_t pinPidLoop;
    int32_t pinLimitSw;
    TaskHandle_t pidTaskHandle;
};