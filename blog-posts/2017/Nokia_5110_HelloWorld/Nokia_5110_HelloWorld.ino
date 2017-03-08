#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_PCD8544.h>

// Define the pins used to control the LCD module
#define LCD_SCLK    13
#define LCD_DIN     11
#define LCD_DC      5
#define LCD_CS      7
#define LCD_RST     6

// Create a global instance of the display object
Adafruit_PCD8544 display = Adafruit_PCD8544(
  LCD_SCLK, LCD_DIN, LCD_DC, LCD_CS, LCD_RST
);

void setup() {
  // Start the display and set a good contrast
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  display.println();
  display.println();
  display.println(" Hello World");
  display.display();
  delay(1000);
}
