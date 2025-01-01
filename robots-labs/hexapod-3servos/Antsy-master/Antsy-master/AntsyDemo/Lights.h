#ifndef RGLIGHTS_H
#define RGLIGHTS_H

#define RIGHT_LED_PIN 5
#define LEFT_LED_PIN 6

void LightsEnable()
{
	pinMode( RIGHT_LED_PIN, OUTPUT );
	pinMode( LEFT_LED_PIN, OUTPUT );
}

void LightsSet( bool left, bool right )
{
	digitalWrite( LEFT_LED_PIN, left );
	digitalWrite( RIGHT_LED_PIN, right );
}
#endif