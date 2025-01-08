/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 28 May 2015
  by Michael C. Miller
  modified 8 Nov 2013
  by Scott Fitzgerald

  http://arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

void setup() {
  myservo.attach(10);  // attaches the servo on GIO2 to the servo object
  Serial.begin(9600); 
  Serial.println("Setup completo. Servo anexado.");
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) { // Vai de 0 a 180 graus
    myservo.write(pos); // Define o servo na posição 'pos'
    Serial.print("Posição atual: ");
    Serial.println(pos);
    
    if (pos == 0 || pos == 90 || pos == 180) {
      Serial.print("Posição especial: ");
      Serial.println(pos);
      delay(2000); // Atraso de 2 segundos nas posições 0, 90 e 180
    } else {
      delay(15); // Pequeno atraso para permitir que o servo se mova
    }
  }

  for (int pos = 180; pos >= 0; pos--) { // Volta de 180 a 0 graus
    myservo.write(pos); // Define o servo na posição 'pos'
    Serial.print("Posição atual: ");
    Serial.println(pos);
    
    if (pos == 0 || pos == 90 || pos == 180) {
      Serial.print("Posição especial: ");
      Serial.println(pos);
      delay(2000); // Atraso de 2 segundos nas posições 0, 90 e 180
    } else {
      delay(15); // Pequeno atraso para permitir que o servo se mova
    }
  }
}
