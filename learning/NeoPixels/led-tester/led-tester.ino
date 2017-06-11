#include "FastLED.h"

#define NUM_LEDS            1
#define DATA_PIN            5
int ANIMATION_DELAY         = 250;
bool ledOn                  = false;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  leds[0] = CRGB(255,0,0);
  leds[1] = CRGB(255,0,0);
  FastLED.show();
  delay(ANIMATION_DELAY);

  digitalWrite(LED_BUILTIN, LOW);
  leds[0] = CRGB(0,2525,0);
  leds[1] = CRGB(0,2525,0);
  FastLED.show();
  delay(ANIMATION_DELAY);

  digitalWrite(LED_BUILTIN, HIGH);
  leds[0] = CRGB(0,0,255);
  leds[1] = CRGB(0,0,255);
  FastLED.show();
  delay(ANIMATION_DELAY);

  digitalWrite(LED_BUILTIN, LOW);
  leds[0] = CRGB(255,255,255);
  leds[1] = CRGB(255,255,255);
  FastLED.show();
  delay(ANIMATION_DELAY);
}

CRGB randomColor() {
  return CRGB( random(20, 255), random(20, 255), random(20, 255) );
}


