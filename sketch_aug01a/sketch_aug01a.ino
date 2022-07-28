#include <IRremote.h>



#define trigPin 13
#define echoPin 12
#define btn 2
#define led 3
#define buzzer 1
#define receiver 6

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
//  irrecv.enableIRIn(); // Start the receiver
}
void loop() {
  bool a = digitalRead(btn);
  Serial.println(a);
  digitalWrite(led, a);
  digitalWrite(buzzer, a);
  delay(500);
}

