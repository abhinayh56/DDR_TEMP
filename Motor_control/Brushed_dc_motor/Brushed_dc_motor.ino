#include "TB6612FNG.h"

TB6612FNG motorDriver;

double voltInMotorA = 0;
double voltInMotorB = 0;

void setup()
{
  Serial.begin(115200);
  
  motorDriver.config(7, 12, 255);
  motorDriver.configMotorA(4, 6, 5, false);
  motorDriver.configMotorB(8, 9, 10, true);

  motorDriver.init();
  motorDriver.initMotorA();
  motorDriver.initMotorB();

  motorDriver.enableMotorA();
  motorDriver.enableMotorB();
}

void loop()
{
  while(Serial.available() > 0)
  {
    // voltInMotorA = Serial.readString().toDouble();
    voltInMotorB = Serial.readString().toDouble();
  }
  
  Serial.print(voltInMotorA);
  Serial.print("\t");
  Serial.println(voltInMotorB);

  motorDriver.commandMotorA(voltInMotorA);
  motorDriver.commandMotorB(voltInMotorB);

  delay(10);
}
