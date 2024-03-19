#pragma once

#include <math.h>
#include <DCMotor.h>

struct DeadReckoningData {
    double Angle;
    double X;
    double Y;
};

typedef void (*moveDirection)(double x, double y);

class DeadReckoning{
    public:
    DeadReckoning(){};

    void init(DCMotor *RightMotor, DCMotor *LeftMotor, moveDirection moveCallback, double wheelRadius, double b, double DT);

    DeadReckoningData getData();

    void setMoveDirection(moveDirection callback);
    void setTarget(double x, double y);

    private:
    static void Task(void *arg);

    moveDirection moveCallback;

    DCMotor *DCMotorLeft;
    DCMotor *DCMotorRight;
    double wheelRadius;
    double b;
    double DT;

    double targetX = -200;
    double targetY = -200;

    DeadReckoningData Data;

    TaskHandle_t TaskHandle;
};