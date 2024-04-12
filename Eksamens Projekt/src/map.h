#pragma once

#include <math.h>
#include <Arduino.h>

double map_double(double x, double x_min, double x_max, double y_min, double y_max);

int32_t map_int(int32_t x, int32_t x_min, int32_t x_max, int32_t y_min, int32_t y_max);