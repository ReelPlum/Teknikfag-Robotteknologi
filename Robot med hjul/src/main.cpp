#include <Arduino.h>
#include <DCMotor.h>


DCMotor motor(false, 17, 26, 27, 1, 18, 19, 23, 1, 19500, 12, 0.1, 4000, -100, 100, 100, 200, 1990);

/*
For at bruge motor classen starter man den med DCMotor motor som set ovenover. Kig ind i files DCMotor.h for at se hvordan den virker.
Der er ikke implementeret positions styring endnu men hvis position_mode er sat til false vil den bare køre (tror jeg)
For at få motoren til at starte kaldes der motor.init();
*/

void setup(){
  Serial.begin(115200);

  log_i("AMEN NU MÅ DU KRAFTÆDMNE!!");
  motor.init();
}

void loop(){
}