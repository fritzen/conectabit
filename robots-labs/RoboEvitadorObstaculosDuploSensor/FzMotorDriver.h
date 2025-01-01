 /* ---------------------------------------------------------------------------------------
 * Autor / Author: Eduardo Fritzen eduardo@fritzen.io
 * Não é permitida o uso para fins comerciais deste código sem a autrorização por escrito do autor
 * Use of this code for commercial purposes is not permitted without written authorization from the author
 * ---------------------------------------------------------------------------------------
 */
#ifndef FzMotorDriver_h
#define FzMotorDriver_h
 
#include <Arduino.h>
#include <stdio.h>
#include <inttypes.h>

#include <Servo.h>

#define DEFAULT_SPEED 125 // até 255
#define SERVO_STOP 90
#define SERVO_MAX 120
#define SERVO_MIN 60


//L293D - FECHADO DE ACORDO COM A PLACA SHIELD  
#define DEFAULT_MOTOR_PIN_1 10 //PWM
#define DEFAULT_MOTOR_PIN_2 12

#define DEFAULT_MOTOR_PIN_3 11 //PWM
#define DEFAULT_MOTOR_PIN_4 13

//declaracao dos pinos utilizados para controlar a velocidade de rotacao
#define DEFAULT_MOTOR_ENA_PIN 5
#define DEFAULT_MOTOR_ENB_PIN 6


// Servo
#define DEFAULT_SERVO_MOTOR_A_PIN 10
#define DEFAULT_SERVO_MOTOR_B_PIN 12


#define SENSOR_THRESHOLD 400

enum TipoMotor {
    L9110,
    L293,
    SERVO
};


class FzMotorDriver
{
    public:
        

        FzMotorDriver();
        void forward();
        void backward();
        void turnRight();
        void turnLeft();
        void turnRightAxis();
        void turnLeftAxis();
        void stop();
        void test();
        
        void attach(TipoMotor _tipo);
        void attach(uint8_t pinServo1, uint8_t pinServo2);
        void attach(uint8_t pin1MA, uint8_t pinPwmA, uint8_t pin1MB, uint8_t pinPwmB);
        void attach(uint8_t pin1A, uint8_t pin2A, uint8_t pin1B, uint8_t pin2B, uint8_t pinENMA, uint8_t pinENMB);
        

        void setSpeed(uint16_t speed1, uint16_t speed2);
        
 
    private:
         
        Servo _servoA;
        Servo _servoB;

        uint8_t _pin1MA;
        uint8_t _pin2MA; //pode não ter
        uint8_t _pin1MB;
        uint8_t _pin2MB; //pode não ter

        uint8_t _pinPwmA; //ENMA
        uint8_t _pinPwmB; //ENMB

        uint8_t _pinServoA;
        uint8_t _pinServoB;

        uint16_t _speedA = 255;
        uint16_t _speedB = 255;

        TipoMotor _tipo = L9110;

};
 
#endif