#include "FastLED.h"

/* ********************************************************************************************************
 *  Config
 ******************************************************************************************************** */
#define NUM_LEDS            36
#define DATA_PIN            3
int ANIMATION_DELAY         = 50;

CRGB leds[NUM_LEDS];

/* ********************************************************************************************************
 *    Pin Out
 * ********************************************************************************************************
 *    A1 .................... Button (to GND)
 *    D3 .................... LED Data Line
 *    
 * ****************************************************************************************************** */
 
void setup() {
  pinMode(A1, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  if(digitalRead(A1) == 0) {
    runSpin();
  }
}

// ********************************************************************************************************
CRGB randomColor() {
  return CRGB( random(20, 255), random(20, 255), random(20, 255) );
}

void runSpin() {
  // Play a random animation
  switch(random(1, 11)) {
    case 1: spinAnimation1(); break;
    case 2: spinAnimation2(); break;
    case 3: spinAnimation3(); break;
    case 4: spinAnimation4(); break;
    case 5: spinAnimation5(); break;
    case 6: spinAnimation6(); break;
    case 7: spinAnimation7(); break;
    case 8: spinAnimation8(); break;
    case 9: spinAnimation9(); break;
    case 10: spinAnimation10(); break;
    /*
    case 11: spinAnimation11(); break;
    case 12: spinAnimation12(); break;
    case 13: spinAnimation13(); break;
    case 14: spinAnimation14(); break;
    */
    default: spinAnimation1(); break;
  }

  // Render the number
  renderNumber(random(1, 7));

  // Hack for easy debouncing
  delay(75);
}

void clearDisplay() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

void renderNumber(int number) {
  if(number == 1) {
    one();
  }
  else if(number == 2) {
    two();
  }
  else if(number == 3) {
    three();
  }
  else if(number == 4) {
    four();
  }
  else if(number == 5) {
    five();
  }
  else if(number == 6) {
    six();
  }
  else {
    
  }
}

void one() {
  clearDisplay();
  short pins[] = {33,32,26,20,14,8,9,7};
  colorArray(pins, 8, ANIMATION_DELAY/2, randomColor());

  delay(250);
}

void two() {
  clearDisplay();
  short pins[] = {33,32,31,25,19,20,21,15,9,8,7};
  colorArray(pins, 11, ANIMATION_DELAY/2, randomColor());
}

void three() {
  clearDisplay();
  short pins[] = {33,32,31,25,19,13,7,8,9,20,21};
  colorArray(pins, 11, ANIMATION_DELAY/2, randomColor());
}

void four() {
  clearDisplay();
  short pins[] = {33,27,21,20,19,25,31,13,7};
  colorArray(pins, 9, ANIMATION_DELAY/2, randomColor());
}

void five() {
  clearDisplay();
  short pins[] = {31,32,33,27,21,20,19,13,7,8,9};
  colorArray(pins, 11, ANIMATION_DELAY/2, randomColor());
}

void six() {
  clearDisplay();
  short pins[] = {31,32,33,27,21,20,19,13,7,8,9,15};
  colorArray(pins, 12, ANIMATION_DELAY/2, randomColor());
}

void unknown() {
  clearDisplay();
  leds[0] = CRGB(0, 42, 255);
  leds[5] = CRGB(0, 42, 255);
  leds[7] = CRGB(0, 42, 255);
  leds[10] = CRGB(0, 42, 255);
  leds[14] = CRGB(0, 42, 255);
  leds[15] = CRGB(0, 42, 255);
  leds[20] = CRGB(0, 42, 255);
  leds[21] = CRGB(0, 42, 255);
  leds[25] = CRGB(0, 42, 255);
  leds[28] = CRGB(0, 42, 255);
  leds[30] = CRGB(0, 42, 255);
  leds[35] = CRGB(0, 42, 255);
  FastLED.show();
}

void spinAnimation1() {
  clearDisplay();

  short a[] = {0,2,4,7,9,11,12,14,16,19,21,23,24,26,28,31,33,35};
  colorArray(a, 18, ANIMATION_DELAY, CRGB(125, 66, 241));

  short b[] = {1,3,5,6,8,10,13,15,17,18,20,22,25,27,29,30,32,34};
  colorArray(b, 18, ANIMATION_DELAY, CRGB(241, 237, 66));
  
  delay(250);
}

void spinAnimation2() {
  clearDisplay();

  short a[] = {35,29,23,17,11,5};
  colorArray(a, 6, ANIMATION_DELAY*2, randomColor());
  delay(75);
  
  short b[] = {34,28,22,16,10,4};
  colorArray(b, 6, ANIMATION_DELAY*2, randomColor());
  delay(75);
  
  short c[] = {33,27,21,15,9,3};
  colorArray(c, 6, ANIMATION_DELAY*2, randomColor());
  delay(75);

  short d[] = {32,26,20,14,8,2};
  colorArray(d, 6, ANIMATION_DELAY*2, randomColor());
  delay(75);

  short e[] = {31,25,19,13,7,1};
  colorArray(e, 6, ANIMATION_DELAY*2, randomColor());
  delay(75);

  short f[] = {30,24,18,12,6,0};
  colorArray(f, 6, ANIMATION_DELAY*2, randomColor());
  
  delay(250);
}

void spinAnimation3() {
  short offset = 0;
  
  for(short i = 0; i < 6; i++) {
    clearDisplay();
    leds[0 + offset] = randomColor();
    leds[6 + offset] = randomColor();
    leds[12 + offset] = randomColor();
    leds[18 + offset] = randomColor();
    leds[24 + offset] = randomColor();
    leds[30 + offset] = randomColor();
    FastLED.show();
    delay(ANIMATION_DELAY * 3);

    offset ++;
  }
  
  delay(250);
}

void spinAnimation4() {
  clearDisplay();

  for(short j = 0; j < 6; j++) {
    for(short i = 0; i < NUM_LEDS; i++) {
      leds[i] = randomColor();
    }
    FastLED.show();
    delay(ANIMATION_DELAY * 2);
  }

  delay(250);
}

void spinAnimation5() {
  clearDisplay();

  for(short i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
    FastLED.show();
    delay(20);
  }

  delay(250);
}

void spinAnimation6() {
  clearDisplay();

  for(short i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
  }

  FastLED.show();
  delay(50);

  CRGB black = CRGB(0,0,0);
  for(short i = NUM_LEDS; i >= 0; i--) {
    leds[i] = black;
    FastLED.show();
    delay(20);
  }
  
  delay(250);
}

void spinAnimation7() {
  short offset = 0;

  for(short i = 0; i < 6; i++) {
    clearDisplay();
    CRGB color = CRGB(random(0,255), random(0,255), random(0,255));
    leds[0 + offset] = color;
    leds[1 + offset] = color;
    leds[2 + offset] = color;
    leds[3 + offset] = color;
    leds[4 + offset] = color;
    leds[5 + offset] = color;
    
    FastLED.show();
    delay(ANIMATION_DELAY * 3);
    offset += 6;
  }
  
  delay(250);
}

void spinAnimation8() {
  short offset = 0;

  for(short i = 0; i < 6; i++) {
    clearDisplay();
    CRGB color = CRGB(random(0,255), random(0,255), random(0,255));
    leds[35 - offset] = color;
    leds[34 - offset] = color;
    leds[33 - offset] = color;
    leds[32 - offset] = color;
    leds[31 - offset] = color;
    leds[30 - offset] = color;
    
    FastLED.show();
    delay(ANIMATION_DELAY * 3);
    offset += 6;
  }
  
  delay(250);
}

void spinAnimation9() {
  clearDisplay();

  short middle[] = {14, 15, 21, 20};
  colorArray(middle, 4, ANIMATION_DELAY, CRGB(255, 0, 0));

  short inner[] = {7,8,9,10,16,22,28,27,26,25,19,13};
  colorArray(inner, 12, ANIMATION_DELAY, CRGB(0, 255, 0));

  short outer[] = {30,24,18,12,6,0,1,2,3,4,5,11,17,23,29,35,34,33,32,31};
  colorArray(outer, 20, ANIMATION_DELAY, CRGB(0, 0, 255));
  
  delay(250);
}

void spinAnimation10() {
  clearDisplay();

  short middle[] = {30,31,32,33,34,35,29,23,17,11,5,4,3,2,1,0,6,12,18,24,25,26,27,28,22,16,10,9,8,7,13,19,20,21,15,14};
  colorArray(middle, 36, ANIMATION_DELAY, randomColor());
  
  delay(250);
}

/*
void spinAnimation11() {
  clearDisplay();

  short a[] = {0};
  colorArray(a, 1, 0, randomColor());
  delay(20);

  short b[] = {1,6};
  colorArray(b, 2, 0, randomColor());
  delay(20);

  short c[] = {2,7,12};
  colorArray(c, 3, 0, randomColor());
  delay(20);

  short d[] = {3,8,13,18};
  colorArray(d, 4, 0, randomColor());
  delay(20);

  short e[] = {4,9,14,19,24};
  colorArray(e, 5, 0, randomColor());
  delay(20);

  short f[] = {5,10,15,20,25,30};
  colorArray(f, 6, 0, randomColor());
  delay(20);

  short g[] = {11,16,21,26,31};
  colorArray(g, 5, 0, randomColor());
  delay(20);

  short h[] = {17,22,27,32};
  colorArray(h, 4, 0, randomColor());
  delay(20);

  short i[] = {23,28,33};
  colorArray(i, 3, 0, randomColor());
  delay(20);

  short j[] = {29,34};
  colorArray(j, 2, 0, randomColor());
  delay(20);

  short k[] = {35};
  colorArray(k, 1, 0, randomColor());
  delay(20);
  
  delay(250);
}

void spinAnimation12() {
  clearDisplay();

  short k[] = {35};
  colorArray(k, 1, 0, randomColor());
  delay(20);

  short j[] = {29,34};
  colorArray(j, 2, 0, randomColor());
  delay(20);

  short i[] = {23,28,33};
  colorArray(i, 3, 0, randomColor());
  delay(20);

  short h[] = {17,22,27,32};
  colorArray(h, 4, 0, randomColor());
  delay(20);

  short g[] = {11,16,21,26,31};
  colorArray(g, 5, 0, randomColor());
  delay(20);

  short f[] = {5,10,15,20,25,30};
  colorArray(f, 6, 0, randomColor());
  delay(20);

  short e[] = {4,9,14,19,24};
  colorArray(e, 5, 0, randomColor());
  delay(20);

  short d[] = {3,8,13,18};
  colorArray(d, 4, 0, randomColor());
  delay(20);

  short c[] = {2,7,12};
  colorArray(c, 3, 0, randomColor());
  delay(20);
  
  short b[] = {1,6};
  colorArray(b, 2, 0, randomColor());
  delay(20);

  short a[] = {0};
  colorArray(a, 1, 0, randomColor());
  delay(20);
  
  delay(250);
}

void spinAnimation13() {
  clearDisplay();

  short a[] = {35,29,23,17,11,5,4,3,2,1,0,6,12,18,24,30,31,32,33,34};
  colorArray(a, 20, ANIMATION_DELAY/2, randomColor());
  delay(75);
  short b[] = {21,15,14,20};
  colorArray(b, 4, ANIMATION_DELAY/2, randomColor());
  
  delay(250);
}

void spinAnimation14() {
  clearDisplay();

  short a[] = {21,15,14,20};
  colorArray(a, 4, ANIMATION_DELAY/2, randomColor());
  delay(75);
  short b[] = {28,27,26,25,19,13,7,8,9,10,16,22};
  CRGB color = randomColor();
  for(short i = 0; i < 12; i++) {
    colorSingle(b[i], ANIMATION_DELAY, color);
  }
  
  delay(250);
}

*/

void colorArray(short arr[], int arrSize, int animationDelay, CRGB color) {
  for (int i = 0; i < arrSize; i++) {
    leds[arr[i]] = color;
    FastLED.show();
    delay(animationDelay);
  }
}

void colorSingle(short pixel, int animationDelay, CRGB color) {
  leds[pixel] = color;
  FastLED.show();
  delay(animationDelay);
}


