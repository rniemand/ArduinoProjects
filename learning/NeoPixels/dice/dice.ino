 #include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 36
#define DATA_PIN 3
#define ANIMATION_DELAY 50
CRGB leds[NUM_LEDS];

void setup() {
  pinMode(A1, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  if(digitalRead(A1) == 0) {
    runSpin();
  }
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
  leds[10] = CRGB(51, 161, 241);
  leds[16] = CRGB(51, 161, 241);
  leds[22] = CRGB(51, 161, 241);
  leds[28] = CRGB(51, 161, 241);
  leds[34] = CRGB(51, 161, 241);
  FastLED.show();
}

void two() {
  clearDisplay();
  leds[8] = CRGB(233, 0, 255);
  leds[9] = CRGB(233, 0, 255);
  leds[10] = CRGB(233, 0, 255);
  leds[16] = CRGB(233, 0, 255);
  leds[20] = CRGB(233, 0, 255);
  leds[21] = CRGB(233, 0, 255);
  leds[22] = CRGB(233, 0, 255);
  leds[26] = CRGB(233, 0, 255);
  leds[32] = CRGB(233, 0, 255);
  leds[33] = CRGB(233, 0, 255);
  leds[34] = CRGB(233, 0, 255);
  FastLED.show();
}

void three() {
  clearDisplay();
  leds[8] = CRGB(255, 0, 0);
  leds[9] = CRGB(255, 0, 0);
  leds[10] = CRGB(255, 0, 0);
  leds[14] = CRGB(255, 0, 0);
  leds[20] = CRGB(255, 0, 0);
  leds[21] = CRGB(255, 0, 0);
  leds[22] = CRGB(255, 0, 0);
  leds[26] = CRGB(255, 0, 0);
  leds[32] = CRGB(255, 0, 0);
  leds[33] = CRGB(255, 0, 0);
  leds[34] = CRGB(255, 0, 0);
  FastLED.show();
}

void four() {
  clearDisplay();
  leds[8] = CRGB(22, 255, 0);
  leds[14] = CRGB(22, 255, 0);
  leds[15] = CRGB(22, 255, 0);
  leds[16] = CRGB(22, 255, 0);
  leds[20] = CRGB(22, 255, 0);
  leds[22] = CRGB(22, 255, 0);
  leds[28] = CRGB(22, 255, 0);
  leds[34] = CRGB(22, 255, 0);
  FastLED.show();
}

void five() {
  clearDisplay();
  leds[8] = CRGB(238, 255, 0);
  leds[9] = CRGB(238, 255, 0);
  leds[10] = CRGB(238, 255, 0);
  leds[14] = CRGB(238, 255, 0);
  leds[20] = CRGB(238, 255, 0);
  leds[21] = CRGB(238, 255, 0);
  leds[22] = CRGB(238, 255, 0);
  leds[28] = CRGB(238, 255, 0);
  leds[32] = CRGB(238, 255, 0);
  leds[33] = CRGB(238, 255, 0);
  leds[34] = CRGB(238, 255, 0);
  FastLED.show();
}

void six() {
  clearDisplay();
  leds[8] = CRGB(255, 0, 254);
  leds[9] = CRGB(255, 0, 254);
  leds[10] = CRGB(255, 0, 254);
  leds[14] = CRGB(255, 0, 254);
  leds[16] = CRGB(255, 0, 254);
  leds[20] = CRGB(255, 0, 254);
  leds[21] = CRGB(255, 0, 254);
  leds[22] = CRGB(255, 0, 254);
  leds[28] = CRGB(255, 0, 254);
  leds[32] = CRGB(255, 0, 254);
  leds[33] = CRGB(255, 0, 254);
  leds[34] = CRGB(255, 0, 254);
  FastLED.show();
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
  colorArray(a, 18, CRGB(125, 66, 241));

  short b[] = {1,3,5,6,8,10,13,15,17,18,20,22,25,27,29,30,32,34};
  colorArray(b, 18, CRGB(241, 237, 66));
  
  delay(250);
}

void spinAnimation2() {
  CRGB color = CRGB(random(0,255), random(0,255), random(0,255));
  
  for(short i = 0; i < 5; i++) {
    clearDisplay();
    leds[0] = CRGB(color);
    leds[2] = CRGB(color);
    leds[4] = CRGB(color);
    leds[6] = CRGB(color);
    leds[8] = CRGB(color);
    leds[10] = CRGB(color);
    leds[12] = CRGB(color);
    leds[14] = CRGB(color);
    leds[16] = CRGB(color);
    leds[18] = CRGB(color);
    leds[20] = CRGB(color);
    leds[22] = CRGB(color);
    leds[24] = CRGB(color);
    leds[26] = CRGB(color);
    leds[28] = CRGB(color);
    leds[30] = CRGB(color);
    leds[32] = CRGB(color);
    leds[34] = CRGB(color);
    FastLED.show();
    delay(ANIMATION_DELAY * 3);

    clearDisplay();
    leds[1] = CRGB(color);
    leds[3] = CRGB(color);
    leds[5] = CRGB(color);
    leds[7] = CRGB(color);
    leds[9] = CRGB(color);
    leds[11] = CRGB(color);
    leds[13] = CRGB(color);
    leds[15] = CRGB(color);
    leds[17] = CRGB(color);
    leds[19] = CRGB(color);
    leds[21] = CRGB(color);
    leds[23] = CRGB(color);
    leds[25] = CRGB(color);
    leds[27] = CRGB(color);
    leds[29] = CRGB(color);
    leds[31] = CRGB(color);
    leds[33] = CRGB(color);
    leds[35] = CRGB(color);
    FastLED.show();
    delay(ANIMATION_DELAY * 3);
  }
  
  delay(250);
}

void spinAnimation3() {
  short offset = 0;
  
  for(short i = 0; i < 6; i++) {
    CRGB color = CRGB(random(0,255), random(0,255), random(0,255));
    
    clearDisplay();
    leds[0 + offset] = CRGB(color);
    leds[6 + offset] = CRGB(color);
    leds[12 + offset] = CRGB(color);
    leds[18 + offset] = CRGB(color);
    leds[24 + offset] = CRGB(color);
    leds[30 + offset] = CRGB(color);
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
      leds[i] = CRGB(random(0,255), random(0,255), random(0,255));
    }
    FastLED.show();
    delay(ANIMATION_DELAY * 2);
  }

  delay(250);
}

void spinAnimation5() {
  clearDisplay();

  for(short i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(random(0,255), random(0,255), random(0,255));
    FastLED.show();
    delay(20);
  }

  delay(250);
}

void spinAnimation6() {
  clearDisplay();

  for(short i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(random(0,255), random(0,255), random(0,255));
  }

  FastLED.show();
  delay(50);

  for(short i = NUM_LEDS; i >= 0; i--) {
    leds[i] = CRGB(0,0,0);
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
  colorArray(middle, 4, CRGB(255, 0, 0));

  short inner[] = {7,8,9,10,16,22,28,27,26,25,19,13};
  colorArray(inner, 12, CRGB(0, 255, 0));

  short outer[] = {30,24,18,12,6,0,1,2,3,4,5,11,17,23,29,35,34,33,32,31};
  colorArray(outer, 20, CRGB(0, 0, 255));
  
  delay(250);
}

void spinAnimation10() {
  clearDisplay();

  short middle[] = {30,31,32,33,34,35,29,23,17,11,5,4,3,2,1,0,6,12,18,24,25,26,27,28,22,16,10,9,8,7,13,19,20,21,15,14};
  colorArray(middle, 36, CRGB(random(0, 255), random(0, 255), random(0, 255)));
  
  delay(250);
}

void colorArray(short arr[], int arrSize, CRGB color) {
  for (int i = 0; i < arrSize; i++) {
    leds[arr[i]] = color;
    FastLED.show();
    delay(ANIMATION_DELAY);
  }
}



