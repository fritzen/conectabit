#include "MotorTest.h"


void MotorTest::robotSetup() {
    motor.attach(TYPE_L293); //TYPE_SERVO, TYPE_L293
}


void MotorTest::robotLoop() {
  
  Serial.println("motor.turnLeft();");
  motor.turnLeft();
  delay(2000);

  Serial.println("motor.turnRight();");
  motor.turnRight();
  delay(2000);

  Serial.println("motor.turnLeftAxis();");
  motor.turnLeftAxis();
  delay(2000);

  Serial.println("motor.turnRightAxis();");
  motor.turnRightAxis();
  delay(2000);

  Serial.println("motor.forward();");
  motor.forward();
  delay(2000);


  Serial.println("motor.backward();");
  motor.backward();
  delay(2000);


  Serial.println("motor.stop();");
  motor.stop();
  delay(2000);

}
