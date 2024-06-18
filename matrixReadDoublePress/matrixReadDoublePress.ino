/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender
  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.
  created 21 Aug 2010
  by Michael Margolis
  This code is in the public domain.
  adapted from Ethernet library examples
*/

/* PINOUT
looking at my button matrix with pins on the left
D1
D6
D2
D0
Then rest of pins to breadboard (for voltage ladder)
*/

using namespace std;

#include <string.h>
#include <iostream>

#define voltageReadPin A0

#define MILLIS_TO_SEC 100
#define COOLDOWN_MILLIS 2000

bool colFlags[] = {false, false, false, false};
int columnPins[] = {16, 4, 12, 5}; // actual pins
// int columnPins[] = {D0, D2, D6, D1}; // attempt at converting (NOT TESTED)

int baseNum = 65; // ASCII "A"

// char row0[4] = {'1', '2', '3', 'A'};
// char row1[4] = {'4', '5', '6', 'B'};
// char row2[4] = {'7', '8', '9', 'C'};
// // char row3[4] = {'*', '0', '#', 'D' };
// char row3[4] = {'*', '0', '#', '!'};
// char* matrix[4] = {row0, row1, row2, row3};

int readCol(int colPin) {
  digitalWrite(colPin, LOW);
  int rawReading = analogRead(voltageReadPin);
  float voltage = rawReading * (3.3 / 1023.0);
  digitalWrite(colPin, HIGH);

  if (voltage < 0.4)
    return 0;
  if (voltage < 1.2)
    return 1;
  if (voltage < 2.0)
    return 2;
  if (voltage < 3)
    return 3;
  return -1;
}

char readMatrix(int last[2], unsigned long &lTime) {
  for (int col = 0; col < 4; col++) {
    int colPin = columnPins[col];
    int row = readCol(colPin);
    if (row != -1 && !colFlags[col]) {
      colFlags[col] = true;
      Serial.println(millis() - lTime);
      int toReturn = -1;
      if(millis() - lTime <= COOLDOWN_MILLIS && last[0] == col && last[1] == row) {
        toReturn = baseNum + (row * 8) + 1 + (col * 2);
      }
      lTime = millis();
      last[0] = col;
      last[1] = row;
      if(toReturn != -1) {
        return toReturn;
      }
      if ((baseNum + (row * 8) + (col * 2)) <= 90) {
        return (baseNum + (row * 8) + (col * 2));
      } else {
        return 0;
      }
      // if(millis() - lTime >= COOLDOWN_MILLIS) {
      //   return (baseNum + (row * 8) + (col * 2));
      // } else {
      //   lTime = millis();
      //   return (baseNum + (row * 8) + 1 + (col * 2));
      // }
    }
    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  return '\0';
}

void setup() {
  Serial.begin(115200);
  pinMode(voltageReadPin, INPUT);
  for (int colPin : columnPins) {
    pinMode(colPin, OUTPUT);
    digitalWrite(colPin, HIGH);
  }
}

unsigned long lastButtonTime = millis();
int lastButton[2] = {-1, -1};

void loop() {
  char curVal = readMatrix(lastButton, lastButtonTime);
  if(curVal != '\0')
  {
    Serial.println(curVal);
    delay(100);
  }
}

// int num = 0;
// int coolThing[2] = {-1, -1};

// void weirdFunc(int list[2], int &nm) {
//   list[0] = 5;
//   list[1] = 2;
//   nm = 234;
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Hello!");
//   Serial.println(num);
//   num = 2;
//   weirdFunc(coolThing, num);
// }

// void loop() {
//   Serial.print("0: ");
//   Serial.println(coolThing[0]);
//   Serial.print("1: ");
//   Serial.println(coolThing[1]);
//   Serial.print("num: ");
//   Serial.println(num);
//   delay(1000);
// }