#include <Stabilizer.h>
#include <math.h>


void Stabilizer::Init(DCMotor RightMotor, DCMotor LeftMotor){
    //Initialize stuff here

    this->DT = .01;

    this->sensorFusion.setup(1);
    this->anglePID.init(this->DT, 1200);

    this->RightMotor = RightMotor;
    this->LeftMotor = LeftMotor;

    //start update task
    xTaskCreate(
        this->Update,
        "Stabilizer PID task",
        10000,
        this, //< Pointer gets forwarded to the task
        1,
        NULL);
};

void Stabilizer::Update(void *arg){
    Stabilizer *p = static_cast<Stabilizer *>(arg);


    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->anglePID.get_dt();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        p->ReadSensors();

        p->anglePID.update(p->wanted_angle, p->current_angle, &(p->ctrl_angle), p->integration_threshold);

        //x er en indstilling ligesom k værdierne i PID. Tune den som ønsket :)
        double x = 1;
        double value = p->current_angle + x * p->wx;


        //Set motors
        p->RightMotor.set_velocity(value + p->extraSpeedRight);
        p->LeftMotor.set_velocity(value + p->extraSpeedLeft);
    }
};

void Stabilizer::SetExtraEngineSpeed(double right, double left){
    //Set extra engine speed for turning etc.
    this->extraSpeedRight = right;
    this->extraSpeedLeft = left;
};

double Stabilizer::ReadSensors(){
    //Read IMU sensors
    double accx = 1;
    double accy = 1;

    double acc = atan(accx/accy);

    double wx = 1;

    double gyro = this->current_angle + this->DT * wx;

    this->wx = wx;
    this->acc = acc;

    this->current_angle = this->sensorFusion.calculateValue(acc, gyro);
};