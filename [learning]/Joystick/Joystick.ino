#include "joystick.cpp"

const int JOY_BTN     = 2;
const int JOY_X       = A0;
const int JOY_Y       = A1;
const int X_THRESHOLD = 100;
const int Y_THRESHOLD = 100;

Joystick* j = new Joystick(JOY_X, JOY_Y, JOY_BTN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(JOY_BTN, INPUT_PULLUP);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  j->setThresholds(100, 100, 100, 100);
}

void loop() {
  // put your main code here, to run repeatedly:
  int xPos = analogRead(JOY_X);
  int yPos = analogRead(JOY_Y);
  String xValue = "";
  String yValue = "";

  j->update();

  Serial.print("x:");
  Serial.print(j->x);
  Serial.print(", y:");
  Serial.print(j->y);
  Serial.print(" ");
  Serial.print(j->xDirection);
  Serial.print(" ");
  Serial.print(j->yDirection);
  Serial.print(" ");
  Serial.print(j->btn);
  Serial.println();

  delay(100);
}
