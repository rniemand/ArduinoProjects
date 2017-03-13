#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

enum Screen {
  GAME,
  START
};

class Screens {
  Screen currentScreen;
  Adafruit_PCD8544 display;
  
  public:
    Screens(Adafruit_PCD8544& _display) 
      : display(_display) // initilizer list
    {
      //display = _display;
      display.begin();
    };

    void setCurrentScreen(Screen s) {
      currentScreen = s;
    }
    
    void showStart() {
      // if( currentScreen != START ) { return; }

      Serial.println(display.height());
      
      display.clearDisplay();
        
      display.println();
      display.println("  PONG (1.0)");
      display.println();
      display.println(" Press START");
      
      display.display();
    };
};
