#include "Timer_utils.h"
#include "TB6612FNG.h"
#include "L298N.h"
#include "Quadrature_enc.h"
#include "LPF_1.h"
#include "PID_P.h"

#define MAIN_LOOP_FREQ 250.0

Timer_utils timer(MAIN_LOOP_FREQ);

TB6612FNG motorDriver;
Quadrature_enc encoderMotorA;
Quadrature_enc encoderMotorB;

LPF_1<double> lpf_wMotorA;
LPF_1<double> lpf_wMotorB;
PID_P<double> controllerMotorA;
PID_P<double> controllerMotorB;

double actual_W_motorA_raw  = 0.0;
double actual_W_motorB_raw  = 0.0;
double actual_W_motorA_filt = 0.0;
double actual_W_motorB_filt = 0.0;

double target_W_motorA = 0.0;
double target_W_motorB = 0.0;

double Kp = 1.88;
double Ki = 14.0;
double Kd = 0.005;
double fc = 5.0;
double u_motorA = 0;
double u_motorB = 0;

void setup()
{
  Serial.begin(115200);

  timer.init(MAIN_LOOP_FREQ);

  motorDriver.config(7, 12.0, 255);
  motorDriver.configMotorA(4, 5, 6, false);
  motorDriver.configMotorB(8, 9, 10, true);

  motorDriver.init();
  motorDriver.initMotorA();
  motorDriver.initMotorB();

  motorDriver.enableMotorA();
  motorDriver.enableMotorB();

  encoderMotorA.config(5800, 2, 3, []() { encoderMotorA.isrFunction(); }, true);
  encoderMotorB.config(5800, 18, 19, []() { encoderMotorB.isrFunction(); }, true);
  encoderMotorA.init();
  encoderMotorB.init();

  lpf_wMotorA.config(fc, 1.0/MAIN_LOOP_FREQ);
  lpf_wMotorB.config(fc, 1.0/MAIN_LOOP_FREQ);

  controllerMotorA.init(1.0/MAIN_LOOP_FREQ, Kp, Ki, Kd, 9.4, 12.0, false);
  controllerMotorB.init(1.0/MAIN_LOOP_FREQ, Kp, Ki, Kd, 9.4, 12.0, false);
}

void loop()
{
  encoderMotorA.update();
  encoderMotorB.update();

  actual_W_motorA_raw = encoderMotorA.getAngularSpeed();
  actual_W_motorB_raw = encoderMotorB.getAngularSpeed();

  actual_W_motorA_filt = lpf_wMotorA.update(actual_W_motorA_raw);
  actual_W_motorB_filt = lpf_wMotorB.update(actual_W_motorB_raw);
  
  while (Serial.available() > 0)
  {
    target_W_motorA = Serial.readString().toDouble() * DEG_TO_RAD;
    target_W_motorB = target_W_motorA;
  }
  
  u_motorA = controllerMotorA.update(target_W_motorA, actual_W_motorA_filt);
  u_motorB = controllerMotorB.update(target_W_motorB, actual_W_motorB_filt);

  motorDriver.commandMotorA(u_motorA);
  motorDriver.commandMotorB(u_motorB);

  // Serial.print(target_W_motorB * RAD_TO_DEG);
  // Serial.print(",");
  // Serial.print(actual_W_motorB_filt * RAD_TO_DEG);
  // Serial.print(",");
  // Serial.print(u_motorB);
  // Serial.print(",");
  // Serial.println(0);

  timer.sleep();
}
