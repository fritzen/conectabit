#include "FioMotorDriver.h"
#include <Arduino.h>

// Implementação para ServoMotorDriver
ServoMotorDriver::ServoMotorDriver(int leftPin, int rightPin) {
    this->leftPin = leftPin;
    this->rightPin = rightPin;
}

void ServoMotorDriver::setup() {
    leftServo.attach(leftPin);
    rightServo.attach(rightPin);
}

void ServoMotorDriver::forward() {
    setLeftAngle(90);  // Exemplo de movimento forward
    setRightAngle(90); // Exemplo de movimento forward
}

void ServoMotorDriver::backward() {
    setLeftAngle(0);   // Exemplo de movimento backward
    setRightAngle(0);  // Exemplo de movimento backward
}

void ServoMotorDriver::left() {
    setLeftAngle(90);  // Parar o motor esquerdo
    setRightAngle(135); // Exemplo de movimento para virar à direita
}

void ServoMotorDriver::right() {
    setLeftAngle(45);  // Exemplo de movimento para virar à esquerda
    setRightAngle(90); // Parar o motor direito
}

void ServoMotorDriver::stop() {
    setLeftAngle(90);  // Exemplo de posição neutra
    setRightAngle(90); // Exemplo de posição neutra
}

void ServoMotorDriver::leftAxle() {
    setLeftAngle(0);   // Exemplo de movimento backward
    setRightAngle(90); // Exemplo de movimento forward
}

void ServoMotorDriver::rightAxle() {
    setLeftAngle(90);  // Exemplo de movimento forward
    setRightAngle(0);  // Exemplo de movimento backward
}

void ServoMotorDriver::setLeftAngle(int angle) {
    leftServo.write(angle);
}

void ServoMotorDriver::setRightAngle(int angle) {
    rightServo.write(angle);
}

// Implementação para L293MotorDriver
L293MotorDriver::L293MotorDriver(int leftEnablePin, int leftInput1Pin, int leftInput2Pin, int rightEnablePin, int rightInput1Pin, int rightInput2Pin) {
    this->leftEnablePin = leftEnablePin;
    this->leftInput1Pin = leftInput1Pin;
    this->leftInput2Pin = leftInput2Pin;
    this->rightEnablePin = rightEnablePin;
    this->rightInput1Pin = rightInput1Pin;
    this->rightInput2Pin = rightInput2Pin;
}

void L293MotorDriver::setup() {
    pinMode(leftEnablePin, OUTPUT);
    pinMode(leftInput1Pin, OUTPUT);
    pinMode(leftInput2Pin, OUTPUT);
    pinMode(rightEnablePin, OUTPUT);
    pinMode(rightInput1Pin, OUTPUT);
    pinMode(rightInput2Pin, OUTPUT);
}

void L293MotorDriver::forward() {
    digitalWrite(leftInput1Pin, HIGH);
    digitalWrite(leftInput2Pin, LOW);
    analogWrite(leftEnablePin, 255); // Full speed

    digitalWrite(rightInput1Pin, HIGH);
    digitalWrite(rightInput2Pin, LOW);
    analogWrite(rightEnablePin, 255); // Full speed
}

void L293MotorDriver::backward() {
    digitalWrite(leftInput1Pin, LOW);
    digitalWrite(leftInput2Pin, HIGH);
    analogWrite(leftEnablePin, 255); // Full speed

    digitalWrite(rightInput1Pin, LOW);
    digitalWrite(rightInput2Pin, HIGH);
    analogWrite(rightEnablePin, 255); // Full speed
}

void L293MotorDriver::left() {
    digitalWrite(leftInput1Pin, LOW);
    digitalWrite(leftInput2Pin, LOW);
    analogWrite(leftEnablePin, 0); // Parar o motor esquerdo

    digitalWrite(rightInput1Pin, HIGH);
    digitalWrite(rightInput2Pin, LOW);
    analogWrite(rightEnablePin, 255); // Full speed
}

void L293MotorDriver::right() {
    digitalWrite(leftInput1Pin, HIGH);
    digitalWrite(leftInput2Pin, LOW);
    analogWrite(leftEnablePin, 255); // Full speed

    digitalWrite(rightInput1Pin, LOW);
    digitalWrite(rightInput2Pin, LOW);
    analogWrite(rightEnablePin, 0); // Parar o motor direito
}

void L293MotorDriver::leftAxle() {
    digitalWrite(leftInput1Pin, LOW);
    digitalWrite(leftInput2Pin, HIGH);
    analogWrite(leftEnablePin, 255); // Full speed

    digitalWrite(rightInput1Pin, HIGH);
    digitalWrite(rightInput2Pin, LOW);
    analogWrite(rightEnablePin, 255); // Full speed
}

void L293MotorDriver::rightAxle() {
    digitalWrite(leftInput1Pin, HIGH);
    digitalWrite(leftInput2Pin, LOW);
    analogWrite(leftEnablePin, 255); // Full speed

    digitalWrite(rightInput1Pin, LOW);
    digitalWrite(rightInput2Pin, HIGH);
    analogWrite(rightEnablePin, 255); // Full speed
}

void L293MotorDriver::stop() {
    digitalWrite(leftInput1Pin, LOW);
    digitalWrite(leftInput2Pin, LOW);
    analogWrite(leftEnablePin, 0);

    digitalWrite(rightInput1Pin, LOW);
    digitalWrite(rightInput2Pin, LOW);
    analogWrite(rightEnablePin, 0);
}
