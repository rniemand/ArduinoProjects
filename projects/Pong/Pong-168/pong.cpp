#include "Arduino.h"
#include "joystick.h"
#include "pong.h"

// using * allows lazy assignment (provided all names match) <- not a safe / good idea
PongGame::PongGame(int gameSpeed, Joystick& joystick)
  : _joystick(joystick)
{
  _ballWidth = 2;
  _paddleWidth = 2;
  _paddleHeight = 10;
  _boundaryHeight = 2;
  _gameSpeed = gameSpeed;
}

