#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.println((5.0 * analogRead(A0) * 100.0)/1024.0);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  
  Serial.println();
  delay(1000);
}
