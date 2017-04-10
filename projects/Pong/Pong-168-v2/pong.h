#include "Arduino.h"
#include "joystick.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

class Pong {
  private:
    Joystick& joystick;
    Adafruit_PCD8544& display;
    short ballWidth, paddleWidth, paddleHeight, boundaryHeight, paddleYLock;
    short collision_bottomY, collision_rightX, collision_topY, collision_leftX, halfBallWidth;
    unsigned long lastDebounceTime;

    void readButtons();
    void showGameOver();
    void showStartScreen();
    void runGameLoop();
    void movePlayerPaddle();
    void moveAiPaddle();
    void moveBall(), drawBoundaries(), drawPaddles(), drawBall();
    void checkCollisions(), pauseGame(), resumeGame(), resetBall();
    void showScores(), checkCollisionBottom(), checkCollisionRight();
    void checkCollisionTop(), checkCollisionLeft();

  public:
    int gameSpeed, debugEnabled;
    bool onStartScreen, inGame, isGameOver, gamePaused;
    short paddle1Y, paddle2Y, ballX, ballY, ballSpeedX, ballSpeedY, player1Score, player2Score, maxScore;
    
    Pong(int _gameSpeed, Joystick& _joystick, Adafruit_PCD8544& _display);
    void gameLoop();
};


