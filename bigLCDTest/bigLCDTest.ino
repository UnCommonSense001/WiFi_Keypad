#include <LiquidCrystal_I2C.h>

/* PINOUT
5V - VCC
G - GND
D2 - SDA
D1 - SCL
*/

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27, and a 20 char wide, 4 line display (super big)

char packetBuffer[41];

void displayData(char* data) {
  Serial.println("Displaying data...");
  lcd.clear();
  lcd.setCursor(0, 0);
  if(strlen(packetBuffer) <= 20) {
    Serial.println(data);
    lcd.print(data);
  } else if(strlen(packetBuffer) <= 40) {
    Serial.println("less than 40 chars - 2lines");
    char firstLine[21];
    strncpy(firstLine, data, 20);
    firstLine[20] = 0;
    Serial.println(firstLine);
    lcd.print(firstLine);
    strncpy(data, data + 20, 20);
    data[20] = 0;
    Serial.println(data);
    lcd.setCursor(0, 1); // x, y
    lcd.print(data);
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println();
  Serial.println();
  Serial.println();
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  // strcpy(packetBuffer, "ABCDEFGHIJKLMNOPQRST"); // works fine
  strcpy(packetBuffer, "onetwothreefourfivesixseveneightnineten!");
  Serial.println(packetBuffer);
  Serial.println(strlen(packetBuffer));
  displayData(packetBuffer);
}

  // lcd.print("A23456789AB23456789B");
  // lcd.print("A23456789AB23456789BC23456789CD23456789D");
  // lcd.setCursor(0, 1); // x, y
  // lcd.print("BBBBBBBBBBBBBBBBBBBZC");
  // lcd.setCursor(0, 2);
  // lcd.print("CCCCCCCCCCCCCCCCCCCZD");
  // lcd.setCursor(0, 3);
  // lcd.print("DDDDDDDDDDDDDDDDDDDZE");

  // count up!
  // for(int i=0; i<=9999999999999999; i+=99)
  // {
  //   lcd.clear();
  //   lcd.print(i);
  //   delay(40);
  // }
// }

void loop(){}