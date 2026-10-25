#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <Arduino.h>
#include <stdint.h>

class TB6612FNG
{
public:
  TB6612FNG();

  ~TB6612FNG();

  void config(uint8_t pinStandby, double voltInDriver, uint16_t pwmMax);

  void configMotorA(uint8_t pinPWM_A, uint8_t pinINA_1, uint8_t pinINA_2, bool reverse);

  void configMotorB(uint8_t pinPWM_B, uint8_t pinINB_1, uint8_t pinINB_2, bool reverse);

  void init();

  void initMotorA();

  void initMotorB();

  void disableMotorA();

  void disableMotorB();

  void enableMotorA();

  void enableMotorB();

  void commandMotorA(double voltInMotorA);

  void commandMotorB(double voltInMotorB);

  void eStopDrive();

  void enableDrive();

private:
  uint8_t m_pinPWM_A;
  uint8_t m_pinINA_1;
  uint8_t m_pinINA_2;

  uint8_t m_pinPWM_B;
  uint8_t m_pinINB_1;
  uint8_t m_pinINB_2;

  uint8_t m_pinStandby;

  bool m_reverseMotorA;
  bool m_reverseMotorB;

  double m_voltInDriver;

  uint16_t m_pwmMax;
};

#endif  // TB6612FNG_H