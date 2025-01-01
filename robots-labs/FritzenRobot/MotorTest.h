#ifndef MotorTest_h
	#define MotorTest_h

  	#if defined(ARDUINO) && ARDUINO >= 100
    	#include <Arduino.h>
  	#else
    	#include <WProgram.h>
		#include <pins_arduino.h>
	#endif

  #include "src/FioMotorDriver.h"
  #include "src/FioSensorDistance.h"
  #include "src/TimerFreeTone.h"


class MotorTest
{
  public:
    void robotSetup();
    void robotLoop();
  private:
     FioMotorDriver motor;
     FioSensorDistance distance;
    
};
  
#endif
