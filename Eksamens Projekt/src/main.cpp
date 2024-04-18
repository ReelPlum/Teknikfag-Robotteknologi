#include <Global.h>
#include <Arduino.h>
#include <Stabilizer.h>
#include <WebServer.h>
#include <DeadReckoning.h>
#include <PulsingLed.h>
#include <DCMotor.h>
#include <Buzzer.h>

// Setup classes
DCMotor motorR(false, false, 5, DCR_ENCB, DCR_ENCA, 1, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, StabilizerSpeed, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);
DCMotor motorL(false, false, 5, DCL_ENCA, DCL_ENCB, 1, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, StabilizerSpeed, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);

PulsingLed led(8, 8, 19500, POSITIONMODE_LED_PIN, 0.25);

Stabilizer stabilizer;
DeadReckoning deadReckoning;
Buzzer angleBuzzer;

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
  else if (subtype == 'e'){
    Pid* anglePID = stabilizer.getPid();
    return anglePID->get_error();
  }
  else if (subtype == 'v'){
    return motorL.getEncoderPosition();
  }
  else if (subtype == 'h'){
    return motorR.getEncoderPosition();
  };
  
  //If it asks for something weird for some stupid reason
  return 0.0;
};

const double res = 1000;

void ChangeCallback(double *paramValue, char subtype)
{

  // if (subtype == 'x'){
  //   // rotate
  //   if (LocationMode)
  //   {
  //     return;
  //   }

  //   SpeedX = (*paramValue) / res * (-6000);

  //   stabilizer.SetExtraEngineSpeed(-SpeedX, SpeedX);

  // }
  else if (subtype == 'y'){
    // forward / backward
    stabilizer.SetExtraAngle((*paramValue) / res * (3));
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
    stabilizer.SetKI((*paramValue)/res);
  };
  if (subtype == 'p'){
    stabilizer.SetKP((*paramValue)/res);
  };
  if (subtype == 't'){
    stabilizer.setWantedAngle((*paramValue)/res);
  }
  if (subtype == 'b'){
    angleBuzzer.toggle();
  }
};

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Start DC motors (Tracking encoders etc.)
  motorR.init(MotorKI, 0, MotorKP);
  motorL.init(MotorKI, 0, MotorKP);

  //Start Dead Reckoning Task
  deadReckoning.init(&motorR, &motorL, WHEELRADIUS, WHEELB, DeadReckoningSpeed);

  //Start Stabilizer for Robot
  stabilizer.init(&motorR, &motorL, MotorKD, MotorK);

  //Start the angle buzzer
  angleBuzzer.init(BUZZER_PIN, BUZZER_PWM_CH, 4200, &stabilizer);

  //Start pulsing LED
  led.init();

  init_web("IOT_NET", "esp32esp", ChangeCallback, UpdateCallback);
};

void loop()
{
  //log_i("Current speed: %f, %f", SpeedX, SpeedY);
  // log_i("Gyrosens %f", stabilizer.getGyroSens());
  // log_i("KI: %f", stabilizer.getKI());
  // log_i("KP: %f", stabilizer.getKP());
  // log_i("K: %f", stabilizer.getK());
  //log_i("CtrlAngle: %f", stabilizer.getCtrlAngle());
  //log_i("Target Angle: %f", stabilizer.getTargetAngle());

  delay(2000);
};