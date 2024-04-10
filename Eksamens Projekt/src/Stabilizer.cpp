#include <Stabilizer.h>
#include <math.h>

#define WIRE_PORT Wire
#define AD0_VAL 1

double radiansToDegrees(double radians)
{
    return radians * 180 / PI;
}

void Stabilizer::init(DCMotor *RightMotor, DCMotor *LeftMotor)
{
    // Initialize stuff here

    this->DT = .01;

    this->sensorFusion.setup(.05);
    this->anglePID.init(this->DT, 1200);

    this->RightMotor = RightMotor;
    this->LeftMotor = LeftMotor;

    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);
    this->myICM.begin(WIRE_PORT, AD0_VAL);

    // start update task
    xTaskCreate(
        this->Update,
        "Stabilizer PID task",
        10000,
        this, //< Pointer gets forwarded to the task
        1,
        NULL);
};

void Stabilizer::Update(void *arg)
{
    Stabilizer *p = static_cast<Stabilizer *>(arg);

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->anglePID.get_dt();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        // Check if sensors are ready
        if (!p->myICM.dataReady())
        {
            continue;
        }

        p->ReadSensors();

        p->anglePID.update(p->wanted_angle, p->current_angle, &(p->ctrl_angle), p->integration_threshold);

        // x er en indstilling ligesom k værdierne i PID. Tune den som ønsket :)
        double x = 1;
        double value = (p->current_angle + x * p->wx);

        if (abs(p->current_angle) >= 90)
        {
            // Stop motors
            //  p->RightMotor->set_velocity_deg(0);
            //  p->LeftMotor->set_velocity_deg(0);

            log_i("0");

            p->RightMotor->set_PWM(0);
            p->LeftMotor->set_PWM(0);
        }
        else
        {
            // log_i("Angle: %f", p->current_angle);
            //  Set motors
            // p->RightMotor->set_velocity_deg(value + p->extraSpeedRight);
            // p->LeftMotor->set_velocity_deg(value + p->extraSpeedLeft);

            // Get velocity
            int32_t velR = p->RightMotor->calculate_degtovel(value + p->extraSpeedRight);
            int32_t velL = p->LeftMotor->calculate_degtovel(value + p->extraSpeedLeft);

            p->RightMotor->set_PWM(velR);
            p->LeftMotor->set_PWM(velL);
        };

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};

void Stabilizer::SetExtraEngineSpeed(double right, double left)
{
    // Set extra engine speed for turning etc.
    this->extraSpeedRight = right;
    this->extraSpeedLeft = left;
};

void Stabilizer::ReadSensors()
{
    // Read IMU sensors
    this->myICM.getAGMT();

    double acc = 0;
    if (this->myICM.accY() != 0)
    {
        acc = atan(this->myICM.accZ() / this->myICM.accY());
    }
    else
    {
        acc = 0;
    }

    this->wx = this->myICM.gyrZ();

    double gyro = this->current_angle + (this->DT) * this->wx;

    this->gyro = gyro;
    this->acc = acc;

    this->current_angle = this->sensorFusion.calculateValue(-radiansToDegrees(acc), gyro);
};