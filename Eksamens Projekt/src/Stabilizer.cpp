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

void Stabilizer::setWantedAngle(double WantedAngle){
    this->wanted_angle = WantedAngle;
}

void Stabilizer::init(DCMotor *RightMotor, DCMotor *LeftMotor, double kp, double ki, double kd, double k)
{
    // Initialize stuff here

    this->DT = StabilizerSpeed;
    this->kd = kd;

    this->sensorFusion.setup(k);
    this->anglePID.init(this->DT, 50000);

    this->anglePID.set_kd(kd);
    this->anglePID.set_kp(kp);
    this->anglePID.set_ki(ki);

    this->RightMotor = RightMotor;
    this->LeftMotor = LeftMotor;

    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);
    this->myICM.begin(WIRE_PORT, AD0_VAL);

    // start update task
    xTaskCreatePinnedToCore(
        this->BalanceTask,
        "Stabilizer PID Task",
        StabilizerPIDStack,
        this, //< Pointer gets forwarded to the task
        StabilizerPIDPriority,
        &(this->balanceTaskHandle),
        StabilizerPIDCore
        );

    xTaskCreatePinnedToCore(
        this->AngleTask,
        "Stabilizer Angle Task",
        StabilizerAngleStack,
        this, //< Pointer gets forwarded to the task
        StabilizerAnglePriority,
        &(this->angleTaskHandle),
        StabilizerAngleCore
        );
};

Pid* Stabilizer::getPid(){
    return &(this->anglePID);
}

double Stabilizer::getK(){
    return this->sensorFusion.getK();
}

double Stabilizer::getKD(){
    return this->kd;
}

void Stabilizer::SetKD(double KD){
    this->kd = KD;

    this->anglePID.set_kd(KD);
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

double Stabilizer::getCtrlAngle(){
    return this->ctrl_angle;
}

void Stabilizer::AngleTask(void *arg){
    Stabilizer *p = static_cast<Stabilizer *>(arg);

    while (!p->myICM.dataReady());

    log_i("Ready!");

    TickType_t xTimeIncrement = configTICK_RATE_HZ * (StabilizerSpeed / 5); //5 angle measurements per stabilizer task
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    { // loop tager mindre end 18us * 2
        p->ReadSensors();

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}

void Stabilizer::BalanceTask(void *arg)
{
    Stabilizer *p = static_cast<Stabilizer *>(arg);

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->anglePID.get_dt();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        // Check if sensors are ready
        #ifdef OutputStabilizerLoop
            digitalWrite(TestOutputPin, HIGH);
        #endif

        //Taget angle = 0 + extra angle
        p->anglePID.update(p->getTargetAngle(), p->current_angle, &(p->ctrl_angle), IntegrationThreshold, p->wx);

        if (fabs(p->current_angle) >= 45.0)
        {
            // // Stop motors
            p->RightMotor->set_PWM(0);
            p->LeftMotor->set_PWM(0);
        }
        else
        {
            // Get velocity
            int32_t velR = p->RightMotor->calculate_degtovel(p->ctrl_angle);
            int32_t velL = p->LeftMotor->calculate_degtovel(p->ctrl_angle);

            //Set PWM
            p->RightMotor->set_PWM(velR);
            p->LeftMotor->set_PWM(velL);
        };

        #ifdef OutputStabilizerLoop
            digitalWrite(TestOutputPin, LOW);
        #endif

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

    this->wx = (this->myICM.gyrX());

    double gyro = this->current_angle + (this->DT) * this->wx;

    this->gyro = gyro;
    this->acc = acc;

    this->current_angle = this->sensorFusion.calculateValue(-radiansToDegrees(acc), gyro);
};