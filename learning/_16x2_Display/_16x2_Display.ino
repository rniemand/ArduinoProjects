#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
  LiquidCrystal(uint8_t rs, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
*/

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 1);                                               // <-- Works, but can't print to line 2
  //lcd.begin(16, 2);                                             // <-- Doesn't work, blank screen
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}
