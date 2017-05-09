#include "LedControl.h"

// pin 12 is connected to the DIN 
// pin 11 is connected to the CLK 
// pin 10 is connected to LOAD 

const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(12, 11, 10, NUM_DISPLAYS);
int NUM_ROWS            = 32;       // NUM_DISPLAYS * 8
int                     rows[32];   // Same as NUM_ROWS
int                     currentChar[8];
int currentCharIdx      = 0;
int currentCharLen      = 0;
String message          = "TESTING THIS LITTLE SCREEN OUT";
int messageIdx          = -1;
int messageLen          = 0;
bool messageChanged     = false;
String serailText;

const int A[]     = { B01111100, B01111110, B00010011, B00010011, B01111110, B01111100 };
const int B[]     = { B01000001, B01111111, B01111111, B01001001, B01001001, B01111111, B00110110 };
const int C[]     = { B00011100, B00111110, B01100011, B01000001, B01000001, B01100011, B00100010 };
const int D[]     = { B01000001, B01111111, B01111111, B01000001, B01100011, B00111110, B00011100 };
const int E[]     = { B01000001, B01111111, B01111111, B01001001, B01011101, B01000001, B01100011 };
const int F[]     = { B01000001, B01111111, B01111111, B01001001, B00011101, B00000001, B00000011 };
const int G[]     = { B00011100, B00111110, B01100011, B01000001, B01010001, B01110011, B01110010 };
const int H[]     = { B01111111, B01111111, B00001000, B00001000, B01111111, B01111111 };
const int I[]     = { B01000001, B01111111, B01111111, B01000001 };
const int J[]     = { B00110000, B01110000, B01000000, B01000001, B01111111, B00111111, B00000001 };
const int K[]     = { B01000001, B01111111, B01111111, B00001000, B00011100, B01110111, B01100011 };
const int L[]     = { B01000001, B01111111, B01111111, B01000001, B01000000, B01100000, B01110000 };
const int M[]     = { B01111111, B01111111, B00001110, B00011100, B00001110, B01111111, B01111111 };
const int N[]     = { B01111111, B01111111, B00000110, B00001100, B00011000, B01111111, B01111111 };
const int O[]     = { B00011100, B00111110, B01100011, B01000001, B01100011, B00111110, B00011100 };
const int P[]     = { B01000001, B01111111, B01111111, B01001001, B00001001, B00001111, B00000110 };
const int Q[]     = { B00011110, B00111111, B00100001, B01110001, B01111111, B01011110 };
const int R[]     = { B01000001, B01111111, B01111111, B00001001, B00011001, B01111111, B01100110 };
const int S[]     = { B00100110, B01101111, B01001101, B01011001, B01110011, B00110010 };
const int T[]     = { B00000011, B01000001, B01111111, B01111111, B01000001, B00000011 };
const int U[]     = { B01111111, B01111111, B01000000, B01000000, B01111111, B01111111 };
const int V[]     = { B00011111, B00111111, B01100000, B01100000, B00111111, B00011111 };
const int W[]     = { B01111111, B01111111, B00110000, B00011000, B00110000, B01111111, B01111111 };
const int X[]     = { B01000011, B01100111, B00111100, B00011000, B00111100, B01100111, B01000011 };
const int Y[]     = { B00000111, B01001111, B01111000, B01111000, B01001111, B00000111 };
const int Z[]     = { B01000111, B01100011, B01110001, B01011001, B01001101, B01100111, B01110011 };
const int SPACE[] = { B00000000, B00000000, B00000000 };

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
    case 'A': copyLetter(A, 6); break;
    case 'B': copyLetter(B, 7); break;
    case 'C': copyLetter(C, 7); break;
    case 'D': copyLetter(D, 7); break;
    case 'E': copyLetter(E, 7); break;
    case 'F': copyLetter(F, 7); break;
    case 'G': copyLetter(G, 7); break;
    case 'H': copyLetter(H, 6); break;
    case 'I': copyLetter(I, 4); break;
    case 'J': copyLetter(J, 7); break;
    case 'K': copyLetter(K, 7); break;
    case 'L': copyLetter(L, 7); break;
    case 'M': copyLetter(M, 7); break;
    case 'N': copyLetter(N, 7); break;
    case 'O': copyLetter(O, 7); break;
    case 'P': copyLetter(P, 7); break;
    case 'Q': copyLetter(Q, 6); break;
    case 'R': copyLetter(R, 7); break;
    case 'S': copyLetter(S, 6); break;
    case 'T': copyLetter(T, 6); break;
    case 'U': copyLetter(U, 6); break;
    case 'V': copyLetter(V, 6); break;
    case 'W': copyLetter(W, 7); break;
    case 'X': copyLetter(X, 7); break;
    case 'Y': copyLetter(Y, 6); break;
    case 'Z': copyLetter(Z, 7); break;
    default: copyLetter(SPACE, 3); break;
  }
}

void copyLetter(int data[], int charLen) {
  currentCharLen = charLen;

  Serial.print("New letter: ");
  
  for(int i = 0; i < charLen; i++) {
    currentChar[i] = data[i];

    Serial.print(data[i]);
    Serial.print(",");
  }

  Serial.print(" @ ");
  Serial.print(charLen);

  Serial.println();
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

