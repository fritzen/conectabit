#ifndef LIGHTSENSORS_H
#define LIGHTSENSORS_H

#define RIGHT_LIGHT_SENSE_PIN 0
#define LEFT_LIGHT_SENSE_PIN 7

#define LIGHT_SENSE_LEFT_TRIM 0 //Default trims to compensate for offset sensors readings
#define LIGHT_SENSE_RIGHT_TRIM 0 //Default trims to compensate for offset sensors readings

//#define LIGHT_SENSE_DARK_THRESHOLD 100 //Below 100 is considered dark
#define LIGHT_SENSE_LIGHT_THRESHOLD 200 //Above 200 is considered light
#define LIGHT_SENSE_TURN_THRESHOLD 75 //Greater than 75 difference in LEFT and RIGHT will trigger turn action

class LightSensors
{
  public:
    enum WALK_CMDS
    {
      WALK_FWD,
      WALK_REV,
      WALK_LEFT,
      WALK_RIGHT,
      WALK_STOP
    };

    enum LightSensorModes
    {
      LIGHT_SENSE_OFF,
      LIGHT_SENSE_LIGHT,
      LIGHT_SENSE_DARK
    }
    lightSensorMode = LIGHT_SENSE_OFF;

    LightSensors() {}

    int SeekLight();
    int SeekDark();

    void UpdateTrims( int left, int right );

  private:
    int _sensor_left_trim;
    int _sensor_right_trim;
    bool _sensors_enabled;
};

extern LightSensors myLightSensors;

#endif
