/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 13
#define echoPin 12
#define forward 11
#define backward 10
#define btn 2
#define led 4

int state = 0;

void setup() {
//  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  long duration, distance;
  if(digitalRead(btn) == HIGH)
  {
    state = !state;
    while(digitalRead(btn) == HIGH)
    {
      delay(1);
    }
  }
    
    
  if(state != 0)
  {
      digitalWrite(led, HIGH);
      digitalWrite(trigPin, LOW);  // Added this line
      delayMicroseconds(2); // Added this line
      digitalWrite(trigPin, HIGH);
       //  delayMicroseconds(1000); - Removed this line
      delayMicroseconds(10); // Added this line
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      if (distance < 25) {  // This is where the LED On/Off happens
        digitalWrite(forward,LOW); // When the Red condition is met, the Green LED should turn off
//      digitalWrite(backward,HIGH);
      }
      else if (distance > 22) {
        digitalWrite(forward,HIGH);
//        digitalWrite(backward,LOW);
      }
      else
      {
        digitalWrite(forward,LOW);
        digitalWrite(backward,LOW);
      }
//      if (distance >= 200 || distance <= 0){
//        Serial.println("Out of range");
//      }
//      else {
//        Serial.print(distance);
//        Serial.println(" cm");
//      }
      
  }
  else
  {
      digitalWrite(trigPin, LOW);
      digitalWrite(backward,LOW);
      digitalWrite(forward,LOW);
      digitalWrite(led,LOW);
  }
  delay(50);
  
}
