#include <Global.h>
#include <Arduino.h>
#include <Stabilizer.h>
#include <WebServer.h>
#include <DeadReckoning.h>
#include <PulsingLed.h>
#include <DCMotor.h>
#include <Buzzer.h>

// Setup classes
DCMotor motorL(DCR_ENCB, DCR_ENCA, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, DCMotorSpeed, CtrlMin, CtrlMax, ImpulsesPerRotation, DCMotorSpeed);
DCMotor motorR(DCL_ENCA, DCL_ENCB, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, DCMotorSpeed, CtrlMin, CtrlMax, ImpulsesPerRotation, EncoderFullRotation);

PulsingLed led(8, 8, 19500, POSITIONMODE_LED_PIN, 0.25);

Stabilizer stabilizer;
DeadReckoning deadReckoning;
Buzzer angleBuzzer;

double UpdateCallback(char subtype)
{
  DeadReckoningData data = deadReckoning.getData();

  if (subtype == 'f')
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
  if (subtype == 'y'){
    // forward / backward
    stabilizer.SetExtraAngle((*paramValue) / res * (1));
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
  pinMode(TestOutputPin, OUTPUT);

  //Start DC motors (Tracking encoders etc.)
  motorR.init(MotorKI, 0, MotorKP);
  motorL.init(MotorKI, 0, MotorKP);

  //Start Dead Reckoning Task
  deadReckoning.init(&motorR, &motorL, WHEELRADIUS, WHEELB, DeadReckoningSpeed);

  //Start Stabilizer for Robot
  stabilizer.init(&motorR, &motorL, MotorKD, MotorK);

  //Start the angle buzzer
  angleBuzzer.init(BUZZER_PIN, BUZZER_PWM_CH, Buzzer_Freq_Max, Buzzer_Freq_Min, &stabilizer);

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

  //DeadReckoningData location = deadReckoning.getData();

  //log_i("X: %f", location.X);
  //log_i("Y: %f", location.Y);

  //log_i("Encoder Position Right: %f", motorR.getEncoderPosition() / (ImpulsesPerRotation * DCEncoderGearing));
  //log_i("Encoder Position Left: %f", motorL.getEncoderPosition() / (ImpulsesPerRotation * DCEncoderGearing));

  delay(2000);
};