
#include <IRremote.h>
#define FORWARD 2575714394 //0xFF18E7
#define LEFT 1320813602 //0xFF10EF
#define RIGHT 1587577092 //0xFF5AA5
#define BACKWARD 476746140 //0xFF4AB5
#define RPT 1253111734 //0xFFFFFFFF

int receiver = 6; // Signal Pin of IR receiver to Arduino Digital Pin 6
int forward = 10;
int backward = 11;

int start_forward = 0;
int start_backward = 0;
int stop_motor = 1;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'


/*-----( Function )-----*/
unsigned long translateIR() // takes action based on IR code received
{
  
  if((results.value == FORWARD || results.value == BACKWARD || results.value == LEFT || results.value == RIGHT || results.value == RPT))
  {
    Serial.println(results.value);
    return results.value;
  }
  else
  {
      return 0;
  }
  delay(500);
} 


void setup() {
  // put your setup code here, to run once:
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
//  analogWrite(backward, 255);
//  analogWrite(forward, 255);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long tecla = 0;

//  if (stop_motor == 1)
//  {
//    start_forward = 0;
//    start_backward = 0;
//    analogWrite(forward, 0);
//    analogWrite(backward, 0);
//  }
//  else if(start_forward == 1)
//  {
//    stop_motor = 0;
//    start_backward = 0;
//    analogWrite(forward, 100);
//    analogWrite(backward, 0);
//  }
//  else if(start_backward == 1)
//  {
//    stop_motor = 0;
//    start_forward = 0;
//    analogWrite(forward, 0);
//    analogWrite(backward, 100);
//  }
//  
//   if(irrecv.decode(&results))
//   {
//      tecla = translateIR();
//      irrecv.resume();
//      
//   }
//  
//  if(tecla == FORWARD)
//  {
//    start_backward = 0;
//    start_forward = 1;
//    stop_motor = 0;
//  }
//  else if(tecla == BACKWARD)
//  {
//    start_backward = 1;
//    start_forward = 0;
//    stop_motor = 0;
//  }
  
  analogWrite(forward, 0);
  for(int i = 0; i <= 255; i++)
  {
    analogWrite(backward, i);
    delay(10);
  }
  delay(2000);
  for(int i = 255; i >= 0; i--)
  {
    analogWrite(backward, i);
    delay(10);
  }
  analogWrite(backward, 0);
  for(int i = 0; i <= 255; i++)
  {
    analogWrite(forward, i);
    delay(10);
  }
  delay(2000);
  for(int i = 255; i >= 0; i--)
  {
    analogWrite(forward, i);
    delay(10);
  }

  

}
