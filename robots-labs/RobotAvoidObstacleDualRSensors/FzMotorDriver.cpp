#include "FzMotorDriver.h"
#include <Arduino.h>

FzMotorDriver::FzMotorDriver() {
}

void FzMotorDriver::attach(uint8_t pin1MA, uint8_t pin2MA, uint8_t pin1MB, uint8_t pin2MB)
{

  // Pinos de direção Motor A
  _pin1MA = pin1MA; // IN1
  _pin2MA = pin2MA; // IN2
  
  // Pinos de direção Motor B
  _pin1MB = pin1MB; // IN3
  _pin2MB = pin2MB; // IN4
  
  pinMode(_pin1MA, OUTPUT);
  pinMode(_pin2MA, OUTPUT);
  pinMode(_pin1MB, OUTPUT);
  pinMode(_pin2MB, OUTPUT);
}

void FzMotorDriver::forward()
{
  // Motor A: IN1=HIGH, IN2=LOW
  digitalWrite(_pin1MA, HIGH);
  digitalWrite(_pin2MA, LOW);
  
  // Motor B: IN3=HIGH, IN4=LOW
  digitalWrite(_pin1MB, HIGH);
  digitalWrite(_pin2MB, LOW);
}

void FzMotorDriver::backward()
{
  // Motor A: IN1=LOW, IN2=HIGH
  digitalWrite(_pin1MA, LOW);
  digitalWrite(_pin2MA, HIGH);
  
  // Motor B: IN3=LOW, IN4=HIGH
  digitalWrite(_pin1MB, LOW);
  digitalWrite(_pin2MB, HIGH);
}

void FzMotorDriver::turnRight()
{
  // Motor A: Forward
  digitalWrite(_pin1MA, HIGH);
  digitalWrite(_pin2MA, LOW);
  
  // Motor B: Stop
  digitalWrite(_pin1MB, LOW);
  digitalWrite(_pin2MB, LOW);
}

void FzMotorDriver::turnRightAxis()
{
  // Motor A: Forward
  digitalWrite(_pin1MA, HIGH);
  digitalWrite(_pin2MA, LOW);
  
  // Motor B: Backward
  digitalWrite(_pin1MB, LOW);
  digitalWrite(_pin2MB, HIGH);
}

void FzMotorDriver::turnLeft()
{
  // Motor A: Stop
  digitalWrite(_pin1MA, LOW);
  digitalWrite(_pin2MA, LOW);
  
  // Motor B: Forward
  digitalWrite(_pin1MB, HIGH);
  digitalWrite(_pin2MB, LOW);
}

void FzMotorDriver::turnLeftAxis()
{
  // Motor A: Backward
  digitalWrite(_pin1MA, LOW);
  digitalWrite(_pin2MA, HIGH);
  
  // Motor B: Forward
  digitalWrite(_pin1MB, HIGH);
  digitalWrite(_pin2MB, LOW);
}

void FzMotorDriver::stop()
{
  // Ambos os motores: IN1=LOW, IN2=LOW
  digitalWrite(_pin1MA, LOW);
  digitalWrite(_pin2MA, LOW);
  digitalWrite(_pin1MB, LOW);
  digitalWrite(_pin2MB, LOW);
}