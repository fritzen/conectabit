#include "HCSR04.h"

HCSR04::HCSR04(int trigPin, int echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}

void HCSR04::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long HCSR04::getDistance() {
  // Gera o pulso para medir a distância
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);

  // Mede o tempo do retorno do eco
  long duration = pulseIn(echoPin, HIGH, 10000); // Timeout de 30 milissegundos

  // Verifica se a leitura é válida (duration >= 100)
  if (duration >= 100) {
    // Remove a leitura mais antiga do total se já houver 50 leituras válidas
    if (validReadings == 50) {
      totalDuration -= readings[currentIndex];
    } else {
      validReadings++;
    }

    // Adiciona a nova leitura ao total
    totalDuration += duration;

    // Armazena a nova leitura no array
    readings[currentIndex] = duration;
    currentIndex = (currentIndex + 1) % MAX_READINGS; // Avança o índice circularmente

    // Calcula a média das leituras válidas
    averageDuration = totalDuration / validReadings;
    return averageDuration;
  } else {
    // Se não houver leituras válidas, retorna 0
    if (validReadings == 0) {
      return 0;
    }
    // Caso contrário, retorna a última média válida
    return averageDuration;
  }
}
