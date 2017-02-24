#include "Arduino.h"
#include "joystick.cpp"

class PongGame {
  Joystick joystick;
  int ballWidth;
  int paddleWidth;
  int paddleHeight;
  int boundaryHeight;
  int gameSpeed;

  public:
    PongGame(int _gameSpeed, Joystick _joystick) {
      ballWidth = 2;
      paddleWidth = 2;
      paddleHeight = 10;
      boundaryHeight = 2;
      gameSpeed = _gameSpeed;
      joystick = _joystick;

      Serial.println("hello world from pong");
    }
};
