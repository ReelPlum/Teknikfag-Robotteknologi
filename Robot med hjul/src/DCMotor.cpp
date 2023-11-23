#include <DCMotor.h>

DCMotor::DCMotor(bool position_mode, int32_t pid_loop_pin, int32_t enc_a_pin, int32_t enc_b_pin, int32_t limit_sw_pin,
                 int32_t hbridge_ina_pin, int32_t hbridge_inb_pin, int32_t hbridge_pwm_pin,
                 int32_t pwm_channel, int32_t pwm_frequency_hz, int32_t pwm_resolution_bits, double dt, double pid_max_ctrl_value,
                 double min_ctrl_value, double max_ctrl_value, double max_vel, double integration_threshold, double impulses_per_rotation)
{
    this->pid_loop_pin = pid_loop_pin;
    this->enc_a_pin = enc_a_pin;
    this->enc_b_pin = enc_b_pin;
    this->limit_sw_pin = limit_sw_pin;
    this->hbridge_ina_pin = hbridge_ina_pin;
    this->hbridge_inb_pin = hbridge_inb_pin;
    this->hbridge_pwm_pin = hbridge_pwm_pin;
    this->pwm_channel = pwm_channel;
    this->pwm_frequency_hz = pwm_frequency_hz;
    this->pwm_resolution_bits = pwm_resolution_bits;
    this->pwm_min = 0;
    this->pwm_max = pow(2, this->pwm_resolution_bits) - 1;
    this->dt = dt;
    this->pid_max_ctrl_value = pid_max_ctrl_value;
    this->min_ctrl_value = min_ctrl_value;
    this->max_ctrl_value = max_ctrl_value;
    this->integration_threshold = integration_threshold;
    this->position_mode = position_mode;
    this->max_vel = max_vel;
    this->impulses_per_rotation = impulses_per_rotation;


    this->pidPos.init(this->dt, this->pid_max_ctrl_value);
    this->pidVel.init(this->dt, this->pid_max_ctrl_value);
}

void DCMotor::init()
{
    // Implementér initialisering her
    ESP32Encoder::useInternalWeakPullResistors = UP;                // Enable the weak pull up resistors
    this->encoder.attachFullQuad(this->enc_a_pin, this->enc_b_pin); // Attache pins for use as encoder pins
    this->encoder.clearCount();

    this->hbridge.begin(this->hbridge_pwm_pin, this->hbridge_ina_pin, this->hbridge_inb_pin, this->pwm_frequency_hz, this->pwm_resolution_bits, this->pwm_channel, this->pid_max_ctrl_value);

    pinMode(this->pid_loop_pin, OUTPUT);
    pinMode(this->limit_sw_pin, INPUT);

    log_i("Initializing!");

    // Start task
    xTaskCreate(
        this->pidTask,
        "Pid Task",
        10000,
        this, //< Pointer gets forwarded to the task
        1,
        NULL);
}

// Andre medlemsfunktioner og operationer, f.eks. setPos, waitMove, home, osv.
// ...

// Andre private medlemsvariabler og hjælpefunktioner
// ...

// Statiske medlemsfunktioner til brug i xTaskCreatePinnedToCore
void DCMotor::pidTask(void *arg)
{


    DCMotor *p = static_cast<DCMotor *>(arg);

    // Implementering af PID-task
    // ...
    int64_t prev_pos = p->current_pos;

    TickType_t xTimeIncrement = configTICK_RATE_HZ * p->pidPos.get_dt();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    { // loop tager mindre end 18us * 2
        digitalWrite(p->pid_loop_pin, HIGH);

        log_i("Hej mor");

        p->current_pos = p->encoder.getCount();
        p->current_vel = (p->current_pos - prev_pos) / p->dt;

        if (p->position_mode)
        {
            p->pidPos.update(p->req_pos, p->current_pos, &(p->ctrl_pos), p->integration_threshold);

            p->req_vel = constrain(p->ctrl_pos, -(p->max_vel), p->max_vel);
        }

        p->pidVel.update(p->req_vel, p->current_vel, &(p->ctrl_vel), 100000);

        // log_i("Verdies %f and %f", current_vel, req_pos);
        p->hbridge.set_pwm(p->ctrl_vel);

        prev_pos = p->current_pos;
        digitalWrite(p->pid_loop_pin, LOW);
        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}

double DCMotor::get_speed(){
    return this->current_vel / this->impulses_per_rotation;
}

void DCMotor::waitMove()
{
    // Implementering af waitMove
    // ...
    while (abs(req_pos - current_pos) > 10)
    {
        vTaskDelay(1);
    }
}

void DCMotor::home()
{
    // Implementering af home
    // ...
    
}

void DCMotor::set_pos(double pos)
{
    // Set position of DC motor
    this->req_pos = pos;
}