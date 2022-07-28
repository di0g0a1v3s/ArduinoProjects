#include <IRremote.h>


#define WHEEL_RIGHT_BACK 11 //blue
#define WHEEL_RIGHT_FRONT 10 //black
#define WHEEL_LEFT_BACK 9 //orange
#define WHEEL_LEFT_FRONT 6 //green
#define VENTILATOR 5 //brown
#define LED_RED 7 //brown
#define CHARGING A0 //green
#define BUMPER A1 //blue
#define BUTTON A2 //orange
#define BUZZER 3 //black
#define CONTACT_SENSORS A3 //yellow ?
#define UNKNOWN_ A4 //grey ?
#define LED_BLUE 2 //purple ?
#define ECHO 12 //yellow
#define TRIG 8 //orange
#define IR 4 //purple

#define CODE_FRONT 0xA2E205A6
#define CODE_RIGHT 0x8576B25B
#define CODE_START_STOP 0x52A3E788
#define CODE_LEFT 0x15E5DD8E
#define CODE_BACK 0xCDDD9034
#define CODE_GRID_CLEAN 0xF002CFC4
#define CODE_SPOT_CLEAN 0x52E6CBFC
#define CODE_CORNER_CLEAN 0xD59EDB7F
#define CODE_HOME 0xD2A29631
#define CODE_VENTILATOR 0x440DA57A
#define CODE_SEARCH 0xBDBF441B

#define PERIOD 100
#define BUZZER_TIME 100
#define BUZZER_FREQ 250

#define MODE_CLASSIC 1
#define MODE_MANUAL 2
#define MODE_SPOT 3
#define MODE_CORNERS 4

#define STATE_FRONT 1
#define STATE_REVERSE 2
#define STATE_LEFT 3
#define STATE_RIGHT 4
#define STATE_STOP 5

#define VENTILATOR_ON 0
#define VENTILATOR_OFF 1

int ventilator_state = VENTILATOR_ON;

int mode = MODE_CLASSIC;
int state = STATE_FRONT;
unsigned long time_stop_turning_classic;
int num_periods_spot = 0;

IRrecv irrecv(IR);
decode_results results;

void setup() {
  // put your setup code here, to run once:

  pinMode(WHEEL_RIGHT_BACK, OUTPUT);
  pinMode(WHEEL_RIGHT_FRONT, OUTPUT);
  pinMode(WHEEL_LEFT_BACK, OUTPUT);
  pinMode(WHEEL_LEFT_FRONT, OUTPUT);
  pinMode(VENTILATOR, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CHARGING, INPUT);
  pinMode(BUMPER, INPUT);
  
  pinMode(BUTTON, INPUT);
  pinMode(CONTACT_SENSORS, INPUT);
  pinMode(UNKNOWN_, INPUT);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  irrecv.enableIRIn();
  irrecv.blink13(true);
  Serial.begin(9600);
  randomSeed(analogRead(A5));
  
  
  
}

void loop() {


  interpretCommandIR();
  Routine();
  delay(PERIOD);
  
}

void turnRight()
{
  digitalWrite(WHEEL_LEFT_BACK, LOW);
  digitalWrite(WHEEL_RIGHT_FRONT, LOW);
  digitalWrite(WHEEL_LEFT_FRONT, HIGH);
  digitalWrite(WHEEL_RIGHT_BACK, HIGH);

}

void turnLeft()
{
  digitalWrite(WHEEL_LEFT_FRONT, LOW);
  digitalWrite(WHEEL_RIGHT_BACK, LOW);
  digitalWrite(WHEEL_LEFT_BACK, HIGH);
  digitalWrite(WHEEL_RIGHT_FRONT, HIGH);

}

void forward()
{
  digitalWrite(WHEEL_LEFT_BACK, LOW);
  digitalWrite(WHEEL_RIGHT_BACK, LOW);
  digitalWrite(WHEEL_LEFT_FRONT, HIGH);
  digitalWrite(WHEEL_RIGHT_FRONT, HIGH);
}

void reverse()
{
  digitalWrite(WHEEL_LEFT_FRONT, LOW);
  digitalWrite(WHEEL_RIGHT_FRONT, LOW);
  digitalWrite(WHEEL_LEFT_BACK, HIGH);
  digitalWrite(WHEEL_RIGHT_BACK, HIGH);
}

void forwardSlow()
{
  digitalWrite(WHEEL_LEFT_BACK, LOW);
  digitalWrite(WHEEL_RIGHT_BACK, LOW);
  analogWrite(WHEEL_LEFT_FRONT, 100);
  analogWrite(WHEEL_RIGHT_FRONT, 100);
}

void ventilator()
{
  if(ventilator_state == VENTILATOR_ON)
  {
    digitalWrite(VENTILATOR, HIGH);
  }
  else if(ventilator_state == VENTILATOR_OFF)
  {
    digitalWrite(VENTILATOR, LOW);
  }
}

void buzz()
{
  tone(BUZZER, BUZZER_FREQ , BUZZER_TIME);
}

bool bump()
{
  int value = analogRead(BUMPER);
  if(value > 200)
    return true;
  else
    return false;      

}

void Stop()
{
  digitalWrite(WHEEL_LEFT_FRONT, LOW);
  digitalWrite(WHEEL_RIGHT_FRONT, LOW);
  digitalWrite(WHEEL_LEFT_BACK, LOW);
  digitalWrite(WHEEL_RIGHT_BACK, LOW);
}

void Routine()
{
  switch(mode)
  {
    case MODE_CLASSIC:
      ventilator_state = VENTILATOR_ON;
      ventilator();
      if(state != STATE_REVERSE)
      {
        if(bump())
        {
          reverse();
          while(bump())
            delay(10);

          int turn = random (0,2);
          if(turn == 0)
          {
            turnLeft();
            state = STATE_LEFT;
          }
          else
          {
            turnRight();
            state = STATE_RIGHT;
          }
          time_stop_turning_classic = millis() + random(1000,3000);
        }
        else if(state == STATE_FRONT)
        {
          forward();
        }
        else if(state == STATE_LEFT || state == STATE_RIGHT)
        {
          if(millis() > time_stop_turning_classic)
          {
            forward();
            state = STATE_FRONT;
          }
        }
      }
      break; 

     case MODE_MANUAL:
      if(bump())
      {
        reverse();
        while(bump())
          delay(10);
        Stop();
        state = STATE_STOP;
      }
      break;

     case MODE_SPOT:
      if(bump())
      {
        mode = MODE_CLASSIC;
        break;
      }
      ventilator_state = VENTILATOR_ON;
      ventilator();
      if(num_periods_spot >=5)
      {
        digitalWrite(WHEEL_LEFT_FRONT, HIGH);
        num_periods_spot = 0;
      }
      else
      {
        digitalWrite(WHEEL_LEFT_FRONT, LOW);
      }
      
      digitalWrite(WHEEL_LEFT_BACK, LOW);
      digitalWrite(WHEEL_RIGHT_BACK, LOW);
      digitalWrite(WHEEL_RIGHT_FRONT, HIGH);
      num_periods_spot++;
      
      break;
      
      
        
  }
}

bool nearObstacle()
{
  if(calculateDistance() < 30)
    return true;
   return false;
}

int calculateDistance()
{
  long duration;
  int distance;
  int timeout = 10000;
  // Clears the trigPin
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO, HIGH, timeout);
  if(duration == 0) duration = timeout;
  // Calculating the distance
  distance = duration*0.034/2;
  return distance;

}

void interpretCommandIR()
{
  if (irrecv.decode(&results)){
    switch(results.value){
      case CODE_FRONT:
        if(mode == MODE_MANUAL)
        {
          if(state != STATE_REVERSE)
          {
            forward();
            state = STATE_FRONT;
          }
          else
          {
            Stop();
            state = STATE_STOP;
          }
        }
        break;
       case CODE_LEFT:
        if(mode == MODE_MANUAL)
        {
          if(state != STATE_RIGHT)
          {
            turnLeft();
            state = STATE_LEFT;
          }
          else
          {
            Stop();
            state = STATE_STOP;
          }
        }
        break;
       case CODE_RIGHT:
        if(mode == MODE_MANUAL)
        {
          if(state != STATE_LEFT)
          {
            turnRight();
            state = STATE_RIGHT;
          }
          else
          {
            Stop();
            state = STATE_STOP;
          }
        }
        break;
       case CODE_BACK:
        if(mode == MODE_MANUAL)
        {
          if(state != STATE_FRONT)
          {
            reverse();
            state = STATE_REVERSE;
          }
          else
          {
            Stop();
            state = STATE_STOP;
          }
        }
        break;

       case CODE_START_STOP:
        if(mode != MODE_MANUAL)
        {
          mode = MODE_MANUAL;
          Stop();
          state = STATE_STOP;
          
          ventilator_state = VENTILATOR_OFF;
          ventilator();
        }
        else if(state != STATE_STOP)
        {
          Stop();
          state = STATE_STOP;
          
          ventilator_state = VENTILATOR_OFF;
          ventilator();
        }
        else
        {
          mode = MODE_CLASSIC;
          state = STATE_FRONT;
        }
        break;

        
       case CODE_VENTILATOR:
        if(ventilator_state == VENTILATOR_OFF)
          ventilator_state = VENTILATOR_ON;
        else if(ventilator_state == VENTILATOR_ON)
          ventilator_state = VENTILATOR_OFF;
          
        ventilator();
        break;
       
       case CODE_SEARCH:
        tone(BUZZER, 3000 , 100);
        break;

       case CODE_SPOT_CLEAN:
        mode = MODE_SPOT;
        break;
        
    }
    irrecv.resume();
  }
  
}






void decodeIR()
{
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        switch (results.decode_type){
            case NEC: Serial.println("NEC"); break ;
            case SONY: Serial.println("SONY"); break ;
            case RC5: Serial.println("RC5"); break ;
            case RC6: Serial.println("RC6"); break ;
            case DISH: Serial.println("DISH"); break ;
            case SHARP: Serial.println("SHARP"); break ;
            case JVC: Serial.println("JVC"); break ;
            case SANYO: Serial.println("SANYO"); break ;
            case MITSUBISHI: Serial.println("MITSUBISHI"); break ;
            case SAMSUNG: Serial.println("SAMSUNG"); break ;
            case LG: Serial.println("LG"); break ;
            case WHYNTER: Serial.println("WHYNTER"); break ;
            case AIWA_RC_T501: Serial.println("AIWA_RC_T501"); break ;
            case PANASONIC: Serial.println("PANASONIC"); break ;
            case DENON: Serial.println("DENON"); break ;
          default:
            case UNKNOWN: Serial.println("UNKNOWN"); break ;
        }
        Serial.println();
        
        irrecv.resume();
        
  }
}



