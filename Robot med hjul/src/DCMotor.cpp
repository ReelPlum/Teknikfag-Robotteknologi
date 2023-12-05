#include <DCMotor.h>

// DCMotor motor(false, 17, 26, 27, 1, 18, 19, 23, 1, 19500, 12, 0.1, 4000, -100, 100, 100, 200, 1990);

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

    pinMode(this->pid_loop_pin, OUTPUT);
    pinMode(this->limit_sw_pin, INPUT);

    ESP32Encoder::useInternalWeakPullResistors = UP;                // Enable the weak pull up resistors

}

void DCMotor::init(double ki, double kd, double kp)
{
    // Implementér initialisering her
    log_i("Initializing!");

    log_i("%i", this->enc_a_pin);
    log_i("%i", this->enc_b_pin);

    this->encoder.attachFullQuad(this->enc_a_pin, this->enc_b_pin); // Attache pins for use as encoder pins
    this->encoder.clearCount();

    this->hbridge.begin(this->hbridge_pwm_pin, this->hbridge_ina_pin, this->hbridge_inb_pin, this->pwm_frequency_hz, this->pwm_resolution_bits, this->pwm_channel, this->pid_max_ctrl_value);

    this->pidPos.init(this->dt, this->pid_max_ctrl_value);
    this->pidVel.init(this->dt, this->pid_max_ctrl_value);

    this->pidVel.set_ki(ki);
    this->pidVel.set_kd(kd);
    this->pidVel.set_kp(kp);

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

        //log_i("Hej mor");

        p->current_pos = p->encoder.getCount();
        p->current_vel = (p->current_pos - prev_pos) / p->dt;

        if (p->position_mode)
        {
            p->pidPos.update(p->req_pos, p->current_pos, &(p->ctrl_pos), p->integration_threshold);

            p->req_vel = constrain(p->ctrl_pos, -(p->max_vel), p->max_vel);
        }

        (p->current_vel / p->ctrl_vel)/p->dt;

        // log_i("Verdies %f and %f", current_vel, req_pos);
        p->hbridge.set_pwm(p->ctrl_vel);

        //log_i("Current pos")
        //log_i("Req vel %f og current vel %f",p->req_vel, p->current_vel);
        //log_i("Ctrl vel: %f", p->ctrl_vel);

        prev_pos = p->current_pos;
        digitalWrite(p->pid_loop_pin, LOW);
        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
    }
}

double DCMotor::get_velocity()
{
    // Returns rotational velocity
    return (this->current_vel / this->impulses_per_rotation) * 2*PI;
}

double DCMotor::get_acceleration()
{
    // Returns rotational velocity
    return (this->current_vel - this->ctrl_vel)/this->dt;
}

void DCMotor::set_velocity(double velocity)
{
    // Set angle velocity
    this->req_vel = velocity;
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