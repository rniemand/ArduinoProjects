#include "LedControl.h"

// pin 12 is connected to the DataIn 
// pin 11 is connected to the CLK 
// pin 10 is connected to LOAD 

const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(12, 11, 10, NUM_DISPLAYS);
int NUM_ROWS            = 32;
int                     rows[32];

void setup() {
  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }
}


void loop() {
  // http://wayoda.github.io/LedControl/pages/software
  shiftRows();
  rows[0] = random(1, 255);
  render();
  
  delay(75);
}

void shiftRows() {
  for( int i = NUM_ROWS; i >= 0; i-- ) {
    rows[i] = (int) rows[i-1];
  }
}

int workDisplayNumber(int rowNumber) {
  for(int i = 0; i < NUM_DISPLAYS; i++) {
    int maxNum = (7 + (i * 8)) - 1; // 0-7 | 8-15 ...
    if(rowNumber <= maxNum) {
      return i;
    }
  }

  return 99;
}

int workColumnNum(int screenNum, int rowNumber) {
  if(screenNum == 0) {
    return rowNumber;
  }

  int takeAway = (8 * screenNum) - 1;
  return rowNumber - takeAway;
}

void render() {
  for(int i = 0; i < NUM_ROWS; i++) {
    int screenNum = workDisplayNumber(i);
    int colNum = workColumnNum(screenNum, i);
    int value = rows[i];
    
    lc.setRow(screenNum, colNum, value);
  }
}


