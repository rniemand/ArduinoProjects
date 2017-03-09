#pragma once

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class Joystick {
  public:
    Joystick(int xPin, int yPin, int buttonPin);
    
    int x, y, btn;
    Direction xDirection, yDirection;
    
    void setThresholdY(int left, int right);
    void setThresholdsX(int up, int down);
    void setThresholds(int up, int down, int left, int right);
    void update();
    
  protected:
    int _pinX, _pinY, _pinBtn, _upThreshold, _downThreshold, _leftThreshold, _rightThreshold;
};

