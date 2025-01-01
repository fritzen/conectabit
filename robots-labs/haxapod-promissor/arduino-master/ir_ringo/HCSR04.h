#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

class HCSR04 {
public:
  HCSR04(int trigPin, int echoPin);

  void begin();
  long getDistance();

private:
  int trigPin;
  int echoPin;
  long averageDuration;
  static const int MAX_READINGS = 51;
  long readings[MAX_READINGS];
  int currentIndex;
  int validReadings;
  long totalDuration;
};

#endif // HCSR04_H
