//Motor A
#define A_IA 10 //velociadade (porta deve ser PWM): 0 = parado, 255 = rápido
#define A_IB 11 //Direção (bit)

//Motor B
#define B_IA 9 //velociadade (porta deve ser PWM): 0 = parado, 255 = rápido
#define B_IB 6 //Direção (bit)

int speed = 60;

void setup() {
  pinMode(A_IA, OUTPUT);
  pinMode(A_IB, OUTPUT);
  pinMode(B_IA, OUTPUT);
  pinMode(B_IB, OUTPUT);
}

void loop() {

  //rodar esquerda, velocidade atual
  runMotor(A_IA, A_IB, false, speed);
  runMotor(B_IA, B_IB, false, speed);
  delay(1000);
  //rodar direita, velocidade atual
  runMotor(A_IA, A_IB, true, speed);
  runMotor(B_IA, B_IB, true, speed);
  delay(1000);

  //atualizar velocidade
  speed=speed+30;
  if (speed>255)speed=60;

}

void runMotor(uint8_t  speedPin, uint8_t  dirPin, bool reverse, int speed) {
  if (reverse) {
    analogWrite(speedPin, 255 - speed);    
  } else {
    analogWrite(speedPin, speed);  
  }
  
  digitalWrite(dirPin, reverse);
}