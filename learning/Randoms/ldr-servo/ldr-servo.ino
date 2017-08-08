#include <Servo.h>

#define SERVO_01      8
#define SERVO_02      9
#define LDR1          7

Servo servo1;
Servo servo2;

int pos1 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready...");

  pinMode(LDR1, INPUT);
  
  servo1.attach(SERVO_01);
  servo2.attach(SERVO_02);
}

void loop() {
   digitalWrite(LDR1, HIGH);
   int ldrValue = analogRead(A0);

   Serial.println(ldrValue);

   delay(100);
}

void moveServos() {
  for (pos1 = 0; pos1 <= 180; pos1 += 1) {
    servo1.write(pos1);
    servo2.write(pos1);
    delay(15);
  }

  delay(5000);
  
  for (pos1 = 180; pos1 >= 0; pos1 -= 1) {
    servo1.write(pos1);
    servo2.write(pos1);
    delay(15);
  }

  delay(5000);
}

