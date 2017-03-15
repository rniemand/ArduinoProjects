// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C   lcd(0x27, 16, 2);
RTC_DS3231          rtc;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char monthsOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void setup () {
  Wire.begin();
  Serial.begin(9600);

  delay(50); // wait for console opening

  setupLcd();
  setupRtc();

  // setTime(2017, 3, 15, 21, 27, 0);
  
}

void loop () {
  DateTime now = rtc.now();
  
  //lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(now.day() < 10 ? "0" + String(now.day()) : now.day());
  lcd.print(" ");
  lcd.print(monthsOfTheYear[now.month() - 1]);
  
  lcd.setCursor(0, 1);
  lcd.print(now.hour() < 10 ? "0" + String(now.hour()) : now.hour());
  lcd.print(":");
  lcd.print(now.minute() < 10 ? "0" + String(now.minute()) : now.minute());
  lcd.print(":");
  lcd.print(now.second() < 10 ? "0" + String(now.second()) : now.second());
  
  //Serial.println();
  delay(100);
}


void setupLcd() {
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Booting...");
}

void setupRtc() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

void setTime(int y, int m, int d, int h, int i, int s) {
  Serial.println("Setting time");
  rtc.adjust(DateTime(y, m, d, h, i, s));
}


