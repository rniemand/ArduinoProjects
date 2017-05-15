#include <IRremote.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
int RECV_PIN = 11; 

Adafruit_SSD1306 display(OLED_RESET);
IRrecv irrecv(RECV_PIN);
decode_results results;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  irrecv.enableIRIn();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println(F("Waiting..."));
  display.display();
  Serial.println("done with setup");
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    displayCode(&results);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}

void displayCode(decode_results *results) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Code:"));
  display.println("");
  display.println(results->value, HEX);
  display.display();
}




