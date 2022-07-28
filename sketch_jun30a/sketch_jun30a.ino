int motor=3;     //Declara Pin del motor
 
void setup() {
  Serial.begin(9600); 
  Serial.println("Eliga de 0 a 9 la velocidad de giro del motor");
}
 
 
void loop() {
 
  // Si hay algun valor en la Consola Serial
  if (Serial.available()){
 
    //Variable donde se guarda el caracter enviado desde teclado
    char a = Serial.read();
 
    // Si el caracter ingresado esta entre 0 y 9
    if (a>='0' && a<='9'){ 
 
      //Variable para escalar el valor ingresado a rango de PWM
      int velocidad = map(a,'0','9',0,255);
      //Escritura de PWM al motor
      analogWrite(motor,velocidad);
      //Mensaje para el usuario
      Serial.print("El motor esta girando a la velocidad ");
      Serial.println(a);
 
    } else { 
 
      Serial.print("Velocidad invalida");
      Serial.println(a);
 
    }
  }
}
