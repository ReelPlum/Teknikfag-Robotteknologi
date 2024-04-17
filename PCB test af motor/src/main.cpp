#include <Arduino.h>
#include <DCMotor.h>
#include <Global.h>

DCMotor motorR(false, true, 5, DCR_ENCA, DCR_ENCB, 1, DCR_INA, DCR_INB, DCR_PWM, DCR_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);
DCMotor motorL(false, true, 5, DCL_ENCA, DCL_ENCB, 1, DCL_INA, DCL_INB, DCL_PWM, DCL_PWMCH, PWM_Freq, PWM_Res, DT, PID_CtrlMax, CtrlMin, CtrlMax, MaxVel, IntegrationThreshold, ImpulsesPerRotation);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial)
    ;

  log_i("hej");

  motorR.init(KI, KD, KP);
  motorL.init(KI, KD, KP);

  motorR.set_velocity(-1000);
  motorL.set_velocity(-1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //motorR.set_velocity(500);
  //motorL.set_velocity(500);

}
