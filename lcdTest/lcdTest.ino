#include <LiquidCrystal_I2C.h>

/* PINOUT
5V - VCC
G - GND
D2 - SDA
D1 - SCL
*/
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  delay(5000);
  lcd.setCursor(0,0);
  lcd.print("Reminder to fix");
  lcd.setCursor(1, 1); // x, y
  lcd.print("the contrast!");
}


void loop()
{
}