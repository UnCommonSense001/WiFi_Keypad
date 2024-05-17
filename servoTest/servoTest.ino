/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 28 May 2015
  by Michael C. Miller
  modified 8 Nov 2013
  by Scott Fitzgerald

  http://arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myServo;

int pos = 0;

void changePosition(int num, bool wrap = false) {
  pos += num;
  if((pos > 180 && !wrap) || (pos < 0 && wrap)) {
    pos = 180;
  }
  if((pos < 0 && !wrap) || (pos > 180 && wrap)) {
    pos = 0;
  }
}

void setup() {
  myServo.attach(D4);  // attaches the servo on D4
  myServo.write(0);
  delay(1500);
}

void loop() {
  myServo.write(pos);
  delay(5);
  changePosition(1, true);

  // for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myServo.write(pos);  // tell servo to go to position in variable 'pos'
  //   delay(15);           // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
  //   myServo.write(pos);                  // tell servo to go to position in variable 'pos'
  //   delay(15);                           // waits 15ms for the servo to reach the position
  // }
}
