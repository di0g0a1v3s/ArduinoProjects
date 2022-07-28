
#define forward 11
#define left 10
#define btn 2
#define led 4

int state = 0;

void setup() {
  Serial.begin(9600);
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(left, OUTPUT);
}

void loop() {
  
    
  if(digitalRead(btn) == HIGH)
  {
    btn_press();
    while(digitalRead(btn) == HIGH)
    {
      delay(1);
    }
  }
  if(state == 1)
    forward_();
  else if(state == 2)
    turn_left();
   else
     stop();

  delay(10);

  
  
}

void forward_()
{
  analogWrite(forward, 255);
  analogWrite(left, 0);
}

void turn_left()
{
  analogWrite(left, 255);
  analogWrite(forward, 255);
}
void stop()
{
  analogWrite(left, 0);
  analogWrite(forward, 0);
}



void btn_press()
{
  
  state = state + 1;
  state = state%3;
  if(state == 1)
    analogWrite(led, 255);
  else if(state == 2)
    analogWrite(led,150);
   else
     analogWrite(led,0);
   Serial.println(state);
}





