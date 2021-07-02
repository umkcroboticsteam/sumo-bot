#include "Arduino.h"
#include "Surface.h"

Surface::Surface()
{
	int flPin = 24;
	int blPin = 52;
	int frPin = 25;
	int brPin = 53;

}

// Read IR value
bool Surface::front_left() // LOW-HIGH
{
	return digitalRead(flPin);
}

bool Surface::front_right()
{
	return digitalRead(frPin);
}

bool Surface::back_left()
{
	pinMode(blPin, INPUT);
	return digitalRead(blPin);
}

bool Surface::back_right() 
{
	return digitalRead(brPin);
}