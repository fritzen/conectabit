#include "LMotorController.h"
#include "Arduino.h"

void runMotor(uint8_t  speedPin, uint8_t  dirPin, bool reverse, int speed) {
  if (reverse) {
    analogWrite(speedPin, 255 - speed);    
  } else {
    analogWrite(speedPin, speed);  
  }
  
  digitalWrite(dirPin, reverse);
}
//L9110S pwm - PWM signal to input IA to control the motor speed and a digital output to input IB to control its direction.
//LMotorController::LMotorController(int ena, int in1, int enb, int in3, double motorAConst, double motorBConst)
LMotorController::LMotorController(int A_IA_PWM, int A_IB, int B_IA_PWM, int B_IB, double motorAConst, double motorBConst)
{

    _motorAConst = motorAConst;
    _motorBConst = motorBConst;
    //_ena - _enb = A-IA / B-IA (A-1 OU B-1)
	_A_IA_PWM = A_IA_PWM;
	_A_IB = A_IB;

	_B_IA_PWM = B_IA_PWM;
	_B_IB = B_IB;

	pinMode(_A_IA_PWM, OUTPUT);
	pinMode(_A_IB, OUTPUT);

	pinMode(_B_IA_PWM, OUTPUT);
	pinMode(_B_IB, OUTPUT);

}


void LMotorController::move(int leftSpeed, int rightSpeed, int minAbsSpeed)
{
    if (rightSpeed < 0)
    {
        rightSpeed = min(rightSpeed, -1*minAbsSpeed);
        rightSpeed = max(rightSpeed, -255);
    }
    else if (rightSpeed > 0)
    {
        rightSpeed = max(rightSpeed, minAbsSpeed);
        rightSpeed = min(rightSpeed, 255);
    }
    
    int realRightSpeed = map(abs(rightSpeed), 0, 255, minAbsSpeed, 255);

    if (leftSpeed < 0)
    {
        leftSpeed = min(leftSpeed, -1*minAbsSpeed);
        leftSpeed = max(leftSpeed, -255);
    }
    else if (leftSpeed > 0)
    {
        leftSpeed = max(leftSpeed, minAbsSpeed);
        leftSpeed = min(leftSpeed, 255);
    }
    
    int realLeftSpeed = map(abs(leftSpeed), 0, 255, minAbsSpeed, 255);
    
    // digitalWrite(_in3, rightSpeed > 0 ? HIGH : LOW);
    // digitalWrite(_in1, leftSpeed > 0 ? HIGH : LOW);
    // analogWrite(_ena, realRightSpeed * _motorAConst);
    // analogWrite(_enb, realLeftSpeed * _motorBConst);


    runMotor(_A_IA_PWM, _A_IB, rightSpeed > 0 ? HIGH : LOW, abs(realRightSpeed) * _motorAConst);
    runMotor(_B_IA_PWM, _B_IB, leftSpeed > 0 ? HIGH : LOW, abs(realLeftSpeed) * _motorBConst);
}


void LMotorController::move(int speed, int minAbsSpeed)
{
    int direction = 1;
    
    if (speed < 0)
    {
        direction = -1;
        
        speed = min(speed, -1*minAbsSpeed);
        speed = max(speed, -255);
    }
    else
    {
        speed = max(speed, minAbsSpeed);
        speed = min(speed, 255);
    }
    
    if (speed == _currentSpeed) return;
    
    int realSpeed = max(minAbsSpeed, abs(speed));
    
    // digitalWrite(_in1, speed > 0 ? HIGH : LOW);
    // digitalWrite(_in3, speed > 0 ? HIGH : LOW);
    // analogWrite(_ena, realSpeed * _motorAConst);
    // analogWrite(_enb, realSpeed * _motorBConst);
    

    runMotor(_A_IA_PWM, _A_IB, speed > 0 ? HIGH : LOW, abs(realSpeed) * _motorAConst);
    runMotor(_B_IA_PWM, _B_IB, speed > 0 ? HIGH : LOW, abs(realSpeed) * _motorBConst);


    _currentSpeed = direction * realSpeed;
}


void LMotorController::move(int speed)
{
    if (speed == _currentSpeed) return;
    
    if (speed > 255) speed = 255;
    else if (speed < -255) speed = -255;
    
    // digitalWrite(_in1, speed > 0 ? HIGH : LOW);
    // digitalWrite(_in3, speed > 0 ? HIGH : LOW);
    // analogWrite(_ena, abs(speed) * _motorAConst);
    // analogWrite(_enb, abs(speed) * _motorBConst);
    

    runMotor(_A_IA_PWM, _A_IB, speed > 0 ? HIGH : LOW, abs(speed) * _motorAConst);
    runMotor(_B_IA_PWM, _B_IB, speed > 0 ? HIGH : LOW, abs(speed) * _motorBConst);

    _currentSpeed = speed;
}


void LMotorController::turnLeft(int speed, bool kick)
{
    // digitalWrite(_in1, HIGH);
    // digitalWrite(_in3, LOW);

    
    // if (kick)
    // {
    //     analogWrite(_ena, 255);
    //     analogWrite(_enb, 255);
    
    //     delay(100);
    // }
    
    // analogWrite(_ena, speed * _motorAConst);
    // analogWrite(_enb, speed * _motorBConst);

    runMotor(_A_IA_PWM, _A_IB, true, speed * _motorAConst);
    runMotor(_B_IA_PWM, _B_IB, false, speed * _motorAConst);
}


void LMotorController::turnRight(int speed, bool kick)
{
    // digitalWrite(_in1, LOW);
    // digitalWrite(_in3, HIGH);

 
    // if (kick)
    // {
    //     analogWrite(_ena, 255);
    //     analogWrite(_enb, 255);
    
    //     delay(100);
    // }
    
    // analogWrite(_ena, speed * _motorAConst);
    // analogWrite(_enb, speed * _motorBConst);


    runMotor(_A_IA_PWM, _A_IB, false, speed * _motorAConst);
    runMotor(_B_IA_PWM, _B_IB, true, speed * _motorAConst);

}


void LMotorController::stopMoving()
{
    // digitalWrite(_in1, LOW);
    // digitalWrite(_in3, LOW);
    // digitalWrite(_ena, HIGH);
    // digitalWrite(_enb, HIGH);
    runMotor(_A_IA_PWM, _A_IB, false, 0);
    runMotor(_B_IA_PWM, _B_IB, false, 0);

    _currentSpeed = 0;
}



