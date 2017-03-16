#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_PCD8544.h>

/* ***********************************************************************
* Nokia 5110 Screen Testing
* ************************************************************************
* 
* Libs required for this project to work:
* 
*   https://github.com/adafruit/Adafruit-GFX-Library
*   https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
*   
* ***********************************************************************
* Pin Outs
* ***********************************************************************
* 
*   1 - RST        -------------------------- D6
*   2 - CE (SCE)   -------------------------- D7
*   3 - D/C        -------------------------- D5
*   4 - DIN (MOSI) -------------------------- D11
*   5 - SCLK       -------------------------- D13
*   6 - VCC        -------------------------- 5v
*   7 - BL         -------------------------- 3v
*   8 - GND        -------------------------- GND
*   
*********************************************************************** */

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
