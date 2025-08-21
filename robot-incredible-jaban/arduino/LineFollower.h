#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include "FioMotorDriver.h"

class LineFollower {
public:
    LineFollower(MotorDriver* motorDriver, int leftSensorPin, int rightSensorPin, int obstacleSensorPin, int leftThreshold, int rightThreshold, int obstacleThreshold);
    void robotSetup();
    void robotLoop();
    void playStartupSound();
    void playObstacleBeep();
    void playLineBeep();

private:
    MotorDriver* motorDriver;
    int leftSensorPin;
    int rightSensorPin;
    int obstacleSensorPin;
    int leftThreshold;
    int rightThreshold;
    int obstacleThreshold;
    int readSensor(int pin);
};

#endif // LINEFOLLOWER_H
