/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 16 servos, one after the other

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID  300 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID2  330 // this is the 'maximum' pulse length count (out of 4096)

#define SERVO_MAO_ESQUERDA 15
#define SERVO_BRACO_ESQUERDO 14
#define SERVO_OMBRO_ESQUERDO 13
#define SERVO_MAO_DIREITA 12
#define SERVO_BRACO_DIREITO 11
#define SERVO_OMBRO_DIREITO 10
#define SERVO_QUADRIL_ESQUERDO 9
#define SERVO_JOELHO_1_ESQUERDO 8
#define SERVO_JOELHO_2_ESQUERDO 7
#define SERVO_TORNOZELO_ESQUERDO 6
#define SERVO_PE_ESQUERDO 5
#define SERVO_QUADRIL_DIREITO 4
#define SERVO_JOELHO_1_DIREITO 3
#define SERVO_JOELHO_2_DIREITO 2
#define SERVO_TORNOZELO_DIREITO 1
#define SERVO_PE_DIREITO 0



// our servo # counter
uint8_t servonum = 15;

String inData = "";

#define SOP '<'
#define EOP '>'

bool started = false;
bool ended = false;


void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  // Drive each servo one at a time
  if (Serial.available() >= 8) {
     while(Serial.available() > 0)
      {
        char inChar = Serial.read();
        
        if(inChar == SOP)
        {
           inData = "";
           started = true;
           ended = false;
        }
        else if(inChar == EOP)
        {
           if (inData.length() == 6) {
             ended = true;
             break;
           }
        }
        else
        {
            inData = inData + inChar;
        }

      }
  }

     if(started && ended) {

           started = false;
           ended = false;
           
          String servoID = inData.substring(0, 3); //0 skip, 3 char
          short value = inData.substring(3).toInt(); //2_Dxxx

          if (servoID == "M_E") {
              pwm.setPWM(SERVO_MAO_ESQUERDA, 0, value);
          }
          if (servoID == "C_E") {
              pwm.setPWM(SERVO_BRACO_ESQUERDO, 0, value);
          }
          if (servoID == "O_E") {
              pwm.setPWM(SERVO_OMBRO_ESQUERDO, 0, value);
          }
          if (servoID == "Q_E") {
              pwm.setPWM(SERVO_QUADRIL_ESQUERDO, 0, value);
          }
          if (servoID == "1_E") {
              pwm.setPWM(SERVO_JOELHO_1_ESQUERDO, 0, value);
          }
          if (servoID == "2_E") {
              pwm.setPWM(SERVO_JOELHO_2_ESQUERDO, 0, value);
          }
          if (servoID == "T_E") {
              pwm.setPWM(SERVO_TORNOZELO_ESQUERDO, 0, value);
          }
          if (servoID == "P_E") {
              pwm.setPWM(SERVO_PE_ESQUERDO, 0, value);
          }
          if (servoID == "M_D") {
              pwm.setPWM(SERVO_MAO_DIREITA, 0, value);
          }
          if (servoID == "C_D") {
              pwm.setPWM(SERVO_BRACO_DIREITO, 0, value);
          }   
          if (servoID == "O_D") {
              pwm.setPWM(SERVO_OMBRO_DIREITO, 0, value);
          }
          if (servoID == "Q_D") {
              pwm.setPWM(SERVO_QUADRIL_DIREITO, 0, value);
          }                                    
          if (servoID == "1_D") {
              pwm.setPWM(SERVO_JOELHO_1_DIREITO, 0, value);
          }  
          if (servoID == "2_D") {
              pwm.setPWM(SERVO_JOELHO_2_DIREITO, 0, value);
          }
          if (servoID == "T_D") {
              pwm.setPWM(SERVO_TORNOZELO_DIREITO, 0, value);
          }
          if (servoID == "P_D") {
              pwm.setPWM(SERVO_PE_DIREITO, 0, value);
          }                         
          Serial.println("Chegou " + servoID + " - " + value);
     }
 // Serial.println("Delay");
 delay(100);
}
