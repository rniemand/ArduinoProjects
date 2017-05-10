#include "LedControl.h"

// pin 12 is connected to DIN 
// pin 11 is connected to   CLK 
// pin 10 is connected to LOAD 

const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(12, 11, 10, NUM_DISPLAYS);
int NUM_ROWS            = 32;       // NUM_DISPLAYS * 8
uint8_t                 rows[32];   // Same as NUM_ROWS
uint8_t                 currentChar[8];
int currentCharIdx      = 0;
uint8_t currentCharLen  = 0;
String message          = "#$[]_{}";
int messageIdx          = -1;
int messageLen          = 0;
bool messageChanged     = false;
String serailText;

const uint8_t _0[]          = { B00111110, B01111111, B01110001, B01011001, B01001101, B01111111, B00111110 };
const uint8_t _1[]          = { B01000000, B01000010, B01111111, B01111111, B01000000, B01000000 };
const uint8_t _2[]          = { B01100010, B01110011, B01011001, B01001001, B01101111, B01100110 };
const uint8_t _3[]          = { B00100010, B01100011, B01001001, B01001001, B01111111, B00110110 };
const uint8_t _4[]          = { B00011000, B00011100, B00010110, B01010011, B01111111, B01111111, B01010000 };
const uint8_t _5[]          = { B00100111, B01100111, B01000101, B01000101, B01111101, B00111001 };
const uint8_t _6[]          = { B00111100, B01111110, B01001011, B01001001, B01111001, B00110000 };
const uint8_t _7[]          = { B00000011, B00000011, B01110001, B01111001, B00001111, B00000111 };
const uint8_t _8[]          = { B00110110, B01111111, B01001001, B01001001, B01111111, B00110110 };
const uint8_t _9[]          = { B00000110, B01001111, B01001001, B01101001, B00111111, B00011110 };
const uint8_t A[]           = { B01111100, B01111110, B00010011, B00010011, B01111110, B01111100 };
const uint8_t B[]           = { B01000001, B01111111, B01111111, B01001001, B01001001, B01111111, B00110110 };
const uint8_t C[]           = { B00011100, B00111110, B01100011, B01000001, B01000001, B01100011, B00100010 };
const uint8_t D[]           = { B01000001, B01111111, B01111111, B01000001, B01100011, B00111110, B00011100 };
const uint8_t E[]           = { B01000001, B01111111, B01111111, B01001001, B01011101, B01000001, B01100011 };
const uint8_t F[]           = { B01000001, B01111111, B01111111, B01001001, B00011101, B00000001, B00000011 };
const uint8_t G[]           = { B00011100, B00111110, B01100011, B01000001, B01010001, B01110011, B01110010 };
const uint8_t H[]           = { B01111111, B01111111, B00001000, B00001000, B01111111, B01111111 };
const uint8_t I[]           = { B01000001, B01111111, B01111111, B01000001 };
const uint8_t J[]           = { B00110000, B01110000, B01000000, B01000001, B01111111, B00111111, B00000001 };
const uint8_t K[]           = { B01000001, B01111111, B01111111, B00001000, B00011100, B01110111, B01100011 };
const uint8_t L[]           = { B01000001, B01111111, B01111111, B01000001, B01000000, B01100000, B01110000 };
const uint8_t M[]           = { B01111111, B01111111, B00001110, B00011100, B00001110, B01111111, B01111111 };
const uint8_t N[]           = { B01111111, B01111111, B00000110, B00001100, B00011000, B01111111, B01111111 };
const uint8_t O[]           = { B00011100, B00111110, B01100011, B01000001, B01100011, B00111110, B00011100 };
const uint8_t P[]           = { B01000001, B01111111, B01111111, B01001001, B00001001, B00001111, B00000110 };
const uint8_t Q[]           = { B00011110, B00111111, B00100001, B01110001, B01111111, B01011110 };
const uint8_t R[]           = { B01000001, B01111111, B01111111, B00001001, B00011001, B01111111, B01100110 };
const uint8_t S[]           = { B00100110, B01101111, B01001101, B01011001, B01110011, B00110010 };
const uint8_t T[]           = { B00000011, B01000001, B01111111, B01111111, B01000001, B00000011 };
const uint8_t U[]           = { B01111111, B01111111, B01000000, B01000000, B01111111, B01111111 };
const uint8_t V[]           = { B00011111, B00111111, B01100000, B01100000, B00111111, B00011111 };
const uint8_t W[]           = { B01111111, B01111111, B00110000, B00011000, B00110000, B01111111, B01111111 };
const uint8_t X[]           = { B01000011, B01100111, B00111100, B00011000, B00111100, B01100111, B01000011 };
const uint8_t Y[]           = { B00000111, B01001111, B01111000, B01111000, B01001111, B00000111 };
const uint8_t Z[]           = { B01000111, B01100011, B01110001, B01011001, B01001101, B01100111, B01110011 };
const uint8_t SPACE[]       = { B00000000, B00000000, B00000000 };
const uint8_t COLON[]       = { B00000000, B01100110, B01100110, B00000000 };
const uint8_t SEMI_COLON[]  = { B00000000, B10000000, B11100110, B01100110, B00000000 };
const uint8_t LESS_THAN[]   = { B00001000, B00011100, B00110110, B01100011, B01000001 };
const uint8_t GREA_THAN[]   = { B00000000, B01000001, B01100011, B00110110, B00011100, B00001000 };
const uint8_t BRA_OPEN[]    = { B00000000, B00011100, B00111110, B01100011, B01000001 };
const uint8_t BRA_CLOSE[]   = { B00000000, B01000001, B01100011, B00111110, B00011100 };
const uint8_t PERCENT[]     = { B01000110, B01100110, B00110000, B00011000, B00001100, B01100110, B01100010 };
const uint8_t EQUALS[]      = { B00100100, B00100100, B00100100, B00100100, B00100100, B00100100 };
const uint8_t PLUS[]        = { B00001000, B00001000, B00111110, B00111110, B00001000, B00001000 };
const uint8_t MINUS[]       = { B00001000, B00001000, B00001000, B00001000, B00001000 };
const uint8_t TIMES[]       = { B00001000, B00101010, B00111110, B00011100, B00011100, B00111110, B00101010, B00001000 };
const uint8_t COMMA[]       = { B00000000, B10000000, B11100000, B01100000 };
const uint8_t PERIOD[]      = { B00000000, B01100000, B01100000 };
const uint8_t EXCLAMATION[] = { B00000000, B00000110, B01011111, B01011111, B00000110 };
const uint8_t QUESTION[]    = { B00000010, B00000011, B01010001, B01011001, B00001111, B00000110 };
const uint8_t AT[]          = { B01111110, B01111111, B01000001, B01011101, B01011101, B00011111, B00011110 };
const uint8_t HASH[]        = { B00010100, B01111111, B01111111, B00010100, B01111111, B01111111, B00010100 };
const uint8_t DOLLAR[]      = { B00100100, B00101110, B01101011, B01101011, B00111010, B00010010 };
const uint8_t SQR_OPEN[]    = { B00000000, B01111111, B01111111, B01000001, B01000001 };
const uint8_t SQR_CLOSE[]   = { B00000000, B01000001, B01000001, B01111111, B01111111 };
const uint8_t UNDERSCORE[]  = { B01000000, B01000000, B01000000, B01000000, B01000000 };
const uint8_t SQUIG_OPEN[]  = { B00001000, B00001000, B00111110, B01110111, B01000001, B01000001 };
const uint8_t SQUIG_CLOSE[] = { B01000001, B01000001, B01110111, B00111110, B00001000, B00001000 };

void setup() {
  Serial.begin(9600);
  
  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }

  messageLen = message.length();
  Serial.println(messageLen);
  
  shiftNextLetter();
}

void loop() {
  readSerial();
  
  // Handle a serial command
  if(serailText.length() > 0) {
    Serial.print("Serial command: ");
    Serial.println(serailText);

    clearDisplays();
    
    message = serailText;
    messageLen = message.length();
    messageIdx = -1;
    serailText = "";
    
    shiftNextLetter();
  }
  
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
  delay(50);
}

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
    
    default: copyLetter(SPACE, 3); break;
  }
}

void copyLetter(uint8_t data[], int charLen) {
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

void readSerial() {
  while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() > 0) {
        char c = Serial.read();  //gets one byte from serial buffer
        serailText += c; //makes the string readString
      } 
    }
}

