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
// https://github.com/adafruit/Adafruit-GFX-Library
// https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 7, 6);

const int BTN_UP         = 3;
const int BTN_DOWN       = 2;
const int BTN_ACTION     = 4;
const int BALL_WIDTH     = 2;
const int PADDLE_WIDTH   = 2;
const int PADDLE_HEIGHT  = 10;
const int PADDLEY_LOCK   = (display.width() - 2);
const int BOUNDRY_HEIGHT = 2;

bool inGame         = true;
bool gamePaused     = false;
int btnUpState      = 0; 
int btnDownState    = 0;
int btnActionState  = 0;
int paddle1Y        = (display.height() / 2) - (10 / PADDLE_WIDTH);
int paddle2Y        = (display.height() / 2) - (10 / PADDLE_WIDTH);
int ballX           = display.width()/2;
int ballY           = display.height()/2;
int ballSpeedX      = 1;
int ballSpeedY      = 1;
int player1Score    = 0;
int player2Score    = 0;

unsigned long lastDebounceTime    = 0;
unsigned long debounceDelay       = 200;

void setup()
{
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_ACTION, INPUT_PULLUP);
  
  Serial.begin(9600);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  // testFullScreenHorizontal();
  readButtons();
  
  runGameLoop();

  delay(10); // HACK for simple debouncing
}

// Pong Code
void runGameLoop() {
  if( gamePaused ) return;
  
  display.clearDisplay();
  
  movePaddels();
  moveBall();
  
  drawBoundaries();
  drawPaddles();
  drawBall();
  
  display.display();
}

void readButtons() {
  btnUpState = digitalRead(BTN_UP) == 0;
  btnDownState = digitalRead(BTN_DOWN) == 0;
  btnActionState = digitalRead(BTN_ACTION) == 0;

  // super simple button press limiting
  if( btnActionState == 1 && (millis() - lastDebounceTime) > debounceDelay ) {
    lastDebounceTime = millis();

    if( inGame == true ) {
      gamePaused = !gamePaused;
      Serial.println("toggling game paused state!");
    }
  }
}

void movePaddels() {
  // Player has pressed the UP button
  if( btnUpState == 1 ) {
    // Check to see if we are colliding with the top boundary
    if( paddle1Y > BOUNDRY_HEIGHT ) {
      paddle1Y -= 1;
    }
  }

  // Player has pressed the DOWN button
  if( btnDownState == 1 ) {
    // Check to see if we are colliding with the bottom boundary
    if( (paddle1Y + PADDLE_HEIGHT) < (display.height() - BOUNDRY_HEIGHT) ) {
      paddle1Y += 1;
    }
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
  checkCollissions();
  
  ballX += ballSpeedX;
  ballY += ballSpeedY;
}

void printBallPos() {
  Serial.print("Ball Position: (x: ");
  Serial.print(ballX, DEC);
  Serial.print(") (y: ");
  Serial.print(ballY, DEC);
  Serial.print(")");
  Serial.println();
}

void checkCollissions() {
  // printBallPos();
  
  // Collission with BOTTOM bar
  int bottomY = display.height() - 2;

  if( ballY + (BALL_WIDTH / 2) >= bottomY ) {
    ballSpeedY = ballSpeedY * -1;
    Serial.println("Collission BOTTOM");
  }

  // Collission with RIGHT side
  int rightX = display.width();

  if( ballX + BALL_WIDTH >= rightX ) {
    ballSpeedX = ballSpeedX * -1;
    Serial.println("Collission RIGHT");
  }

  // Collission with TOP side
  int topY = 0 + 2;

  if( ballY <= topY ) {
    ballSpeedY = ballSpeedY * -1;
    Serial.println("Collission TOP");
  }

  // Collission with PLAYERS side
  int leftX = 0 + 2;

  // Ensure that the ball is on the players side
  if( ballX + BALL_WIDTH <= leftX ) {
    // Check for collission with paddle
    if( (ballY >= paddle1Y) && (ballY <= (paddle1Y + PADDLE_HEIGHT)) ) {
      ballSpeedX = ballSpeedX * -1;
      Serial.println("Collided with players paddle");
    } else {
      player2Score += 1;
      resetBall();
      Serial.println("Collission LEFT");
      showScores();
    }
  }
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
  
  display.clearDisplay();
  resumeGame();
}

