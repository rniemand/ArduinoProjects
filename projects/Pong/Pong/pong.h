class PongGame {
  private:
    Joystick& _joystick;
    int _ballWidth, _paddleWidth, _paddleHeight, _boundaryHeight, _gameSpeed;

  public:
    PongGame(int _gameSpeed, Joystick& _joystick);
};


