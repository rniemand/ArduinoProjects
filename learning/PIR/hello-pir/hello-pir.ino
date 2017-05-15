int PIR_PIN     = 8;
int lastState   = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  int value = digitalRead(PIR_PIN);
  if(lastState != value) {
    Serial.println(value);
  }
  if(value == HIGH && lastState != value) {
    Serial.println("high");
  }

  lastState = value;
  delay(100);
}
