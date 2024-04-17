#include <Global.h>
#include <Arduino.h>
#include <Stabilizer.h>
#include <WebServer.h>
#include <DeadReckoning.h>
#include <PulsingLed.h>
#include <DCMotor.h>
#include <Buzzer.h>

// Setup classes
DCMotor motorR(false, false, 5, DCR_ENCA, DCR_ENCB, 1, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);
DCMotor motorL(false, false, 5, DCL_ENCA, DCL_ENCB, 1, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);

PulsingLed led(8, 8, 19500, POSITIONMODE_LED_PIN, 0.25);

Stabilizer stabilizer;
DeadReckoning deadReckoning;
Buzzer angleBuzzer;

double SpeedX;
double SpeedY;

double LocationX;
double LocationY;

bool LocationMode = false;

void buzzerAngleChangeCallback(double *angle){
  angleBuzzer.change_freq(map_int(*angle, 0, 45, 4200, 3700));
};

double UpdateCallback(char subtype)
{
  DeadReckoningData data = deadReckoning.getData();
  direction location = deadReckoning.getTarget();

  if (subtype == 'u')
  {
    return data.X;

  }
  else if (subtype == 'r')
  {
    return data.Y;

  }
  else if (subtype == 'a')
  {
    return data.Angle;
  }
  else if (subtype == 'x'){
    return location.x;
  }
  else if (subtype == 'y'){
    return location.y;
  }
  else if (subtype == 'd'){
    return stabilizer.getGyroSens();
  }
  else if (subtype == 'k'){
    return stabilizer.getK();
  }
  else if (subtype == 'i'){
    return stabilizer.getKI();
  }
  else if (subtype == 'p'){
    return stabilizer.getKP();
  }
  else if (subtype == 't'){
    return stabilizer.getTargetAngle();
  }
  else if (subtype == 'e'){
    Pid* anglePID = stabilizer.getPid();
    return anglePID->get_error();
  };
  
  //If it asks for something weird for some stupid reason
  return 0.0;
};

const double res = 1000;

void ChangeCallback(double *paramValue, char subtype)
{

  if (subtype == 'x'){
    // rotate
    log_i("%i", LocationMode);
    if (LocationMode)
    {
      return;
    }

    log_i("Setting speed");

    SpeedX = (*paramValue) / res * (-6000);

    stabilizer.SetExtraEngineSpeed(-SpeedX, SpeedX);

  }
  else if (subtype == 'y'){
    // forward / backward
    if (LocationMode)
    {
      return;
    }

    log_i("Setting speed");

    SpeedY = (*paramValue) / res * (6);

    stabilizer.SetExtraAngle(SpeedY * 10);
  }
  else if (subtype == 'a'){
    // location x
    LocationX = *paramValue / res;

    deadReckoning.setTarget(LocationX, LocationY);
  }
  else if  (subtype == 'b'){
    // location y
    LocationY = *paramValue / res;

    deadReckoning.setTarget(LocationX, LocationY);
  }
  else if (subtype == 'l'){
    // location toggle
    SpeedY = 0;
    SpeedX = 0;

    if ((int)(*paramValue) == 1)
    {
      LocationMode = true;
    }
    else
    {
      LocationMode = false;
    }
  }
  if (subtype == 'd'){

    //change kd on stabilizer
    stabilizer.SetKD((*paramValue)/res);

  }
  if (subtype == 'k'){
    //change k on stabilizer

    if ((*paramValue)/res > 1.0) {
      stabilizer.SetK(1);
      return;
    }
    if ((*paramValue)/res < 0) {
      stabilizer.SetK(0);
      return;
    }
    
    stabilizer.SetK((*paramValue)/res);

  };
  if (subtype == 'i'){
    //change ki on stabilizer
    
    stabilizer.SetKI((*paramValue)/res);

  };
  if (subtype == 'p'){
    log_i("Setting kp!!");

    //change kp on stabilizer
    log_i("kp: %f", (*paramValue)/res);
    
    stabilizer.SetKP((*paramValue)/res);

  };
  if (subtype == 't'){
    stabilizer.setWantedAngle((*paramValue)/res);
  }
};

void MoveCallback(double x, double y)
{
  if (!LocationMode)
  {
    return;
  }

  SpeedX = x * (180);
  SpeedY = y * (180);

  stabilizer.SetExtraEngineSpeed(SpeedY - SpeedX, SpeedY + SpeedX);
};

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  led.init();

  deadReckoning.init(&motorR, &motorL, MoveCallback, WHEELRADIUS, b, DT);

  motorR.init(KI, KD, KP);
  motorL.init(KI, KD, KP);

  stabilizer.init(&motorR, &motorL, .5);

  angleBuzzer.init(BUZZER_PIN, BUZZER_PWM_CH, 4200);
  //stabilizer.RegisterAngleCallback(buzzerAngleChangeCallback);

  init_web("IOT_NET", "esp32esp", ChangeCallback, UpdateCallback);
};

double i = 0;
void loop()
{
  //log_i("Current speed: %f, %f", SpeedX, SpeedY);
  // log_i("Gyrosens %f", stabilizer.getGyroSens());
  // log_i("KI: %f", stabilizer.getKI());
  // log_i("KP: %f", stabilizer.getKP());
  // log_i("K: %f", stabilizer.getK());

  Pid* anglePID = stabilizer.getPid();

  log_i("Error: %f", anglePID->get_error());

  log_i("Target Angle: %f", stabilizer.getTargetAngle());

  delay(2000);
};