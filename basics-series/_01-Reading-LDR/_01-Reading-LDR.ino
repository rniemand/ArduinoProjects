#define LDRPIN    A0

void setup() {
  Serial.begin(9600);
  pinMode(LDRPIN, INPUT);
}

void loop() {
  Serial.println(analogRead(LDRPIN));
  delay(250);
}
