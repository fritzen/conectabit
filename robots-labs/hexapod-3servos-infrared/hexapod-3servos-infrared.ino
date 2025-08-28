#include "ServoEx.h"
#include "Walking.h"
#include "ActivityTimer.h"

// Definição dos pinos dos sensores infravermelhos
#define SENSOR_ESQUERDA A0  
#define SENSOR_DIREITA A1  
#define SENSOR_MEIO A2    

// Valores de limiar para detecção de chão e obstáculo
#define LIMIAR_LINHA 100         // Valor de referência para detectar o chão (ajuste conforme o ambiente)


void setup()
{
  Serial.begin(38400);

  // Configuração dos pinos dos sensores como entrada
  pinMode(SENSOR_ESQUERDA, INPUT);
  pinMode(SENSOR_DIREITA, INPUT);
  pinMode(SENSOR_MEIO, INPUT);

  initializeServos();

  SetServoCenter(2000); // Set servos to center values over 2000ms period
  delay(1000);

  myActivityTimer.setTimeoutThreshold(15); 
  myActivityTimer.setAlertInterval(10);  

  currentWalkCommand.moveSpeed = 85; 
}

// Define as direções possíveis
// Define as direções possíveis
enum Direcao {
  FRENTE,
  ESQUERDA,
  DIREITA
};

void loop() {
  // Lê os valores dos sensores
  int valorEsquerda = analogRead(SENSOR_ESQUERDA);
  int valorDireita = analogRead(SENSOR_DIREITA);
  int valorMeio = analogRead(SENSOR_MEIO);

  // Verifica se os sensores detectaram linha preta
  bool linhaEsquerda = valorEsquerda < LIMIAR_LINHA;
  bool linhaDireita = valorDireita < LIMIAR_LINHA;
  bool linhaMeio = valorMeio < LIMIAR_LINHA;

  // Debug via Serial
  Serial.print("Sensores E|M|D: ");
  Serial.print(valorEsquerda);
  Serial.print("|");
  Serial.print(valorMeio);
  Serial.print("|");
  Serial.print(valorDireita);
  Serial.print(" | Linha E|M|D: ");
  Serial.print(linhaEsquerda);
  Serial.print("|");
  Serial.print(linhaMeio);
  Serial.print("|");
  Serial.print(linhaDireita);
  Serial.print(" | ");

  static Direcao direcaoAtual = FRENTE;
  static bool estaCorrigindo = false;

  // Se encontrou linha no meio durante correção, volta para frente
  if (linhaMeio && estaCorrigindo) {
    direcaoAtual = FRENTE;
    estaCorrigindo = false;
    Serial.println("ACAO: Linha centralizada - Seguindo em frente");
  }
  // Se não está corrigindo, verifica se precisa iniciar correção
  else if (!estaCorrigindo) {
    if (linhaEsquerda) {
      direcaoAtual = ESQUERDA;
      estaCorrigindo = true;
      Serial.println("ACAO: Iniciando correção para esquerda");
    }
    else if (linhaDireita) {
      direcaoAtual = DIREITA;
      estaCorrigindo = true;
      Serial.println("ACAO: Iniciando correção para direita");
    }
    else {
      Serial.println("ACAO: Seguindo em frente");
    }
  }

  // Executa o movimento
  switch(direcaoAtual) {
    case FRENTE:
      WalkForward(1, currentWalkCommand.moveSpeed);
      break;
    case ESQUERDA:
      TurnLeft(1, currentWalkCommand.moveSpeed);
      break;
    case DIREITA:
      TurnRight(1, currentWalkCommand.moveSpeed);
      break;
  }

  myActivityTimer.updateActivityTimer();
  delay(1);
}