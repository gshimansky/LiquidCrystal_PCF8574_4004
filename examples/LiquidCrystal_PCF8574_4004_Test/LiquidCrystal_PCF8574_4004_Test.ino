#include <Wire.h>
#include <LiquidCrystal_PCF8574_4004.h>

LiquidCrystal_PCF8574_4004 lcd(0x20);

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("SETUP!!!");
  Wire.begin(D2, D1);

  // set up the LCD's number of columns and rows:
  lcd.begin(40, 4);
  lcd.display();

  // Print a messages to the LCD.
  lcd.print("0123456789012345678901234567890123456789");

  String str1 = "https://github.com/";
  lcd.setCursor((40 - str1.length()) / 2, 1);
  lcd.print(str1);

  String str2 = "gshimansky/LiquidCrystal_PCF8574_4004";
  lcd.setCursor((40 - str2.length()) / 2, 2);
  lcd.print(str2);
}

void loop() {
  // print the number of seconds since reset:
  lcd.setCursor(0, 3);
  lcd.print(millis() / 1000);
  delay(250);
  Serial.println(millis() / 1000);
}
