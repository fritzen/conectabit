#include <Servo.h>
#include "FzMotorDriver.h"

// Definição dos pinos dos motores
const int MOTOR_PWM_A = 5;  // PWM Motor 1
const int MOTOR_DIR_A = 4;  // Direction Motor 1
const int MOTOR_PWM_B = 6;  // PWM Motor 2
const int MOTOR_DIR_B = 7;  // Direction Motor 2

// Definição dos pinos para os sensores IR
const int SENSOR_IR_ESQ = A6;  // Sensor IR esquerdo no pino analógico A0
const int SENSOR_IR_DIR = A7;  // Sensor IR direito no pino analógico A1

const int BUZZER = 8;

// Define o valor de threshold para detectar obstáculos
// Quanto menor o valor analógico, mais próximo está o obstáculo
const int THRESHOLD = 500;  // Ajuste este valor conforme a necessidade do seu sensor

FzMotorDriver motor;

void setup() {
  pinMode(SENSOR_IR_ESQ, INPUT);
  pinMode(SENSOR_IR_DIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("SETUP");
  
  motor.attach(MOTOR_DIR_A, MOTOR_PWM_A, MOTOR_DIR_B, MOTOR_PWM_B);
}

void alertBuzzer() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  // Leitura dos sensores IR
  int valorSensorEsq = analogRead(SENSOR_IR_ESQ);
  int valorSensorDir = analogRead(SENSOR_IR_DIR);

  // Debug - imprime os valores dos sensores
  Serial.print("Esq: "); 
  Serial.print(valorSensorEsq); 
  Serial.print(" | Dir: "); 
  Serial.println(valorSensorDir);

  bool obstaculoEsq = (valorSensorEsq <= THRESHOLD);
  bool obstaculoDir = (valorSensorDir <= THRESHOLD);

  if (!obstaculoEsq && !obstaculoDir) {
    // Nenhum obstáculo - segue em frente
    motor.forward();
  } 
  else if (obstaculoEsq && obstaculoDir) {
    // Obstáculo em ambos os lados
    alertBuzzer();
    motor.backward();
    delay(800);  // Recua por mais tempo
    motor.turnRightAxis();
    delay(1000); // Gira 90 graus
    motor.stop();
  }
  else if (obstaculoEsq) {
    motor.backward();
    delay(50);  // Recua por mais tempo
    motor.turnLeft();  // Vira para direita
    delay(100);
  }
  else if (obstaculoDir) {
    motor.backward();
    delay(50);  // Recua por mais tempo
    motor.turnRight();   // Vira para esquerda
    delay(100);
  }
  

}