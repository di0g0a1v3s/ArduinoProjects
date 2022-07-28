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
#include <Servo.h>
#include <IRremote.h>


#define BTN_PIN 2
#define LED_PIN 3
#define SERVO_PIN 4
#define RECEIVER_PIN 6
#define FORWARD_PIN 7
#define BACKWARD_PIN 8
#define SPEED_PIN 9
#define ECHO_PIN 12
#define TRIG_PIN 13

#define LEFT_ANGLE 90
#define RIGHT_ANGLE 140
#define CENTER_ANGLE 120

#define FORWARD_CODE 2575714394 //0xFF18E7
#define LEFT_CODE 1320813602 //0xFF10EF
#define RIGHT_CODE 1587577092 //0xFF5AA5
#define BACKWARD_CODE 476746140 //0xFF4AB5
#define STOP_CODE 1794249504
#define RPT_CODE 1253111734 //0xFFFFFFFF

Servo servo;
/*-----( Declare objects )-----*/
IRrecv irrecv(RECEIVER_PIN);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
int state = 0;
int steering_wheel = 0;
int engine = 0;
unsigned long previous_code = 0;

void setup() {
//  Serial.begin (9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(FORWARD_PIN, OUTPUT);
  pinMode(BACKWARD_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the RECEIVER_PIN
  servo.attach(SERVO_PIN);
  servo.write(CENTER_ANGLE);
}

void loop() {
  long distance_;
  unsigned long IR_code = 0;
  if(digitalRead(BTN_PIN) == HIGH)
  {
    state = state + 1;
    state = state % 3;
    Serial.print("State:");
    Serial.println(state);
    while(digitalRead(BTN_PIN) == HIGH)
    {
      delay(1);
    }
  }
    
    
  if(state == 1)
  {
      digitalWrite(LED_PIN, HIGH);
        
      if(irrecv.decode(&results))
      {
        Serial.println("oi");
        IR_code = translateIR();
        
   
        if (IR_code != 0)
        {
          if(IR_code == RPT_CODE)
            IR_code = previous_code;
            
          if(IR_code == LEFT_CODE)
          {
            if(steering_wheel > -1)
              steering_wheel = steering_wheel - 1;
          }
          else if(IR_code == RIGHT_CODE)
          {
            if(steering_wheel < 1)
              steering_wheel = steering_wheel + 1;
          }
          else if(IR_code == FORWARD_CODE)
          {
            if(engine < 1)
              engine = engine + 1;
          }
          else if(IR_code == BACKWARD_CODE)
          {
            if(engine > -1)
              engine = engine - 1;
          }
          else if(IR_code == STOP_CODE)
          {
            engine = 0;
            steering_wheel = 0;
          }
      
          previous_code = IR_code;
    
       }
       delay(100);
       irrecv.resume();
        
    }

    if (distance() < 25) 
    {
      if(engine > 0)
        engine = 0;
    }
    
  }
  else if(state == 2)
  {
    steering_wheel = 0;
    distance_ = distance();
    if(distance_ > 30)
    {
      engine = 1;
    }
    else if(distance_ < 25)
    {
      engine = -1;
    }
    else
    {
      engine = 0;
    }
    
  }
  else
  {
      engine = 0;
      steering_wheel = 0;
      digitalWrite(LED_PIN,LOW);
  }
  
  if(steering_wheel == -1)
    servo.write(LEFT_ANGLE);
  else if(steering_wheel == 1)
    servo.write(RIGHT_ANGLE);
  else
    servo.write(CENTER_ANGLE);

  if(engine > 0)
  {
    digitalWrite(BACKWARD_PIN,LOW);
    digitalWrite(FORWARD_PIN,HIGH);
  }
  else if(engine < 0)
  {
    digitalWrite(BACKWARD_PIN,HIGH);
    digitalWrite(FORWARD_PIN,LOW);
  }
  else
  {
    digitalWrite(BACKWARD_PIN,LOW);
    digitalWrite(FORWARD_PIN,LOW);
  }
  analogWrite(SPEED_PIN, 255);//abs(engine)*255/10);
//  Serial.print("Engine:");
//  Serial.println(engine);
//  Serial.print("Wheel:");
//  Serial.println(steering_wheel);

   
  
}

unsigned long translateIR() // takes action based on IR code received
{
  
  if((results.value == FORWARD_CODE || results.value == BACKWARD_CODE || results.value == LEFT_CODE || results.value == RIGHT_CODE || results.value == RPT_CODE || results.value == STOP_CODE))
  {
    Serial.println(results.value);
    return results.value;
  }
  else
  {
    Serial.println(results.value);
    return 0;
  }
  
} 

long distance()
{
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  //Serial.println(distance);
  return distance;
}

