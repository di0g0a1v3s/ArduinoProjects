// IN1 no pino 7
//IN2 no pino 8
//ENA no pino 9

void setup(){
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
}

void loop(){
//Girar em um sentido
digitalWrite(7, HIGH); 
analogWrite(9, 255);  //Motor com rotação máxima (0-255)
digitalWrite(8, LOW);
delay(3000);
//Girar no sentido inverso ao anterior
digitalWrite(7, LOW);
analogWrite(9, 150);  //Motor com rotação quase mediana (0-255)
digitalWrite(8, HIGH);
delay(3000);
}
