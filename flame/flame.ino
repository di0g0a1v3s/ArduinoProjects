int flameSensorPin = 0; // the cell and 10K pulldown are connected to a0
int led_red = 3;
int buzzer = 2;
int flameSensorReading[10]; // the analog reading from the analog resistor divider
int i = 0;
int fire = 0;

void setup(void) { 
Serial.begin(9600); 
}

void loop(void) {
flameSensorReading[i] = analogRead(flameSensorPin); 
analogWrite(led_red, flameSensorReading[i]/4);
fire = 1;
for(int j = 0; j < 10; j++)
{
  if(flameSensorReading[j] == 0)
    fire = 0;
}
if(fire == 1)
  digitalWrite(buzzer, HIGH);
else
  digitalWrite(buzzer, LOW);
Serial.print("Analog reading = ");
Serial.println(flameSensorReading[i]); // the raw analog reading 
delay(10);
i = i + 1;
i = i % 10;
}
