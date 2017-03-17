//#include <Wire.h>
//#include "RTClib.h"
//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C   lcd(0x27, 16, 2);
//RTC_DS3231          rtc;
int loopCounter     = 0;

//char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
//char monthsOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void setup () {
  //Wire.begin();
  Serial.begin(9600);
  Serial.setTimeout(5);
  Serial.println("Booting...");

  //setupLcd();
  //setupRtc();

  Serial.println("Ready");
}

void loop () {
  loopCounter++;
  
  handleSerialInput();
  
  if(loopCounter >= 5) {
    //updateDisplay();
    loopCounter = 0;
  }

  delay(20);
}

/*
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

void updateDisplay() {
  DateTime now = rtc.now();

  // Date row
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(now.day() < 10 ? "0" + String(now.day()) : now.day());
  lcd.print(" ");
  lcd.print(monthsOfTheYear[now.month() - 1]);

  // Time row
  lcd.setCursor(0, 1);
  lcd.print(now.hour() < 10 ? "0" + String(now.hour()) : now.hour());
  lcd.print(":");
  lcd.print(now.minute() < 10 ? "0" + String(now.minute()) : now.minute());
  lcd.print(":");
  lcd.print(now.second() < 10 ? "0" + String(now.second()) : now.second());
}

void setTime(int y, int m, int d, int h, int i, int s) {
  Serial.println("Setting time");
  rtc.adjust(DateTime(y, m, d, h, i, s));
}
*/


void serialPrintTime() {
  /*
  DateTime now = rtc.now();

  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(" ");
  Serial.print(now.day() < 10 ? "0" + String(now.day()) : now.day());
  Serial.print(" ");
  Serial.print(monthsOfTheYear[now.month() - 1]);
  Serial.print("  ");
  Serial.print(now.hour() < 10 ? "0" + String(now.hour()) : now.hour());
  Serial.print(":");
  Serial.print(now.minute() < 10 ? "0" + String(now.minute()) : now.minute());
  Serial.print(":");
  Serial.println(now.second() < 10 ? "0" + String(now.second()) : now.second());
  */
}

void setClockTime(String input) {
  // set 20170317213300

  Serial.println("here");
  Serial.println(input.length());
  
  // Ensure that the string is the correct length
  if(input.length() < 19) {
    return;
  }

  Serial.println("------------");
  Serial.println( input.substring(4, 4) );
  Serial.println("------------");
}

void handleSerialInput() {
  String input = Serial.readString();
  if( input.length() == 0 ) return;

  if(input.startsWith("get") ) {
    serialPrintTime();
  }

  if(input.startsWith("set ")) {
    setClockTime(input);
  }


  Serial.println(input);
}



