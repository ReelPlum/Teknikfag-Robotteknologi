#include <DCMotor.h>

DCMotor::DCMotor(int32_t enc_a_pin, int32_t enc_b_pin,
                 int32_t hbridge_ina_pin, int32_t hbridge_inb_pin, int32_t hbridge_pwm_pin,
                 int32_t pwm_channel, int32_t pwm_frequency_hz, int32_t pwm_resolution_bits, double dt
                 double min_ctrl_value, double max_ctrl_value, double impulses_per_rotation, double FullEncoderRotation)
{
    this->enc_a_pin = enc_a_pin;
    this->enc_b_pin = enc_b_pin;
    this->hbridge_ina_pin = hbridge_ina_pin;
    this->hbridge_inb_pin = hbridge_inb_pin;
    this->hbridge_pwm_pin = hbridge_pwm_pin;
    this->pwm_channel = pwm_channel;
    this->pwm_frequency_hz = pwm_frequency_hz;
    this->pwm_resolution_bits = pwm_resolution_bits;
    this->pwm_min = 0;
    this->pwm_max = pow(2, this->pwm_resolution_bits) - 1;
    this->dt = dt;
    this->min_ctrl_value = min_ctrl_value;
    this->max_ctrl_value = max_ctrl_value;
    this->impulses_per_rotation = impulses_per_rotation;
    this->FullEncoderRotation = FullEncoderRotation;

    ESP32Encoder::useInternalWeakPullResistors = puType::up; // Enable the weak pull up resistors
}

void DCMotor::init()
{
    // Implementér initialisering her

    this->encoder.attachFullQuad(this->enc_a_pin, this->enc_b_pin); // Attache pins for use as encoder pins
    this->encoder.clearCount();

    this->hbridge.begin(this->hbridge_ina_pin, this->hbridge_inb_pin, this->pwm_frequency_hz, this->pwm_resolution_bits, this->pwm_channel, this->pid_max_ctrl_value);

    // Start task
    xTaskCreatePinnedToCore(
        this->pidTask,
        "Pid Task",
        DCMotorStack,
        this, //< Pointer gets forwarded to the task
        DCMotorPriority,
        &(this->pidTaskHandle),
        DCMotorCore
        );
}

double DCMotor::calculate_degtovel(double val)
{
    return (val / 360.0) * this->impulses_per_rotation;
};

void DCMotor::set_PWM(int32_t pwm)
{
    this->hbridge.set_pwm(pwm);
};

double DCMotor::getEncoderPosition(){
    return this->current_pos;
}

// Statiske medlemsfunktioner til brug i xTaskCreatePinnedToCore
void DCMotor::pidTask(void *arg)
{

    DCMotor *p = static_cast<DCMotor *>(arg);

    // Implementering af PID-task
    // ...
    double prev_pos = p->current_pos;
    double last_vel = p->current_vel;

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->dt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        #ifdef OutputDCMotorLoop
            digitalWrite(TestOutputPin, HIGH);
        #endif

        p->current_pos = p->encoder.getCount();
        p->current_vel = (p->current_pos - prev_pos) / p->dt;

        p->acceleration = (p->current_vel - last_vel) / p->dt;

        last_vel = p->current_vel;
        prev_pos = p->current_pos;

        #ifdef OutputDCMotorLoop
            digitalWrite(TestOutputPin, LOW);
        #endif

        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}

double DCMotor::get_acceleration()
{
    // Returns rotational velocity
    return (this->acceleration / this->FullEncoderRotation) * TWO_PI;
}

double DCMotor::get_velocity()
{
    // Returns rotational velocity
    return (this->current_vel / this->FullEncoderRotation) * TWO_PI;
}