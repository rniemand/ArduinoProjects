#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display(13, 11, 5, 7, 6); // SCLK, DIN, DC, CS, RST
float value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  showVoltage();

  value = analogRead(A0);
  
  delay(500);
}

void showVoltage() {
  display.clearDisplay();
  
  display.println();
  
  display.println("Testing");
  display.print(" > ");
  display.print(value);

  display.display();
}

