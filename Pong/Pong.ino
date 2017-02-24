/* Nokia 5100 LCD Example Code
   Graphic LCD Pin ---------- Arduino Pin
       1-VCC       ----------------  5V
       2-GND       ----------------  GND
       3-SCE       ----------------  7
       4-RST       ----------------  6
       5-D/C       ----------------  5
       6-DN(MOSI)  ----------------  11
       7-SCLK      ----------------  13
       8-LED       - 330 Ohm res --  9
*/

/* ***********************************************************************
* Pong 1.0 (Richard Niemand)
* ************************************************************************
* My first attempt at working with the Nokia 5110 screen and
* Arduino. There is a lo of room for improvements on the code
* and I always welcome pull requests :)
* 
* Libs required for this project to work:
* 
*   https://github.com/adafruit/Adafruit-GFX-Library
*   https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
*   
* ***********************************************************************
* Pin Outs
* ***********************************************************************
* 
* (Nokia 5110 screen)
* 
*   1 - RST        -------------------------- 6
*   2 - CE (SCE)   -------------------------- 7
*   3 - D/C        -------------------------- 5
*   4 - DIN (MOSI) -------------------------- 11
*   5 - SCLK       -------------------------- 13
*   6 - VCC        -------------------------- 5v
*   7 - BL         -------------------------- 3v
*   8 - GND        -------------------------- GND
*   
* (Joystick Module)
* 
*   1 - SW         -------------------------- 4
*   2 - Y          -------------------------- A1
*   3 - X          -------------------------- A0
*   4 - 5v         -------------------------- 5v
*   5 - GND        -------------------------- GND
*********************************************************************** */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//#include "pong.cpp"
#include "joystick.cpp"
#include "screens.cpp"

const int BALL_WIDTH     = 2;
const int PADDLE_WIDTH   = 2;
const int PADDLE_HEIGHT  = 10;
const int BOUNDRY_HEIGHT = 2;
const int GAME_SPEED     = 15; // higher = slower
const int MAX_SCORE      = 5;
const bool DEBUG_ENABLED = false;
const int JOY_X          = A0;
const int JOY_Y          = A1;
const int JOY_BTN        = 4;

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 7, 6); // SCLK, DIN, DC, CS, RST
Joystick* joy = new Joystick(JOY_X, JOY_Y, JOY_BTN);
//Screens* screens = new Screens(display);

// Game screens
bool onStartScreen        = true;
bool inGame               = false;
bool onInstructionScreen  = false;
bool isGameOver           = false;

const int PADDLEY_LOCK   = (display.width() - 2);
bool gamePaused          = false;
int paddle1Y             = (display.height() / 2) - (10 / PADDLE_WIDTH);
int paddle2Y             = (display.height() / 2) - (10 / PADDLE_WIDTH);
int ballX                = display.width()/2;
int ballY                = display.height()/2;
int ballSpeedX           = -1;
int ballSpeedY           = 1;
int player1Score         = 0;
int player2Score         = 0;

unsigned long lastDebounceTime    = 0;
unsigned long debounceDelay       = 200;

void setup()
{
  pinMode(JOY_BTN, INPUT_PULLUP);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);  

  Serial.begin(9600);

  // PongGame* pong = new PongGame(GAME_SPEED, *joy);

  joy->setThresholds(100, 100, 100, 100);
  //screens->setCurrentScreen(START);
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  // testFullScreenHorizontal();
  readButtons();

  showGameOver();
  showStartScreen();
  showInstructionScreen();
  runGameLoop();

  delay(GAME_SPEED);
}

// Pong Code
void runGameLoop() {
  if( gamePaused || !inGame ) {
    return;
  }
  
  display.clearDisplay();
  
  movePaddles();
  moveBall();
  
  drawBoundaries();
  drawPaddles();
  drawBall();
  
  display.display();
}

void movePaddles() {
  moveAiPaddle();
  movePlayerPaddle();
}

void movePlayerPaddle() {
  // Player has pressed the UP button
  if( joy->xDirection == UP ) {
    // Check to see if we are colliding with the top boundary
    if( paddle1Y > BOUNDRY_HEIGHT ) {
      paddle1Y -= 1;
    }
  }

  // Player has pressed the DOWN button
  if( joy->xDirection == DOWN ) {
    // Check to see if we are colliding with the bottom boundary
    if( (paddle1Y + PADDLE_HEIGHT) < (display.height() - BOUNDRY_HEIGHT) ) {
      paddle1Y += 1;
    }
  }
}

void moveAiPaddle() {
  if( ballY > paddle2Y && ballSpeedX > 0 ) {
    paddle2Y += 1;
    return;
  }

  if( ballY < paddle2Y && ballSpeedX > 0 ) {
    paddle2Y -= 1;
    return;
  }
}

void drawBoundaries() {
  display.drawLine(0, 0, display.width(), 0, BLACK);
  display.drawLine(0, 1, display.width(), 1, BLACK);

  display.drawLine(0, display.height()-2, display.width(), display.height()-2, BLACK);
  display.drawLine(0, display.height()-1, display.width(), display.height()-1, BLACK);
}

void drawPaddles() {
  // Left paddle
  // x, y, width, height, color
  display.drawRect(0, paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);

  // Right paddle
  display.drawRect(PADDLEY_LOCK, paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);
}

void drawBall() {
  // x0, y0, r, color
  display.fillCircle(ballX, ballY, BALL_WIDTH, BLACK);
}

void moveBall() {
  checkCollisions();
  
  ballX += ballSpeedX;
  ballY += ballSpeedY;
}

void pauseGame() {
  if( !inGame ) return;
  gamePaused = true;
}

void resumeGame() {
  if( !inGame ) return;
  gamePaused = false;
}

void resetBall() {
  ballX = display.width()/2;
  ballY = display.height()/2;
}

// ========================================== >
//IO
void readButtons() {
  joy->update();

  // super simple button press limiting
  if( joy->btn == 1 && (millis() - lastDebounceTime) > debounceDelay ) {
    lastDebounceTime = millis();

    if( onInstructionScreen == true ) {
      onStartScreen = false;
      onInstructionScreen = false;
      inGame = true;
    }

    if( inGame == true ) {
      gamePaused = !gamePaused;
      Serial.println("toggling game paused state!");
    }

    if( onStartScreen == true ) {
      onStartScreen = false;
      onInstructionScreen = true;
      inGame = false;
    }
  }
}

// ========================================== >
// Additional screens
void showScores() {
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
  if( player1Score >= MAX_SCORE || player2Score >= MAX_SCORE ) {
    isGameOver = true;
  }
  
  display.clearDisplay();
  resumeGame();
}

void showStartScreen() {
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

void showInstructionScreen() {
  if( !onInstructionScreen ) {
    return;
  }

  display.clearDisplay();
  
  display.print("First to ");
  display.print(MAX_SCORE, DEC);
  display.print(" wins! Press START to begin");
  display.println();

  display.display();
}

void showGameOver() {
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

  if( player1Score >= MAX_SCORE ) {
    display.println("  YOU win!");  
  } else {
    display.println("  CPU wins!");  
  }

  display.display();
  delay(2000);
}

// ========================================== >
// Collision Detection
void checkCollisions() {
  // printBallPos();

  checkCollisionBottom();
  checkCollisionRight();
  checkCollisionTop();
  checkCollisionLeft();
}

void logCollision(String point, String additional) {
  if( !DEBUG_ENABLED ) {
    return;
  }
  
  Serial.print("COLLISION: ");
  Serial.print(point);
  Serial.print(" ");
  
  Serial.print("Ball (x: ");
  Serial.print(ballX - BALL_WIDTH, DEC);
  Serial.print("-");
  Serial.print(ballX + BALL_WIDTH, DEC);
  Serial.print(", y:");
  Serial.print(ballY - BALL_WIDTH, DEC);
  Serial.print("-");
  Serial.print(ballY + BALL_WIDTH, DEC);
  Serial.print(")");

  Serial.print(" Paddle 1: (x: 0, y:");
  Serial.print(paddle1Y, DEC);
  Serial.print("-");
  Serial.print(paddle1Y + PADDLE_HEIGHT, DEC);
  Serial.print(")");

  Serial.print(" Paddle 2: (");
  Serial.print(PADDLEY_LOCK, DEC);
  Serial.print(",");
  Serial.print(paddle2Y, DEC);
  Serial.print("-");
  Serial.print(paddle2Y + PADDLE_HEIGHT, DEC);
  Serial.print(")");

  Serial.print(" ");
  Serial.print(additional);

  Serial.println();
}

int collision_bottomY = display.height() - 2;
int collision_rightX  = display.width() - 2;
int collision_topY    = 2;
int collision_leftX   = 2;
int halfBallWidth     = BALL_WIDTH / 2;

void checkCollisionBottom() {
  // If the ball is above the boundaries there is no need to check
  if( ballY + (BALL_WIDTH) < collision_bottomY ){
    return;
  }

  // Flip the speed of the ball to change direction
  ballSpeedY = ballSpeedY * -1;
  logCollision("BOTTOM", "");
}

void checkCollisionRight() {
  // Check if the ball is close to the right
  if( (ballX + BALL_WIDTH) < collision_rightX ) {
    return;
  }

  // Check if we went over the top of the paddle
  if( (ballY + halfBallWidth) < paddle2Y ) {
    player1Score += 1;
    resetBall();
    logCollision("LEFT ", "Point P1 (over)");
    showScores();
    return;
  }

  // Check if we went under the bottom of the paddle
  if( (ballY - halfBallWidth) > (paddle2Y + PADDLE_HEIGHT) ) {
    player1Score += 1;
    resetBall();
    logCollision("LEFT ", "Point P1 (under)");
    showScores();
    return;
  }
  
  // Ball collided with the paddle
  ballSpeedX = ballSpeedX * -1;
  logCollision("RIGHT ", "Hit paddle");
}

void checkCollisionTop() {
  // Check to see if the ball is at the top
  if( ballY > collision_topY ) {
    return;
  }

  ballSpeedY = ballSpeedY * -1;
  logCollision("TOP   ", "");
}

void checkCollisionLeft() {
  // Check if the ball is close to the left
  if( (ballX + BALL_WIDTH) > collision_leftX ) {
    return;
  }

  // Check if we went over the top of the paddle
  if( (ballY + halfBallWidth) < paddle1Y ) {
    player2Score += 1;
    resetBall();
    logCollision("LEFT ", "Point CPU (over)");
    showScores();
    return;
  }

  // Check if we went under the bottom of the paddle
  if( (ballY - halfBallWidth) > (paddle1Y + PADDLE_HEIGHT) ) {
    player2Score += 1;
    resetBall();
    logCollision("LEFT ", "Point CPU (under)");
    showScores();
    return;
  }
  
  // Ball collided with the paddle
  ballSpeedX = ballSpeedX * -1;
  logCollision("LEFT  ", "Hit paddle");
}
