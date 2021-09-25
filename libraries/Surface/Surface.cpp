#include "Arduino.h"
#include "Surface.h"

Surface::Surface(int flPin, int frPin, int blPin, int brPin)
{
	this->flPin = flPin;
	this->frPin = frPin;
	this->blPin = blPin;
	this->brPin = brPin;
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
	pinMode(blPin, INPUT); // does this need to be here?
	return digitalRead(blPin);
}

bool Surface::back_right() 
{
	return digitalRead(brPin);
}