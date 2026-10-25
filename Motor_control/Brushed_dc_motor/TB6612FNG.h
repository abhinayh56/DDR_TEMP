#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <Arduino.h>
#include <stdint.h>

class TB6612FNG {
public:
  TB6612FNG()
  {
  }

  ~TB6612FNG()
  {
  }

  void config(uint8_t pinStandby, double voltInDriver, uint16_t pwmMax)
  {
    m_pinStandby = pinStandby;
    m_voltInDriver = voltInDriver;
    m_pwmMax = pwmMax;
  }

  void configMotorA(uint8_t pinPWM_A, uint8_t pinINA_1, uint8_t pinINA_2, bool reverse)
  {
    m_pinPWM_A = pinPWM_A;
    m_reverseMotorA = reverse;

    if(reverse)
    {
      m_pinINA_1 = pinINA_2;
      m_pinINA_2 = pinINA_1;
    }
    else
    {
      m_pinINA_1 = pinINA_1;
      m_pinINA_2 = pinINA_2;
    }
  }

  void configMotorB(uint8_t pinPWM_B, uint8_t pinINB_1, uint8_t pinINB_2, bool reverse)
  {
    m_pinPWM_B = pinPWM_B;
    m_reverseMotorB = reverse;

    if(reverse)
    {
      m_pinINB_1 = pinINB_2;
      m_pinINB_2 = pinINB_1;
    }
    else
    {
      m_pinINB_1 = pinINB_1;
      m_pinINB_2 = pinINB_2;
    }
  }

  void init()
  {
    digitalWrite(m_pinStandby, HIGH);
  }

  void initMotorA()
  {
    pinMode(m_pinPWM_A, OUTPUT);
    pinMode(m_pinINA_1, OUTPUT);
    pinMode(m_pinINA_2, OUTPUT);
  }

  void initMotorB()
  {
    pinMode(m_pinPWM_B, OUTPUT);
    pinMode(m_pinINB_1, OUTPUT);
    pinMode(m_pinINB_2, OUTPUT);
  }

  void disableMotorA()
  {
    digitalWrite(m_pinINA_1, LOW);
    digitalWrite(m_pinINA_2, LOW);
    analogWrite(m_pinPWM_A, 0);
  }

  void disableMotorB()
  {
    digitalWrite(m_pinINB_1, LOW);
    digitalWrite(m_pinINB_2, LOW);
    analogWrite(m_pinPWM_B, 0);
  }

  void enableMotorA()
  {
    digitalWrite(m_pinINA_1, HIGH);
    digitalWrite(m_pinINA_2, HIGH);
    analogWrite(m_pinPWM_A, 0);
  }

  void enableMotorB()
  {
    digitalWrite(m_pinINB_1, HIGH);
    digitalWrite(m_pinINB_2, HIGH);
    analogWrite(m_pinPWM_B, 0);
  }

  void commandMotorA(double voltInMotorA)
  {
    voltInMotorA = constrain(voltInMotorA, -m_voltInDriver, m_voltInDriver);

    if(voltInMotorA < 0)
    {
      voltInMotorA = -voltInMotorA;
      digitalWrite(m_pinINA_1, HIGH);
      digitalWrite(m_pinINA_2, LOW);
    }
    else
    {
      digitalWrite(m_pinINA_1, LOW);
      digitalWrite(m_pinINA_2, HIGH);
    }

    analogWrite(m_pinPWM_A, constrain(static_cast<int>(voltInMotorA * static_cast<double>(m_pwmMax) / m_voltInDriver), 0, m_pwmMax));
  }

  void commandMotorB(double voltInMotorB)
  {
    voltInMotorB = constrain(voltInMotorB, -m_voltInDriver, m_voltInDriver);

    if(voltInMotorB < 0)
    {
      voltInMotorB = -voltInMotorB;
      digitalWrite(m_pinINB_1, HIGH);
      digitalWrite(m_pinINB_2, LOW);
    }
    else
    {
      digitalWrite(m_pinINB_1, LOW);
      digitalWrite(m_pinINB_2, HIGH);
    }
    
    analogWrite(m_pinPWM_B, constrain(static_cast<int>(voltInMotorB * static_cast<double>(m_pwmMax) / m_voltInDriver), 0, m_pwmMax));
  }

  void eStopDrive()
  {
    digitalWrite(m_pinStandby, LOW);

    digitalWrite(m_pinINA_1, LOW);
    digitalWrite(m_pinINA_2, LOW);
    analogWrite(m_pinPWM_A, 0);
    digitalWrite(m_pinINB_1, LOW);
    digitalWrite(m_pinINB_2, LOW);
    analogWrite(m_pinPWM_B, 0);
  }

  void enableDrive()
  {
    digitalWrite(m_pinStandby, HIGH);

    digitalWrite(m_pinINA_1, LOW);
    digitalWrite(m_pinINA_2, LOW);
    analogWrite(m_pinPWM_A, 0);
    digitalWrite(m_pinINB_1, LOW);
    digitalWrite(m_pinINB_2, LOW);
    analogWrite(m_pinPWM_B, 0);
  }

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