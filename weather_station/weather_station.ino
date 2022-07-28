
#include "DHT.h"
#include <MaxMatrix.h>
#include <avr/pgmspace.h>

#define ON_OFF_COMMAND "onoff"
#define INCREASE_INTENSITY_COMMAND "b++"
#define DECREASE_INTENSITY_COMMAND "b--"
#define DECREASE_SPEED_COMMAND "s--"
#define INCREASE_SPEED_COMMAND "s++"
#define SET_AUTO_BRIGHTNESS_COMMAND "autob"
#define SHOW_COMMAND "show"
#define LOOP_TEXT_COMMAND "loop"
#define SET_TEXT_COMMAND "set"
#define PLAY_NOTE_COMMAND "play"

#define BUZZER_TIME 200
#define BUZZER_FREQ 500

#define MAX_LED_BRIGHTNESS 120
#define MAX_MATRIX_INTENSITY 15


#define BuzzerPin 5
#define MatrixDinPin 6
#define MatrixCsPin 7
#define MatrixClkPin 8
#define RedLedPin 9
#define GreenLedPin 10
#define BlueLedPin 11
#define DhtPin 12
#define LdrPin A0
#define ButtonPin A1

#define DHTTYPE DHT11
#define maxInUse 1

PROGMEM const unsigned char CH[] = {
  3, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // !
  3, 8, B00000011, B00000000, B00000011, B00000000, B00000000, // "
  5, 8, B00010100, B00111110, B00010100, B00111110, B00010100, // #
  4, 8, B00100100, B01101010, B00101011, B00010010, B00000000, // $
  5, 8, B01100011, B00010011, B00001000, B01100100, B01100011, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  1, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  3, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  3, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  2, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  4, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  2, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  4, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  3, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000, // 9
  2, 8, B01010000, B00000000, B00000000, B00000000, B00000000, // :
  2, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  3, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  3, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  3, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  4, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A
  4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B
  4, 8, B00111110, B01000001, B01000001, B00100010, B00000000, // C
  4, 8, B01111111, B01000001, B01000001, B00111110, B00000000, // D
  4, 8, B01111111, B01001001, B01001001, B01000001, B00000000, // E
  4, 8, B01111111, B00001001, B00001001, B00000001, B00000000, // F
  4, 8, B00111110, B01000001, B01001001, B01111010, B00000000, // G
  4, 8, B01111111, B00001000, B00001000, B01111111, B00000000, // H
  3, 8, B01000001, B01111111, B01000001, B00000000, B00000000, // I
  4, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  4, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  4, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // O
  4, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  4, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  4, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  4, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  4, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  4, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  2, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  4, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  2, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  3, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  4, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  3, 8, B00000000, B00000011, B00000011, B00000000, B00000000, // ` (replaced by º)
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};


MaxMatrix m(MatrixDinPin, MatrixCsPin, MatrixClkPin, maxInUse);
byte buffer[10];
String tempHumText = String("T:  . `C H:  % ");
String extraString = String("");
String displayString;
DHT dht(DhtPin, DHTTYPE);
bool on = true;
int intensity = 2;
float temperature, humidity;
int led_brightness = 2;
bool auto_brightness = true;
bool loopText = false;
bool displayText = false;
int text_speed = 100;

void setup(){
  dht.begin();
  Serial.begin(9600); // Define a porta serie para comunicação
  m.init(); // module initialize
  m.setIntensity(intensity); // dot matix intensity 0-15
  pinMode(BlueLedPin,OUTPUT);
  pinMode(GreenLedPin,OUTPUT);
  pinMode(RedLedPin,OUTPUT);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}
void loop() {
  
  if(on)
  {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    setRGBLed();
    
    int button = analogRead(ButtonPin);
    int ldr = analogRead(LdrPin);
    if(auto_brightness)
      setAutoBrightness(ldr);
    if(button > 1000)
    {
      tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
      displayText = true;
    }
    if(loopText || displayText)
    {
      displayTempAndHum();
      displayText = false;
    }
      
  }
  else
  {
    int button = analogRead(ButtonPin);
    if(button > 1000)
      on = true;
  }
  
  String command = readCommand();

  interpretCommand(command);

}


void interpretCommand(String command)
{
  if(command.length()>0)
  {
    if(command.equals(ON_OFF_COMMAND))
    {
      tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
      on = !on;
      if(!on)
      {
        analogWrite(RedLedPin, 0);
        analogWrite(GreenLedPin, 0);
        analogWrite(BlueLedPin, 0);
      }
    }
    else if(on)
    {
      if(command.equals(INCREASE_INTENSITY_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        auto_brightness = false;
        
        led_brightness += (int)MAX_LED_BRIGHTNESS/15;
        if(led_brightness > MAX_LED_BRIGHTNESS) led_brightness = MAX_LED_BRIGHTNESS;
        intensity = (int)led_brightness * (MAX_MATRIX_INTENSITY/MAX_LED_BRIGHTNESS);
        m.setIntensity(intensity);
      }
      else if(command.equals(DECREASE_INTENSITY_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        auto_brightness = false;
        led_brightness -= (int)MAX_LED_BRIGHTNESS/15;;
        if(led_brightness < 0) led_brightness = 0;
        intensity = (int)led_brightness * (MAX_MATRIX_INTENSITY/MAX_LED_BRIGHTNESS);
        m.setIntensity(intensity);
      }
      else if(command.equals(SET_AUTO_BRIGHTNESS_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        auto_brightness = true;
      }
      else if(command.equals(DECREASE_SPEED_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        text_speed += 25;
        if(text_speed > 200) text_speed = 200;
      }
      else if(command.equals(INCREASE_SPEED_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        text_speed -= 25;
        if(text_speed < 25) text_speed = 25;
      }
      else if(command.equals(SHOW_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        sendHumTempSerial();
        displayText = true;
      } 
      else if(command.equals(LOOP_TEXT_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        loopText = !loopText;   
      }
      else if(command.startsWith(SET_TEXT_COMMAND))
      {
        tone(BuzzerPin, BUZZER_FREQ , BUZZER_TIME);
        int index1 = command.indexOf('"');
        int index2 = command.lastIndexOf('"');
        if(index1 != -1 && index2 != -1 && index2 > index1)
        {
          extraString = command.substring(index1+1, index2);
        }
        else
        {
          extraString = String("");
        }
      }
      else if(command.startsWith(PLAY_NOTE_COMMAND))
      {
        int index1 = command.indexOf(' ');
        int index2 = command.lastIndexOf(' ');
        if(index1 != -1 && index2 != -1 && index2 > index1)
        {
          int freq = command.substring(index1, index2).toInt();
          int delay_ = command.substring(index2).toInt();
          tone(BuzzerPin, freq , delay_);
        }
        
      }
    }
  }
}

void setAutoBrightness(int ldr)
{
  led_brightness = map(ldr, 0, 1024, 0, MAX_LED_BRIGHTNESS);
  intensity = map(ldr, 0, 1024, 0, MAX_MATRIX_INTENSITY);
}

void displayTempAndHum()
{
  char tempF[6]; 
  char humF[6];
  dtostrf(temperature, 5, 1, tempF);
  dtostrf(humidity, 2, 0, humF);
  tempHumText.setCharAt(2, tempF[1]);
  tempHumText.setCharAt(3, tempF[2]);
  tempHumText.setCharAt(5, tempF[4]);
  tempHumText.setCharAt(11, humF[0]);
  tempHumText.setCharAt(12, humF[1]);
  displayString = String(tempHumText + extraString + "        ");
  printStringWithShift(displayString, text_speed); // (text, scrolling speed)
}
// Display=the extracted characters with scrolling
void printCharWithShift(char c, int shift_speed) {
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7 * c, 7);
  m.writeSprite(32, 0, buffer);
  m.setColumn(32 + buffer[0], 0);

  for (int i = 0; i < buffer[0] + 1; i++)
  {
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}
// Extract the characters from the text string
void printStringWithShift(String s, int shift_speed) {
  m.setIntensity(intensity);
  int i = 0;
  while (i < s.length()) {
    printCharWithShift(s.charAt(i), shift_speed);
    i++;
  }
}

String readCommand()
{
  String readString;
  while (Serial.available()) {
    delay(10);  //small delay to allow input buffer to fill

    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ';') {
      break;
    }  //breaks out of capture loop to print readstring
    readString += c; 
  } //makes the string readString 
  return readString; 
}

void sendHumTempSerial()
{
  Serial.print("T: ");
  Serial.print(temperature);
  Serial.print("ºC ");
  Serial.print("H: ");
  Serial.print(humidity);
  Serial.println("%");
}


void setRGBLed()
{

  if(temperature < 10) //BLUE
  {
    analogWrite(RedLedPin, 0);
    analogWrite(GreenLedPin, 0);
    analogWrite(BlueLedPin, led_brightness);
  }
  else if(temperature >= 10 && temperature < 20) //YELLOW
  {
    analogWrite(RedLedPin, led_brightness);
    analogWrite(GreenLedPin, led_brightness/2);
    analogWrite(BlueLedPin, 0);
  }
  else if(temperature >= 20 && temperature < 30) //ORANGE
  {
    analogWrite(RedLedPin, led_brightness);
    analogWrite(GreenLedPin, led_brightness/4);
    analogWrite(BlueLedPin, 0);
  }
  else if(temperature >= 30) //RED
  {
    analogWrite(RedLedPin, led_brightness);
    analogWrite(GreenLedPin, 0);
    analogWrite(BlueLedPin, 0);
  }
    
}

