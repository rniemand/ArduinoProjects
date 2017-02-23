#include "Arduino.h"

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class Joystick {
  int pinX, pinY, pinBtn, upThreshold, downThreshold, leftThreshold, rightThreshold;
  public:
    Direction xDirection, yDirection;
    
    Joystick(int xPin, int yPin, int buttonPin) {
      pinX = xPin;
      pinY = yPin;
      pinBtn = buttonPin;

      upThreshold = 512 + 100;
      downThreshold = 512 - 100;
      leftThreshold = 512 + 100;
      rightThreshold = 512 - 100;
    };
    
    int x, y, btn;

    void update() {
      x = analogRead(pinX);
      y = analogRead(pinY);
      btn = digitalRead(pinBtn) == 0;

      // Set direction for X
      if( x < upThreshold ) { xDirection = UP; }
      else if( x > downThreshold ) { xDirection = DOWN; }
      else { xDirection = NONE; }

      // Set direction for Y
      if( y < rightThreshold ) { yDirection = RIGHT; }
      else if( y > leftThreshold ) { yDirection = LEFT; }
      else { yDirection = NONE; }
    }

    void setThresholdsX(int up, int down) {
      upThreshold = 512 - up;
      downThreshold = 512 + down;
    }

    void setThresholdY(int left, int right) {
      leftThreshold = 512 + left;
      rightThreshold = 512 - right;
    }

    void setThresholds(int up, int down, int left, int right) {
      setThresholdsX(up, down);
      setThresholdY(left, right);
    }
};



