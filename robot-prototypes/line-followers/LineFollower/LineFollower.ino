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

// Variáveis dinâmicas para threshold de cada sensor
int leftThreshold = 0;
int centerThreshold = 0;
int rightThreshold = 0;

// Leituras dos sensores
int leftSensorValue = 0;
int centerSensorValue = 0;
int rightSensorValue = 0;

// Variáveis de estado do U-TURN
bool uTurn = false;
bool alertTriggered = false;

void setup() {
  Serial.begin(9600);
  
  // Configura pinos dos motores
  pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
  
  // Configura pinos dos sensores
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(CENTER_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);

  // Configura LED embutido
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Garante motores parados antes da calibração
  digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
  digitalWrite(LEFT_MOTOR_PIN_1, LOW);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  delay(100);

  // Calibração dinâmica com LED piscando
  calibrateThresholds();
  
  Serial.print("Left Threshold: ");
  Serial.print(leftThreshold);
  Serial.print(" Center Threshold: ");
  Serial.print(centerThreshold);
  Serial.print(" Right Threshold: ");
  Serial.println(rightThreshold);
  Serial.println("START");
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  readSensors();
  
  // Detecta obstáculo e inicia U-TURN
  if (!uTurn && (centerSensorValue < centerThreshold)) {
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

// Calibra thresholds individuais piscando o LED durante cada amostra
void calibrateThresholds() {
  long leftSum = 0;
  long rightSum = 0;
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    
    leftSum += analogRead(LEFT_SENSOR_PIN);
    rightSum += analogRead(RIGHT_SENSOR_PIN);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  
  leftThreshold = (leftSum / 5) + 50;
  centerThreshold = 180;
  rightThreshold = (rightSum / 5) + 50;
}

void readSensors() {
  leftSensorValue = analogRead(LEFT_SENSOR_PIN);
  centerSensorValue = analogRead(CENTER_SENSOR_PIN);
  rightSensorValue = analogRead(RIGHT_SENSOR_PIN);
}

// Controle normal usando sensores laterais
void simpleControl() {
  bool leftOnLine = (leftSensorValue < leftThreshold);
  bool rightOnLine = (rightSensorValue < rightThreshold);
  
  if (leftOnLine && !rightOnLine) {
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  }
  else if (rightOnLine && !leftOnLine) {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
  }
  else {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
  }
}

// Procedimento de U-TURN girando sempre à direita
void uTurnRoutine() {
  digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
  
  if (!alertTriggered && (rightSensorValue < rightThreshold)) {
    alertTriggered = true;
  }
  
  if (alertTriggered && (rightSensorValue >= rightThreshold)) {
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
  Serial.print(uTurn);
  Serial.print(" | LT: ");
  Serial.print(leftThreshold);
  Serial.print(" | CT: ");
  Serial.print(centerThreshold);
  Serial.print(" | RT: ");
  Serial.println(rightThreshold);
}
