#include "Arduino.h"
#include "Surface.h"

Surface::Surface(int fl, int fr, int bl, int br)
{
	flPin = fl;
	frPin = fr;
	blPin = bl;
	brPin = br;
	pinMode(blPin, INPUT);
	pinMode(brPin, INPUT);
	pinMode(frPin, INPUT);
	pinMode(flPin, INPUT);
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
	return digitalRead(blPin);
}

bool Surface::back_right() 
{
	return digitalRead(brPin);
}

// This prints out what each IR sensor is reading
void Surface::printPins()
{
  Serial.print("Back left pin reading: ");
  Serial.println(back_left());
 
  Serial.print("Back right pin reading: ");
  Serial.println(back_right());

  Serial.print("Front right pin reading: ");
  Serial.println(front_right());

  Serial.print("Front left pin reading: ");
  Serial.println(front_left());
}