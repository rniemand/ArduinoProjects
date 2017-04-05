#include <SparkFun_Tlc5940.h>

// http://tronixstuff.com/2013/10/21/tutorial-arduino-tlc5940-led-driver-ic/


byte one[]      = {1,0,0,0,0,0,1};
byte two[]      = {0,1,1,1,0,1,1};
byte three[]    = {1,1,0,1,0,1,1};
byte four[]     = {1,0,0,1,1,0,1};
byte five[]     = {1,1,0,1,1,1,0};
byte six[]      = {1,1,1,1,1,1,0};
byte seven[]    = {1,0,0,0,0,1,1};
byte eight[]    = {1,1,1,1,1,1,1};
byte nine[]     = {1,1,0,1,1,1,1};
byte zero[]     = {1,1,1,0,1,1,1};

int _delay      = 300;


void setup() {
  //Serial.begin(9600);
  Tlc.init(0); // initialise TLC5940 and set all channels off
  Tlc.update();

  updateDisplay(one, 1);
}

void loop() {
  updateDisplay(one, 0);
  updateDisplay(one, 6);
  delay(_delay);
  updateDisplay(two, 0);
  updateDisplay(two, 6);
  delay(_delay);
  updateDisplay(three, 0);
  updateDisplay(three, 6);
  delay(_delay);
  updateDisplay(four, 0);
  updateDisplay(four, 6);
  delay(_delay);
  updateDisplay(five, 0);
  updateDisplay(five, 6);
  delay(_delay);
  updateDisplay(six, 0);
  updateDisplay(six, 6);
  delay(_delay);
  updateDisplay(seven, 0);
  updateDisplay(seven, 6);
  delay(_delay);
  updateDisplay(eight, 0);
  updateDisplay(eight, 6);
  delay(_delay);
  updateDisplay(nine, 0);
  updateDisplay(nine, 6);
  delay(_delay);
  updateDisplay(zero, 0);
  updateDisplay(zero, 6);
  delay(_delay);
}

void updateDisplay(byte pins[], int offset) {
  for(int i = 0; i < 7; i++) {
    int pinNumber = offset + i;
    int brightness = pins[i] ? 512 : 0;
    
    Tlc.set(pinNumber, (pins[i] == 1 ? 512 : 0));
    //if(brightness > 0) {
      //Serial.print("Setting pin-");
      //Serial.print(pinNumber);
      //Serial.println(" to HIGH");
    //}
  }
  
  Tlc.update();
}

void cycle() {
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, 100);
  }
  Tlc.update();
  delay(1000);
  
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, 200);
  }
  Tlc.update();
  delay(1000);
  
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, 400);
  }
  Tlc.update();
  delay(1000);
  
  for (int i = 0; i < 16; i++) {
    Tlc.set(i, 800);
  }
  Tlc.update();
  delay(1000);
}

