#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display(7, 6, 5, 4, 8); // SCLK, DIN, DC, CS, RST
float value = 0;
float voltage = 0;
int battVolts; 

float maxVoltage = 18.0;
float maxReadVoltage = 3.6;
float readingSteps = maxReadVoltage / 1023;


void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);

  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  showVoltage();

  battVolts=getBandgap(); 

  value = analogRead(A1);

  float currentReadingVoltage = value * readingSteps;
  float aa = (currentReadingVoltage / maxReadVoltage) * 100;
  float bob = (aa * maxVoltage) / 100;
  
  voltage = bob;
  
  delay(500);
}

void showVoltage() {
  display.clearDisplay();
  
  display.println();
  
  display.println("Testing");
  display.print(" > ");
  display.println(value);
  display.print(" > ");
  display.println(voltage);
  display.print(" > ");
  display.println(battVolts);

  display.display();
}

int getBandgap(void) // Returns actual value of Vcc (x 100)
    {
        
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     // For mega boards
     const long InternalReferenceVoltage = 1115L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc reference
        // MUX4 MUX3 MUX2 MUX1 MUX0  --> 11110 1.1V (VBG)         -Selects channel 30, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR)| (0<<MUX5) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
  
#else
     // For 168/328 boards
     const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
        // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
       
#endif
     delay(50);  // Let mux settle a little to get a more stable A/D conversion
        // Start a conversion  
     ADCSRA |= _BV( ADSC );
        // Wait for it to complete
     while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
        // Scale the value
     int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value 
     return results;
 
    }

