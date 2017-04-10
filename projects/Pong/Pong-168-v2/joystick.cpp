#pragma once
#include "Arduino.h"
#include "joystick.h"

// FYI: http://stackoverflow.com/questions/8964164/single-class-has-a-class-redefinition-error

Joystick::Joystick(int xPin, int yPin, int buttonPin) {
  _pinX = xPin;
  _pinY = yPin;
  _pinBtn = buttonPin;
  
  _upThreshold = 512 + 100;
  _downThreshold = 512 - 100;
  _leftThreshold = 512 + 100;
  _rightThreshold = 512 - 100;
}

void Joystick::setThresholdY(int left, int right) {
  _leftThreshold = 512 + left;
  _rightThreshold = 512 - right;
}

void Joystick::setThresholdsX(int up, int down) {
  _upThreshold = 512 - up;
  _downThreshold = 512 + down;
}

void Joystick::setThresholds(int up, int down, int left, int right) {
  setThresholdsX(up, down);
  setThresholdY(left, right);
}

void Joystick::update() {
  x = analogRead(_pinX);
  y = analogRead(_pinY);
  btn = digitalRead(_pinBtn) == 0;

  // Set direction for X
  if( x < _upThreshold ) { xDirection = UP; }
  else if( x > _downThreshold ) { xDirection = DOWN; }
  else { xDirection = NONE; }

  // Set direction for Y
  if( y < _rightThreshold ) { yDirection = RIGHT; }
  else if( y > _leftThreshold ) { yDirection = LEFT; }
  else { yDirection = NONE; }
};


