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

// L293D - Pinos padrão para IN1, IN2, IN3, IN4
#define DEFAULT_MOTOR_IN1 4  // Motor A - Input 1
#define DEFAULT_MOTOR_IN2 5  // Motor A - Input 2
#define DEFAULT_MOTOR_IN3 6  // Motor B - Input 3
#define DEFAULT_MOTOR_IN4 7  // Motor B - Input 4

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
        
        // Novo método attach simplificado para L293D
        void attach(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
 
    private:
        uint8_t _pin1MA; // IN1
        uint8_t _pin2MA; // IN2
        uint8_t _pin1MB; // IN3
        uint8_t _pin2MB; // IN4
};
 
#endif