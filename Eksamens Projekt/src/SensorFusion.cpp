#include <SensorFusion.h>

void SensorFusion::setup(double k)
{
    this->k = k;
};

double SensorFusion::calculateValue(double a, double b)
{
    double value = this->k * a + (1 - this->k) * b;

    this->value = value;

    return value;
};