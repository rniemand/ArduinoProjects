String readString;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    readSerial();

    if (readString.length() >0) {
      if(readString.startsWith("ON")) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED is now ON");
      }
      else if(readString.startsWith("OFF")) {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED is now OFF");
      }
      else {
        Serial.print("Unknown Command: ");
        Serial.println(readString);
      }
      
      readString="";
    } 
}

void readSerial() {
  while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        readString += c; //makes the string readString
      } 
    }
}

