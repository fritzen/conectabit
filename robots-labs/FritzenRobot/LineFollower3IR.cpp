#include "LineFollower3IR.h"
#include "src/FioMotorDriver.h"
#include "src/TimerFreeTone.h"

#ifdef LineFollower3IR_h


void LineFollower3IR::robotSetup() {
    pinMode(LEFT_SENSOR_PIN,INPUT_PULLUP);
    pinMode(CENTER_SENSOR_PIN,INPUT_PULLUP);
    pinMode(RIGHT_SENSOR_PIN,INPUT_PULLUP);
    motor.attach(TYPE_L293); //TYPE_SERVO, TYPE_L293
}


void LineFollower3IR::readLineFollowerSensors() {
  
     left_sensor = analogRead(LEFT_SENSOR_PIN);
     right_sensor = analogRead(RIGHT_SENSOR_PIN);
     center_sensor = analogRead(CENTER_SENSOR_PIN);

    if (left_sensor > SENSOR_THRESHOLD) {
      left_sensor = BLACK;
    } else {
      left_sensor = WHITE;
    }

    if (center_sensor > SENSOR_THRESHOLD) {
      center_sensor = BLACK;
    } else {
      center_sensor = WHITE;
    }

    if (right_sensor > SENSOR_THRESHOLD) {
      right_sensor = BLACK;
    } else {
      right_sensor = WHITE;
    }

        
     Serial.print(left_sensor);
     Serial.print("x");
     Serial.print(center_sensor); 
     Serial.print("x");
     Serial.print(right_sensor); 
     Serial.print("*");

    
 }


void LineFollower3IR::robotLoop() {
	  readLineFollowerSensors();
    if (centralizing) {
      if (center_sensor == BLACK) {
          centralizing = false;
      }
    } else {
        if (left_sensor == BLACK ) {
           motor.turnLeft();
           centralizing = true;
        } else if (right_sensor == BLACK) {             
            motor.turnRight();
            centralizing = true;
        } else {
            motor.forward();
        }
    }

    Serial.println(";");
}
#endif
