#include <Move.h>
#include <Sight.h>
#include <Surface.h>
#include <NewPing.h>

/**
 * Globals
 */
Move move;
Sight sight;
Surface surface;
int turn = 0;
bool escapeRight = LOW;
bool escapeLeft = LOW;
int flPin = 50; 
int blPin = 52;
int frPin = 51; 
int brPin = 53;

// This prints out what each IR sensor is reading
void printPins()
{
  Serial.print("Back left pin reading: ");
  Serial.println(digitalRead(blPin));
 
  Serial.print("Back right pin reading: ");
  Serial.println(digitalRead(brPin));

  Serial.print("Front right pin reading: ");
  Serial.println(digitalRead(frPin));

  Serial.print("Front left pin reading: ");
  Serial.println(digitalRead(flPin));
  //delay(1000); // use only for debugging. dont run with testing on field
}

NewPing sensors[7] = {
  NewPing( 6, 6, 200),
  NewPing( 9, 9, 200),
  NewPing( 16, 16, 200),
  NewPing( 13, 13, 200),
  NewPing( 14, 14, 200),
  NewPing( 15, 15, 200),
  NewPing( 10, 10, 200),
};

int threshold_min = 20; // [cm] threshold distance to stop motors for sensor 3 and 4 which looks straight ahead
int threshold_max = 35; // [cm] threshold distance to stop motors for sensor 3 and 4 which looks straight ahead
int threshold_30 = 35; // [cm] threshold distance to stop motors for sensor 2 and 5 which look with 30 degree angles
int threshold_90 = 35; // [cm] threshold distance to stop motors for sensor 1 and 6 which look with 90 degree angles


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


  //sets the robot to drive foward and go 60% power
  move.power(153);
  move.forward();
}
  

void loop() 
{
/**
 * Sonic sensor sequence
 */
 // sensor num 1, 90 degree left sensor. Sensor looks for obstacles 90 deg. on left side 
 if(sensors[0].ping_cm() < threshold_90 && sensors[0].ping_cm() > 0){
  move.right();
  delay(100);
 }
 // sensor num 6, 90 degree right sensor. Sensor looks for obstavles 90 deg. on right side
 else{
  if(sensors[5].ping_cm() < threshold_90 && sensors[5].ping_cm() > 0) {
    move.left();
    delay(100);
  }
  // sensor num 2, 30 degree left sensor. Sensor looks for obstacle 30 deg. on left side
  else{
    if(sensors[1].ping_cm() <= threshold_30 && sensors[1].ping_cm() > 0){
      move.right();
      delay(100);
    }
    // sensor num 5, 30 degree right sensor. Sensor looks for obstavle 30 deg. on right side
    else{
      if(sensors[4].ping_cm() <= threshold_30 && sensors[4].ping_cm() > 0){
        move.left();
        delay(100);
      }
      else{
        // sensor num 3 & 4, 0 degree left side. Final angle adjustment when sensor 3 sees an object sensor 4 does not
        if(sensors[2].ping_cm() < threshold_max && (sensors[3].ping_cm() > threshold_max || sensors[3].ping_cm() == 0)){
          move.right();
          delay(100);
        }
        // sensor num 3 & 4, 0 degree right side. Final angle adjustment when sensor 4 sees an object sensor 3 does not
        if(sensors[3].ping_cm() < threshold_max && (sensors[2].ping_cm() > threshold_max || sensors[2].ping_cm() == 0)){
          move.left();
          delay(100);
        }
        // sensor num 3 & 4, 0 degree both sensor. Robot drives forward
        if(sensors[2].ping_cm() > threshold_max && sensors[3].ping_cm() > threshold_max){
          move.forward();
        }
    }
  }
 }
  

/**
 * Escape Sequence
 */

digitalWrite(blPin, HIGH);
digitalWrite(brPin, HIGH);
digitalWrite(frPin, HIGH);
digitalWrite(flPin, HIGH);

 
//printPins();


// if either of the front sensors see the line, robot must escape
if(digitalRead(frPin) == 0 ) 
{
    escapeLeft = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4); 
    move.backward();
    delay(100); 
    
}
else if(digitalRead(flPin) == 0)
{
    escapeRight = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4);
    move.backward();
    delay(100); 
}
else if(digitalRead(brPin) == 0)
{
  escapeLeft = HIGH;
  turn = random(2,4);
  move.backward();
  delay(100); 
}
else if(digitalRead(blPin) == 0)
{
  escapeRight = HIGH;
  turn = random(2,4);
  move.backward();
  delay(100); 
}

//     escape right loop
while(escapeRight == HIGH) 
{
    if(turn > 0) 
    {
    // decrement turn iterator and move right 1 second per decrement
      turn -= 1;
      move.right();
      delay(350);
    }
    
    else
    {
    // break the loop once the turn iterator is 0
      turn = 0;
      escapeRight = LOW;
    }
}

//     escape left loop
while(escapeLeft == HIGH) 
{
      if(turn > 0) 
      {
        // decrement turn iterator and move right 1 second per decrement
        turn -= 1;
        move.left();
        delay(350);
      }

      else
      {
        // break the loop once the turn iterator is 0
        turn = 0;
        escapeLeft = LOW;
      }
}
    // search
    if(escapeLeft != HIGH && escapeRight != HIGH) 
    {
      move.forward(); 
    }
 }  
}
