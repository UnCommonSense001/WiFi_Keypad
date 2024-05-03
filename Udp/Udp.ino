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

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <iostream>
#include <WiFiCredentials.h>

#define voltageReadPin A0

const char* ssid = SSID;
const char* password = PSK;

IPAddress apIP(192, 168, 1, 1);
IPAddress myIP(192, 168, 1, 2);
IPAddress wemiBIP(192, 168, 1, 3);

#define MILLIS_TO_SEC 1000

const char* hostname = "wemi-a";
const char* destination_IP = "192.168.1.3";

unsigned int localPort = 8888;  // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

char ack[] = "acknowledged\r\n";
char pressed[] = "pressed\r\n";
char released[] = "released\r\n";

WiFiUDP Udp;

bool colFlags[] = {false, false, false, false};
int columnPins[] = {16, 4, 12, 5}; // actual pins
// int columnPins[] = {D0, D2, D6, D1}; // attempt at converting (NOT TESTED)

/* === WRONG ON PURPOSE FOR TESTING === */
// int columnPins[] = {16, 16, 16, 16}; // dummy for test
/* ==================================== */

char row0[4] = { '1', '2', '3', 'A' };
char row1[4] = {'4', '5', '6', 'B' };
char row2[4] = {'7', '8', '9', 'C' };
char row3[4] = {'*', '0', '#', 'D' };
char* matrix[4] = {row0, row1, row2, row3};

void sendPacket(const char* addresss, string msg) {
  Udp.beginPacket(addresss, 8888);
  Udp.write(msg.c_str(), msg.size());
  Udp.endPacket();
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

char readMatrix() {
  for (int col = 0; col < sizeof row0 / sizeof row0[0]; col++) {
    int colPin = columnPins[col];
    int row = readCol(colPin);
    if (row != -1 && !colFlags[col]) {
      colFlags[col] = true;
      return matrix[row][col];
    }
    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  return '\0';
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

  delay(3000);
  Serial.println("sending startup 1!");
  sendPacket(destination_IP, "startup 1!\r\n");
  delay(1000);
  Serial.println("sending startup 2!!");
  sendPacket(destination_IP, "startup 2!!\r\n");
  delay(1000);
  Serial.println("sending startup 3!!!");
  sendPacket(destination_IP, "startup 3!!!\r\n");
  Serial.println("ALL 3 STARTUPS SENT");
}

unsigned long startTime = millis();

void loop() {
  unsigned long curTime = millis();

  // WARNING: CAUSES WiFi DISCONNECTION AFTER FIRST TWO TESTS IN SETUP - char curVal = readMatrix(); // this causes WiFi to disconnect
  // WARNING: CAUSES WiFi DISCONNECTION AFTER FIRST TWO TESTS IN SETUP - readCol(columnPins[0]); // and this too
  // WARNING: CAUSES WiFi DISCONNECTION AFTER FIRST TWO TESTS IN SETUP - analogRead(voltageReadPin);
  digitalWrite(columnPins[0], LOW); // OK
  // digitalWrite(columnPins[0], HIGH); // OK

  if(curTime - startTime >= 1 * MILLIS_TO_SEC)
  {
    sendPacket(destination_IP, "hello!!!\r\n");
    startTime = curTime;
  }

  // char curVal = readMatrix();
  // if (!curVal == '\0')
  // {
  //   Serial.println(curVal);
  //   sendPacket(destination_IP, "hello!!!\r\n");
  // }
}

// unsigned long startTime = millis();

// void loop() {
//   unsigned long curTime = millis();

//   if(curTime - startTime >= 1 * MILLIS_TO_SEC)
//   {
//     Serial.println("sending message");
//     sendPacket(destination_IP, "message\r\n");
//     startTime = curTime;
//   }
  // buttonState = digitalRead(buttonPin);
  // if (buttonState == HIGH && !buttonFlag) {
  //   Serial.println("PRESSED");
  //   sendPacket(destination_IP, "pressed\r\n");
  //   buttonFlag = true;
  // } else if (buttonState == LOW && buttonFlag) {
  //   Serial.println("RELEASED");
  //   sendPacket(destination_IP, "released\r\n");
  //   buttonFlag = false;
  // }

  // if there's data available, read a packet
  // int packetSize = Udp.parsePacket();
  // if (packetSize) {
  //   // read the packet into packetBufffer
  //   int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  //   packetBuffer[n] = 0;
  //   bool respond = true;

  //   if (memcmp(&ack, &packetBuffer, sizeof(ack) / sizeof(char)) == 0) {
  //     Serial.println("MEMCMP ACK");
  //     respond = false;
  //   } else if (memcmp(&pressed, &packetBuffer, sizeof(pressed) / sizeof(char)) == 0) {
  //     Serial.println("LED ON");
  //     digitalWrite(ledPin, HIGH);        
  //   } else if (memcmp(&released, &packetBuffer, sizeof(released) / sizeof(char)) == 0) {
  //     Serial.println("LED OFF");
  //     digitalWrite(ledPin, LOW);
  //   } else {
  //     Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());
  //     Serial.println("Contents:");
  //     Serial.println(packetBuffer);
  //   }
  //   if(respond) {
  //     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  //     Udp.write(ReplyBuffer);
  //     Udp.endPacket();
  //   }
  // }
// }

/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/