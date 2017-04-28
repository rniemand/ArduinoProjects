//We always have to include the library
#include "LedControl.h"

// pin 12 is connected to the DataIn 
// pin 11 is connected to the CLK 
// pin 10 is connected to LOAD 

LedControl lc = LedControl(12, 11, 10, 4);
int devices = lc.getDeviceCount();
int numScreens = 0;
int startingPos = 0;
int maxPos = 0;
int currentPos = 0;

void setup() {
  for(int address = 0; address < devices; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }

  numScreens = lc.getDeviceCount();
  startingPos = numScreens * 8;
  maxPos = startingPos;
  currentPos = startingPos;
}

const byte A[] = { B11111100, B11111110, B00010010, B00010010, B11111110, B11111100, B00000000 };
const byte B[] = { B11111110, B11111110, B10010010, B10010010, B11111110, B01101100, B00000000 };

void loop() {
  // http://wayoda.github.io/LedControl/pages/software
  
  drawLetter(A, currentPos);

  currentPos --;
  delay(250);
}

void clearDisplays() {
  for(int address = 0; address < devices; address++) {
    lc.clearDisplay(address);
  }
}

int getScreenNumber(int pos) {
  return ((int) pos / numScreens) - 1;
}

void drawLetter(byte letter[], int pos) {
  for(int i = 0; i < 7; i++) {
    if(pos > maxPos) return;
    
    int screenNumber = getScreenNumber(pos);
    int actualPos = pos - (8 * screenNumber);
    
    lc.setRow(screenNumber, actualPos, letter[i]);
    pos++;
  }
}



