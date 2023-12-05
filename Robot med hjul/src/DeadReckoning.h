#include <math.h>

double CalculateAcceleration(double angleAcceleration, double wheelRadius);

double CalculateVelocity(double angleVelocity, double wheelRadius);

void GetPosition(double *currentX, double *currentY, double *currentAngle, double aR, double aL, double wR, double wL, double b, double t);