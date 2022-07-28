byte my_numbers[]{
  B11011110,
  B10000010,
  B11101100,
  B11100110,
  B10110010,
  B01110110,
  B01111110,
  B11000010,
  B11111110,
  B11110110,
  B00000000
};
void setup() {
  DDRD = B11111111;
  int i;
  for( i = 0; i < 3; i++)
  {
    tone(8,1000,50);
    delay(150);
  }
}

void loop() {
  DDRD = B11111111;
  int i = 0;
  for( i = 0; i < 10; i++)
  {
    PORTD = my_numbers[i];
    tone(8,1000,50);
    delay(1000);
  }
}
