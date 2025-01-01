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
#include <PiezoEffects.h>
#include "Walking.h"
#include "Gamepad.h"
#include "Lights.h"
#include "LightSensors.h"
#include "ActivityTimer.h"

//Piezo buzzer on Digital Pin 12 with flashing LEDs on 5 and 6
PiezoEffects mySounds( 12, 5, 6 );

//####################################################//
//Setup
//####################################################//

void setup()
{
  Serial.begin(38400);

  GamepadEnable();

  LightsEnable();

  mySounds.play( soundCuddly );

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
      mySounds.play( soundSad );
      myActivityTimer.updateAlertInterval();
    }
  }

  //Update currentWalkCommand based on gamepad button states
  if ( my_gamepad.update_button_states() )
  {
    myActivityTimer.updateActivityTimer(); //Update activity timer so we don't timeout.

    if ( my_gamepad.button_press_up() ) currentWalkCommand.fwdBack = 1;
    else if ( my_gamepad.button_press_down() ) currentWalkCommand.fwdBack = -1;
    else currentWalkCommand.fwdBack = 0;

    if ( my_gamepad.button_press_left() ) currentWalkCommand.leftRight = - 10;
    else if ( my_gamepad.button_press_right() ) currentWalkCommand.leftRight = 10;
    else currentWalkCommand.leftRight = 0;

    //Check for select press.. If true toggle light sensing mode
    if ( my_gamepad.button_press_select() )
    {
      if ( myLightSensors.lightSensorMode == LightSensors::LIGHT_SENSE_OFF )
      {
        myLightSensors.lightSensorMode = LightSensors::LIGHT_SENSE_LIGHT;
        mySounds.play( soundMode2 );
      }
      else if ( myLightSensors.lightSensorMode == LightSensors::LIGHT_SENSE_LIGHT )
      {
        myLightSensors.lightSensorMode = LightSensors::LIGHT_SENSE_DARK;
        mySounds.play( soundMode3 );
      }
      else
      {
        myLightSensors.lightSensorMode = LightSensors::LIGHT_SENSE_OFF;
        mySounds.play( soundMode1 );
      }
      delay(1000);
      my_gamepad.update_button_states();
    }

    //Check other buttons for sound and motion demonstrations
    if ( my_gamepad.button_press_b() )
    {
      mySounds.play( soundInsect1 );
      Wiggle(3, currentWalkCommand.moveSpeed);
      my_gamepad.update_button_states();
    }
    if ( my_gamepad.button_press_a() )
    {
      mySounds.play( soundOhh );
      Shiver(2, currentWalkCommand.moveSpeed);
      my_gamepad.update_button_states();
    }
    if ( my_gamepad.button_press_ta() )
    {
      mySounds.play( soundLaugh );
      Giggle(2, currentWalkCommand.moveSpeed);
      my_gamepad.update_button_states();
    }
    if ( my_gamepad.button_press_tb() )
    {
      mySounds.play( soundWhistle );
      Wave(4, currentWalkCommand.moveSpeed);
      my_gamepad.update_button_states();
    }
  }
  else //No new button states from gamepad. Cancel previous walk command.
  {
    currentWalkCommand.fwdBack = 0;
    currentWalkCommand.leftRight = 0;
  }

  //Issue gait commands based on currentWalkCommand values
  if ( currentWalkCommand.fwdBack > 0 ) //Moving forward
  {
    DriveForward(currentWalkCommand.leftRight, 0, currentWalkCommand.moveSpeed);
  }
  else if ( currentWalkCommand.fwdBack < 0 ) //Moving backward
  {
    DriveBackward(currentWalkCommand.leftRight, 0, currentWalkCommand.moveSpeed);
  }
  else if( currentWalkCommand.leftRight < 0 ) //Turning left only
  {
    TurnLeft(0, currentWalkCommand.moveSpeed);
  }
  else if( currentWalkCommand.leftRight > 0 ) //Turning right only
  {
    TurnRight(0, currentWalkCommand.moveSpeed);
  }

  //Else there are no currentWalkCommand values.. perform light seeking if enabled
  else if ( myLightSensors.lightSensorMode != LightSensors::LIGHT_SENSE_OFF )
  {
    int lightSenseCommand = -1;
    if ( myLightSensors.lightSensorMode == LightSensors::LIGHT_SENSE_LIGHT ) lightSenseCommand = myLightSensors.SeekLight();
    else lightSenseCommand = myLightSensors.SeekDark();
    switch( lightSenseCommand )
    {
    case LightSensors::WALK_FWD:
      LightsSet( HIGH, HIGH );
      DriveForward(0, 0, currentWalkCommand.moveSpeed);
      myActivityTimer.updateActivityTimer();
      break;
    case LightSensors::WALK_REV:
      LightsSet( HIGH, HIGH );
      WalkBackward(0, currentWalkCommand.moveSpeed);
      myActivityTimer.updateActivityTimer();
      break;
    case LightSensors::WALK_LEFT:
      LightsSet( HIGH, LOW );
      TurnLeft(0, currentWalkCommand.moveSpeed);
      myActivityTimer.updateActivityTimer();
      break;
    case LightSensors::WALK_RIGHT:
      LightsSet( LOW, HIGH );
      TurnRight(0, currentWalkCommand.moveSpeed);
      myActivityTimer.updateActivityTimer();
      break;
    case LightSensors::WALK_STOP:
      LightsSet( LOW, LOW );
      break;
    }
  }

  delay(50);
}
