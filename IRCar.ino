#include <IRremote.h>


const unsigned int IR_RECEIVER_PIN = 7;

IRrecv ir_receiver(IR_RECEIVER_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  ir_receiver.enableIRIn(); // Start the receiver
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);
}

void loop() {
  
  long int decCode=0;
  if(ir_receiver.decode(&results)) {
    
    decCode = results.value;
    Serial.println(decCode);
    
    switch(decCode) {
      case -522189063:
      digitalWrite(12, HIGH);  //Establishes backward direction of Channel A
      digitalWrite(9, LOW);   //Disengage the Brake for Channel A
      analogWrite(3, 255);    //Spins the motor on Channel A at half speed
  
      //Motor B forward @ full speed
      digitalWrite(13, HIGH); //Establishes forward direction of Channel B
      digitalWrite(8, LOW);   //Disengage the Brake for Channel B
      analogWrite(11, 255);
      break;
      case -522148263:
      digitalWrite(12, HIGH);  //Establishes backward direction of Channel A
      digitalWrite(9, LOW);   //Disengage the Brake for Channel A
      analogWrite(3, 255);
      
      digitalWrite(8, HIGH);
      break;
      case -522172743:
      digitalWrite(13, HIGH); //Establishes forward direction of Channel B
      digitalWrite(8, LOW);   //Disengage the Brake for Channel B
      analogWrite(11, 255);
      
      digitalWrite(9, HIGH);
      break;
      case -522156423:
      digitalWrite(12, LOW);  //Establishes backward direction of Channel A
      digitalWrite(9, LOW);   //Disengage the Brake for Channel A
      analogWrite(3, 255);    //Spins the motor on Channel A at half speed
  
      //Motor B forward @ full speed
      digitalWrite(13, LOW); //Establishes forward direction of Channel B
      digitalWrite(8, LOW);   //Disengage the Brake for Channel B
      analogWrite(11, 255);
      break;
      case -1963903704:
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      break;
      default:
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
    }
    ir_receiver.resume();
  }
  
}
