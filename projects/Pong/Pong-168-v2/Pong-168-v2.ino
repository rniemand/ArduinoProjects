/* ***********************************************************************
* Pong 1.0 (Richard Niemand)
* ************************************************************************
* My first attempt at working with the Nokia 5110 screen and
* Arduino. There is a lo of room for improvements on the code
* and I always welcome pull requests :)
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
* (Nokia 5110 screen)
* 
*   1 - RST        -------------------------- 6
*   2 - CE (SCE)   -------------------------- 7
*   3 - D/C        -------------------------- 5
*   4 - DIN (MOSI) -------------------------- 11
*   5 - SCLK       -------------------------- 13
*   6 - VCC        -------------------------- 5v
*   7 - BL         -------------------------- 3v
*   8 - GND        -------------------------- GND
*   
* (Joystick Module)
* 
*   1 - SW         -------------------------- 4
*   2 - Y          -------------------------- A1
*   3 - X          -------------------------- A0
*   4 - 5v         -------------------------- 5v
*   5 - GND        -------------------------- GND
*********************************************************************** */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//#include "joystick.h"
#include "pong.h"

const short JOY_X          = A0;
const short JOY_Y          = A1;
const short JOY_BTN        = 4;

Adafruit_PCD8544          display(12, 11, 5, 7, 6); // SCLK, DIN, DC, CS, RST
Joystick                  joy(JOY_X, JOY_Y, JOY_BTN);
Pong                      pong(15, joy, display);

void setup()
{
  pinMode(JOY_BTN, INPUT_PULLUP);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);  

  //Serial.begin(9600);
  joy.setThresholds(100, 100, 100, 100);
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  pong.gameLoop();
}

