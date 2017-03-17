#include <SparkFun_Tlc5940.h>
/*
#include <tlc_animations.h>
#include <tlc_config.h>
#include <tlc_fades.h>
#include <tlc_progmem_utils.h>
#include <tlc_servos.h>
#include <tlc_shifts.h>
*/

// http://tronixstuff.com/2013/10/21/tutorial-arduino-tlc5940-led-driver-ic/

void setup()
{
  //Serial.begin(9600);
  Tlc.init(0); // initialise TLC5940 and set all channels off
  Tlc.update();
}

int i = 0;
bool forward = true;
int brightness = 512;

void loop() {
  if(i == 0) {
    i = 1;
    forward = true;
    adjustBrightness();
  }
  
  //Serial.println(i);
  
  Tlc.set((forward ? i-1 : i+1), 0);
  Tlc.set(i, brightness);
  Tlc.update();
  
  if(i == 15) { 
    forward = false;
    adjustBrightness();
  }

  i = forward ? i+1 : i-1;
  
  delay(50);
}

void adjustBrightness() {
  brightness += 512;

  if(brightness > 2048) {
    brightness = 512;
  }
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

