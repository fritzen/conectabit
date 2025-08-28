#include "ActivityTimer.h"

ActivityTimer myActivityTimer;

void ActivityTimer::setTimeoutThreshold( unsigned long seconds )
{
	_activitiyTimeoutThreshold = seconds * 1000; //store as milliseconds
}

void ActivityTimer::updateActivityTimer()
{
	_lastActivityTimestamp = millis();
}

bool ActivityTimer::checkActivityTimer()
{
	//Returning true if the activity timer has crossed the threshold (inactive robot)
	return ( _lastActivityTimestamp + _activitiyTimeoutThreshold < millis() );
}

void ActivityTimer::setAlertInterval( unsigned long seconds )
{
	_alertInterval = seconds * 1000; //store as milliseconds
}

bool ActivityTimer::checkAlertInterval()
{
	//Returning true if the activity alert has crossed the threshold (time to alert)
	return ( _lastActivityAlert + _alertInterval < millis() );
}

void ActivityTimer::updateAlertInterval()
{
	_lastActivityAlert = millis();
}