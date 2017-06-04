#include "FastLED.h"

#define NUM_LEDS            2
#define DATA_PIN            2
int ANIMATION_DELAY         = 250;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  leds[0] = CRGB(255,0,0);
  leds[1] = CRGB(255,0,0);
  FastLED.show();
  delay(ANIMATION_DELAY);

  leds[0] = CRGB(0,2525,0);
  leds[1] = CRGB(0,2525,0);
  FastLED.show();
  delay(ANIMATION_DELAY);

  leds[0] = CRGB(0,0,255);
  leds[1] = CRGB(0,0,255);
  FastLED.show();
  delay(ANIMATION_DELAY);

  leds[0] = CRGB(255,255,255);
  leds[1] = CRGB(255,255,255);
  FastLED.show();
  delay(ANIMATION_DELAY);
}

CRGB randomColor() {
  return CRGB( random(20, 255), random(20, 255), random(20, 255) );
}


