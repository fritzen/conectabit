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

const int BUZZER = 4;

// Define o valor de threshold para detectar obstáculos
// Quanto menor o valor analógico, mais próximo está o obstáculo
const int THRESHOLD = 100;  // Ajuste este valor conforme a necessidade do seu sensor

FzMotorDriver motor;

void setup() {
  // Definição dos pinos como entrada
  pinMode(SENSOR_IR_ESQ, INPUT);
  pinMode(SENSOR_IR_DIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Inicializa a comunicação serial para depuração
  Serial.begin(9600);

  Serial.println("SETUP");
  
  motor.attach(MOTOR_DIR_A, MOTOR_PWM_A, MOTOR_DIR_B, MOTOR_PWM_B);
  motor.setSpeed(120, 100);
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

  // Lógica de evitar obstáculos
  // Note que para sensores IR, valores menores indicam objetos mais próximos
  if (valorSensorEsq < THRESHOLD && valorSensorDir < THRESHOLD) {
    // Obstáculo detectado em ambos os sensores
    motor.backward();
    delay(600);
    motor.turnRightAxis();
    delay(1000);
    motor.stop();
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
  } 
  else if (valorSensorEsq > THRESHOLD && valorSensorDir > THRESHOLD) {
    // Nenhum obstáculo detectado
    motor.forward();
  } 
  else if (valorSensorEsq < valorSensorDir) {
    // Obstáculo mais próximo à esquerda
    motor.turnRight();
  } 
  else {
    // Obstáculo mais próximo à direita
    motor.turnLeft();
  }
  
  delay(5); // Pequeno atraso para evitar oscilações
}