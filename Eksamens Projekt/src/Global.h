#pragma once

#include<Arduino.h>

// General DC motor stuff
const double DT = .1;
const double KI = 1;
const double KD = 0;
const double KP = 0.05;
const int32_t PWM_Freq = 19500;
const int32_t PWM_Res = 12;
const double PID_CtrlMax = 2000;
const double CtrlMin = -100;
const double CtrlMax = 100;
const double MaxVel = 6000;
const double IntegrationThreshold = 100000;
const double ImpulsesPerRotation = 1990;

// DC motor right
const int32_t DCR_ENCA = 4;
const int32_t DCR_ENCB = 13;
const int32_t DCR_INA = 26;
const int32_t DCR_INB = 27;
const int32_t DCR_PWM = 2;
const int32_t DCR_PWMCH = 1;

// DC motor left
const int32_t DCL_ENCA = 16;
const int32_t DCL_ENCB = 17;
const int32_t DCL_INA = 23;
const int32_t DCL_INB = 25;
const int32_t DCL_PWM = 23;
const int32_t DCL_PWMCH = 3;

// Robot Dimensions
const double WHEELRADIUS = 7; // cm
const double b = 15;          // cm

//Led lys
const double POSITIONMODE_LED_PIN = 19;

//buzzer
const int32_t BUZZER_PIN = 0;
const int32_t BUZZER_PWM_CH = 6;