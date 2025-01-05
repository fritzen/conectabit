 /* ---------------------------------------------------------------------------------------
 * Autor / Author: Eduardo Fritzen - eduardo@fritzen.io
 * Não é permitido o uso para fins comerciais deste código sem a autorização por escrito do autor
 * Use of this code for commercial purposes is not permitted without written authorization from the author
 * ---------------------------------------------------------------------------------------
 */
#include "FzMotorDriver.h"

#include <Arduino.h>

FzMotorDriver::FzMotorDriver()
{

}


 void FzMotorDriver::attach(TipoMotor tipo)
 {
      _tipo = tipo;

      _pinPwmA = DEFAULT_MOTOR_PIN_1; //pwm
      _pin1MA = DEFAULT_MOTOR_PIN_2; // direction

      _pinPwmB = DEFAULT_MOTOR_PIN_3; //pwm
      _pin1MB = DEFAULT_MOTOR_PIN_4; // direction
           
      
      pinMode(_pin1MA, OUTPUT);
      pinMode(_pin1MB, OUTPUT);
      pinMode(_pinPwmA, OUTPUT);
      pinMode(_pinPwmB, OUTPUT);

      setSpeed(DEFAULT_SPEED, DEFAULT_SPEED);
 }


void FzMotorDriver::attach(uint8_t pin1MA, uint8_t pinPwmA, uint8_t pin1MB, uint8_t pinPwmB)
{
      _tipo = L9110;

      _pin1MA = pin1MA; // direction
      _pin1MB = pin1MB; // direction
      _pinPwmA = pinPwmA; //pwm
      _pinPwmB = pinPwmB; //pwm
      
      pinMode(_pin1MA, OUTPUT);
      pinMode(_pin1MB, OUTPUT);
      pinMode(_pinPwmA, OUTPUT);
      pinMode(_pinPwmB, OUTPUT);

      setSpeed(DEFAULT_SPEED, DEFAULT_SPEED);
}

void FzMotorDriver::attach(uint8_t pin1MA, uint8_t pin2MA, uint8_t pin1MB, uint8_t pin2MB, uint8_t pinPwmA, uint8_t pinPwmB)
{
  
  _tipo = L293;

  _pin1MA = pin1MA; //direction
  _pin2MA = pin2MA; //direction
  _pin1MB = pin1MB; //direction
  _pin2MB = pin2MB; //direction
  _pinPwmA = pinPwmA; //pwm
  _pinPwmB = pinPwmB; //pwm
  
  pinMode(_pin1MA, OUTPUT);
  pinMode(_pin2MA, OUTPUT);
  pinMode(_pin1MB, OUTPUT);
  pinMode(_pin2MB, OUTPUT);
  pinMode(_pinPwmA, OUTPUT);
  pinMode(_pinPwmB, OUTPUT);

  setSpeed(DEFAULT_SPEED, DEFAULT_SPEED);
}

void FzMotorDriver::attach(uint8_t pinServo1, uint8_t pinServo2)
{

    _tipo = SERVO;

    _pinServoA = pinServo1;
    _pinServoB = pinServo2;    
    

    _servoA.detach();
    _servoB.detach();
    pinMode(_pinServoA, OUTPUT);
    pinMode(_pinServoB, OUTPUT);  
    
    _servoA.attach(_pinServoA);
    _servoB.attach(_pinServoB);
}

void FzMotorDriver::test(){
      int pos = 0; 
            for (pos = 0; pos <= 180; pos += 1) {
          _servoA.write(pos);
         _servoB.write(pos);
          delay(15);
        }
        for (pos = 180; pos >= 0; pos -= 1) {
          _servoA.write(pos);
         _servoB.write(pos);
          delay(15);
        }
}

void FzMotorDriver::setSpeed(uint16_t pwm1, uint16_t pwm2) {
      _speedA = pwm1;
      _speedB = pwm2;
      analogWrite(_pinPwmA, _speedA);
      analogWrite(_pinPwmB, _speedB);
}

void FzMotorDriver::backward()
{
  if (_tipo == SERVO) {
    _servoA.write(SERVO_MIN);
    _servoB.write(SERVO_MAX);      
  } else {


    digitalWrite(_pin1MA, HIGH); // Direção para a frente no motor A
    digitalWrite(_pin1MB, HIGH); // Direção para a frente no motor B

    analogWrite(_pinPwmA, _speedA);
    analogWrite(_pinPwmB, _speedB);

    if (_tipo == L293) {
      digitalWrite(_pin2MA, LOW);
      digitalWrite(_pin2MB, LOW);
    }

  }  
}
void FzMotorDriver::forward()
{
  if (_tipo == SERVO) {
    _servoA.write(SERVO_MAX);
    _servoB.write(SERVO_MIN);
  } else {
        digitalWrite(_pin1MA, LOW); // Direção para trás no motor A
        digitalWrite(_pin1MB, LOW); // Direção para trás no motor B

        analogWrite(_pinPwmA, _speedA);
        analogWrite(_pinPwmB, _speedB);

        if (_tipo == L293) {
          digitalWrite(_pin2MA, HIGH);
          digitalWrite(_pin2MB, HIGH);
        }
  }
}

void FzMotorDriver::turnRight()
{
  if (_tipo == SERVO) {
    _servoA.write(SERVO_MIN);
    _servoB.write(SERVO_STOP);
  } else {
        digitalWrite(_pin1MA, LOW);        
        digitalWrite(_pin1MB, HIGH);

        analogWrite(_pinPwmA, _speedA);
        analogWrite(_pinPwmB, 0);// Desliga o motor B

        if (_tipo == L293) {
          digitalWrite(_pin2MA, LOW);
          digitalWrite(_pin2MB, LOW);
        }

  }
}

void FzMotorDriver::turnRightAxis() 
{ 
  if (_tipo == SERVO) {
    _servoA.write(SERVO_MAX);
    _servoB.write(SERVO_MAX);
  } else {
        digitalWrite(_pin1MA, LOW);
        digitalWrite(_pin1MB, HIGH);

        analogWrite(_pinPwmA, _speedA);
        analogWrite(_pinPwmB, _speedB);


        if (_tipo == L293) {
          digitalWrite(_pin2MA, HIGH);
          digitalWrite(_pin2MB, LOW);
        }
  }
}

void FzMotorDriver::turnLeft()
{
  if (_tipo == SERVO) {
    _servoA.write(SERVO_STOP);
    _servoB.write(SERVO_MAX);
  } else {
        digitalWrite(_pin1MA, HIGH);        
        digitalWrite(_pin1MB, LOW);

        analogWrite(_pinPwmA, 0); // Desliga o motor A
        analogWrite(_pinPwmB, _speedB);

        if (_tipo == L293) {
          digitalWrite(_pin2MA, LOW);
          digitalWrite(_pin2MB, LOW);
        }

  }
}

void FzMotorDriver::turnLeftAxis() 
{ 
  if (_tipo == SERVO) {
    _servoA.write(SERVO_MIN);
    _servoB.write(SERVO_MIN);
  } else {
        digitalWrite(_pin1MA, HIGH);
        digitalWrite(_pin1MB, LOW);

        analogWrite(_pinPwmA, _speedA);
        analogWrite(_pinPwmB, _speedB);


        if (_tipo == L293) {
          digitalWrite(_pin2MA, LOW);
          digitalWrite(_pin2MB, HIGH);
        }
  }
}

void FzMotorDriver::stop() //PONTO MORTO = TUDO LOW
{
  if (_tipo == SERVO) {
    _servoA.write(SERVO_STOP);
    _servoB.write(SERVO_STOP);
  } else {
      digitalWrite(_pin1MA, HIGH);   
      digitalWrite(_pin1MB, HIGH);

      analogWrite(_pinPwmA, 0);
      analogWrite(_pinPwmB, 0);
      
      if (_tipo == L293) {
        digitalWrite(_pin2MA, HIGH);
        digitalWrite(_pin2MB, HIGH);
      }
  }
}

