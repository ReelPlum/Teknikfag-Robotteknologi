#pragma once

#include <Arduino.h>

// General DC motor stuff
#define MotorKI 1.0
#define MotorKD 9.5
#define MotorKP 250.0
#define MotorK 0.02

#define PWM_Freq 19500
#define PWM_Res 12
#define PID_CtrlMax 6000.0
#define CtrlMin -10000.0
#define CtrlMax 10000.0
#define MaxVel 6000.0
#define IntegrationThreshold 5.0
#define ImpulsesPerRotation 2000.0

//Task Stack sizes
#define WebsocketStack 10000
#define StabilizerPIDStack 10000
#define BuzzerStack 3000
#define PulsingLedStack 3000
#define DCMotorStack 10000
#define DeadReckoningStack 3000
#define SyncTaskStack 10000
#define StabilizerAngleStack 10000

//Task priorities
#define WebsocketPriority 25
#define StabilizerPIDPriority 25
#define PulsingLedPriority 1
#define BuzzerPriority 1
#define DCMotorPriority 24
#define DeadReckoningPriority 23
#define SyncTaskPriority 2
#define StabilizerAnglePriority 24

//Task Cores
#define WebsocketCore 0
#define StabilizerPIDCore 1
#define PulsingLedCore 1
#define BuzzerCore 1
#define DCMotorCore 1
#define DeadReckoningCore 1
#define SyncTaskCore 1
#define StabilizerAngleCore 1

//Update Speeds
#define WebsocketSyncSpeed 0.5
#define StabilizerSpeed 0.005
#define LedReloadSpeed 0.1
#define BuzzerSpeed .25
#define AngleTaskSpeed .0025
#define DeadReckoningSpeed .05

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
#define DCL_PWMCH 3

// Robot Dimensions
#define WHEELRADIUS 0.075 // cm
#define WHEELB 0.15          // cm

//Led lys
#define POSITIONMODE_LED_PIN 19

//buzzer
#define BUZZER_PIN 33
#define BUZZER_PWM_CH 6