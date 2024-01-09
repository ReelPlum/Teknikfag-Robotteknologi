#pragma once

#include <Arduino.h>
#include <DCMotor.h>
#include <DeadReckoning.h>

#define WHEELRADIUS 5
#define DT .1
#define b 24.5

double currentX = 0;
double currentY = 0;
double currentAngle = 0;