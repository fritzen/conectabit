//-----------------------------------------------------------------
//-- CC BY SA (http://ottodiy.com)
//-- 20 January 2017 
//-- TRIMS servo calibration values added
//-- double check your US (Ultra sound) module some have TRIG and ECHO swapped!
//-----------------------------------------------------------------
//-- Otto will avoid obstacles with this code!
//-----------------------------------------------------------------
#include <Servo.h> 
#include "Oscillator.h"
#include "US.h"
#include "Otto.h"

/*
                 --------------- 
                |     O   O     |
                |---------------|
RIGHT LEG 3==>  |               | <== LEFT LEG 2
                 --------------- 
                    ||     ||
RIGHT FOOT 5==>   -----   ------  <== LEFT FOOT 4
                 |-----   ------|
*/

#define PIN_LEFT_LEG   2   
#define PIN_RIGHT_LEG  3  
#define PIN_LEFT_FOOT  4  
#define PIN_RIGHT_FOOT 5 
 

#define TRIM_LEFT_LEG   -20
#define TRIM_RIGHT_LEG  -50
#define TRIM_LEFT_FOOT  32
#define TRIM_RIGHT_FOOT 42

Otto Otto;  //This is Otto!

void setup()
{
  Serial.begin(9600);
  Serial.println("hello");
  
  //Set the servo pins
  Otto.init(PIN_LEFT_LEG,PIN_RIGHT_LEG,PIN_LEFT_FOOT,PIN_RIGHT_FOOT,true);
  Otto.setTrims(TRIM_LEFT_LEG,TRIM_RIGHT_LEG, TRIM_LEFT_FOOT, TRIM_RIGHT_FOOT);

  Otto.sing(S_connection); //Otto wake up!
  Otto.home();

}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() 
{
  bool obstacleDetected = false;
  
  obstacleDetected = obstacleDetector(); 
  
   if(obstacleDetected)
   { 
         Otto.sing(S_surprise); 
         Otto.walk(7,1000,FORWARD); 
         delay(1000);
         Otto.sing(S_happy);
         Otto.turn(10,1000,RIGHT);                
         delay(2000); 
          
    }        
    else
   { 
          Otto.walk(-1,1000,BACKWARD);  
    }          
 }  

///////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable
bool obstacleDetector()
{
   int distance = Otto.getDistance();
   
   Serial.println(distance);
   
   if(distance<15)
   {
      return true;
   }
   else
   {
      return false;
   }

}
