/* ***********************************************************************
* Simple Relay Test (1.0.0)
* ************************************************************************
* 
* My first attempt at working with a RELAY module.
* 
* Buy Relay   : https://goo.gl/Bjo84P
* Source Code : https://goo.gl/PCQQNn
* 
* ***********************************************************************
* Pin Outs
* ***********************************************************************
* 
* (Relay Module)
* 
*   1 - VCC        -------------------------- 5v
*   2 - GND        -------------------------- GND
*   3 - IN         -------------------------- D7
*   
* (Button)
*   1 - GND        -------------------------- GND
*   2 - CON        -------------------------- D2
*********************************************************************** */

// NOTE: If using serial some digital pins don't seem to work with the relay?!!?
const int PIN_RELAY            = 7;
const int PIN_BUTTON           = 2;
const bool DEBUG               = true;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay    = 250;
int lastButtonState            = 0;

bool relayOn = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

void loop() {
  readButton();
  switchRelay();
  delay(100);
}

void switchRelay() {
  if( relayOn ) {
    digitalWrite(PIN_RELAY, LOW);
    return;
  }

  digitalWrite(PIN_RELAY, HIGH);
}

void debug(String message) {
  if( !DEBUG ) return;
  Serial.println(message);
}

void readButton() {
  // Get the current state of the button
  int currentState = digitalRead(PIN_BUTTON) == 0;

  // Check to see if enough time has passed since we last acted on a button push
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Check to see if the button is being pressed
    if(currentState == 1 ) {
      lastDebounceTime = millis();
      relayOn = !relayOn;

      debug(relayOn ? "Turning relay ON" : "Turning relay OFF");
    }
  }
}

