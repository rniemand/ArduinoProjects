#include "pong.h"
// using * allows lazy assignment (provided all names match) <- not a safe / good idea

Pong::Pong(int _gameSpeed, Joystick& _joystick, Adafruit_PCD8544& _display) 
  : joystick(_joystick), display(_display)
{
  ballWidth       = 2;
  paddleWidth     = 2;
  paddleHeight    = 10;
  boundaryHeight  = 2;
  maxScore        = 5;
  debugEnabled    = false;
  gameSpeed       = _gameSpeed;

  onStartScreen   = true;
  inGame          = false;
  isGameOver      = false;
  gamePaused      = false;

  paddleYLock     = (display.width() - 2);
  paddle1Y        = (display.height() / 2) - (10 / paddleWidth);
  paddle2Y        = (display.height() / 2) - (10 / paddleWidth);
  ballX           = display.width()/2;
  ballY           = display.height()/2;
  ballSpeedX      = -1;
  ballSpeedY      = 1;
  player1Score    = 0;
  player2Score    = 0;

  unsigned long lastDebounceTime    = 0;
  
  collision_bottomY   = display.height() - 2;
  collision_rightX    = display.width() - 2;
  collision_topY      = 2;
  collision_leftX     = 2;
  halfBallWidth       = ballWidth / 2;
}

void Pong::gameLoop() {
  readButtons();
  showGameOver();
  showStartScreen();
  runGameLoop();
  
  delay(gameSpeed);
}

void Pong::readButtons() {
  joystick.update();

  // super simple button press limiting
  if( joystick.btn == 1 && (millis() - lastDebounceTime) > 200 ) {
    lastDebounceTime = millis();

    if( inGame == true ) {
      gamePaused = !gamePaused;
    }

    if( onStartScreen == true ) {
      onStartScreen = false;
      inGame = true;
    }
  }
}

void Pong::showGameOver() {
  if( !isGameOver ) {
    return;
  }

  gamePaused = true;
  inGame = false;
  onStartScreen = true;
  isGameOver = false;
  player1Score = 0;
  player2Score = 0;

  display.clearDisplay();

  display.println();
  display.println("  GAME OVER");
  display.println();

  if( player1Score >= maxScore ) {
    display.println("  YOU win!");  
  } else {
    display.println("  CPU wins!");  
  }

  display.display();
  delay(2000);
}

void Pong::showStartScreen() {
  //screens->showStart();
  
  if( !onStartScreen ) {
    return;
  }
  
  display.clearDisplay();
  
  display.println();
  display.println("  PONG (1.0)");
  display.println();
  display.println(" Press START");

  display.display();
}

void Pong::runGameLoop() {
  if( gamePaused || !inGame ) {
    return;
  }
  
  display.clearDisplay();
  
  moveAiPaddle();
  movePlayerPaddle();
  moveBall();
  
  drawBoundaries();
  drawPaddles();
  drawBall();
  
  display.display();
}

void Pong::movePlayerPaddle() {
  // Player has pressed the UP button
  if( joystick.xDirection == UP ) {
    // Check to see if we are colliding with the top boundary
    if( paddle1Y > boundaryHeight ) {
      paddle1Y -= 1;
    }
  }

  // Player has pressed the DOWN button
  if( joystick.xDirection == DOWN ) {
    // Check to see if we are colliding with the bottom boundary
    if( (paddle1Y + paddleHeight) < (display.height() - boundaryHeight) ) {
      paddle1Y += 1;
    }
  }
}

void Pong::moveAiPaddle() {
  if( ballY > paddle2Y && ballSpeedX > 0 ) {
    paddle2Y += 1;
    return;
  }

  if( ballY < paddle2Y && ballSpeedX > 0 ) {
    paddle2Y -= 1;
    return;
  }
}

void Pong::drawBoundaries() {
  display.drawLine(0, 0, display.width(), 0, BLACK);
  display.drawLine(0, 1, display.width(), 1, BLACK);

  display.drawLine(0, display.height()-2, display.width(), display.height()-2, BLACK);
  display.drawLine(0, display.height()-1, display.width(), display.height()-1, BLACK);
}

void Pong::drawPaddles() {
  // Left paddle
  // x, y, width, height, color
  display.drawRect(0, paddle1Y, paddleWidth, paddleHeight, BLACK);

  // Right paddle
  display.drawRect(paddleYLock, paddle2Y, paddleWidth, paddleHeight, BLACK);
}

void Pong::drawBall() {
  // x0, y0, r, color
  display.fillCircle(ballX, ballY, ballWidth, BLACK);
}

void Pong::moveBall() {
  checkCollisions();
  
  ballX += ballSpeedX;
  ballY += ballSpeedY;
}

void Pong::pauseGame() {
  if( !inGame ) return;
  gamePaused = true;
}

void Pong::resumeGame() {
  if( !inGame ) return;
  gamePaused = false;
}

void Pong::resetBall() {
  ballX = display.width()/2;
  ballY = display.height()/2;
}

void Pong::showScores() {
  pauseGame();
  display.clearDisplay();
  
  display.println();
  
  display.print("Player: ");
  display.print(player1Score);
  display.println();

  display.print("CPU:    ");
  display.print(player2Score);
  display.println();

  display.display();
  delay(1000);

  // Check to see if it is Game Over
  if( player1Score >= maxScore || player2Score >= maxScore ) {
    isGameOver = true;
  }
  
  display.clearDisplay();
  resumeGame();
}

void Pong::checkCollisions() {
  // printBallPos();

  checkCollisionBottom();
  checkCollisionRight();
  checkCollisionTop();
  checkCollisionLeft();
}

void Pong::checkCollisionBottom() {
  // If the ball is above the boundaries there is no need to check
  if( ballY + (ballWidth) < collision_bottomY ){
    return;
  }

  // Flip the speed of the ball to change direction
  ballSpeedY = ballSpeedY * -1;
}

void Pong::checkCollisionRight() {
  // Check if the ball is close to the right
  if( (ballX + ballWidth) < collision_rightX ) {
    return;
  }

  // Check if we went over the top of the paddle
  if( (ballY + halfBallWidth) < paddle2Y ) {
    player1Score += 1;
    resetBall();
    showScores();
    return;
  }

  // Check if we went under the bottom of the paddle
  if( (ballY - halfBallWidth) > (paddle2Y + paddleHeight) ) {
    player1Score += 1;
    resetBall();
    showScores();
    return;
  }
  
  // Ball collided with the paddle
  ballSpeedX = ballSpeedX * -1;
}

void Pong::checkCollisionTop() {
  // Check to see if the ball is at the top
  if( ballY > collision_topY ) {
    return;
  }

  ballSpeedY = ballSpeedY * -1;
}

void Pong::checkCollisionLeft() {
  // Check if the ball is close to the left
  if( (ballX + ballWidth) > collision_leftX ) {
    return;
  }

  // Check if we went over the top of the paddle
  if( (ballY + halfBallWidth) < paddle1Y ) {
    player2Score += 1;
    resetBall();
    showScores();
    return;
  }

  // Check if we went under the bottom of the paddle
  if( (ballY - halfBallWidth) > (paddle1Y + paddleHeight) ) {
    player2Score += 1;
    resetBall();
    showScores();
    return;
  }
  
  // Ball collided with the paddle
  ballSpeedX = ballSpeedX * -1;
}

