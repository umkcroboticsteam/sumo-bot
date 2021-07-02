#include "Arduino.h"
#include "Move.h"

Move::Move()
{
  powerValue = 255;
  pwm1 = 11; // Power to right motor
  pwm2 = 10; // Power to left motor
  dir1 = 12; // Direction of right motor (low is forward)
  dir2 = 13; // Direction of left motor (low is forward)
  // bool true = HIGH
  // bool false = LOW
}

Move* Move::power(int setPower)
{
  powerValue = setPower;
  return this;
}

int Move::getPower(){
	return powerValue;
}

void Move::set(int right, int left, bool dirR, bool dirL)
{
  analogWrite(pwm1, right);
  analogWrite(pwm2, left);
  digitalWrite(dir1, dirR);   
  digitalWrite(dir2, dirL);
}

void Move::forward()
{
  analogWrite(pwm1, powerValue);
  analogWrite(pwm2, powerValue);
  digitalWrite(dir1, LOW);   
  digitalWrite(dir2, LOW);
}
void Move::backward()
{
	analogWrite(pwm1, powerValue);
	analogWrite(pwm2, powerValue);
	digitalWrite(dir1, HIGH);
	digitalWrite(dir2, HIGH);

}
void Move::left()
{
  analogWrite(pwm1, powerValue);
  analogWrite(pwm2, powerValue);
	digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
}
void Move::right()
{
  analogWrite(pwm1, powerValue);
  analogWrite(pwm2, powerValue);
  digitalWrite(dir1, HIGH);
	digitalWrite(dir2, LOW);

}
void Move::halt()
{
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  digitalWrite(dir1, LOW);   
  digitalWrite(dir2, LOW);
}

void Move::turn(int degree, char direction){
	int orginalPower = getPower();
	int timeDelay = 100; // how much it takes to turn 15 degree
	power(255); 

	switch(degree) {
    		case 15:
      			if(direction == 'R'){
        			right();
        			delay(timeDelay);
      			}
      			else {
        			left();
        			delay(timeDelay);
     			 }
      		break;

    		case 30: // All degrees will be divisible by 15. So to get a degree to turn it will multiple it by 15.
      			if(direction == 'R'){
        			right();
        			delay(2 * timeDelay);
     			}
      			else {
        			left();
        			delay(2 * timeDelay);
      			}
      			break;

    		case 45:
      			if(direction == 'R'){
        			right();
        			delay(3 * timeDelay);
      			}
      			else {
        			left();
        			delay(3 * timeDelay);
      			}
      			break;

    		case 60:
      			if(direction == 'R'){
        			right();
        			delay(4 * timeDelay);
      			}
      			else {
        			left();
        			delay(4 * timeDelay);
      			}
      			break;

    		case 90:
      			if(direction == 'R'){
        			right();
        			delay(6 * timeDelay);
      			}
      			else {
        			left();
        			delay(6 * timeDelay);
      			}
      			break;
    
    		case 135:
      			if(direction == 'R'){
        			right();
        			delay(9 * timeDelay);
      			}
      			else {
        			left();
        			delay(9 * timeDelay);
      			}
      			break;

    		case 180:
      			if(direction == 'R'){
        			right();
        			delay(12 * timeDelay);
      			}
      			else {
        			left();
        			delay(12 * timeDelay);
      			}
      			break;

    		default: // if error it will do 360 turn. For testing or bug finding.
      			left();
      			delay(24 * timeDelay);
      			break;
  	}
  power(orginalPower); // sets power to before it started to turn.

}

	
