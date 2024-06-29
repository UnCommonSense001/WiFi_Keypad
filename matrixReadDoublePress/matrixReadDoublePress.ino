/* PINOUT
looking at my button matrix with pins on the left
D7
D6
D5
D0
Then rest of pins to breadboard (for voltage ladder)
*/

using namespace std;

#include <string.h>
#include <iostream>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiCredentials.h>

#define voltageReadPin A0
#define TIMEOUT 1000

const char* ssid = SSID;
const char* password = PSK;

IPAddress apIP(192, 168, 1, 1);
IPAddress myIP(192, 168, 1, 2);
IPAddress wemiBIP(192, 168, 1, 3);

const char* hostname = "wemi-a";
const char* destination_IP = "192.168.1.3";
unsigned int localPort = 8888;

// const int MAX_MESSAGE_SIZE = UDP_TX_PACKET_MAX_SIZE;
const int MAX_MESSAGE_SIZE = 40;

// char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
char packetBuffer[41]; // limit it so that it fits on the top 2 lines of the display (2 * 20 = 40), add one for the null character at the end (40 + 1 = 41)

WiFiUDP Udp;

bool colFlags[] = {false, false, false, false};
const int columnPins[] = {D0, D5, D6, D7};

const int baseNum = 65; // ASCII "A"
const int maxNum = 90; // ASCII "Z"

struct button_t {
  int col;
  int row;
  unsigned long time;
  char character;
};

button_t lastButton;
string msgToSend = "";

void sendPacket(const char* address, string msg) {
  Udp.beginPacket(address, 8888);
  Udp.write(msg.c_str(), msg.size());
  Udp.endPacket();
}

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

      if(msgToSend.size() < MAX_MESSAGE_SIZE) {
        if(compareButtons(lButton, curButton) && isValid(curButton) && curButton.time - lButton.time <= TIMEOUT) {
          char newChar = lButton.character + 1;
          msgToSend = msgToSend + newChar;
          clearButton(lButton);
        } else if(isValid(lButton) && isValid(curButton)) {
          msgToSend = msgToSend + lButton.character;
          setButton(lButton, curButton);
        } else if(isValid(lButton) && (!isValid(curButton) || curButton.character == 93)) {
          msgToSend = msgToSend + lButton.character;
          clearButton(lButton);
        } else {
          setButton(lButton, curButton);
        }
        if(curButton.character == 93) {
          msgToSend += ' ';
        }
      }

      if(!isValid(curButton)) {
        if(curButton.character == 95) {
          clearButton(curButton);
          Serial.println("SENDING over WiFi (UDP!");
          if(msgToSend != "") {
            sendPacket(destination_IP, msgToSend);
          } else {
            sendPacket(destination_IP, "+");
          }
          delay(250);
          msgToSend = "";
        }
        else if(curButton.character == 91) {
          if (!msgToSend.empty()) {
            msgToSend.pop_back();
          }
        }
      }
    }

    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  WiFi.config(myIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);

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
    msgToSend = msgToSend + lastButton.character;
    clearButton(lastButton);
  }
  Serial.println(msgToSend.c_str());
  delay(85);
}