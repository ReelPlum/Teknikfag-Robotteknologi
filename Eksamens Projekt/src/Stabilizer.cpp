#include <Stabilizer.h>
#include <math.h>

#define WIRE_PORT Wire
#define AD0_VAL 1

double radiansToDegrees(double radians){
    return radians * 180/PI;
}

void Stabilizer::Init(DCMotor RightMotor, DCMotor LeftMotor){
    //Initialize stuff here

    this->DT = .01;

    this->sensorFusion.setup(.8);
    this->anglePID.init(this->DT, 1200);

    this->RightMotor = RightMotor;
    this->LeftMotor = LeftMotor;

    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);
    this->myICM.begin(WIRE_PORT, AD0_VAL);

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
        //Check if sensors are ready
        if (!p->myICM.dataReady()){
            continue;
        }

        p->ReadSensors();

        p->anglePID.update(p->wanted_angle, p->current_angle, &(p->ctrl_angle), p->integration_threshold);

        //x er en indstilling ligesom k værdierne i PID. Tune den som ønsket :)
        double x = 1;
        double value = p->current_angle + x * p->wx;

        log_i("Value: %f", value);

        //Set motors
        p->RightMotor.set_velocity(value + p->extraSpeedRight);
        p->LeftMotor.set_velocity(value + p->extraSpeedLeft);

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};

void Stabilizer::SetExtraEngineSpeed(double right, double left){
    //Set extra engine speed for turning etc.
    this->extraSpeedRight = right;
    this->extraSpeedLeft = left;
};

double Stabilizer::ReadSensors(){
    //Read IMU sensors
    this->myICM.getAGMT();

    double acc = atan(this->myICM.accZ()/this->myICM.accY());

    this->wx = this->myICM.gyrX();

    double gyro = this->current_angle + this->DT * this->wx;

    this->gyro = gyro;
    this->acc = acc;

    this->current_angle = this->sensorFusion.calculateValue(radiansToDegrees(acc), gyro);
};