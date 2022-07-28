 int LMotor=2;
 int RMotor=3;
int val = 0;
int off = 0;
int reverse;
int forward;
int circle;

 //int move;
int turn;
int aheadY;
int turnX;
 


void setup() {
 
 pinMode (5,OUTPUT); 
 pinMode (6,OUTPUT);
 pinMode (11,OUTPUT); 
 pinMode (12,OUTPUT);
 
 pinMode(LMotor,OUTPUT);
 pinMode(RMotor,OUTPUT);
 pinMode(13,HIGH);
 analogWrite(LMotor, 255);
 analogWrite(RMotor,255);
 Serial.begin(9600);
}

void loop() {
  
 
 
 
 
}

