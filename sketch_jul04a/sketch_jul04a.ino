void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  analogWrite(3, rand()%256);
  analogWrite(5, rand()%256);
  analogWrite(6, rand()%256);
  delay(500);

}
