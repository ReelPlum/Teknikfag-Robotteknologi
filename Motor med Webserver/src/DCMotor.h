#include <ESP32Encoder.h>
#include "global.h"
#include "pid.h"
#include "hbridge.h"

TaskHandle_t PidTaskHandle;
TaskHandle_t MotionTaskHandle;
ESP32Encoder encoder;
Pid pid_vel(DT_S, PID_MAX_CTRL_VALUE);
Pid pid_pos(DT_S, PID_MAX_CTRL_VALUE);
H_Bridge hbridge;

void update(double *paramValue, char subtype);

void init_dc();