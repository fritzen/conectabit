#ifndef USX_h
#define USX_h
#include "Arduino.h"

class USX
{
public:
	USX();
	void init(int pinTrigger, int pinEcho);
	USX(int pinTrigger, int pinEcho);
	float read();

private:
	int _pinTrigger;
	int _pinEcho;
	long TP_init();

};

#endif //USX_h