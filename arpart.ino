/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
Servo myservo2;
int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(A0,INPUT);
  myservo2.attach(10);
  pinMode(A1,INPUT);
  Serial.begin(9600);
}

void loop() {
  int read = digitalRead(5);
 if(read == 1){
   myservo.write(90);
 }else{
   myservo.write(0);
 }

  int read1 = digitalRead(3);
 if(read1 == 1){
   myservo2.write(90);
 }else{
   myservo2.write(0);
 }
 
}
