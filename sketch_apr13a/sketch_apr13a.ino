int btn1 = 13;
int btn2 = 12;
int led = 2;

void setup() 
{
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(led, OUTPUT);
}

void loop() 
{
  int btn1R = digitalRead(btn1);
  int btn2R = digitalRead(btn2);
  
  if(btn1R == 0)
  {
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
  }
  else if(btn2R == 0)
  {
    digitalWrite(led, LOW);
    delay(10);
    digitalWrite(led, HIGH);
    delay(10);
  }
}
