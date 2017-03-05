short RELAY_PIN   = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10);
  Serial.println("Ready for commands");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
     String cmd = Serial.readString();

     if(cmd == "1" || cmd == "ON" || cmd == "on") {
      Serial.println("On we go");
      digitalWrite(RELAY_PIN, LOW);
     }

     if(cmd == "0" || cmd == "OFF" || cmd == "off") {
      Serial.println("Off we go");
      digitalWrite(RELAY_PIN, HIGH);
     }
  }
}
