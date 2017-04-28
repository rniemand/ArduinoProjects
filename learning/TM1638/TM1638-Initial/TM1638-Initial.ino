#include <InvertedTM1638.h>
#include <TM1638.h>
#include <TM1638QYF.h>
#include <TM1640.h>
#include <TM16XX.h>
#include <TM16XXFonts.h>

const int stb = 8;
const int clk = 9;
const int dio = 10;
TM1638 module(dio, clk, stb);

void setup() {
  Serial.begin(9600);
  
  pinMode(stb, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);

  reset();
  sendCommand(0x8a); // activate and set brightness to max
}

// https://github.com/rjbatista/tm1638-library

void loop() {
  //demoLeds();

  //sendCommand(0x01);

  /*
  for(int i = 0; i < 99999999; i++) {
    module.setDisplayToDecNumber(i,0,true);
    delay(5);
  }
  */

  module.setDisplayToDecNumber(12345678,0,true);
  setLed(1, 0);
  setLed(1, 1);
  setLed(1, 2);
  setLed(1, 3);
  setLed(1, 4);
  setLed(1, 5);
  setLed(1, 6);
  setLed(1, 7);

  delay(10000);
}

void demoLedButtons() {
  uint8_t buttons = readButtons();
 
  for(uint8_t position = 0; position < 8; position++)
  {
    uint8_t mask = 0x1 << position;
    setLed(buttons & mask ? 1 : 0, position);
  }
}

void demoLeds() {
  for(int i = 0; i < 8; i++) {
    if(i > -1 ) { setLed(0, i-1); }
    if(i == 0) { setLed(0, 7); }
    
    setLed(1, i);
    delay(100);
  }
}

void sendCommand(uint8_t value) {
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, value);
  digitalWrite(stb, HIGH);
}

void reset() {
  sendCommand(0x40); // set auto increment mode
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0xc0);   // set starting address to 0
  
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(dio, clk, LSBFIRST, 0x00);
  }
  
  digitalWrite(stb, HIGH);
}

uint8_t readButtons(void) {
  uint8_t buttons = 0;
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0x42);
 
  pinMode(dio, INPUT);
 
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(dio, clk, LSBFIRST) << i;
    buttons |= v;
  }
 
  pinMode(dio, OUTPUT);
  digitalWrite(stb, HIGH);
  
  return buttons;
}

void setLed(uint8_t value, uint8_t position) {
  pinMode(dio, OUTPUT);
  
  sendCommand(0x44);
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0xC1 + (position << 1));
  shiftOut(dio, clk, LSBFIRST, value);
  digitalWrite(stb, HIGH);
}


