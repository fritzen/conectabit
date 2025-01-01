#include "USX.h"

//****** US ******//
USX::USX(){
}

USX::USX(int pinTrigger, int pinEcho){
  USX::init(pinTrigger,pinEcho);
}

void USX::init(int pinTrigger, int pinEcho)
{
  _pinTrigger = pinTrigger;
  _pinEcho = pinEcho;
  pinMode( _pinTrigger , OUTPUT );
  pinMode( _pinEcho , INPUT );
}

long USX::TP_init()
{
    digitalWrite(_pinTrigger, LOW);
    delayMicroseconds(2);
    digitalWrite(_pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pinTrigger, LOW);
    long microseconds = pulseIn(_pinEcho,HIGH,40000); //40000
    return microseconds;
}

float USX::read(){
  long microseconds = USX::TP_init();
  long distance;
  distance = microseconds/29/2;
  if (distance == 0){
    distance = 999;
  }
  return distance;
}