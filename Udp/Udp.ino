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
using namespace std;

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <iostream>
#include <WiFiCredentials.h>

// #ifndef STASSID
// #define STASSID "wemosD1Mini"
// #define STAPSK "wemosD1Mini"
// #endif

const int buttonPin = 16; // D0
int buttonState = 0;
bool buttonFlag = false;

const int ledPin = 5;  // D1, 220ohm resistor

const char* ssid = SSID;
const char* password = PSK;

IPAddress apIP(192, 168, 1, 1);
IPAddress myIP(192, 168, 1, 2);
IPAddress wemiBIP(192, 168, 1, 3);

#define MILLIS_TO_SEC 1000

const char* hostname = "wemi-a";
// const char* destination_host = "wemi-b";
const char* destination_IP = "192.168.1.3";

unsigned int localPort = 8888;  // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

char ack[] = "acknowledged\r\n";
char pressed[] = "pressed\r\n";
char released[] = "released\r\n";

WiFiUDP Udp;

void sendPacket(const char* addresss, string msg) {
  Udp.beginPacket(addresss, 8888);
  Udp.write(msg.c_str(), msg.size());
  Udp.endPacket();
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

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

// unsigned long startTime = millis();

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && !buttonFlag) {
    sendPacket(destination_IP, "pressed\r\n");
    buttonFlag = true;
  } else {
    if (buttonState == LOW && buttonFlag) {
      sendPacket(destination_IP, "released\r\n");
      buttonFlag = false;
    }
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    // Serial.println("Contents:");
    // Serial.println(packetBuffer);

    if (memcmp(&ack, &packetBuffer, sizeof(ack) / sizeof(char)) == 0) {
      Serial.println("MEMCMP ACK");
    } else {
      if (memcmp(&pressed, &packetBuffer, sizeof(pressed) / sizeof(char)) == 0) {
        Serial.println("LED ON");
        digitalWrite(ledPin, HIGH);
      } else {
        if (memcmp(&released, &packetBuffer, sizeof(released) / sizeof(char)) == 0) {
          Serial.println("LED OFF");
          digitalWrite(ledPin, LOW);
        } else {
          Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());
          Serial.println("Contents:");
          Serial.println(packetBuffer);
        }
      }
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      Udp.endPacket();
    }
  }
}
// } else {
//   if(memcmp(&released, &packetBuffer, sizeof(ack)/sizeof(char)) == 0)
//   {
//     Serial.println("LED OFF");
//     digitalWrite(ledPin, LOW);
//   }

// char ack[] = "acknowledged\r\n";
// bool equal = true;
// for(int i=0; i<sizeof(ack)/sizeof(char); i++) {
//   if(packetBuffer[i] != ack[i]) {
//     equal = false;
//   }
// }
// if(!equal) {
//   Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());
//   Serial.println("Contents:");
//   Serial.println(packetBuffer);

//   // send a reply, to the IP address and port that sent us the packet we received
//   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//   Udp.write(ReplyBuffer);
//   Udp.endPacket();
// }
// else {
//   Serial.println("ACK recieved.");
// }
// }
// // Serial.println(WiFi.localIP());
// // Serial.println(WiFi.getHostname());
// // if there's data available, read a packet
// int packetSize = Udp.parsePacket();
// if (packetSize) {
//   Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());

//   // read the packet into packetBufffer
//   int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
//   packetBuffer[n] = 0;
//   // Serial.println("Contents:");
//   // Serial.println(packetBuffer);

//   char ack[] = "acknowledged\r\n";
//   bool equal = true;
//   for(int i=0; i<sizeof(ack)/sizeof(char); i++) {
//     if(packetBuffer[i] != ack[i]) {
//       equal = false;
//     }
//   }
//   if(!equal) {
//     Serial.println("Contents:");
//     Serial.println(packetBuffer);

//     // send a reply, to the IP address and port that sent us the packet we received
//     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//     Udp.write(ReplyBuffer);
//     Udp.endPacket();
//   } else {
//     Serial.println("ACK recieved.");
//   }
// }
// }

/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/
