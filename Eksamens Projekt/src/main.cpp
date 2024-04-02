#include <Arduino.h>
#include <Stabilizer.h>
#include <WebServer.h>
#include <DeadReckoning.h>
#include <Global.h>

// Setup classes
DCMotor motorR(false, false, 5, DCR_ENCA, DCR_ENCB, 1, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);
DCMotor motorL(false, false, 5, DCL_ENCA, DCL_ENCB, 1, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);

Stabilizer stabilizer;
DeadReckoning deadReckoning;

double SpeedX;
double SpeedY;

double LocationX;
double LocationY;

bool LocationMode = false;

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
  };
  
  //If it asks for something weird for some stupid reason
  return 0.0;
};

void ChangeCallback(double *paramValue, char subtype)
{
  switch (subtype)
  {
  case 'x':
    // rotate
    if (!LocationMode)
    {
      break;
    }

    SpeedX = (*paramValue) / 1000.0 * (-6000);

    stabilizer.SetExtraEngineSpeed(SpeedY - SpeedX, SpeedY + SpeedX);

    break;
  case 'y':
    // forward / backward
    if (!LocationMode)
    {
      break;
    }

    SpeedY = (*paramValue) / 1000.0 * (-6000);

    stabilizer.SetExtraEngineSpeed(SpeedY - SpeedX, SpeedY + SpeedX);

    break;
  case 'a':
    // location x
    LocationX = *paramValue / 1000.0;

    deadReckoning.setTarget(LocationX, LocationY);
    break;
  case 'b':
    // location y
    LocationY = *paramValue / 1000.0;

    deadReckoning.setTarget(LocationX, LocationY);
    break;
  case 'l':
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
    break;
  };
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

  deadReckoning.init(&motorR, &motorL, MoveCallback, WHEELRADIUS, b, DT);

  motorR.init(KI, KD, KP);
  motorL.init(KI, KD, KP);

  stabilizer.init(&motorR, &motorL);

  init_web("IOT_NET", "esp32esp", ChangeCallback, UpdateCallback);
};

void loop()
{
  // put your main code here, to run repeatedly:
  log_i("Angle: %f", stabilizer.current_angle);
  delay(100);
};