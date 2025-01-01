#ifndef ACTIVITYTIMER_H
#define ACTIVITYTIMER_H

#include <Arduino.h>

class ActivityTimer
{
	public:
		ActivityTimer() {}
		void setTimeoutThreshold( unsigned long seconds );
		void updateActivityTimer();
		bool checkActivityTimer();

		void setAlertInterval( unsigned long seconds );
		bool checkAlertInterval();
		void updateAlertInterval();

	private:
		unsigned long _activitiyTimeoutThreshold = 10000; //10 seconds
		unsigned long _lastActivityTimestamp = millis();

		unsigned long _alertInterval = 5000; //5 seconds
		unsigned long _lastActivityAlert = millis();
};

extern ActivityTimer myActivityTimer;

#endif
