const int PIN_RED     = 3;
const int PIN_GREEN   = 6;
const int PIN_BLUE    = 9;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  randomSeed(analogRead(0));
}

void loop() {
  int pin = random(3);

  if(pin == 0) {
    analogWrite(PIN_BLUE, random(96));
  }
  else if(pin == 1) {
    analogWrite(PIN_GREEN, random(96));
  }
  else {
    analogWrite(PIN_RED, random(96));
  }
  
  delay(random(3000));
}
