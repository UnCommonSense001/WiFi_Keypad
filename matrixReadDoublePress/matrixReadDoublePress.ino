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
#define TIMEOUT 1500

bool colFlags[] = {false, false, false, false};
int columnPins[] = {16, 4, 12, 5}; // actual pins
// int columnPins[] = {D0, D2, D6, D1}; // attempt at converting (NOT TESTED)

int baseNum = 65; // ASCII "A"
int maxNum = 90; // ASCII "Z"

//WITH TIME 
struct button_t {
  int col;
  int row;
  unsigned long time;
  char character;
};

button_t lastButton;

char msgToSend[40];

void setButton(button_t &b, int bCol, int bRow, unsigned long &bTime, char bCharacter) {
  b.col = bCol;
  b.row = bRow;
  b.time = bTime;
  b.character = bCharacter;
}

void setButton(button_t &bDest, button_t &bOrigin) {
  bDest.col = bOrigin.col;
  bDest.row = bOrigin.row;
  bDest.time = bOrigin.time;
  bDest.character = bOrigin.character;
}

// bool compareRowsAndCols(button_t &b1, button_t &b2) {
//   if(b1.col == b2.col && b1.row == b2.row) {
//     return true;
//   }
//   return false;
// }

bool compareButtons(button_t &b1, button_t &b2) {
  if(b1.character == b2.character) {
    return true;
  }
  return false;
}

bool isValid(button_t &b1) {
  if(b1.character >= baseNum && b1.character <= maxNum) {
    return true;
  }
  return false;
}

void clearButton(button_t &b1) {
  unsigned long time = millis();
  setButton(b1, -1, -1, time, 0);
}

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

void readMatrix(button_t &lButton) {
  for (int col = 0; col < 4; col++) {
    int colPin = columnPins[col];
    int row = readCol(colPin);
    if (row != -1 && !colFlags[col]) {
      colFlags[col] = true;

      unsigned long curTime = millis();
      button_t curButton;
      char unshiftedChar = baseNum + (row * 8) + (col * 2);
      char shiftedChar = unshiftedChar + 1;
      setButton(curButton, col, row, curTime, unshiftedChar);

      if(compareButtons(lButton, curButton) && isValid(curButton) && curButton.time - lButton.time <= TIMEOUT) {
        Serial.println(char(lButton.character + 1));
        clearButton(lButton);
      } else if(isValid(lButton) && isValid(curButton)) {
        Serial.println(lButton.character);
        setButton(lButton, curButton);
      } else if(isValid(lButton) && !isValid(curButton)) {
        Serial.println(lButton.character);
        clearButton(lButton);
      } else {
        setButton(lButton, curButton);
      }
    }
    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  // return '\0';
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
  readMatrix(lastButton);
  unsigned long curTime = millis();
  if(isValid(lastButton) && curTime - lastButton.time >= TIMEOUT) {
    Serial.println(lastButton.character);
    clearButton(lastButton);
  }
  delay(85);
}