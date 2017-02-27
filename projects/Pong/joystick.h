#ifndef RN_JOYSICK_H
#define RN_JOYSICK_H

class Joystick {
  public:
    Direction xDirection, yDirection;
  
  protected:
    int pinX, pinY, pinBtn, upThreshold, downThreshold, leftThreshold, rightThreshold;
}

#endif
