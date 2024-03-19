#include <Global.h>

#include <DCMotor.h>
#include <DeadReckoning.h>
#include <WebServer.h>

const int32_t WHEELRADIUS = 5;
const double DT = .01;
const double b = 24.5;

double currentX = 0;
double currentY = 0;
double currentAngle = 0;

DCMotor motorR(false, 17, 33, 32, 1, 5, 4, 2, 1, 19500, 12, DT, 4000, -100, 100, 6000, 100000, 1990);
DCMotor motorL(false, 17, 27, 26, 1, 18, 19, 23, 2, 19500, 12, DT, 4000, -100, 100, 6000, 100000, 1990);

DeadReckoning deadReckoning;

double getData(char subtype){
  DeadReckoningData data = deadReckoning.getData();

  if (subtype == 'x')
  {
    return data.X;

  }
  else if (subtype == 'y')
  {
    return data.Y;

  }
  else if (subtype == 'a')
  {
    return data.Angle;

  }
  
  //If it asks for something weird for some stupid reason
  return 0.0;
}

double SpeedX = 0;
double SpeedY = 0;

double CurrX = 0;
double CurrY = 0;

void MoveCallback(double x, double y){
  SpeedX = x * (-6000);
  SpeedY = y * (-6000);

  //stabilizer.SetExtraEngineSpeed(SpeedY - SpeedX, SpeedY + SpeedX);
};

void updateMain(double *paramValue, char subtype)
{
  switch(subtype){
    case 'x':
      //rotate 
      CurrX = (*paramValue)/1000;
      break;
    case 'y':
      //forward
      CurrY = (*paramValue)/1000;

      break;
  };
  //Calculate directions
  //SpeedX = CurrX * (-6000);
  //SpeedY = CurrY * (-6000);
} 

void setup()
{
  Serial.begin(115200);

  delay(100);

  deadReckoning.init(&motorR, &motorL, MoveCallback, WHEELRADIUS, b, DT);

  motorR.init(1, 0, 0.05);
  motorL.init(1, 0, 0.05);

  init_web(updateMain, getData);
}

void loop()
{
  motorR.set_velocity(SpeedY - SpeedX);
  motorL.set_velocity(SpeedY + SpeedX);
}