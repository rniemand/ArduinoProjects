const int stb = 8;
const int clk = 9;
const int dio = 10;

void setup() {
  pinMode(stb, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);

  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, B10001000); 
  digitalWrite(stb, HIGH);

  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, B00000001); 
  digitalWrite(stb, HIGH);
}

void loop() {
}
