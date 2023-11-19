#include <Servo.h>

Servo servo;
#define SERVO_PIN 9
#define INCREMENT 1
#define MIN_ANGLE 10
#define MAX_ANGLE 100

int currentAngle = 90;

// Defina a flag de idioma aqui
#define PORTUGUESE  // Comente esta linha para usar o idioma Inglês

#ifdef PORTUGUESE
  #define OPEN_TEXT "Pressione A para aumentar o ângulo e D para diminuir o ângulo"
  #define CURRENT_ANGLE_TEXT "Ângulo atual: "
  #define MAX_ANGLE_TEXT "Ângulo máximo atingido!"
  #define MIN_ANGLE_TEXT "Ângulo mínimo atingido!"
#else
  #define OPEN_TEXT "Press A to increase the angle and D to decrease the angle"
  #define CURRENT_ANGLE_TEXT "Current angle: "
  #define MAX_ANGLE_TEXT "Maximum angle reached!"
  #define MIN_ANGLE_TEXT "Minimum angle reached!"
#endif

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(currentAngle);
  Serial.println(OPEN_TEXT);
  Serial.print(CURRENT_ANGLE_TEXT);
  Serial.println(currentAngle);
  Serial.println("---------------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == 'A' || input == 'a') {
      if (currentAngle + INCREMENT <= MAX_ANGLE) {
        currentAngle += INCREMENT;
        servo.write(currentAngle);
        Serial.print(CURRENT_ANGLE_TEXT);
        Serial.println(currentAngle);
      } else {
        Serial.println(MAX_ANGLE_TEXT);
      }
    } else if (input == 'D' || input == 'd') {
      if (currentAngle - INCREMENT >= MIN_ANGLE) {
        currentAngle -= INCREMENT;
        servo.write(currentAngle);
        Serial.print(CURRENT_ANGLE_TEXT);
        Serial.println(currentAngle);
      } else {
        Serial.println(MIN_ANGLE_TEXT);
      }
    }
  }
}