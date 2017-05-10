#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "LedControl.h"

// pin D0 is connected to DIN 
// pin D1 is connected to CLK 
// pin D2 is connected to LOAD 

const char* ssid        = "PickMe";
const char* password    = "fallout312345";
const char* host        = "esp8266sd";
const int NUM_DISPLAYS  = 4;
LedControl lc           = LedControl(D0, D1, D2, NUM_DISPLAYS);
int NUM_ROWS            = 32;       // NUM_DISPLAYS * 8
int                     rows[32];   // Same as NUM_ROWS
int                     currentChar[8];
int currentCharIdx      = 0;
int currentCharLen      = 0;
String message          = "READY";
int messageIdx          = -1;
int messageLen          = 0;
bool messageChanged     = false;
String serailText;

ESP8266WebServer server(80);

int _0[]          = { B00111110, B01111111, B01110001, B01011001, B01001101, B01111111, B00111110 };
int _1[]          = { B01000000, B01000010, B01111111, B01111111, B01000000, B01000000 };
int _2[]          = { B01100010, B01110011, B01011001, B01001001, B01101111, B01100110 };
int _3[]          = { B00100010, B01100011, B01001001, B01001001, B01111111, B00110110 };
int _4[]          = { B00011000, B00011100, B00010110, B01010011, B01111111, B01111111, B01010000 };
int _5[]          = { B00100111, B01100111, B01000101, B01000101, B01111101, B00111001 };
int _6[]          = { B00111100, B01111110, B01001011, B01001001, B01111001, B00110000 };
int _7[]          = { B00000011, B00000011, B01110001, B01111001, B00001111, B00000111 };
int _8[]          = { B00110110, B01111111, B01001001, B01001001, B01111111, B00110110 };
int _9[]          = { B00000110, B01001111, B01001001, B01101001, B00111111, B00011110 };
int A[]           = { B01111100, B01111110, B00010011, B00010011, B01111110, B01111100 };
int B[]           = { B01000001, B01111111, B01111111, B01001001, B01001001, B01111111, B00110110 };
int C[]           = { B00011100, B00111110, B01100011, B01000001, B01000001, B01100011, B00100010 };
int D[]           = { B01000001, B01111111, B01111111, B01000001, B01100011, B00111110, B00011100 };
int E[]           = { B01000001, B01111111, B01111111, B01001001, B01011101, B01000001, B01100011 };
int F[]           = { B01000001, B01111111, B01111111, B01001001, B00011101, B00000001, B00000011 };
int G[]           = { B00011100, B00111110, B01100011, B01000001, B01010001, B01110011, B01110010 };
int H[]           = { B01111111, B01111111, B00001000, B00001000, B01111111, B01111111 };
int I[]           = { B01000001, B01111111, B01111111, B01000001 };
int J[]           = { B00110000, B01110000, B01000000, B01000001, B01111111, B00111111, B00000001 };
int K[]           = { B01000001, B01111111, B01111111, B00001000, B00011100, B01110111, B01100011 };
int L[]           = { B01000001, B01111111, B01111111, B01000001, B01000000, B01100000, B01110000 };
int M[]           = { B01111111, B01111111, B00001110, B00011100, B00001110, B01111111, B01111111 };
int N[]           = { B01111111, B01111111, B00000110, B00001100, B00011000, B01111111, B01111111 };
int O[]           = { B00011100, B00111110, B01100011, B01000001, B01100011, B00111110, B00011100 };
int P[]           = { B01000001, B01111111, B01111111, B01001001, B00001001, B00001111, B00000110 };
int Q[]           = { B00011110, B00111111, B00100001, B01110001, B01111111, B01011110 };
int R[]           = { B01000001, B01111111, B01111111, B00001001, B00011001, B01111111, B01100110 };
int S[]           = { B00100110, B01101111, B01001101, B01011001, B01110011, B00110010 };
int T[]           = { B00000011, B01000001, B01111111, B01111111, B01000001, B00000011 };
int U[]           = { B01111111, B01111111, B01000000, B01000000, B01111111, B01111111 };
int V[]           = { B00011111, B00111111, B01100000, B01100000, B00111111, B00011111 };
int W[]           = { B01111111, B01111111, B00110000, B00011000, B00110000, B01111111, B01111111 };
int X[]           = { B01000011, B01100111, B00111100, B00011000, B00111100, B01100111, B01000011 };
int Y[]           = { B00000111, B01001111, B01111000, B01111000, B01001111, B00000111 };
int Z[]           = { B01000111, B01100011, B01110001, B01011001, B01001101, B01100111, B01110011 };
int SPACE[]       = { B00000000, B00000000, B00000000 };
int COLON[]       = { B00000000, B01100110, B01100110, B00000000 };
int SEMI_COLON[]  = { B00000000, B10000000, B11100110, B01100110, B00000000 };
int LESS_THAN[]   = { B00001000, B00011100, B00110110, B01100011, B01000001 };
int GREA_THAN[]   = { B00000000, B01000001, B01100011, B00110110, B00011100, B00001000 };
int BRA_OPEN[]    = { B00000000, B00011100, B00111110, B01100011, B01000001 };
int BRA_CLOSE[]   = { B00000000, B01000001, B01100011, B00111110, B00011100 };
int PERCENT[]     = { B01000110, B01100110, B00110000, B00011000, B00001100, B01100110, B01100010 };
int EQUALS[]      = { B00100100, B00100100, B00100100, B00100100, B00100100, B00100100 };
int PLUS[]        = { B00001000, B00001000, B00111110, B00111110, B00001000, B00001000 };
int MINUS[]       = { B00001000, B00001000, B00001000, B00001000, B00001000 };
int TIMES[]       = { B00001000, B00101010, B00111110, B00011100, B00011100, B00111110, B00101010, B00001000 };
int COMMA[]       = { B00000000, B10000000, B11100000, B01100000 };
int PERIOD[]      = { B00000000, B01100000, B01100000 };
int EXCLAMATION[] = { B00000000, B00000110, B01011111, B01011111, B00000110 };
int QUESTION[]    = { B00000010, B00000011, B01010001, B01011001, B00001111, B00000110 };
int AT[]          = { B01111110, B01111111, B01000001, B01011101, B01011101, B00011111, B00011110 };
int HASH[]        = { B00010100, B01111111, B01111111, B00010100, B01111111, B01111111, B00010100 };
int DOLLAR[]      = { B00100100, B00101110, B01101011, B01101011, B00111010, B00010010 };
int SQR_OPEN[]    = { B00000000, B01111111, B01111111, B01000001, B01000001 };
int SQR_CLOSE[]   = { B00000000, B01000001, B01000001, B01111111, B01111111 };
int UNDERSCORE[]  = { B01000000, B01000000, B01000000, B01000000, B01000000 };
int SQUIG_OPEN[]  = { B00001000, B00001000, B00111110, B01110111, B01000001, B01000001 };
int SQUIG_CLOSE[] = { B01000001, B01000001, B01110111, B00111110, B00001000, B00001000 };


void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void handleCreate(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);

  Serial.print("Serial command: ");
  Serial.println(path);

  clearDisplays();
  
  message = path;
  messageLen = message.length();
  messageIdx = -1;
  serailText = "";
  
  shiftNextLetter();
  
  returnOK();
}

void setup() {
  Serial.begin(9600);

  for(int address = 0; address < NUM_DISPLAYS; address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 2);
    lc.clearDisplay(address);
  }
  
  messageLen = message.length();
  
  Serial.setDebugOutput(true);
  Serial.print("\n");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if(i == 21){
    Serial.print("Could not connect to");
    Serial.println(ssid);
    while(1) delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/display", HTTP_PUT, handleCreate);
  server.begin();
  shiftNextLetter();
}

void loop() {
  server.handleClient();
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



