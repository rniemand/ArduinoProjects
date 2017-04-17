 #include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 36
#define DATA_PIN 3
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(500000);
}

void loop() {
  for(int i = 0; i < 36; i++) {
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    
    leds[i] = CRGB(r, g, b);
    
    delay(100);
    FastLED.show();
  }

  delay(200);

  for(int i = 0; i < 36; i++) {
    leds[i] = CRGB(0, 0, 0);
    delay(75);
    FastLED.show();
  }

  delay(200);
}




