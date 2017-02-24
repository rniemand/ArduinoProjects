/* ***********************************************************************
* Rotary Encoder Example
* ************************************************************************
* 
* My first attempt at working with a Rotary Encoder module.
* 
* Buy Encoder : -
* Source Code : -
* 
* ***********************************************************************
* Pin Outs
* ***********************************************************************
* 
* (Rotary Encoder)
* 
*   1 - GND        -------------------------- GND
*   2 - +          -------------------------- 5v
*   3 - SW         -------------------------- D2
*   4 - DATA       -------------------------- D3
*   5 - CLK        -------------------------- D4
*********************************************************************** */

const int PIN_BUTON             = 2;
const int PIN_DATA              = 3;
const int PIN_CLK               = 4;

int val;
int encoder0Pos                 = 0;
int pinDataLast                 = LOW;
int dataValue                   = LOW;
int counter                     = 0;
int buttonValue                 = 0;
unsigned long lastDebounceTime  = 0;
unsigned long debounceDelay     = 250;

void setup() { 
  pinMode(PIN_DATA, INPUT);
  pinMode(PIN_CLK, INPUT);
  pinMode(PIN_BUTON, INPUT_PULLUP);
  
  Serial.begin (9600);
} 

void loop() { 
  dataValue = digitalRead(PIN_DATA);

  // Read the encoder value
  if ((pinDataLast == LOW) && (dataValue == HIGH)) {
    if (digitalRead(PIN_CLK) == LOW) {
      Serial.print (" - (");
      encoder0Pos--;
    } else {
      Serial.print (" + (");
      encoder0Pos++;
    }
    
    Serial.print (encoder0Pos);
    Serial.print (")");
    counter ++;
  }

  // Read the button value
  buttonValue = digitalRead(PIN_BUTON) == 0;

  if( buttonValue == 1 ) {
      if ((millis() - lastDebounceTime) > debounceDelay) {
        // Check to see if the button is being pressed
        if(buttonValue == 1 ) {
          lastDebounceTime = millis();
          Serial.print(" Click");
          counter ++;
        }
      }
  }

  if( counter == 5 ) { Serial.println(); counter = 0; }
  pinDataLast = dataValue;
} 




