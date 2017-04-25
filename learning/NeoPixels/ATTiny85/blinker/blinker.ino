#include "FastLED.h"

#define NUM_LEDS            36
#define DATA_PIN            PB2
int ANIMATION_DELAY         = 50;
CRGB                        leds[NUM_LEDS];

const short ONE[]        = {33,32,26,20,14,8,9,7};
const short TWO[]        = {33,32,31,25,19,20,21,15,9,8,7};
const short THREE[]      = {33,32,31,25,19,13,7,8,9,20,21};
const short FOUR[]       = {33,27,21,20,19,25,31,13,7};
const short FIVE[]       = {31,32,33,27,21,20,19,13,7,8,9};
const short SIX[]        = {31,32,33,27,21,20,19,13,7,8,9,15};

/* ********************************************************************************************************
 *  Core
 *  *******************************************************************************************************/
void setup() {
  //pinMode(PB3, OUTPUT);
  //digitalWrite(PB3, HIGH);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  one();
  randomAnimation();
  two();
  randomAnimation();
  three();
  randomAnimation();
  four();
  randomAnimation();
  five();
  randomAnimation();
  six();
  randomAnimation();
}

/* ********************************************************************************************************
 *  Functions
 *  *******************************************************************************************************/
CRGB randomColor() {
  return CRGB( random(20, 255), random(20, 255), random(20, 255) );
}

void simpleFlash() {
  digitalWrite(PB3, LOW);
  delay(1000);
  digitalWrite(PB3, HIGH);
  delay(1000);
}

void clearDisplay() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

void colorArray(short arr[], int arrSize, int ANIMATION_DELAY, CRGB color) {
  for (int i = 0; i < arrSize; i++) {
    leds[arr[i]] = color;
    FastLED.show();
    delay(ANIMATION_DELAY);
  }
}

void colorSingle(short pixel, int animationDelay, CRGB color) {
  leds[pixel] = color;
  FastLED.show();
  delay(animationDelay);
}

/* ********************************************************************************************************
 *  Numbers
 *  *******************************************************************************************************/
void one() {
  clearDisplay();
  colorArray(ONE, 8, ANIMATION_DELAY, randomColor());
  delay(250);
}

void two() {
  clearDisplay();
  colorArray(TWO, 11, ANIMATION_DELAY, randomColor());
}

void three() {
  clearDisplay();
  colorArray(THREE, 11, ANIMATION_DELAY, randomColor());
}

void four() {
  clearDisplay();
  colorArray(FOUR, 9, ANIMATION_DELAY, randomColor());
}

void five() {
  clearDisplay();
  colorArray(FIVE, 11, ANIMATION_DELAY, randomColor());
}

void six() {
  clearDisplay();
  colorArray(SIX, 12, ANIMATION_DELAY, randomColor());
}

/* ********************************************************************************************************
 *  Animations
 *  *******************************************************************************************************/
void randomAnimation() {
  switch(random(1,3)) {
    case 1: animation01(); break;
    case 2: animation02(); break;
    //case 3: animation03(); break;
    //case 4: animation04(); break;
    default: animation01(); break;
  }

  delay(250);
}

void animation01() {
  clearDisplay();
  
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
    FastLED.show();
    delay(ANIMATION_DELAY/2);
  }
}

void animation02() {
  clearDisplay();
  
  for(int i = NUM_LEDS; i >= 0; i--) {
    leds[i] = randomColor();
    FastLED.show();
    delay(ANIMATION_DELAY);
  }
}


