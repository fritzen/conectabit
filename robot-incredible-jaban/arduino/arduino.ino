#include <Servo.h>

// Configuração de Pinos
#define LEFT_SENSOR_PIN A0
#define RIGHT_SENSOR_PIN A1
#define OBSTACLE_SENSOR_PIN A2
#define BUZZER_PIN 11
#define LEFT_SERVO_PIN 9
#define RIGHT_SERVO_PIN 10

// Configuração de Limiares
#define LEFT_THRESHOLD 10
#define RIGHT_THRESHOLD 10
#define OBSTACLE_THRESHOLD 50

// Valores de PWM para controle dos servos
#define SERVO_STOP 90
#define SERVO_FORWARD_LEFT 60
#define SERVO_FORWARD_RIGHT 120
#define SERVO_BACKWARD_LEFT 120
#define SERVO_BACKWARD_RIGHT 60

// Intervalo de atualização (em milissegundos)
#define UPDATE_INTERVAL 50

Servo leftServo;
Servo rightServo;

// Estados do robô
enum RobotState {
  FORWARD,
  TURNING_LEFT,
  TURNING_RIGHT,
  STOPPED
};

RobotState robotState = STOPPED; // Estado inicial do robô

String leftServoState = "Parado"; // Estado inicial do servo esquerdo
String rightServoState = "Parado"; // Estado inicial do servo direito

// Controle de tempo
unsigned long previousMillis = 0;

// Função para ler sensor
int readSensor(int pin) {
  return analogRead(pin);
}

// Função para tocar som
void playBeep(int frequency = 500, int duration = 100) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

// Som de inicialização
void playStartupSound() {
  tone(BUZZER_PIN, 262, 200); // Nota C4
  delay(250);
  tone(BUZZER_PIN, 330, 200); // Nota E4
  delay(250);
  tone(BUZZER_PIN, 392, 200); // Nota G4
  delay(250);
  noTone(BUZZER_PIN);
}

// Mover para frente
void moveForward() {
  leftServoState = "Frente";
  rightServoState = "Frente";
  robotState = FORWARD;
  leftServo.write(SERVO_FORWARD_LEFT); // Servo esquerdo avança
  rightServo.write(SERVO_FORWARD_RIGHT); // Servo direito avança
}

// Ajustar trajetória
void adjustTrajectory(bool turnLeft) {
  if (turnLeft) {
    leftServoState = "Trás";
    rightServoState = "Frente";
    robotState = TURNING_RIGHT;
    leftServo.write(SERVO_BACKWARD_RIGHT); // Servo esquerdo gira para trás
    rightServo.write(SERVO_STOP);  // Servo direito avança
  } else {
    leftServoState = "Frente";
    rightServoState = "Trás";
    robotState = TURNING_LEFT;
    leftServo.write(SERVO_STOP); // Servo esquerdo avança
    rightServo.write(SERVO_BACKWARD_LEFT); // Servo direito gira para trás
  }

  delay(10);
}

// Parar
void stopMoving() {
  leftServoState = "Parado";
  rightServoState = "Parado";
  robotState = STOPPED;
  leftServo.write(SERVO_STOP); // Parada neutra
  rightServo.write(SERVO_STOP); // Parada neutra
}

void setup() {
  Serial.begin(9600);
  // Configurar pinos de sensores
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(OBSTACLE_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Configurar servos
  leftServo.attach(LEFT_SERVO_PIN);
  rightServo.attach(RIGHT_SERVO_PIN);
  // Som de inicialização
  playStartupSound();
}

void loop() {
  // Verifica se já passou o intervalo definido
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= UPDATE_INTERVAL) {
    previousMillis = currentMillis; // Atualiza o tempo anterior

    int leftSensor = readSensor(LEFT_SENSOR_PIN);
    int rightSensor = readSensor(RIGHT_SENSOR_PIN);
    int obstacleSensor = readSensor(OBSTACLE_SENSOR_PIN);

    // Lógica de obstáculos
    // if (obstacleSensor < OBSTACLE_THRESHOLD) {
    //   playBeep(1000, 200);
    //   stopMoving();
    //   return;
    // }

    // Lógica de seguimento de linha
    if (leftSensor < LEFT_THRESHOLD && rightSensor < LEFT_THRESHOLD) {
      // Ambos sensores no branco - move para frente
      moveForward();
    } else if (leftSensor < LEFT_THRESHOLD) {
      // Sensor esquerdo detecta linha escura
      playBeep();
      adjustTrajectory(false); // Ajusta para a direita
    } else if (rightSensor < RIGHT_THRESHOLD) {
      // Sensor direito detecta linha escura
      playBeep();
      adjustTrajectory(true); // Ajusta para a esquerda
    } else {
      stopMoving(); // Caso contrário, pare
    }

    // Depuração linha a linha
    Serial.print("LeftSensor: ");
    Serial.print(leftSensor);
    Serial.print(" | RightSensor: ");
    Serial.print(rightSensor);
    Serial.print(" | ObstacleSensor: ");
    Serial.print(obstacleSensor);
    Serial.print(" | LeftServo: ");
    Serial.print(leftServoState);
    Serial.print(" | RightServo: ");
    Serial.print(rightServoState);
    Serial.print(" | RobotState: ");
    switch (robotState) {
      case FORWARD:
        Serial.println("Indo para frente");
        break;
      case TURNING_LEFT:
        Serial.println("Desviando para esquerda");
        break;
      case TURNING_RIGHT:
        Serial.println("Desviando para direita");
        break;
      case STOPPED:
        Serial.println("Parado");
        break;
    }
  }
}
