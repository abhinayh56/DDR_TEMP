#include "TB6612FNG.h"

TB6612FNG motorDriver;

double voltInMotorA = 0;

void setup()
{
  Serial.begin(115200);
  
  motorDriver.config(7, 12, 255);
  motorDriver.configMotorA(4, 6, 5, false);

  motorDriver.init();
  motorDriver.initMotorA();

  motorDriver.enableMotorA();
}

void loop()
{
  while(Serial.available() > 0)
  {
    voltInMotorA = Serial.readString().toDouble();
  }
  Serial.println(voltInMotorA);

  motorDriver.commandMotorA(voltInMotorA);

  delay(10);
}
