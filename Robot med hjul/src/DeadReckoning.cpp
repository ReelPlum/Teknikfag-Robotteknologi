#include <DeadReckoning.h>

double CalculateAcceleration(double angleAcceleration, double wheelRadius)
{
    return angleAcceleration * wheelRadius;
};

double CalculateVelocity(double angleVelocity, double wheelRadius)
{
    return angleVelocity * wheelRadius;
};

void GetPosition(double *currentX, double *currentY, double *currentAngle, double aR, double aL, double wR, double wL, double b, double t)
{

    double A = (aR + aL) / 2;
    double B = (wR + wL) / 2;
    double C = (aR - aL) / (2 * b);
    double D = (wR - wL) / b;

    *currentY = *currentY - ((A * t + B) * cos(C * (t * t) + D * t + *currentAngle)) / 95;
    *currentX = *currentX - ((A * t + B) * sin(C * (t * t) + D * t + *currentAngle)) / 95;

    *currentAngle = (C * (t * t) + D * t) + *currentAngle;

};

struct direction {
    double x;
    double y;
};

direction FindDirection(double x, double y, double angle, double targetX, double targetY){
    //Find vector to target
        double vX = targetX - x;
    double vY = targetY - y;

    double L = sqrt(vX * vX + vY * vY) / 4;
    if (L <= 10)
    {
        // Stop robot
        direction zeroDir;
        zeroDir.x = 0;
        zeroDir.y = 0;

        return zeroDir;
    }
    // Formula from ChatGPT
    /*
        https://chat.openai.com/share/61943bfc-6008-4a91-aefb-5fdf748e2b42
        Normal coordinatesystem to rotated coordinatesystem

        x' = x * cos(phi) - y * sin(phi)
        y' = x * sin(phi) - y * cos(phi)
    */

    angle += 1/2 * PI;

    direction dir;
    dir.x = vX * cos(angle) - vY * sin(angle);
    dir.y = vX * sin(angle) + vY * cos(angle);

    double L2 = sqrt(dir.x * dir.x + dir.y * dir.y);

    if (L2 > 1)
    {
        // Length cannot be greater than 1
        dir.y = -dir.y / L2;
        dir.x = -dir.x / L2;
    }

    return dir;
};

void DeadReckoning::init(DCMotor *RightMotor, DCMotor *LeftMotor, moveDirection moveCallback, double wheelRadius, double b, double DT){
    this->DCMotorRight = RightMotor;
    this->DCMotorLeft = LeftMotor;
    this->moveCallback = moveCallback;
    this->wheelRadius = wheelRadius;
    this->b = b;
    this->DT = DT;

    //Start Task
    xTaskCreate(
        this->Task,
        "Pid Task",
        10000,
        this, //< Pointer gets forwarded to the task
        1,
        NULL);
};

void DeadReckoning::setTarget(double x, double y){
    //Set target
};

void DeadReckoning::Task(void *arg){
    DeadReckoning *p = static_cast<DeadReckoning *>(arg);

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->DT;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        double aR = CalculateAcceleration(p->DCMotorRight->get_acceleration(), p->wheelRadius);
        double aL = CalculateAcceleration(p->DCMotorLeft->get_acceleration(), p->wheelRadius);
        double wR = CalculateVelocity(p->DCMotorRight->get_velocity(), p->wheelRadius);
        double wL = CalculateVelocity(p->DCMotorLeft->get_velocity(), p->wheelRadius);
        GetPosition(&(p->Data.X), &(p->Data.Y), &(p->Data.Angle), aR, aL, wR, wL, p->b, p->DT);

        // log_i("Accelerations are R: %f, L: %f and velocities are R: %f, L: %f",aR,aL,wR,wL);

        // log_i("Position is %f, %f and angle is %f", currentX, currentY, currentAngle);

        //Update move callback
        direction dir = FindDirection(p->Data.X, p->Data.Y, p->Data.Angle, p->targetX, p->targetY);

        p->moveCallback(dir.x, dir.y);

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};

DeadReckoningData DeadReckoning::getData(){
    return this->Data;
};