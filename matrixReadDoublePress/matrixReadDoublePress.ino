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

// #define MILLIS_TO_SEC 100
#define COOLDOWN_MILLIS 2000

bool colFlags[] = {false, false, false, false};
int columnPins[] = {16, 4, 12, 5}; // actual pins
// int columnPins[] = {D0, D2, D6, D1}; // attempt at converting (NOT TESTED)

int baseNum = 65; // ASCII "A"

unsigned long lastButtonTime = millis();
int lastButton[2] = {-1, -1};
char msgToSend[40];

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
      if(millis() - lTime <= COOLDOWN_MILLISs && last[0] == col && last[1] == row && baseNum + (row * 8) + 1 + (col * 2) <= 90) {
        char toReturn = (baseNum + (row * 8) + 1 + (col * 2));
        Serial.print("--: ");
        Serial.println(toReturn);
        strcat(msgToSend, &toReturn);
      }
      // if(toReturn != -1) {
        // char charToReturn = char(toReturn);
        // Serial.println(charToReturn);
        // strcat(msgToSend, &charToReturn);
      // }
      lTime = millis();
      last[0] = col;
      last[1] = row;
      if ((baseNum + (row * 8) + (col * 2)) <= 90) {
        // return (baseNum + (row * 8) + (col * 2));
        char charToReturn = char(baseNum + (row * 8) + (col * 2));
        Serial.println(charToReturn);
        strcat(msgToSend, &charToReturn);
      } else {
        return 0;
      }
    }
    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  return '\0';
}

void setup() {
  Serial.begin(115200);
  // strcpy(msgToSend, &coolChar); // dest, src
  pinMode(voltageReadPin, INPUT);
  for (int colPin : columnPins) {
    pinMode(colPin, OUTPUT);
    digitalWrite(colPin, HIGH);
  }
}

void loop() {
  char curVal = readMatrix(lastButton, lastButtonTime);
  // Serial.println(msgToSend);
  delay(100);
  // if(curVal != '\0')
  // {
  //   Serial.println(curVal);
  //   Serial.println(msgToSend);
  //   delay(100);
  // }
}