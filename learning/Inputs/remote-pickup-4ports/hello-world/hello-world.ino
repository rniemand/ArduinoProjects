#define   BUTTON_A      8
#define   BUTTON_B      9
#define   BUTTON_C      10
#define   BUTTON_D      11
#define   BUTTON_VT     12

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  pinMode(BUTTON_D, INPUT);
  pinMode(BUTTON_VT, INPUT);

  Serial.println("Ready");
  delay(1000);
}

void loop() { 
  if( digitalRead(BUTTON_A) == HIGH ) {
    Serial.println("Button A - pressed");
  }
  
  if( digitalRead(BUTTON_B) == HIGH ) {
    Serial.println("Button B - pressed");
  }
  
  if( digitalRead(BUTTON_C) == HIGH ) {
    Serial.println("Button C - pressed");
  }
  
  if( digitalRead(BUTTON_D) == HIGH ) {
    Serial.println("Button D - pressed");
  }

  delay(25);
}
