int lightSensorPin = A0;
int analogValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
}

void loop(){
  analogValue = analogRead(lightSensorPin);
  Serial.println(analogValue);
  delay(200);
}
