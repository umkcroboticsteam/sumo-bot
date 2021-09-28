#include <Move.h>
#include <Sight.h>
#include <Surface.h>
#include <NewPing.h>

/**
 * Globals
 */
//FIXME:: should add initializing move, Ir, and sonic sensor to their own class. This will help keep this code clean and organized.
int flPin = 50;          
int frPin = 51;
int blPin = 52;
int brPin = 53;

Move move(11, 10, 12, 13); // pwm1, pwm2, dir1, dir2
Sight sight;
Surface surface(flPin, frPin, blPin, brPin);

// This prints out what each IR sensor is reading
void printPins()
{
  Serial.print("Back left pin reading: ");
  Serial.println(digitalRead(blPin)); // these can be changed to use surface object?
 
  Serial.print("Back right pin reading: ");
  Serial.println(digitalRead(brPin));

  Serial.print("Front right pin reading: ");
  Serial.println(digitalRead(frPin));

  Serial.print("Front left pin reading: ");
  Serial.println(digitalRead(flPin));
  //delay(1000); // use only for debugging. dont run with testing on field
}

NewPing sensors[7] = {
  NewPing( 2, 2, 200), //sensor 1 left 90 degree
  NewPing( 3, 3, 200), //sensor 2 left 45 degree
  NewPing( 4, 4, 200), //sensor 3 left 0 degree
  NewPing( 5, 5, 200), //sensor 4 right 0 degree
  NewPing( 6, 6, 200), //sensor 5 right 45 degree
  NewPing( 7, 7, 200), //sensor 6 right 90 degree
  NewPing( 9, 8, 200)  //sensor 7 back 180 degree
};

int delay_time = 100;

int threshold_min = 20; // [cm] threshold distance to stop motors for sensor 3 and 4 which looks straight ahead
int threshold_max = 75; // [cm] threshold distance to stop motors for sensor 3 and 4 which looks straight ahead
int threshold_back = 15;

void setup() 
{
  /**
   * Setup for IRs
   */
  pinMode(blPin, INPUT);
  pinMode(brPin, INPUT);
  pinMode(frPin, INPUT);
  pinMode(flPin, INPUT);

  Serial.begin(115200);

  // sets the robot to drive forward and go 60% power
  move.power(255); // 153 for 60%, 204 for 80%
  delay(5000);
  move.forward();
}
  
//FIXME:: should make the Sonic sensor, Ir sensor, and drive sequence their own functions to help keep code clean and organized.
void loop() 
{
  /**
   * Sonic sensor sequence
   */
  // sensor num 1, 90 degree left sensor. Sensor looks for obstacles 90 deg. on left side 
  if(sensors[0].ping_cm() < threshold_max && sensors[0].ping_cm() > 0){
    checkEscape();
    move.left();
    delay(delay_time);
  }
  // sensor num 6, 90 degree right sensor. Sensor looks for obstacles 90 deg. on right side
  else if(sensors[5].ping_cm() < threshold_max && sensors[5].ping_cm() > 0) {
    checkEscape();
    move.right();
    delay(delay_time);
  }
  
  // sensor num 2, 45 degree left sensor. Sensor looks for obstacles 30 deg. on left side
  if(sensors[1].ping_cm() <= threshold_max && sensors[1].ping_cm() > 0){
    checkEscape();
    move.left();
    delay(delay_time);
  }
  // sensor num 5, 45 degree right sensor. Sensor looks for obstacles 30 deg. on right side
  else if(sensors[4].ping_cm() <= threshold_max && sensors[4].ping_cm() > 0){
    checkEscape();
    move.right();
    delay(delay_time);
  }

  // if sensor 2 and 3 both see it, full power
  if(sensors[2].ping_cm() <= threshold_max && sensors[2].ping_cm() > 0 && sensors[3].ping_cm() <= threshold_max && sensors[3].ping_cm() > 0){
    checkEscape();
    move.forward(); 
    delay(delay_time);
  }
  else if(sensors[2].ping_cm() <= threshold_max && sensors[2].ping_cm() > 0){
    checkEscape();
    move.left();
    delay(delay_time);
  }
  else if(sensors[3].ping_cm() <= threshold_max && sensors[3].ping_cm() > 0){
    checkEscape();
    move.right();
    delay(delay_time);
  }
  
  // if back sensor sees it, turn around
  if(sensors[6].ping_cm() <= threshold_back && sensors[6].ping_cm() > 0){
    checkEscape();
    //move.turn(180, 'R');
  }
  checkEscape();
}

void checkEscape()
{
  digitalWrite(blPin, HIGH); // move these to escape()?
  digitalWrite(brPin, HIGH);
  digitalWrite(frPin, HIGH);
  digitalWrite(flPin, HIGH);

  bool escapeRight = LOW;
  bool escapeLeft = LOW;
  
  // if either of the front sensors see the line, robot must escape backwards
  if(digitalRead(frPin) == 0){
    escapeLeft = HIGH;
    move.backward();
    delay(200); 
  }
  else if(digitalRead(flPin) == 0){
    escapeRight = HIGH;
    move.backward();
    delay(200); 
  }
  // if either of the back sensors see the line, robot must escape forwards
  else if(digitalRead(brPin) == 0){
    escapeLeft = HIGH;
    move.forward();
    delay(200); 
  }
  else if(digitalRead(blPin) == 0){
    escapeRight = HIGH;
    move.forward();
    delay(200); 
  }
  // perform escape sequence if line is detected
  if(escapeRight == HIGH || escapeLeft == HIGH){
    escape(escapeRight, escapeLeft);
  }
  else{
    move.forward(); 
  }
}

void escape(bool escapeRight, bool escapeLeft)
{ 
  // number of seconds to turn
  int turn = random(2,4);
  move.power(204);
  if(escapeRight == HIGH){
    move.right();
  }
  else if(escapeLeft == HIGH){
    move.left();
  }
  delay(turn * 350); // 350 = 1 sec?
}
