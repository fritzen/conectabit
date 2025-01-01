/******************************************************************************
Walking.cpp
 *****************************************************************************/

#include <ServoEx.h>
#include <Arduino.h>
#include "Walking.h"

//#define DEBUG

WalkingCommand currentWalkCommand =
{
  0,
  0,
  100
};

//####################################################//
//Servo Objects
//####################################################//

ServoEx AntsyServo[3];

void initializeServos()
{
  //initialize servo objects, set min/max range values.
  AntsyServo[CENTER_SERVO].attach(9, CENTER_MIN, CENTER_MAX);      // attaches/activates the center servo to pin 9 sets min/max values
  AntsyServo[RIGHT_SERVO].attach(10, RIGHT_MIN, RIGHT_MAX);  // attaches/activates the right servo to pin 10, sets min/max values
  AntsyServo[LEFT_SERVO].attach(11, LEFT_MIN, LEFT_MAX);  // attaches/activates the left servo to pin 11, sets min/max values
}

//####################################################//
//Center Servo Values & Ranges
//####################################################//

int centerShiftRange = 400;  //Range of this value should be 300-400. Controls the amount of vertical lift provided by the center servo.
int centerRightDown = CENTER_CENTER - centerShiftRange;
int centerLeftDown = CENTER_CENTER + centerShiftRange;
int centerCenter = CENTER_CENTER;

//####################################################//
//Right Servo Values & Ranges
//####################################################//

int rightSweepRange = 200;  //Range of this value should be 100-200. Controls the distance of horizontal sweep motion.
int rightSweepRangeOffset;
int rightFrontSweep = RIGHT_CENTER + rightSweepRange;  //1700
int rightBackSweep = RIGHT_CENTER - rightSweepRange;  //1300


//####################################################//
//Left Servo Values & Ranges
//####################################################//

int leftSweepRange = 200;  //Range of this value should be 100-200. Controls the distance of horizontal sweep motion.
int leftSweepRangeOffset;
int leftFrontSweep = LEFT_CENTER - leftSweepRange;  //1300
int leftBackSweep = LEFT_CENTER + leftSweepRange;  //1700


// Placeholder for interpolation implementation.
int centerServoValue = 1500;   //current positional value being sent to the center servo.
int rightServoValue = 1500;  //current positional value being sent to the right servo.
int leftServoValue = 1500;  //current positional value being sent to the left servo.

// Delay values for walking sequences.
int delayValue = 25;

//####################################################//
//FORWARD WALKING POSES
//####################################################//

void RightUpForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = centerRightDown;
  rightServoValue = (rightFrontSweep - rightOffset);  //1700 normally
  leftServoValue = (leftBackSweep - leftOffset);    //1700 normally
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);

#ifdef DEBUG
  int tempRight = (rightFrontSweep - rightOffset);
  int tempLeft = (leftBackSweep - leftOffset);
  Serial.println("RightUpForward");
  Serial.print("Right: ");
  Serial.println(tempRight);
  Serial.println(" ");
  Serial.print("Left: ");
  Serial.println(tempLeft);
  Serial.println(" ");
#endif
}

void LeftUpForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = centerLeftDown;
  rightServoValue = (rightBackSweep + rightOffset);  //1300 normally
  leftServoValue = (leftFrontSweep + leftOffset);  //1300 normally
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);

#ifdef DEBUG
  int tempRight = (rightBackSweep + rightOffset);
  int tempLeft = (leftFrontSweep + leftOffset);
  Serial.println("LeftUpForward");
  Serial.print("Right: ");
  Serial.println(tempRight);
  Serial.println(" ");
  Serial.print("Left: ");
  Serial.println(tempLeft);
  Serial.println(" ");
#endif

}

void RightCenterForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightFrontSweep - rightOffset);  //1700 normally
  leftServoValue = (leftBackSweep - leftOffset);    //1700 normally
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftCenterForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightBackSweep + rightOffset);  //1300 normally
  leftServoValue = (leftFrontSweep + leftOffset);  //1300 normally
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}



//####################################################//
//BACKWARD WALKING POSES
//####################################################//

void RightUpBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerRightDown);
  rightServoValue = (rightBackSweep - rightOffset);
  leftServoValue = (leftFrontSweep - leftOffset);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftUpBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerLeftDown);
  rightServoValue = (rightFrontSweep + rightOffset);
  leftServoValue = (leftBackSweep + leftOffset);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void RightCenterBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightBackSweep - rightOffset);
  leftServoValue = (leftFrontSweep - leftOffset);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftCenterBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightFrontSweep + rightOffset);
  leftServoValue = (leftBackSweep + leftOffset);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}



//####################################################//
// RIGHT TURN WALKING POSES
//####################################################//

void RightUpTurnRight(unsigned int deltaTime)
{
  centerServoValue = (centerRightDown);
  rightServoValue = (rightBackSweep);
  leftServoValue = (leftBackSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftUpTurnRight(unsigned int deltaTime)
{
  centerServoValue = (centerLeftDown);
  rightServoValue = (rightFrontSweep);
  leftServoValue = (leftFrontSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void RightCenterTurnRight(unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightBackSweep);
  leftServoValue = (leftBackSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftCenterTurnRight(unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightFrontSweep);
  leftServoValue = (leftFrontSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}


//####################################################//
//LEFT TURN WALKING POSES
//####################################################//

void RightUpTurnLeft(unsigned int deltaTime)
{
  centerServoValue = (centerRightDown);
  rightServoValue = (rightFrontSweep);
  leftServoValue = (leftFrontSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftUpTurnLeft(unsigned int deltaTime)
{
  centerServoValue = (centerLeftDown);
  rightServoValue = (rightBackSweep);
  leftServoValue = (leftBackSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void RightCenterTurnLeft(unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightFrontSweep);
  leftServoValue = (leftFrontSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}

void LeftCenterTurnLeft(unsigned int deltaTime)
{
  centerServoValue = (centerCenter);
  rightServoValue = (rightBackSweep);
  leftServoValue = (leftBackSweep);
  SetServo(deltaTime);
  delay(deltaTime);
  delay(delayValue);
}


//####################################################//
//WALK CYCLE FUNCTIONS
//####################################################//

void TurnRight(unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    LeftUpTurnRight(speed);
    LeftCenterTurnRight(speed);
    RightUpTurnRight(speed);
    RightCenterTurnRight(speed);
    i++;
  }
}

void TurnLeft(unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    LeftUpTurnLeft(speed);
    LeftCenterTurnLeft(speed);
    RightUpTurnLeft(speed);
    RightCenterTurnLeft(speed);
    i++;
  }
}

void WalkForward(unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    RightUpForward(0, 0, speed);
    RightCenterForward(0, 0, speed);
    LeftUpForward(0, 0, speed);
    LeftCenterForward(0, 0, speed);
    i++;
  }
}

void WalkBackward(unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    RightUpBackward(0, 0, speed);
    RightCenterBackward(0, 0, speed);
    LeftUpBackward(0, 0, speed);
    LeftCenterBackward(0, 0, speed);
    i++;
  }
}

void DriveForward(int turn, unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  turn = constrain(turn, -10, +10);
  int i = 0;
  while (i <= cycle)
  {
    if (turn >= 1)  //Drive Right
    {
      RightUpForward(0, (turn * 15), speed);
      RightCenterForward(0, (turn * 15), speed);
      LeftUpForward(0, (turn * 15), speed);
      LeftCenterForward(0, (turn * 15), speed);
    }


    if (turn <= -1)  //Drive Left
    {
      RightUpForward((turn * -15), 0, speed);
      RightCenterForward((turn * -15), 0, speed);
      LeftUpForward((turn * -15), 0, speed);
      LeftCenterForward((turn * -15), 0, speed);
    }


    if (turn == 0)  //Drive Straight
    {
      RightUpForward(0, 0, speed);
      RightCenterForward(0, 0, speed);
      LeftUpForward(0, 0, speed);
      LeftCenterForward(0, 0, speed);
    }

    i++;
  }
}

void DriveBackward(int turn, unsigned int cycle, unsigned int speed)
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  turn = constrain(turn, -10, +10);
  int i = 0;
  while (i <= cycle)
  {
    if (turn < 0) //Drive Right
    {
      RightUpBackward(0, (turn * 15), speed);
      RightCenterBackward(0, (turn * 15), speed);
      LeftUpBackward(0, (turn * 15), speed);
      LeftCenterBackward(0, (turn * 15), speed);
    }
    else if (turn > 0) //Drive Left
    {
      RightUpBackward((turn * -15), 0, speed);
      RightCenterBackward((turn * -15), 0, speed);
      LeftUpBackward((turn * -15), 0, speed);
      LeftCenterBackward((turn * -15), 0, speed);
    }
    else //Drive Straight
    {
      RightUpBackward(0, 0, speed);
      RightCenterBackward(0, 0, speed);
      LeftUpBackward(0, 0, speed);
      LeftCenterBackward(0, 0, speed);
    }

    i++;
  }
}

//===================================================================================================
// SetServo: Writes Servo Positions using ServoGroupMove from ServoEX library
//===================================================================================================
void SetServo(unsigned int deltaTime)
{
  ServoGroupMove.start();   //start group move
  AntsyServo[CENTER_SERVO].writeMicroseconds(centerServoValue); //set position to 1st servo, account for error adjustments in defines
  AntsyServo[RIGHT_SERVO].writeMicroseconds(rightServoValue); // "" ""
  AntsyServo[LEFT_SERVO].writeMicroseconds(leftServoValue);
  ServoGroupMove.commit(deltaTime); //commit the group move, pass along an interpolation time value
}

void SetServoCenter(unsigned int deltaTime)
{
  ServoGroupMove.start();   //start group move
  AntsyServo[CENTER_SERVO].writeMicroseconds(CENTER_CENTER); //set position to 1st servo, account for error adjustments in defines
  AntsyServo[RIGHT_SERVO].writeMicroseconds(RIGHT_CENTER); // "" ""
  AntsyServo[LEFT_SERVO].writeMicroseconds(LEFT_CENTER);
  ServoGroupMove.commit(deltaTime); //commit the group move, pass along an interpolation time value
}

void Wiggle( unsigned int cycle, unsigned int speed )
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    centerServoValue = 1700;
    leftServoValue = 1700;
    rightServoValue = 1700;
    SetServo(speed);
    delay(speed);

    centerServoValue = CENTER_CENTER;
    leftServoValue = LEFT_CENTER;
    rightServoValue = RIGHT_CENTER;
    SetServo(speed);
    delay(speed);
    i++;
  }
}

void Shiver( unsigned int cycle, unsigned int speed )
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED / 2, MAX_SPEED / 2);
  int i = 0;
  while (i <= cycle)
  {
    centerServoValue = 1700;
    leftServoValue = LEFT_BACKSWEEP;
    rightServoValue = RIGHT_FRONTSWEEP;
    SetServo(speed);
    delay(speed);

    centerServoValue = 1300;
    leftServoValue = LEFT_FRONTSWEEP;
    rightServoValue = RIGHT_BACKSWEEP;
    SetServo(speed);
    delay(speed);

    centerServoValue = CENTER_CENTER;
    leftServoValue = LEFT_CENTER;
    rightServoValue = RIGHT_CENTER;
    SetServo(speed);
    delay(speed);
    i++;
  }
}

void Giggle( unsigned int cycle, unsigned int speed )
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  while (i <= cycle)
  {
    centerServoValue = CENTER_RIGHTDOWN;
    SetServo(speed);
    delay(speed);

    centerServoValue = CENTER_LEFTDOWN;
    SetServo(speed);
    delay(speed);

    centerServoValue = CENTER_CENTER;
    SetServo(speed);
    delay(speed);
    i++;
  }
}

void Wave( unsigned int cycle, unsigned int speed )
{
  speed = constrain(speed, 1, 100);
  speed = map(speed, 1, 100, MIN_SPEED, MAX_SPEED);
  int i = 0;
  centerServoValue = CENTER_RIGHTDOWN;
  SetServo(speed);
  delay(speed);
  while (i <= cycle)
  {
    rightServoValue = RIGHT_BACKSWEEP;
    SetServo(speed);
    delay(speed);
    rightServoValue = RIGHT_FRONTSWEEP;
    SetServo(speed);
    delay(speed);
    i++;
  }
  rightServoValue = RIGHT_CENTER;
  centerServoValue = CENTER_CENTER;
  SetServo(speed);
  delay(speed);
}