#ifndef FIOMOTORDRIVER_H
#define FIOMOTORDRIVER_H

#include <Servo.h>

// Classe base abstrata
class MotorDriver {
public:
    virtual void setup() = 0;
    virtual void forward() = 0;
    virtual void backward() = 0;
    virtual void left() = 0;
    virtual void right() = 0;
    virtual void stop() = 0;
    virtual void leftAxle() = 0;
    virtual void rightAxle() = 0;
    virtual ~MotorDriver() {}
};

// Classe derivada para motor servo
class ServoMotorDriver : public MotorDriver {
public:
    ServoMotorDriver(int leftPin, int rightPin);
    void setup() override;
    void forward() override;
    void backward() override;
    void left() override;
    void right() override;
    void stop() override;
    void leftAxle() override;
    void rightAxle() override;
    void setLeftAngle(int angle);
    void setRightAngle(int angle);

private:
    Servo leftServo;
    Servo rightServo;
    int leftPin;
    int rightPin;
};

// Classe derivada para motor L293
class L293MotorDriver : public MotorDriver {
public:
    L293MotorDriver(int leftEnablePin, int leftInput1Pin, int leftInput2Pin, int rightEnablePin, int rightInput1Pin, int rightInput2Pin);
    void setup() override;
    void forward() override;
    void backward() override;
    void left() override;
    void right() override;
    void stop() override;
    void leftAxle() override;
    void rightAxle() override;

private:
    int leftEnablePin;
    int leftInput1Pin;
    int leftInput2Pin;
    int rightEnablePin;
    int rightInput1Pin;
    int rightInput2Pin;
};

#endif // FIOMOTORDRIVER_H
