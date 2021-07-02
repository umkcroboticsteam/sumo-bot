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
//FIXME:: should add inilizaing move, Ir, and sonic sensor in there own class. This will help keep this code clean and organize.
int turn = 0;
bool escapeRight = LOW;
bool escapeLeft = LOW;
int flPin = 50;          
int blPin = 52;
int frPin = 51; 
int brPin = 53;

int red_light_pin= 27;
int green_light_pin = 26;
int blue_light_pin = 28;

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

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
  NewPing( 2, 2, 200), //sensor 1 left 90 degree
  NewPing( 3, 3, 200), //sensor 2 left 45 degree
  NewPing( 4, 4, 200), //sensor 3 left 0 degree
  NewPing( 5, 5, 200), //sensor 4 right 0 degree
  NewPing( 6, 6, 200), //sensor 5 right 45 degree
  NewPing( 7, 7, 200), //sensor 6 right 90 degree
  NewPing( 9, 8, 200),//sensor 7 back 180 degree
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

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);


  //sets the robot to drive foward and go 60% power
  move.power(255); // 153 for 60%, 204 for 80%
  delay(5000);
  move.forward();
}
  
//FIXME:: should make the Sonic sensor, Ir sensor, and drive sequace there own functions to help keep code clean and organize.
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
  //RGB_color(255,0,0); // red
 }
 // sensor num 6, 90 degree right sensor. Sensor looks for obstavles 90 deg. on right side
 if(sensors[5].ping_cm() < threshold_max && sensors[5].ping_cm() > 0) {
  checkEscape();
  move.right();
  delay(delay_time);
  //RGB_color(0,0,255); // blue
  }
  // sensor num 2, 45 degree left sensor. Sensor looks for obstacle 30 deg. on left side
  if(sensors[1].ping_cm() <= threshold_max && sensors[1].ping_cm() > 0){
    checkEscape();
    move.left();
    delay(delay_time);
    //RGB_color(255,255,0); //yellow
  }
  // sensor num 5, 45 degree right sensor. Sensor looks for obstavle 30 deg. on right side
  if(sensors[4].ping_cm() <= threshold_max && sensors[4].ping_cm() > 0){
    checkEscape();
    move.right();
    delay(delay_time);
    //RGB_color(0,255,255); // cyan
  }

//if sensor 2 and 3 both see it, full power
  if((sensors[2].ping_cm() <= threshold_max) && (sensors[2].ping_cm() > 0) && (sensors[3].ping_cm() <= threshold_max) && (sensors[3].ping_cm() > 0)){
    checkEscape();
    move.forward(); 
    delay(delay_time);
    //RGB_color(255,255,255); //white
  }
  else if(sensors[2].ping_cm() <= threshold_max && sensors[2].ping_cm() > 0){
    checkEscape();
    move.left();
    delay(delay_time);
    //RGB_color(0,255,0); //green
  }
  else if(sensors[3].ping_cm() <= threshold_max && sensors[3].ping_cm() > 0){
    checkEscape();
    move.right();
    delay(delay_time);
    //RGB_color(255,0,255); //purple
  }
  if((sensors[6].ping_cm() <= threshold_back) && (sensors[6].ping_cm() > 0)){
    checkEscape();
    //move.turn(180, 'R');
   //RGB_color(0,0,0); //off
  }
  checkEscape();


/**
 * Escape Sequence
 */

//printPins();
    // search
    if(escapeLeft != HIGH && escapeRight != HIGH) 
    {
      checkEscape();
      move.forward(); 
    }

}
void checkEscape(){
  
digitalWrite(blPin, HIGH);
digitalWrite(brPin, HIGH);
digitalWrite(frPin, HIGH);
digitalWrite(flPin, HIGH);

  // if either of the front sensors see the line, robot must escape
if(digitalRead(frPin) == 0 ) 
{
    escapeLeft = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4); 
    move.backward();
    delay(200); 
}
else if(digitalRead(flPin) == 0)
{
    escapeRight = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4);
    move.backward();
    delay(200); 
}
else if(digitalRead(brPin) == 0)
{
  escapeLeft = HIGH;
  turn = random(2,4);
  move.forward();
  delay(200); 
}
else if(digitalRead(blPin) == 0)
{
  escapeRight = HIGH;
  turn = random(2,4);
  move.forward();
  delay(200); 
}
if(escapeRight == HIGH || escapeLeft == HIGH){
  escape();
}
else{
      // search
    if(escapeLeft != HIGH && escapeRight != HIGH) 
    {
      move.forward(); 
    }
}
}

void escape(){
  move.power(204);
  if(escapeRight == HIGH){
    doEscapeRight();
  }
  else if(escapeLeft == HIGH){
    doEscapeLeft();
  }
}

void doEscapeRight(){

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
}
void doEscapeLeft(){
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

}
