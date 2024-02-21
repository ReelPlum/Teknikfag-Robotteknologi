#include <DeadReckoning.h>

double CalculateAcceleration(double angleAcceleration, double wheelRadius)
{
    return angleAcceleration*wheelRadius;
}

double CalculateVelocity(double angleVelocity, double wheelRadius)
{
    return angleVelocity * wheelRadius;
}

void GetPosition(double *currentX, double *currentY, double *currentAngle, double aR, double aL, double wR, double wL, double b, double t)
{
    double A = (aR + aL) / 2;
    double B = (wR + wL) / 2;
    double C = (aR - aL) / (2*b);
    double D = (wR - wL) / b;

    *currentY = *currentY - ((A * t + B)*cos(C* (t*t) + D*t + *currentAngle))/95;
    *currentX = *currentX - ((A*t+ B)*sin(C* (t*t) + D*t + *currentAngle))/95;

    *currentAngle = C*(t*t)+D*t + *currentAngle;

}