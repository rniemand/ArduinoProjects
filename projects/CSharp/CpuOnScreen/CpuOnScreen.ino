/* ***********************************************************************
* CPU Testing
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
*********************************************************************** */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

String readString;
Adafruit_PCD8544 display(13, 11, 5, 7, 6); // SCLK, DIN, DC, CS, RST

void setup() {
  Serial.begin(9600);

  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  readSerial();

  if (readString.length() >0) {
    String cpu = getValue(readString, ',', 0);
    String ram = getValue(readString, ',', 1);

    display.clearDisplay();
    display.println();
    display.print("CPU:  ");
    display.println(cpu);
    display.println();
    display.print("RAM:  ");
    display.println(ram);
    display.display();
    
    readString="";
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void readSerial() {
  while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        readString += c; //makes the string readString
      } 
    }
}


