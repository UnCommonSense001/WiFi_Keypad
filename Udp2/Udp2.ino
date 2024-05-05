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

/* PINOUT for LCD
5V - VCC
G - GND
D2 - SDA
D1 - SCL
*/

using namespace std;

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <iostream>
#include <WiFiCredentials.h>

#include <LiquidCrystal_I2C.h>

const char* ssid = SSID;
const char* password = PSK;

IPAddress apIP(192, 168, 1, 1);
IPAddress myIP(192, 168, 1, 3);
IPAddress wemiAIP(192, 168, 1, 2);

#define MILLIS_TO_SEC 1000

const char* hostname = "wemi-b";
// const char* destination_host = "wemi-a";
const char* destination_IP = "192.168.1.2";

unsigned int localPort = 8888;  // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

char ack[] = "acknowledged\r\n";
char pressed[] = "pressed\r\n";
char released[] = "released\r\n";

WiFiUDP Udp;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void sendPacket(const char* addresss, string msg) {
  Udp.beginPacket(addresss, 8888);
  Udp.write(msg.c_str(), msg.size());
  Udp.endPacket();
}

void setup() {
  Serial.begin(115200);

  lcd.init(); // initialize the lcd
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.clear();

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
}

// unsigned long startTime = millis();

int x = 0;
int y = 0;

void loop() {
  // buttonState = digitalRead(buttonPin);
  // if(buttonState == HIGH && !buttonFlag)
  // {
  //   sendPacket(destination_IP, "pressed\r\n");
  //   buttonFlag = true;
  // } else {
  //   if(buttonState == LOW && buttonFlag)
  //   {
  //     sendPacket(destination_IP, "released\r\n");
  //     buttonFlag = false;
  //   }
  // }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    if(n == 1 && packetBuffer[0] == '!')
    {
      Serial.println("CLEAR");
      lcd.clear();
      x = 0;
      y = 0;
    } else { // CHANGE so makes the x and y in range FIRST then displays (clear if necessary (if off screen on second line))
      Serial.println(packetBuffer);
      if(x > 15 && y == 0)
      {
        x = 0;
        y = 1;
      }
      if(x > 15 && y == 1)
      {
        x = 0;
        y = 0;
        lcd.clear();
      }
      lcd.setCursor(x, y);
      lcd.print(packetBuffer);
      x++;      
      delay(50);
    }

    // Serial.println("Contents:");
    // Serial.println(packetBuffer);

    // if(memcmp(&ack, &packetBuffer, sizeof(ack)/sizeof(char)) == 0)
    // {
    //   Serial.println("MEMCMP ACK");
    // } else {
    //   if(memcmp(&pressed, &packetBuffer, sizeof(pressed)/sizeof(char)) == 0)
    //   {
    //     Serial.println("LED ON");
    //     digitalWrite(ledPin, HIGH);
    //   } else {
    //     if(memcmp(&released, & packetBuffer, sizeof(released)/sizeof(char)) == 0)
    //     {
    //       Serial.println("LED OFF");
    //       digitalWrite(ledPin, LOW);
    //     } else {
    //       Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());
    //       Serial.println("Contents:");
    //       Serial.println(packetBuffer);
    //     }
    //   }
    //   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //   Udp.write(ReplyBuffer);
    //   Udp.endPacket();
    // }
  }
}

/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/
