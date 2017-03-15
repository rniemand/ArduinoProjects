#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  //pinMode(2, INPUT_PULLUP);
  //pinMode(3, INPUT_PULLUP);
  
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
