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

//WITH TIME 
struct button_t {
  int col;
  int row;
  bool shifted;
  unsigned long time;
};

button_t lastButton;

char msgToSend[40];

void setButton(button_t &b, int bCol, int bRow, bool bShifted, unsigned long &bTime) {
  b.col = bCol;
  b.row = bRow;
  b.shifted = bShifted;
  b.time = bTime;
}

void setButton(button_t &bDest, button_t &bOrigin) {
  bDest.col = bOrigin.col;
  bDest.row = bOrigin.row;
  bDest.shifted = bOrigin.shifted;
  bDest.time = bOrigin.time;
}

bool compareRowsAndCols(button_t &b1, button_t &b2) {
  if(b1.col == b2.col && b1.row == b2.row) {
    return true;
  }
  return false;
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
      setButton(curButton, col, row, false, curTime);

      char unshiftedChar = baseNum + (curButton.row * 8) + (curButton.col * 2);
      char shiftedChar = unshiftedChar + 1;

      Serial.println(unshiftedChar);
      Serial.println(shiftedChar);

      if(compareRowsAndCols(lButton, curButton) && curTime - lButton.time <= 2000) {
        Serial.println("DBL");
      }

      setButton(lButton, curButton);
      
      // char lastChar = baseNum + (lButton.row * 8) + (lButton.col * 2);

      // if(lButton.row == )


      // if(last[0] == col && last[1] == row && millis() - lTime <= COOLDOWN_MILLIS) {
      //   expTime = millis();
      //   int charInt = baseNum + (row * 8) + 1 + (col * 2)
      //   if(charInt <= 90) {
      //     Serial.println(char(charInt);
      //     last[0] = -1;
      //     last[1] = -1;
      //     lChar = 0;
      //     // double press (BOTH LETTERS)
      //   }
      // } else {
      //   // single press
      //   int charInt = baseNum + (row * 8) + (col * 2)
      //   if(charInt <= 90) {
      //     // letter
      //     Serial.println(char(baseNum + (row * 8) + (col * 2)));
      //   }
      // }


      // if(unshiftedChar == lastChar) {
      //   Serial.println(shiftedChar);
      //   last[0] = -1;
      //   last[1] = -1;
      //   lTime = millis();
      // } else {
      //   if 
      // }

      // if(col == last[0] and row == last[1] && millis() - lTime <= 2000) {
      //   // double press in amount of time
      //   Serial.println(shiftedChar);
      //   last[0] = -1;
      //   last[1] = -1;
      //   lTime = millis();
      // } else if (col != last[0] || row != last[1] || (millis() - lTime > 2000)) {
      //   // double press in longer than amount of time or a different button
      //   // should reset
      //   Serial.println(unshiftedChar);
      //   last[0] = col;
      //   last[1] = row;
      //   lTime = millis();
      // }

      // if(col == last[0] and row == last[1]) {
      //   // double press over however long
      //   if(millis() - lTime <= 2000) {
      //     // double press within 2 sec
      //     System.out.println(char(charInt + 1));
      //   }
      //   last[0] = -1;
      //   last[1] = -1;
      //   lTime = millis();
      // }

      // last[0] = col;
      // last[1] = row;
      // lTime = millis();



      // // Serial.println(millis() - lTime);
      // if(millis() - lTime <= COOLDOWN_MILLIS && last[0] == col && last[1] == row) {
      //   Serial.println("DBL");
      //   if((baseNum + (row * 8) + 1 + (col * 2)) <= 90) {
      //     char toReturn = (baseNum + (row * 8) + 1 + (col * 2));
      //     Serial.print("--: ");
      //     Serial.println(toReturn);
      //     strcat(msgToSend, &toReturn);

      //   }
      // }
      // if(toReturn != -1) {
        // char charToReturn = char(toReturn);
        // Serial.println(charToReturn);
        // strcat(msgToSend, &charToReturn);
      // }
    //   lTime = millis();
    //   last[0] = col;
    //   last[1] = row;
    //   if ((baseNum + (row * 8) + (col * 2)) <= 90) {
    //     // return (baseNum + (row * 8) + (col * 2));
    //     char charToReturn = char(baseNum + (row * 8) + (col * 2));
    //     Serial.println(charToReturn);
    //     strcat(msgToSend, &charToReturn);
    //   } else {
    //     return 0;
    //   }
    // }
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
  // Serial.println(msgToSend);
  delay(100);
  // if(curVal != '\0')
  // {
  //   Serial.println(curVal);
  //   Serial.println(msgToSend);
  //   delay(100);
  // }
}