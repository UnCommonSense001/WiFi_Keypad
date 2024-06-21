#include <LiquidCrystal_I2C.h>

/* PINOUT
5V - VCC
G - GND
D2 - SDA
D1 - SCL
*/

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27, and a 20 char wide, 4 line display (super big)

void setup()
{
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("AAAAAAAAAAAAAAAAAAAZB");
  lcd.setCursor(0, 1); // x, y
  lcd.print("BBBBBBBBBBBBBBBBBBBZC");
  lcd.setCursor(0, 2);
  lcd.print("CCCCCCCCCCCCCCCCCCCZD");
  lcd.setCursor(0, 3);
  lcd.print("DDDDDDDDDDDDDDDDDDDZE");

  // count up!
  // for(int i=0; i<=9999999999999999; i+=99)
  // {
  //   lcd.clear();
  //   lcd.print(i);
  //   delay(40);
  // }
}

void loop(){}