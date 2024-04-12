#include <map.h>

double map_double(double x, double x_min, double x_max, double y_min, double y_max)
{
    double a = (y_max - y_min) / (x_max - x_min);

    return a * x + (y_min - x_min * a);
}

int32_t map_int(int32_t x, int32_t x_min, int32_t x_max, int32_t y_min, int32_t y_max)
{
    double a = ((double)y_max - (double)y_min) / ((double)x_max - (double)x_min);

    return (int32_t)(a * x + (y_min - x_min * a));
}