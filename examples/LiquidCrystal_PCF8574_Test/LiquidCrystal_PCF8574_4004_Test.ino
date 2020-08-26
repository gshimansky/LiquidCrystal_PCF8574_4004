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
  // Print a message to the LCD.
  lcd.print("0123456789012345678901234567890123456789");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  lcd.setCursor(0, 2);
  lcd.print(millis() / 100);
  lcd.setCursor(0, 3);
  lcd.print(millis() / 10);
  delay(250);
  Serial.println(millis() / 1000);
}
