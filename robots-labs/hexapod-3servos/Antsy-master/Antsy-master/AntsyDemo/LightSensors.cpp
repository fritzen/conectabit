#include <Arduino.h>
#include "LightSensors.h"

//#define USB_DEBUG

LightSensors myLightSensors;

int LightSensors::SeekLight()
{
  int left_light_sense;
  int right_light_sense;

  int i = 0;
  while ( ++i < 10 )
  {
    left_light_sense += analogRead( LEFT_LIGHT_SENSE_PIN ) + _sensor_left_trim;
    right_light_sense += analogRead( RIGHT_LIGHT_SENSE_PIN ) + _sensor_right_trim;
  }
  left_light_sense /= i;
  right_light_sense /= i;

#ifdef USB_DEBUG
  Serial.print( "LightSensors::SeekLight() L: " );
  Serial.print( left_light_sense );
  Serial.print( " R: " );
  Serial.println( right_light_sense );
#endif

  if ( left_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD && right_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD )
  {
    return WALK_FWD;
  }
  else if ( right_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD && right_light_sense - left_light_sense > LIGHT_SENSE_TURN_THRESHOLD )
  {
    return WALK_RIGHT;
  }
  else if ( left_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD && left_light_sense - right_light_sense > LIGHT_SENSE_TURN_THRESHOLD )
  {
    return WALK_LEFT;
  }

  return WALK_STOP;
}

int LightSensors::SeekDark()
{
  int left_light_sense;
  int right_light_sense;

  int i = 0;
  while ( ++i < 10 )
  {
    left_light_sense += analogRead( LEFT_LIGHT_SENSE_PIN ) + _sensor_left_trim;
    right_light_sense += analogRead( RIGHT_LIGHT_SENSE_PIN ) + _sensor_right_trim;
  }
  left_light_sense /= i;
  right_light_sense /= i;

#ifdef USB_DEBUG
  Serial.print( "LightSensors::SeekDark() L: " );
  Serial.print( left_light_sense );
  Serial.print( " R: " );
  Serial.println( right_light_sense );
#endif

  if ( left_light_sense < LIGHT_SENSE_LIGHT_THRESHOLD && right_light_sense < LIGHT_SENSE_LIGHT_THRESHOLD )
  {
    return WALK_STOP;
  }
  else if ( right_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD && right_light_sense - left_light_sense > LIGHT_SENSE_TURN_THRESHOLD )
  {
    return WALK_LEFT;
  }
  else if ( left_light_sense > LIGHT_SENSE_LIGHT_THRESHOLD && left_light_sense - right_light_sense > LIGHT_SENSE_TURN_THRESHOLD )
  {
    return WALK_RIGHT;
  }

  return WALK_REV;
}

void LightSensors::UpdateTrims( int left, int right )
{
#ifdef USB_DEBUG
  Serial.print( "LightSensors::UpdateTrims() L: " );
  Serial.print( left );
  Serial.print( " R: " );
  Serial.println( right );
#endif

  //Update sensors trims used on analogRead values
  _sensor_left_trim = left;
  _sensor_right_trim = right;
}

