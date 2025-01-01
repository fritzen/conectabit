#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


#define USMIN  600 
#define USMAX  2400

#define SERVO_1_RETRACT 2000
#define SERVO_1_EXPAND  600

#define SERVO_2_RETRACT 1900
#define SERVO_2_EXPAND  500

#define SERVO_3_RETRACT 1900
#define SERVO_3_EXPAND  500

#define SERVO_4_RETRACT 1900
#define SERVO_4_EXPAND  500

#define SERVO_1_CLIP 2000
#define SERVO_1_ROTATE  600

#define SERVO_2_CLIP 2000
#define SERVO_2_ROTATE  600

#define SERVO_3_CLIP 2000
#define SERVO_3_ROTATE  600

#define SERVO_4_CLIP 2000
#define SERVO_4_ROTATE  600


void setup() {
  Serial.begin(9600);
  Serial.println("Magic Cube!");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);

      movimentar(1, SERVO_2_RETRACT);
      delay(100);
      movimentar(2, SERVO_2_RETRACT);
      delay(100);
      movimentar(3, SERVO_2_RETRACT);
      delay(100);
      movimentar(4, SERVO_2_RETRACT);
      delay(100);
      movimentar(1, SERVO_1_EXPAND);
      delay(100);
      movimentar(2, SERVO_2_EXPAND);
      delay(100);
      movimentar(3, SERVO_3_EXPAND);
      delay(100);
      movimentar(4, SERVO_4_EXPAND);
      delay(100);
}



void movimentar(uint8_t servonum, int us){
    servonum = servonum - 1; 
    pwm.writeMicroseconds(servonum, us); //600 expande - 2000
    delay(600);
}

void claw(uint8_t servonum, int us){
    servonum = servonum + 3; 
    pwm.writeMicroseconds(servonum, us); //600 expande - 2000
    delay(600);
}

void loop() {

      claw(2, SERVO_2_ROTATE);
      delay(1000);
      movimentar(2, SERVO_2_RETRACT);
      delay(1000);
      claw(2, SERVO_2_CLIP);
      delay(1000);
      movimentar(2, SERVO_1_EXPAND);
      delay(1000);

      claw(2, SERVO_1_CLIP);
      delay(100000);

     // for(int i = 500; i< 2000; i++){
       // claw(2, i);
       // Serial.println(i);
    //  }
      
      
  
  //  pwm.writeMicroseconds(servonum, 1900); //600 expande - 2000
  //  delay(500);
  //  pwm.writeMicroseconds(servonum, 600); //600 expande - 2000
  //  delay(500);

  //servonum++;
 // if (servonum > 3) servonum = 0; // Testing the first 8 servo channels

 delay(100);
}
