#include <Servo.h>
#include "FzMotorDriver.h"

// Definição dos pinos
const int TRIGGER_ESQ_PIN = A0;
const int ECHO_ESQ_PIN = A1;
const int TRIGGER_DIR_PIN = A2;
const int ECHO_DIR_PIN = A3;

const int BUZZER = 4;

const int LIMITE_DISTANCIA = 18;


FzMotorDriver motor;

void setup() {
  // Definição dos pinos como entrada ou saída
  pinMode(TRIGGER_ESQ_PIN, OUTPUT);
  pinMode(ECHO_ESQ_PIN, INPUT);
  pinMode(TRIGGER_DIR_PIN, OUTPUT);
  pinMode(ECHO_DIR_PIN, INPUT);


  pinMode(BUZZER, OUTPUT);
  
  // Inicializa a comunicação serial para depuração
  Serial.begin(9600);
  
  motor.attach(L9110);
  motor.setSpeed(120,100);
}


void loop() {
  long duracaoEsq, duracaoDir, distanciaEsq, distanciaDir;

  // Leitura dos sensores de obstáculos
  digitalWrite(TRIGGER_ESQ_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_ESQ_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_ESQ_PIN, LOW);
  duracaoEsq = pulseIn(ECHO_ESQ_PIN, HIGH, 500000); //5000000 s
  distanciaEsq = (duracaoEsq / 2) / 29.1;  // Converte para centímetros
    
  delay(5); // Pequeno atraso para evitar oscilações

  digitalWrite(TRIGGER_DIR_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_DIR_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_DIR_PIN, LOW);
  duracaoDir = pulseIn(ECHO_DIR_PIN, HIGH, 500000);
  distanciaDir = (duracaoDir / 2) / 29.1;  // Converte para centímetros
  

Serial.print("Esq: "); Serial.print(distanciaEsq); Serial.print(" | Dir: "); Serial.print(distanciaDir); Serial.println(" | Sensor Esq: "); 

  
    // Nenhum sensor detecta a linha
    // Lógica de evitar obstáculos
    if (distanciaEsq < LIMITE_DISTANCIA && distanciaDir < LIMITE_DISTANCIA) {
      motor.backward();
      delay(600);
      motor.turnRightAxis();
      delay(1000);
      motor.stop();
      digitalWrite(BUZZER,HIGH);delay(500);digitalWrite(BUZZER,LOW); 
      delay(500);
      digitalWrite(BUZZER,HIGH);delay(500);digitalWrite(BUZZER,LOW); 
     } else if (distanciaEsq >= LIMITE_DISTANCIA && distanciaDir >= LIMITE_DISTANCIA) {
      motor.forward();
    } else if (distanciaEsq < distanciaDir) {
      motor.turnRight();
    } else {
      motor.turnLeft();      
    }
  
  delay(5); // Pequeno atraso para evitar oscilações
}