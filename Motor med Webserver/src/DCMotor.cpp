#include <DCMotor.h>

// PINS SETUP
const int32_t PIN_PID_LOOP = 17;
const int32_t PIN_ENC_A = 26;
const int32_t PIN_ENC_B = 27;

const int32_t PIN_LIMIT_SW = 25;

const int32_t PIN_HBRIDGE_INA = 18; // hbridge INA pin
const int32_t PIN_HBRIDGE_INB = 19; // hbridge INB pin
const int32_t PIN_HBRIDGE_PWM = 23; // hbridge PWM pin


// PWM SETUP
const int32_t PWM_CH = 0;                             // 0 - 7(15?)  = 8(16?) channels
const int32_t PWM_FREQ_HZ = 19500;                    // 1Hz to 40MHz
const int32_t PWM_RES_BITS = 12;                      // 1 to 16(20) bits
const int32_t PWM_MIN = 0;                            // minimum pwm value
const int32_t PWM_MAX = pow(2, PWM_RES_BITS) - 1;     // maximum pwm value

// PID SETUP
const double DT_S = 0.001;
const double PID_MAX_CTRL_VALUE = 4000;
const double MIN_CTRL_VALUE = -100;
const double MAX_CTRL_VALUE = 100;

volatile double req_pos = 100000;
volatile double req_vel = 3000;
volatile int64_t current_pos;
volatile double current_vel;
volatile double max_vel = 5000;

const double integration_threshold = 200;

double ctrl_pos;
double ctrl_vel;

bool mode_pos = true;

// Pid pid_vel(DT_S, PID_MAX_CTRL_VALUE);
// Pid pid_pos(DT_S, PID_MAX_CTRL_VALUE);


void DCMotor::pidTask(void *arg)
{
  int64_t prev_pos = this->current_pos;

  TickType_t xTimeIncrement = configTICK_RATE_HZ * this->pidPos.get_dt();
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  { // loop tager mindre end 18us * 2
    digitalWrite(this->PIN_PID_LOOP, HIGH);

    this->current_pos = this->encoder.getCount();
    this->current_vel = (this->current_pos - prev_pos) / this->DT_S;

    if (this->mode_pos)
    {
      this->pidPos.update(this->req_pos, this->current_pos, &(this->ctrl_pos), this->integration_threshold);

      this->req_vel = constrain(this->ctrl_pos, -(this->max_vel), this->max_vel);
    }

    this->pidVel.update(this->req_vel, this->current_vel, &(this->ctrl_vel), 100000);

    this->hbridge.set_pwm(this->ctrl_vel);

    prev_pos = current_pos;
    digitalWrite(this->PIN_PID_LOOP, LOW);
    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void DCMotor::waitMove()
{
  while (abs(this->req_pos - this->current_pos) > 10)
  {
    vTaskDelay(1);
  }
}

void DCMotor::home()
{
  log_v("home initiated...");
  this->req_vel = 40000;
  this->mode_pos = false;

  while (digitalRead(this->PIN_LIMIT_SW))
  {
    vTaskDelay(1);
  }
  this->req_vel = -1000;
  while (!digitalRead(this->PIN_LIMIT_SW))
  {
    vTaskDelay(1);
  }
  this->req_vel = 0;

  this->mode_pos = true;
  this->req_pos = 0;
  this->encoder.setCount(-14100);
  this->waitMove();
  log_v("home complete.");
}

void DCMotor::setPos(int32_t pos)
{
  this->req_pos = pos;
}

void DCMotor::motionTask(void *arg)
{
  this->home();

  vTaskDelay(5000);
  int32_t n = 0;
  //int32_t steps_pr_deg = 1920;

  //TickType_t xTimeIncrement = configTICK_RATE_HZ/10;
  //TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    log_v("motion_task...");

    //set_pos(steps_pr_deg*(90+n*90)); // 90 deg
    this->setPos(n * 500 * 4 * 4.8); // 1 round
    this->waitMove();
    vTaskDelay(5000);
    n++;
    //vTaskDelayUntil( &xLastWakeTime, xTimeIncrement);
  }
}

void DCMotor::updateValue(double *paramValue, char subtype)
{
  switch (subtype)
  {
  case 'p':
    this->pidPos.set_kp(*paramValue);
    break;
  case 'i':
    this->pidPos.set_ki(*paramValue);
    break;
  case 'd':
    this->pidPos.set_kd(*paramValue);
    break;
  case 'l':
    this->pidVel.set_ki(*paramValue);
    break;
  case 's':
    //pid_pos.set_req_pos(*paramValue);
    this->setPos(*paramValue);
  }
}

double DCMotor::getData(char subtype){
  switch(subtype)
  {
  case 'a':
    return this->current_pos;
  case 'b':
    return this->current_vel;
  case 'c':
    return this->ctrl_pos;
  case 'd':
    return this->ctrl_vel;
  case 'e':
    return this->mode_pos;  
  }
  
  //If it asks for something weird for some stupid reason
  return 0.0;
}

void DCMotor::setup(int32_t pin_pid_loop, int32_t pin_enc_a, int32_t pin_enc_b, int32_t pin_limit_sw, int32_t pin_hbridge_ina, int32_t pin_hbridge_inb, int32_t pin_hbridge_pwm, int32_t pwm_ch, int32_t pwm_freq_hz, int32_t pwm_res_bits, int32_t pwm_min, int32_t pwm_max, double dt_s, double pid_max_ctrl_value, double min_ctrl_value, double max_ctrl_value, ESP32Encoder *encoder) // runs exclusive on core 1
{
  this->PIN_PID_LOOP = pin_pid_loop;
  this->PIN_ENC_A = pin_enc_a;
  this->PIN_ENC_B = pin_enc_b;
  this->PIN_LIMIT_SW = pin_limit_sw;
  this->PIN_HBRIDGE_INA = pin_hbridge_ina;
  this->PIN_HBRIDGE_INB = pin_hbridge_inb;
  this->PIN_HBRIDGE_PWM = pin_hbridge_pwm;
  this->PWM_CH = pwm_ch;
  this->PWM_FREQ_HZ = pwm_freq_hz;
  this->PWM_RES_BITS = pwm_res_bits;
  this->PWM_MIN = pwm_min;
  this->PWM_MAX = pwm_max;
  this->DT_S = dt_s;
  this->PID_MAX_CTRL_VALUE;
  this->MAX_CTRL_VALUE = max_ctrl_value;
  this->MIN_CTRL_VALUE = min_ctrl_value;

  this->pidPos.setup(this->DT_S, this->PID_MAX_CTRL_VALUE);
  this->pidVel.setup(this->DT_S, this->PID_MAX_CTRL_VALUE);

  //disableCore0WDT();
  //disableCore1WDT();
  pinMode(this->PIN_PID_LOOP, OUTPUT);
  pinMode(this->PIN_LIMIT_SW, INPUT);

  ESP32Encoder::useInternalWeakPullResistors = UP; // Enable the weak pull up resistors
  encoder->attachFullQuad(PIN_ENC_A, PIN_ENC_B);    // Attache pins for use as encoder pins
  encoder->clearCount();

  this->hbridge.begin(this->PIN_HBRIDGE_PWM, this->PIN_HBRIDGE_INA, this->PIN_HBRIDGE_INB,
                this->PWM_FREQ_HZ, this->PWM_RES_BITS, this->PWM_CH, this->PID_MAX_CTRL_VALUE);

  // log_v("starting motion task");
  // xTaskCreatePinnedToCore(
  //     motion_task,
  //     "motion_task",
  //     3000, /* Stack size in words */
  //     NULL, /* Task input parameter */
  //     2,    /* Priority of the task from 0 to 25, higher number = higher priority */
  //     &MotionTaskHandle,
  //     0); /* Core where the task should run */
}

// void loop() // runs exclusive on core 1
// {
//   Serial.printf(
//       "req_pos: %.2f  curr_pos: %.2f  ctrl_pos: %.2f  set_vel: %.2f  curr_vel: %.2f ctrl_vel: %.2f\n\r",
//       req_pos, (double)current_pos, ctrl_pos, req_vel, current_vel, ctrl_vel);

//   /*
//   int32_t pid_stack_min =  uxTaskGetStackHighWaterMark(PidTaskHandle);
//   int32_t motion_stack_min = uxTaskGetStackHighWaterMark(MotionTaskHandle);  
//   log_i("min stack avaliable [bytes], pid: %d,  motion: %d", pid_stack_min, motion_stack_min);
//   */

//   vTaskDelay(0.2 * configTICK_RATE_HZ);
// }