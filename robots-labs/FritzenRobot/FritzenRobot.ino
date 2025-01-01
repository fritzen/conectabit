
#include "LineFollower.h"
#include "LineFollower3IR.h"
#include "Explorer.h"
#include "ExplorerStepper.h"
#include "MotorTest.h"

//LineFollower3IR robot;
//LineFollower robot;
//Explorer robot;
//MotorTest robot;
ExplorerStepper robot;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);                   
  while (!Serial);
  
  robot.robotSetup();

  Serial.println("GO FRITZEN I/O ROBOT!");

}

void loop() 
{ 
  robot.robotLoop();
}
