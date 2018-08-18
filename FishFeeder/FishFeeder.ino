//
// This is a simple Arduino program to feed my fish: Blackburn.
// The program drives a servo which is attached to a small bottle
// that contains fish food.
// Every 12 hours the program turn the servo 2 times, with the 
// intention to simulate serving food by turning your arm wrist.
// The program is not intended to be power efficient or precise.
//

#include "MeOrion.h"

const unsigned long TWELVE_HOURS = (12UL * 60 * 60 * 1000);

MePort port(PORT6);
Servo servo;

void setup()
{
   servo.attach(port.pin1());
   servo.write(180);
}

void ShakeBottle()
{
  servo.write(180);
  delay(500);
  servo.write(140);
  delay(500);
  servo.write(180);
}

void ServeFood()
{
  servo.write(180);
  delay(1000);
  servo.write(10);
  delay(1000);
  servo.write(180);
}

void loop()
{
   ShakeBottle();
   ShakeBottle();
   ServeFood();
   ServeFood();
   
   delay(TWELVE_HOURS);
}

