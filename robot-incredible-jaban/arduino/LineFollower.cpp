#include "LineFollower.h"
#include <Arduino.h>
#include "TimerFreeTone.h"

#define BUZZER_PIN 11

LineFollower::LineFollower(MotorDriver* motorDriver, int leftSensorPin, int rightSensorPin, int obstacleSensorPin, int leftThreshold, int rightThreshold, int obstacleThreshold) {
    this->motorDriver = motorDriver;
    this->leftSensorPin = leftSensorPin;
    this->rightSensorPin = rightSensorPin;
    this->obstacleSensorPin = obstacleSensorPin;
    this->leftThreshold = leftThreshold;
    this->rightThreshold = rightThreshold;
    this->obstacleThreshold = obstacleThreshold;
}

void LineFollower::robotSetup() {
    if (motorDriver) {
        motorDriver->setup();
    }
    playStartupSound(); // Toca a música de inicialização
}

void LineFollower::robotLoop() {
    int leftSensorValue = readSensor(leftSensorPin);
    int rightSensorValue = readSensor(rightSensorPin);
    int obstacleSensorValue = readSensor(obstacleSensorPin);

    // Imprime os valores dos sensores no Serial Monitor
    Serial.print("Left Sensor: ");
    Serial.print(leftSensorValue);
    Serial.print(" | Right Sensor: ");
    Serial.print(rightSensorValue);
    Serial.print(" | Obstacle Sensor: ");
    Serial.println(obstacleSensorValue);

    if (obstacleSensorValue < obstacleThreshold) { // Detecta obstáculo
        playObstacleBeep();
        motorDriver->rightAxle(); // Girar para a direita
        delay(500);
        unsigned long startTime = millis();
        while (true) {
            motorDriver->rightAxle(); // Girar para a direita
            leftSensorValue = readSensor(leftSensorPin);
            rightSensorValue = readSensor(rightSensorPin);
            Serial.print("Left Sensor: ");
            Serial.print(leftSensorValue);
            Serial.print(" | Right Sensor: ");
            Serial.println(rightSensorValue);

            if (leftSensorValue > leftThreshold || rightSensorValue > rightThreshold) {
                playLineBeep();
                break; // Linha encontrada
            }
        }
    } else {
       if (leftSensorValue < leftThreshold) { // Somente o sensor esquerdo detectando a linha
            //motorDriver->leftAxle();
            motorDriver->left();
            playLineBeep();
        } else if (rightSensorValue < rightThreshold) { // Somente o sensor direito detectando a linha
            //motorDriver->rightAxle();
            motorDriver->right();
            playLineBeep();
        } else { // Nenhum sensor detectando a linha
             motorDriver->forward();
        }
    }
}

int LineFollower::readSensor(int pin) {
    return analogRead(pin);
}

void LineFollower::playStartupSound() {
    int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
    int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};
    
    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        TimerFreeTone(BUZZER_PIN, melody[thisNote], noteDuration);
        delay(noteDuration * 1.30);
    }
}


void LineFollower::playObstacleBeep() {
    TimerFreeTone(BUZZER_PIN, 1000, 200); // Frequência de 1000 Hz por 200 ms
    delay(200);
}

void LineFollower::playLineBeep() {
    TimerFreeTone(BUZZER_PIN, 500, 100); // Frequência de 500 Hz por 100 ms
    delay(100);
}
