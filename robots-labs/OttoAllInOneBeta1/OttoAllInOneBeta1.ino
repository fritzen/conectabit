#include <Servo.h> 
#include "Oscillator.h"
#include "US.h"
#include "Otto.h"

#define PIN_LEFT_LEG   2   
#define PIN_RIGHT_LEG  3  
#define PIN_LEFT_FOOT  4  
#define PIN_RIGHT_FOOT 5 

#define IR_MIDDLE_SENSOR A0
#define IR_LEFT_SENSOR A1
#define IR_RIGHT_SENSOR A2

#define IR_MIDDLE_THRESHOLD 120 // Limiar do sensor do meio
#define IR_SIDE_THRESHOLD 900   // Limiar dos sensores laterais

// Ajuste fino dos servos: esses parâmetros definem os valores de calibração para os servos,
// permitindo ajustes finos para a posição das pernas e pés do robô. Eles podem ser modificados
// caso o Otto não esteja andando em linha reta ou precise de correções de calibração.
#define TRIM_LEFT_LEG   -30    // Ajuste fino para a posição do servo da perna esquerda
#define TRIM_RIGHT_LEG  10     // Ajuste fino para a posição do servo da perna direita
#define TRIM_LEFT_FOOT  0      // Ajuste fino para a posição do servo do pé esquerdo
#define TRIM_RIGHT_FOOT -0     // Ajuste fino para a posição do servo do pé direito

// Exemplos de situações em que os ajustes finos podem ser úteis:
// 1. Se Otto não está andando em linha reta, ajuste os valores dos trims para corrigir a direção.
// 2. Se as pernas ou os pés de Otto não estão na posição desejada quando em repouso, os trims podem ser
//    usados para fazer pequenos ajustes e garantir uma postura estável.
// 3. Se Otto está tendendo a desviar para um lado ao caminhar, ajuste os trims correspondentes para corrigir
//    essa tendência e manter o equilíbrio durante o movimento.


Otto Otto;  //This is Otto!

void setup()
{
  Serial.begin(9600);
  Serial.println("hello");
  
  //Set the servo pins
  Otto.init(PIN_LEFT_LEG,PIN_RIGHT_LEG,PIN_LEFT_FOOT,PIN_RIGHT_FOOT,true);
  Otto.setTrims(TRIM_LEFT_LEG,TRIM_RIGHT_LEG, TRIM_LEFT_FOOT, TRIM_RIGHT_FOOT);

  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
}

void loop() 
{
  bool obstacleDetected = false;
  
  int middleSensorValue = analogRead(IR_MIDDLE_SENSOR);
  int leftSensorValue = analogRead(IR_LEFT_SENSOR);
  int rightSensorValue = analogRead(IR_RIGHT_SENSOR);


  Serial.print("Middle Sensor: ");
  Serial.print(middleSensorValue);
  Serial.print("\t Left Sensor: ");
  Serial.print(leftSensorValue);
  Serial.print("\t Right Sensor: ");
  Serial.println(rightSensorValue);
  
  
  // Verifica se há um obstáculo à frente
  if (middleSensorValue < IR_MIDDLE_THRESHOLD) {
    Otto.sing(S_surprise); 
    Otto.walk(7,700,FORWARD); 
    delay(1000);
    Otto.sing(S_happy);
    Otto.turn(10,500,RIGHT);                
    delay(2000); 
    obstacleDetected = true;
  }
  
  // Verifica se há um obstáculo para baixo (escada, buraco)
  if (leftSensorValue > IR_SIDE_THRESHOLD || rightSensorValue > IR_SIDE_THRESHOLD) {
    Otto.sing(S_surprise); 
    Otto.walk(7,1000,FORWARD); 
    delay(1000);
    Otto.sing(S_happy);
    Otto.turn(10,1000,RIGHT);                
    delay(2000); 
    obstacleDetected = true;
  }
  
  if (!obstacleDetected) {
    Otto.walk(1,900,BACKWARD);
  }
}
