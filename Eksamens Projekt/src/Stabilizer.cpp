#include <Stabilizer.h>
#include <math.h>

#include <Global.h>

#define WIRE_PORT Wire
#define AD0_VAL 1

double radiansToDegrees(double radians)
{
    return radians * 180 / PI;
}

double Stabilizer::getTargetAngle(){
    return this->wanted_angle + this->extraAngle;
};

void Stabilizer::init(DCMotor *RightMotor, DCMotor *LeftMotor, double gyro_sens)
{
    // Initialize stuff here

    this->DT = .005;
    this->gyro_sens = gyro_sens;

    this->sensorFusion.setup(.05);
    this->anglePID.init(this->DT, 50000);

    this->anglePID.set_kd(0);
    this->anglePID.set_kp(0);
    this->anglePID.set_ki(0);

    this->RightMotor = RightMotor;
    this->LeftMotor = LeftMotor;

    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);
    this->myICM.begin(WIRE_PORT, AD0_VAL);

    pinMode(32, OUTPUT);

    // start update task
    xTaskCreate(
        this->Update,
        "Stabilizer PID task",
        10000,
        this, //< Pointer gets forwarded to the task
        1,
        NULL);
};

Pid* Stabilizer::getPid(){
    return &(this->anglePID);
}

double Stabilizer::getK(){
    return this->sensorFusion.getK();
}

double Stabilizer::getGyroSens(){
    return this->gyro_sens;
}

void Stabilizer::SetGyroSens(double gyro_sens){
    this->gyro_sens = gyro_sens;
}

void Stabilizer::SetKI(double KI){
    this->ki = KI;

    this->anglePID.set_ki(KI);
}

double Stabilizer::getKI(){
    return this->ki;
}

void Stabilizer::SetKP(double KP){
    this->kp = KP;

    this->anglePID.set_kp(KP);
}

double Stabilizer::getKP(){
    return this->kp;
}

void Stabilizer::SetK(double k){
    (this->sensorFusion).setK(k);
}

void Stabilizer::Update(void *arg)
{
    Stabilizer *p = static_cast<Stabilizer *>(arg);

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->anglePID.get_dt();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        // Check if sensors are ready

        digitalWrite(32, HIGH);

        if (!p->myICM.dataReady())
        {
            continue;
        }



        p->ReadSensors();

        //Taget angle = 0 + extra angle
        p->anglePID.update(p->getTargetAngle(), p->current_angle, &(p->ctrl_angle), IntegrationThreshold);

        double value = (p->ctrl_angle + p->gyro_sens * p->wx);

        if (abs(p->current_angle) >= 91)
        {
            // Stop motors
            p->RightMotor->set_PWM(0);
            p->LeftMotor->set_PWM(0);

            return;
        }
        else
        {
            // Get velocity
            int32_t velR = p->RightMotor->calculate_degtovel(value);
            int32_t velL = p->LeftMotor->calculate_degtovel(value);

            //Set PWM
            p->RightMotor->set_PWM(velR);
            p->LeftMotor->set_PWM(velL);
        };

        digitalWrite(32, LOW);

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
};

void Stabilizer::SetExtraAngle(double angle){
    this->extraAngle = angle;
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

    // log_i("Acc: %f", -radiansToDegrees(acc));

    this->wx = this->myICM.gyrX();

    double gyro = this->current_angle + (this->DT) * this->wx;

    // log_i("Gyro: %f", gyro);

    this->gyro = gyro;
    this->acc = acc;

    this->current_angle = this->sensorFusion.calculateValue(-radiansToDegrees(acc), gyro);

    // //Call registered callbacks
    // for (angleChangeCallback callback : this->angleCallbacks){
    //     //Run callback function
    //     callback(&(this->current_angle));
    // };
};

void Stabilizer::RegisterAngleCallback(angleChangeCallback callback){
    this->angleCallbacksNum++;
    this->angleCallbacks[this->angleCallbacksNum] = callback;
};