#include <Arduino.h>
#include <Stabilizer.h>
#include <WebServer.h>
#include <DeadReckoning.h>

//General DC motor stuff
#define DT .1
#define KI 1
#define KD 0
#define KP 0.05
#define PWM_Freq 19500
#define PWM_Res 12
#define PID_CtrlMax 2000
#define CtrlMin -100
#define CtrlMax 100
#define MaxVel 6000
#define IntegrationThreshold 100000
#define ImpulsesPerRotation 1990

//DC motor right
#define DCR_ENCA 33
#define DCR_ENCB 32
#define DCR_INA 5
#define DCR_INB 4
#define DCR_PWM 2
#define DCR_PWMCH 1

//DC motor left
#define DCL_ENCA 27
#define DCL_ENCB 26
#define DCL_INA 18
#define DCL_INB 19
#define DCL_PWM 23
#define DCL_PWMCH 2

//Robot Dimensions
#define WHEELRADIUS 5 //cm
#define b 10 //cm

//Setup classes
DCMotor motorR(false, false, 17, DCR_ENCA, DCR_ENCB, 1, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);
DCMotor motorL(false, false, 17, DCL_ENCA, DCL_ENCB, 1, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);

Stabilizer stabilizer;
DeadReckoning deadReckoning;

double SpeedX;
double SpeedY;

double UpdateCallback(char subtype){
  return 1.0;
};

void ChangeCallback(double *paramValue, char subtype)
{
};

void MoveCallback(double x, double y){
  SpeedX = x * (180);
  SpeedY = y * (180);

  stabilizer.SetExtraEngineSpeed(SpeedY - SpeedX, SpeedY + SpeedX);
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  deadReckoning.init(&motorR, &motorL, MoveCallback, WHEELRADIUS, b, DT);

  motorR.init(KI, KD, KP);
  motorL.init(KI, KD, KP);

  stabilizer.init(&motorR, &motorL);

  init_web("IOT_NET", "esp32esp", ChangeCallback, UpdateCallback);
};

void loop() {
  // put your main code here, to run repeatedly:
  log_i("Angle: %f", stabilizer.current_angle);
  delay(100);
};