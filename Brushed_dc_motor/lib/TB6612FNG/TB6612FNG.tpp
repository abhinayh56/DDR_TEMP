#include "TB6612FNG.h"

template <typename T>
TB6612FNG<T>::TB6612FNG()
{
}

template <typename T>
TB6612FNG<T>::~TB6612FNG()
{
}

template <typename T>
void TB6612FNG<T>::config(uint8_t pinStandby, T voltInDriver, uint16_t pwmMax)
{
  m_pinStandby = pinStandby;
  m_voltInDriver = voltInDriver;
  m_pwmMax = pwmMax;
}

template <typename T>
void TB6612FNG<T>::configMotorA(uint8_t pinPWM_A, uint8_t pinINA_1, uint8_t pinINA_2, bool reverse)
{
  m_pinPWM_A = pinPWM_A;
  m_reverseMotorA = reverse;
  m_pinINA_1 = pinINA_1;
  m_pinINA_2 = pinINA_2;
}

template <typename T>
void TB6612FNG<T>::configMotorB(uint8_t pinPWM_B, uint8_t pinINB_1, uint8_t pinINB_2, bool reverse)
{
  m_pinPWM_B = pinPWM_B;
  m_reverseMotorB = reverse;
  m_pinINB_1 = pinINB_1;
  m_pinINB_2 = pinINB_2;
}

template <typename T>
void TB6612FNG<T>::init()
{
  digitalWrite(m_pinStandby, HIGH);
}

template <typename T>
void TB6612FNG<T>::initMotorA()
{
  pinMode(m_pinPWM_A, OUTPUT);
  pinMode(m_pinINA_1, OUTPUT);
  pinMode(m_pinINA_2, OUTPUT);
}

template <typename T>
void TB6612FNG<T>::initMotorB()
{
  pinMode(m_pinPWM_B, OUTPUT);
  pinMode(m_pinINB_1, OUTPUT);
  pinMode(m_pinINB_2, OUTPUT);
}

template <typename T>
void TB6612FNG<T>::disableMotorA()
{
  digitalWrite(m_pinINA_1, LOW);
  digitalWrite(m_pinINA_2, LOW);
  analogWrite(m_pinPWM_A, 0);
}

template <typename T>
void TB6612FNG<T>::disableMotorB()
{
  digitalWrite(m_pinINB_1, LOW);
  digitalWrite(m_pinINB_2, LOW);
  analogWrite(m_pinPWM_B, 0);
}

template <typename T>
void TB6612FNG<T>::enableMotorA()
{
  digitalWrite(m_pinINA_1, HIGH);
  digitalWrite(m_pinINA_2, LOW);
  analogWrite(m_pinPWM_A, 0);
}

template <typename T>
void TB6612FNG<T>::enableMotorB()
{
  digitalWrite(m_pinINB_1, HIGH);
  digitalWrite(m_pinINB_2, LOW);
  analogWrite(m_pinPWM_B, 0);
}

template <typename T>
void TB6612FNG<T>::commandMotorA(T voltInMotorA)
{
  voltInMotorA = voltInMotorA * ((m_reverseMotorA == true) ? -1 : 1);
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

  analogWrite(m_pinPWM_A, constrain(static_cast<int>(voltInMotorA * static_cast<T>(m_pwmMax) / m_voltInDriver), static_cast<int>(0), static_cast<int>(m_pwmMax)));
}

template <typename T>
void TB6612FNG<T>::commandMotorB(T voltInMotorB)
{
  voltInMotorB = voltInMotorB * ((m_reverseMotorB == true) ? -1 : 1);
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
  
  analogWrite(m_pinPWM_B, constrain(static_cast<int>(voltInMotorB * static_cast<T>(m_pwmMax) / m_voltInDriver), static_cast<int>(0), static_cast<int>(m_pwmMax)));
}

template <typename T>
void TB6612FNG<T>::eStopDrive()
{
  digitalWrite(m_pinStandby, LOW);

  digitalWrite(m_pinINA_1, LOW);
  digitalWrite(m_pinINA_2, LOW);
  analogWrite(m_pinPWM_A, 0);
  digitalWrite(m_pinINB_1, LOW);
  digitalWrite(m_pinINB_2, LOW);
  analogWrite(m_pinPWM_B, 0);
}

template <typename T>
void TB6612FNG<T>::enableDrive()
{
  digitalWrite(m_pinStandby, HIGH);

  digitalWrite(m_pinINA_1, LOW);
  digitalWrite(m_pinINA_2, LOW);
  analogWrite(m_pinPWM_A, 0);
  digitalWrite(m_pinINB_1, LOW);
  digitalWrite(m_pinINB_2, LOW);
  analogWrite(m_pinPWM_B, 0);
}
