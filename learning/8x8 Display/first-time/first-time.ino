//We always have to include the library
#include "LedControl.h"

// pin 12 is connected to the DataIn 
// pin 11 is connected to the CLK 
// pin 10 is connected to LOAD 

LedControl lc = LedControl(12, 11, 10, 4);

void setup() {
  int devices=lc.getDeviceCount();
  
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}


void loop() {
  // http://wayoda.github.io/LedControl/pages/software

  for(int count = 1; count < 255; count++) {
    for(int j = 0; j < 4; j++) {
      for(int i = 0; i < 8; i++) {
        lc.setRow(j, i, count);
        lc.clearDisplay(0);
        lc.clearDisplay(1);
        lc.clearDisplay(2);
        lc.clearDisplay(3);
      }
    }
  }

}
