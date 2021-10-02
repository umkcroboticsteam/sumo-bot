#ifndef Surface_h
#define Surface_h

#include <Arduino.h>

class Surface
{
	public:
		Surface(int, int, int, int);
		bool front_left();
		bool front_right();
		bool back_left();
		bool back_right();
		void printPins();

	private:
		int flPin;
		int blPin;
		int frPin;
		int brPin;
};


#endif
