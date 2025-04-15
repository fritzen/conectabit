// Line Follower Robot com Controle Digital Simplificado (Full Speed)
// U-TURN quando obstáculo detectado (sensor central)
// Obs.: Sensor central continua inicializado, mas não é usado no controle normal.

#define LEFT_SENSOR_PIN A6
#define RIGHT_SENSOR_PIN A7
#define CENTER_SENSOR_PIN A5  

// Motor pins (L293D)
// Motor A - Esquerdo
#define LEFT_MOTOR_PIN_1 5
#define LEFT_MOTOR_PIN_2 6
// Motor B - Direito
#define RIGHT_MOTOR_PIN_1 10
#define RIGHT_MOTOR_PIN_2 9

// Limiar para detectar a linha (valor analógico menor indica linha)
#define THRESHOLD 120

// Leituras dos sensores
int leftSensorValue = 0;
int centerSensorValue = 0;
int rightSensorValue = 0;

// Variáveis de estado do U-TURN
bool uTurn = false;
bool alertTriggered = false;

void setup() {
  Serial.begin(9600);
  
  // Configura os pinos dos motores como saída
  pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
  
  // Configura os pinos dos sensores como entrada
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(CENTER_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  
  Serial.println("START");
  delay(500);
}

void loop() {
  readSensors();
  
  // Se não estiver em U-TURN e detectar obstáculo via sensor central, entra em modo U-TURN
  if (!uTurn && (centerSensorValue < THRESHOLD)) {
    uTurn = true;
    alertTriggered = false;
  }
  
  if (uTurn) {
    uTurnRoutine();
  } else {
    simpleControl();
  }
  
  printDebugInfo();
  delay(10);
}

void readSensors() {
  leftSensorValue = analogRead(LEFT_SENSOR_PIN);
  centerSensorValue = analogRead(CENTER_SENSOR_PIN);
  rightSensorValue = analogRead(RIGHT_SENSOR_PIN);
}

// Controle normal: usa apenas os sensores laterais para seguir a linha
void simpleControl() {
  bool leftOnLine = (leftSensorValue < THRESHOLD);
  bool rightOnLine = (rightSensorValue < THRESHOLD);
  
  // Se apenas o sensor esquerdo detectar a linha, desliga o motor direito para virar à direita
  if (leftOnLine && !rightOnLine) {
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  }
  // Se apenas o sensor direito detectar a linha, desliga o motor esquerdo para virar à esquerda
  else if (rightOnLine && !leftOnLine) {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
  }
  // Se ambos ou nenhum sensor detectarem a linha, segue em frente
  else {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
  }
}

// Procedimento de U-TURN: roda sempre para a direita
// Ao acionar o sensor direito (alerta) e depois ele deixar de detectar a linha,
// finaliza o U-TURN e retoma o controle normal.
void uTurnRoutine() {
  // Gira para a direita: liga apenas o motor esquerdo (mesmo comando de virar à direita)
  digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
  
  // Quando o sensor direito detectar a linha pela primeira vez, ativa o alerta
  if (!alertTriggered && (rightSensorValue < THRESHOLD)) {
    alertTriggered = true;
  }
  
  // Se o alerta já estiver ativo e o sensor direito deixar de detectar a linha, termina o U-TURN
  if (alertTriggered && (rightSensorValue >= THRESHOLD)) {
    uTurn = false;
    alertTriggered = false;
  }
}

void printDebugInfo() {
  Serial.print("L: ");
  Serial.print(leftSensorValue);
  Serial.print(" | C: ");
  Serial.print(centerSensorValue);
  Serial.print(" | R: ");
  Serial.print(rightSensorValue);
  Serial.print(" | uTurn: ");
  Serial.println(uTurn);
}
