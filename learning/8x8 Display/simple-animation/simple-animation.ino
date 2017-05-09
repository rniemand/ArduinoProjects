#include "LedControl.h"

// pin 12 is connected to the DIN 
// pin 11 is connected to the CLK 
// pin 10 is connected to LOAD 

const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(12, 11, 10, NUM_DISPLAYS);
int NUM_ROWS            = 32;       // NUM_DISPLAYS * 8
int                     rows[32];   // Same as NUM_ROWS

void setup() {
  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }
}

void loop() {
  shiftRows();
  rows[0] = random(1, 255);
  render();
  delay(50);
}

void shiftRows() {
  for( int i = NUM_ROWS; i >= 0; i-- ) {
    rows[i] = (int) rows[i-1];
  }
}

int workDisplayNumber(int rowNumber) {
  if(rowNumber <= 7) {
    return 0;
  }
  
  for(int i = 0; i < NUM_DISPLAYS; i++) {
    int maxNumber = 7 + (i * 8);
    
    if(rowNumber <= maxNumber) {
      return i;
    }
  }

  return 99;
}

int workColumnNum(int displayNumber, int rowNumber) {
  // No need to map number if it's the first sreen
  if(displayNumber == 0) {
    return rowNumber;
  }

  // Work out the relative column number for the given screen
  return rowNumber - (8 * displayNumber);
}

void render() {
  for(int i = 0; i < NUM_ROWS; i++) {
    int displayNumber = workDisplayNumber(i);
    int columnNumber  = workColumnNum(displayNumber, i);
    int rowValue      = rows[i];
    
    lc.setRow(displayNumber, columnNumber, rowValue);
  }
}


