#include "LineFollower.h"


LineFollower robot;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);                   
  while (!Serial);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(4, HIGH); 
  digitalWrite(5, HIGH); 
  digitalWrite(6, HIGH); 
  digitalWrite(7, HIGH); 
  digitalWrite(8, HIGH); 
  digitalWrite(9, HIGH); 
  
  robot.robotSetup();

  Serial.println("GO FRITZEN I/O ROBOT!");

}

void loop() 
{ 
  robot.robotLoop();
}
