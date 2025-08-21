#include <Servo.h>

// Configuração de Pinos
#define LEFT_SENSOR_PIN A0
#define RIGHT_SENSOR_PIN A1
#define OBSTACLE_SENSOR_PIN A2
#define BUZZER_PIN 11
#define LEFT_SERVO_PIN 8
#define RIGHT_SERVO_PIN 6

// Configuração de Limiares
#define LEFT_THRESHOLD 100
#define RIGHT_THRESHOLD 100
#define OBSTACLE_THRESHOLD 400

// Valores de PWM para controle dos servos
#define SERVO_STOP 90
#define SERVO_FORWARD_LEFT 80  
#define SERVO_FORWARD_RIGHT 100 
#define SERVO_BACKWARD_LEFT 100
#define SERVO_BACKWARD_RIGHT 80

Servo leftServo;
Servo rightServo;

// Estados do robô
enum RobotState {
    TURNING_LEFT,
    TURNING_RIGHT,
    STOPPED,
    MOVING_FORWARD
};

RobotState robotState = STOPPED;
String leftServoState = "Parado"; 
String rightServoState = "Parado"; 

// Função para ler sensor
int readSensor(int pin) {
    return analogRead(pin);
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

void adjustTrajectoryLeft(bool alternateMode) {
    if (alternateMode) {
        // Alternativa: "uma roda para cada lado"
        leftServoState = "Trás";
        rightServoState = "Frente";
        robotState = TURNING_LEFT;
        leftServo.write(112);  // Roda esquerda para trás
        rightServo.write(112); // Roda direita para frente
    } else {
        // Comportamento original
        leftServoState = "Trás";
        rightServoState = "Frente";
        robotState = TURNING_LEFT;
        leftServo.write(SERVO_STOP);
        rightServo.write(112);
    }
}

void adjustTrajectoryRight(bool alternateMode) {
    if (alternateMode) {
        // Alternativa: "uma roda para cada lado"
        leftServoState = "Frente";
        rightServoState = "Trás";
        robotState = TURNING_RIGHT;
        leftServo.write(78);  // Roda esquerda para frente
        rightServo.write(78); // Roda direita para trás
    } else {
        // Comportamento original
        leftServoState = "Frente";
        rightServoState = "Trás";
        robotState = TURNING_RIGHT;
        leftServo.write(78);
        rightServo.write(SERVO_STOP);
    }
}

// Parar
void stopMoving() {
    leftServoState = "Parado";
    rightServoState = "Parado";
    robotState = STOPPED;
    leftServo.write(SERVO_STOP); // Parada neutra
    rightServo.write(SERVO_STOP); // Parada neutra
}

void moveForward() {
    leftServoState = "Frente";
    rightServoState = "Frente";
    robotState = MOVING_FORWARD;
    leftServo.write(SERVO_FORWARD_LEFT);
    rightServo.write(SERVO_FORWARD_RIGHT);
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

bool toggleBehavior = false;

void loop() {
    int leftSensor = readSensor(LEFT_SENSOR_PIN);
    int rightSensor = readSensor(RIGHT_SENSOR_PIN);
    int obstacleSensor = readSensor(OBSTACLE_SENSOR_PIN);

    // Lógica de alternância entre sensores
    if (leftSensor < LEFT_THRESHOLD && rightSensor > RIGHT_THRESHOLD) {
        adjustTrajectoryLeft(toggleBehavior); // Passa o modo alternado
        delay(20);

    } else if (rightSensor < RIGHT_THRESHOLD && leftSensor > LEFT_THRESHOLD) {
        adjustTrajectoryRight(toggleBehavior); // Passa o modo alternado
        delay(20);
    } else if (leftSensor < LEFT_THRESHOLD && rightSensor < RIGHT_THRESHOLD) {
        moveForward();
    } else {
        stopMoving();
    }

    // Detecção de obstáculos
    if (obstacleSensor < OBSTACLE_THRESHOLD) {
        Serial.println("Detecção de obstáculos");
        stopMoving();

        // Alternar entre os modos
        toggleBehavior = !toggleBehavior;

        // Bloqueia enquanto o obstáculo ainda é detectado
        while (obstacleSensor < OBSTACLE_THRESHOLD) {
            playStartupSound(); // Som de alerta
            obstacleSensor = readSensor(OBSTACLE_SENSOR_PIN); // Atualizar valor do sensor
        }

        noTone(BUZZER_PIN); // Parar o som
    }

    // Depuração (opcional)
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
        case TURNING_LEFT:
            Serial.println("Desviando para esquerda");
            break;
        case TURNING_RIGHT:
            Serial.println("Desviando para direita");
            break;
        case MOVING_FORWARD:
            Serial.println("Movendo para frente");
            break;
        case STOPPED:
            Serial.println("Parado");
            break;
    }
}
