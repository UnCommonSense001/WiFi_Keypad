/*
 === LCD PINOUT ===
  5V - VCC
  G - GND
  D2 - SDA
  D1 - SCL
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
// #include <iostream>
#include <LiquidCrystal_I2C.h>
#include <WiFiCredentials.h>

using namespace std;

const char* ssid = SSID;
const char* password = PSK;

IPAddress apIP(192, 168, 1, 1);
IPAddress myIP(192, 168, 1, 3);

const char* hostname = "wemi-b";
const char* destination_IP = "192.168.1.2";
unsigned int localPort = 8888;

// const int MAX_MESSAGE_SIZE = UDP_TX_PACKET_MAX_SIZE;
const int MAX_MESSAGE_SIZE = 40;

// char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
char packetBuffer[41]; // limit it so that it fits on the top 2 lines of the display (2 * 20 = 40), add one for the null character at the end (40 + 1 = 41)

WiFiUDP Udp;

LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.clear();

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  WiFi.config(myIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  bool wifiFlag = false;
  while (WiFi.status() != WL_CONNECTED) {
    if(!wifiFlag) {
      lcd.print("Connecting...");
      wifiFlag = true;
    }
    Serial.print('.');
    delay(500);
  }
  lcd.setCursor(0, 0);
  lcd.clear();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, MAX_MESSAGE_SIZE + 1);
    packetBuffer[n] = 0;
    lcd.clear();
    if(!(n == 1 && packetBuffer[0] == '+')) {
      Serial.println(packetBuffer);
      lcd.setCursor(0, 0);
      lcd.print(packetBuffer);
    }
    // if(!(n == 1 && packetBuffer[0] == '+')) {
    //   Serial.println(packetBuffer);
    //   lcd.setCursor(0, 0);
    //   lcd.print(packetBuffer);
    //   if(packetSize > 10) {
    //     lcd.setCursor(0, 1); // x, y
    //     lcd.print(packetBuffer.substring(11, 11));
    //   }
    // }
  }
}