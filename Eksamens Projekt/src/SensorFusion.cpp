#include <SensorFusion.h>

SensorFusion::SensorFusion(double k){
    this->k = k;
};

double SensorFusion::calculateValue(double a, double b){
    double value = this->k * a + (this->k - 1)*b;

    this->value = value;

    return value;
};