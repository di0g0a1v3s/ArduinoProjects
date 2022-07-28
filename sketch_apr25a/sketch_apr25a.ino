
int led = 12;
int on = 0;
int btn = 13;

void setup() 
{
  pinMode(led, OUTPUT);
}

void loop() 
{
    int btn1R = digitalRead(btn);
    if(btn1R == 1)
    {
      on = !on;
    }
    if(on)
      digitalWrite(led, LOW);
    //delay(1000);
    //digitalWrite(led, HIGH);
    //delay(1000);
}
