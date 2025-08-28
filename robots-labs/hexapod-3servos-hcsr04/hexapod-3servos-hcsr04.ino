/***********************************************************************************
 *     ______            RobotGeek Antsy (3 Servo Hexapod) Demo            ______
 *     |    |                                                              |    |
 *
 *  Wiring:
 *
 *    Center Servo - Digital Pin 9
 *    Right Servo - Digital Pin 10
 *    Left Servo - Digital Pin 11
 *    Buzzer - Digital Pin 12
 *    IR Receiver - Digital Pin 2
 *    Right LED - Digital Pin 5
 *    Left LED - Digital Pin 6
 *    Right Light Sensor - Analog Pin 0
 *    Left Light Sensor - Analog Pin 7
 *
 *    Jumper for pins 9/10/11 should be set to 'VIN'
 *    Jumper for pins 3/5/6 should be set to '5V'
 *
 *  Control Behavior:
 *
 *    IR Gamepad D-Pad buttons controls movement
 *    IR Gamepad Select button cycles mode ( RemoteControl, RC + Light Seeking, RC + Shade Seeking )
 *    IR Gamepad A,B,TA,TB buttons perform expressions with movement, light, sounds
 *
 ***********************************************************************************/

#include "ServoEx.h"
#include "Walking.h"
#include "ActivityTimer.h"

// Pino de trigger do sensor
const int TRIGGER_PIN = 2;
// Pino de echo do sensor
const int ECHO_PIN = 3;

//####################################################//
//Setup
//####################################################//

void setup()
{
  Serial.begin(38400);

  initializeServos();

  SetServoCenter(2000); //Set servos to center values over 2000ms period
  delay(1000);

  myActivityTimer.setTimeoutThreshold( 15 ); //15 seconds of inactivity until Antsy requests your attention
  myActivityTimer.setAlertInterval( 10 ); //10 seconds between alerts when inactive
}

//####################################################//
//Main Loop
//####################################################//

void loop()
{
  //Always check the ActivityTimer to see if the robot is inactive
  if ( myActivityTimer.checkActivityTimer() )
  {
    //Inactive robot.. is it time to try to get your attention?
    if ( myActivityTimer.checkAlertInterval() )
    {
      myActivityTimer.updateAlertInterval();
    }
  }


      DriveForward(0, 0, currentWalkCommand.moveSpeed);
      myActivityTimer.updateActivityTimer();
  
      // DriveBackward(currentWalkCommand.leftRight, 0, currentWalkCommand.moveSpeed);
      // WalkBackward(0, currentWalkCommand.moveSpeed);
      // myActivityTimer.updateActivityTimer();
  
      // TurnLeft(0, currentWalkCommand.moveSpeed);
      // myActivityTimer.updateActivityTimer();
  
      // TurnRight(0, currentWalkCommand.moveSpeed);
      // myActivityTimer.updateActivityTimer();
  
  delay(50);
}
