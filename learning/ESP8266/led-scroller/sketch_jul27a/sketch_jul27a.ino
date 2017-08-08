#include <LedControl.h>
#include "font.h"

// http://wayoda.github.io/LedControl/pages/hardware.html

// pin D0 is connected to DIN 
// pin D1 is connected to CLK 
// pin D2 is connected to LOAD 

//const char* ssid        = "PickMe";
//const char* password    = "fallout312345";
//const char* host        = "esp8266sd";

const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(D0, D1, D2, NUM_DISPLAYS);
int NUM_ROWS            = 32;       // NUM_DISPLAYS * 8
int                     rows[32];   // Same as NUM_ROWS
int                     currentChar[8];
int currentCharIdx      = 0;
int currentCharLen      = 0;
String message          = "Ready...";
int messageIdx          = -1;
int messageLen          = 0;
bool messageChanged     = false;
String serailText;

void setup() {
  Serial.begin(9600);

  // Configure and clear all displays
  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }

  // Determine the starting letters length
  messageLen = message.length();
  
  //shiftNextLetter();
}

void loop() {
  //lc.setRow(displayNumber, columnNumber, rowValue);

  lc.setRow(0, 0, B00000001);
  lc.setRow(0, 1, B00000001);
  lc.setRow(0, 2, B00000001);
  lc.setRow(0, 3, B00000001);
  lc.setRow(0, 4, B00000001);
  lc.setRow(0, 5, B00000001);
  lc.setRow(0, 6, B00000001);
  lc.setRow(0, 7, B00000001);

  delay(500);

  lc.setRow(0, 0, B00000011);
  lc.setRow(0, 1, B00000011);
  lc.setRow(0, 2, B00000011);
  lc.setRow(0, 3, B00000011);
  lc.setRow(0, 4, B00000011);
  lc.setRow(0, 5, B00000011);
  lc.setRow(0, 6, B00000011);
  lc.setRow(0, 7, B00000011);

  delay(500);

  lc.setRow(0, 0, B00000111);
  lc.setRow(0, 1, B00000111);
  lc.setRow(0, 2, B00000111);
  lc.setRow(0, 3, B00000111);
  lc.setRow(0, 4, B00000111);
  lc.setRow(0, 5, B00000111);
  lc.setRow(0, 6, B00000111);
  lc.setRow(0, 7, B00000111);
  

  delay(500);
}

/*
void loop() {
  shiftRows();

  if(currentCharIdx < currentCharLen) {
    rows[NUM_ROWS - 1] = currentChar[currentCharIdx];
    currentCharIdx++;
  }
  // else if(currentCharIdx == currentCharLen-1) {
  else {
    shiftNextLetter();
    currentCharIdx = 0;
  }
  
  render();
  delay(25);
}
*/

void clearDisplays() {
  for(int i = 0; i < NUM_ROWS; i++) {
    rows[i] = 0;
  }
  
  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.clearDisplay(address);
  }
}

void shiftNextLetter() {
  // There is no message data to display
  if(messageLen == 0) {
    return;
  }

  // We are at the end of the message
  if(messageIdx == (messageLen - 1)) {
    currentChar[0] = 0;
    currentChar[1] = 0;
    currentChar[2] = 0;
    currentChar[3] = 0;
    currentChar[4] = 0;
    currentChar[5] = 0;
    currentChar[6] = 0;
    currentChar[7] = 0;
    
    return;
  }

  // Set our position in the display string
  messageIdx += 1;
  mapLetter();
}

void mapLetter(){
  currentCharIdx = 0;
  
  switch( message[messageIdx] ) {
    case '0': copyLetter(_0,            7); break;
    case '1': copyLetter(_1,            6); break;
    case '2': copyLetter(_2,            6); break;
    case '3': copyLetter(_3,            6); break;
    case '4': copyLetter(_4,            7); break;
    case '5': copyLetter(_5,            6); break;
    case '6': copyLetter(_6,            6); break;
    case '7': copyLetter(_7,            6); break;
    case '8': copyLetter(_8,            6); break;
    case '9': copyLetter(_9,            6); break;
    
    case 'A': copyLetter(A,             6); break;
    case 'B': copyLetter(B,             7); break;
    case 'C': copyLetter(C,             7); break;
    case 'D': copyLetter(D,             7); break;
    case 'E': copyLetter(E,             7); break;
    case 'F': copyLetter(F,             7); break;
    case 'G': copyLetter(G,             7); break;
    case 'H': copyLetter(H,             6); break;
    case 'I': copyLetter(I,             4); break;
    case 'J': copyLetter(J,             7); break;
    case 'K': copyLetter(K,             7); break;
    case 'L': copyLetter(L,             7); break;
    case 'M': copyLetter(M,             7); break;
    case 'N': copyLetter(N,             7); break;
    case 'O': copyLetter(O,             7); break;
    case 'P': copyLetter(P,             7); break;
    case 'Q': copyLetter(Q,             6); break;
    case 'R': copyLetter(R,             7); break;
    case 'S': copyLetter(S,             6); break;
    case 'T': copyLetter(T,             6); break;
    case 'U': copyLetter(U,             6); break;
    case 'V': copyLetter(V,             6); break;
    case 'W': copyLetter(W,             7); break;
    case 'X': copyLetter(X,             7); break;
    case 'Y': copyLetter(Y,             6); break;
    case 'Z': copyLetter(Z,             7); break;

    case 'a': copyLetter(a,             7); break;
    case 'b': copyLetter(b,             7); break;
    case 'c': copyLetter(c,             6); break;
    case 'd': copyLetter(d,             6); break;
    case 'e': copyLetter(e,             6); break;
    case 'f': copyLetter(f,             6); break;
    case 'g': copyLetter(g,             7); break;
    case 'h': copyLetter(h,             7); break;
    case 'i': copyLetter(i,             5); break;
    case 'j': copyLetter(j,             6); break;
    case 'k': copyLetter(k,             7); break;
    case 'l': copyLetter(l,             6); break;
    case 'm': copyLetter(m,             7); break;
    case 'n': copyLetter(n,             6); break;
    case 'o': copyLetter(o,             6); break;
    case 'p': copyLetter(p,             7); break;
    case 'q': copyLetter(q,             7); break;
    case 'r': copyLetter(r,             7); break;
    case 's': copyLetter(s,             6); break;
    case 't': copyLetter(t,             6); break;
    case 'u': copyLetter(u,             7); break;
    case 'v': copyLetter(v,             6); break;
    case 'w': copyLetter(w,             7); break;
    case 'x': copyLetter(x,             7); break;
    case 'y': copyLetter(y,             6); break;
    case 'z': copyLetter(z,             6); break;
    
    case ' ': copyLetter(SPACE,         3); break;
    case ':': copyLetter(COLON,         4); break;
    case ';': copyLetter(SEMI_COLON,    5); break;
    case '<': copyLetter(LESS_THAN,     5); break;
    case '>': copyLetter(GREA_THAN,     6); break;
    case '(': copyLetter(BRA_OPEN,      5); break;
    case ')': copyLetter(BRA_CLOSE,     5); break;
    case '%': copyLetter(PERCENT,       7); break;
    case '=': copyLetter(EQUALS,        6); break;
    case '+': copyLetter(PLUS,          6); break;
    case '-': copyLetter(MINUS,         5); break;
    case '*': copyLetter(TIMES,         8); break;
    case ',': copyLetter(COMMA,         4); break;
    case '.': copyLetter(PERIOD,        3); break;
    case '!': copyLetter(EXCLAMATION,   5); break;
    case '?': copyLetter(QUESTION,      6); break;
    case '@': copyLetter(AT,            7); break;
    case '#': copyLetter(HASH,          7); break;
    case '$': copyLetter(DOLLAR,        6); break;
    case '[': copyLetter(SQR_OPEN,      5); break;
    case ']': copyLetter(SQR_CLOSE,     5); break;
    case '_': copyLetter(UNDERSCORE,    5); break;
    case '{': copyLetter(SQUIG_OPEN,    6); break;
    case '}': copyLetter(SQUIG_CLOSE,   6); break;
    case '^': copyLetter(AMPERSAND,     6); break;
    case '/': copyLetter(FWD_SLASH,     6); break;
    case '\\': copyLetter(BACK_SLASH,   7); break;
    case '~': copyLetter(TILDA,         7); break;
    
    default: copyLetter(SPACE, 3); break;
  }
}

void copyLetter(int data[], int charLen) {
  currentCharLen = charLen;

  for(int i = 0; i < charLen; i++) {
    currentChar[i] = data[i];
  }
}

void shiftRows() {
  for( int i = 0; i < NUM_ROWS; i++ ) {
    rows[i] = rows[i+1];
  }

  rows[NUM_ROWS - 1] = 0;
}

int workDisplayNumber(int rowNumber) {
  if(rowNumber <= 7) {
    return 0;
  }
  
  for(int i = 0; i < NUM_DISPLAYS; i++) {
    int maxNumber = 7 + (i * 8);
    
    if(rowNumber <= maxNumber) {
      return i;
    }
  }

  return 99;
}

int workColumnNum(int displayNumber, int rowNumber) {
  // No need to map number if it's the first sreen
  if(displayNumber == 0) {
    return rowNumber;
  }

  // Work out the relative column number for the given screen
  return rowNumber - (8 * displayNumber);
}

void render() {
  for(int i = 0; i < NUM_ROWS; i++) {
    int displayNumber = workDisplayNumber(i);
    int columnNumber  = workColumnNum(displayNumber, i);
    int rowValue      = rows[i];
    
    lc.setRow(displayNumber, columnNumber, rowValue);
  }
}



