#include <ESP32Encoder.h>
#include "global.h"
#include "pid.h"
#include "hbridge.h"

void update(double *paramValue, char subtype);

double getData(char subtype);

void init_dc();
void set_pos(double pos);

// class DCMotor{
//     public:
//     DCMotor(){};

//     void setup(int32_t pin_pid_loop, int32_t pin_enc_a,);
//     double getData(char subtype);
//     void update(double *paramValue, char subtype);
//     void setPos(int32_t pos);
//     void home();


//     private:
//     void motionTask(void *arg);
//     void waitMove();
//     void pidTask(void *arg);

//     TaskHandle_t PidTaskHandle;
//     TaskHandle_t MotionTaskHandle;
//     ESP32Encoder encoder;
//     Pid pidVel();
//     Pid pidPos();

//     //Pins
//     int32_t PIN_PID_LOOP = 17;
//     int32_t PIN_ENC_A = 26;
//     int32_t PIN_ENC_B = 27;

//     int32_t PIN_LIMIT_SW = 25;

//     int32_t PIN_HBRIDGE_INA = 18; // hbridge INA pin
//     int32_t PIN_HBRIDGE_INB = 19; // hbridge INB pin
//     int32_t PIN_HBRIDGE_PWM = 23; // hbridge PWM pin

//     //PWM
//     const int32_t PWM_CH = 0;                             // 0 - 7(15?)  = 8(16?) channels
//     const int32_t PWM_FREQ_HZ = 19500;                    // 1Hz to 40MHz
//     const int32_t PWM_RES_BITS = 12;                      // 1 to 16(20) bits
//     const int32_t PWM_MIN = 0;                            // minimum pwm value
//     const int32_t PWM_MAX = pow(2, PWM_RES_BITS) - 1;     // maximum pwm value

//     // PID SETUP
//     const double DT_S = 0.001;
//     const double PID_MAX_CTRL_VALUE = 4000;
//     const double MIN_CTRL_VALUE = -100;
//     const double MAX_CTRL_VALUE = 100;

//     //PID
//     int32_t LedState = 0;
//     int32_t SliderVal = 0;
//     double KpVal = 3.1415;
//     double KiVal = 2.71;
//     double KdVal = 42.0;
//     double KdVelVal = 10.0;

//     const double integration_threshold = 200;

//     volatile double req_pos = 100000;
//     volatile double req_vel = 3000;
//     volatile int64_t current_pos;
//     volatile double current_vel;
//     volatile double max_vel = 5000;

//     double ctrl_pos;
//     double ctrl_vel;

//     bool mode_pos = true;
// }