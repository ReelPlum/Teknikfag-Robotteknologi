#pragma once

#include <math.h>
#include <DCMotor.h>

struct DeadReckoningData {
    double Angle;
    double X;
    double Y;
};

class DeadReckoning{
    public:
    DeadReckoning(){};

    void init(DCMotor *RightMotor, DCMotor *LeftMotor, double wheelRadius, double b, double DT);

    DeadReckoningData getData();

    private:
    static void Task(void *arg);

    DCMotor *DCMotorLeft;
    DCMotor *DCMotorRight;
    double wheelRadius;
    double b;
    double DT;

    DeadReckoningData Data;

    TaskHandle_t TaskHandle;
};