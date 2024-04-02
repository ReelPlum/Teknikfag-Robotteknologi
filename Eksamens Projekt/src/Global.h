#pragma once

#include <Arduino.h>

// General DC motor stuff
#define DT .1
#define KI 1
#define KD 0
#define KP 0.05
#define PWM_Freq 19500
#define PWM_Res 12
#define PID_CtrlMax 2000
#define CtrlMin -100
#define CtrlMax 100
#define MaxVel 6000
#define IntegrationThreshold 100000
#define ImpulsesPerRotation 1990

// DC motor right
#define DCR_ENCA 4
#define DCR_ENCB 13
#define DCR_INA 26
#define DCR_INB 27
#define DCR_PWM 2
#define DCR_PWMCH 1

// DC motor left
#define DCL_ENCA 16
#define DCL_ENCB 17
#define DCL_INA 23
#define DCL_INB 25
#define DCL_PWM 23
#define DCL_PWMCH 2

// Robot Dimensions
#define WHEELRADIUS 7 // cm
#define b 15          // cm